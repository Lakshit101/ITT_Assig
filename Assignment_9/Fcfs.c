#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum
{
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} State;

typedef struct PCB
{
    char name[32];
    int pid;

    int burstTime;
    int remainingBurst;

    int ioStart;
    int ioDuration;
    int ioRemaining;

    int executedTime;

    int startTime;
    int completionTime;

    State state;
} PCB;

PCB *createPcb(char *name, int pid, int burst, int ioStart, int ioDur)
{
    PCB *p = (PCB *)malloc(sizeof(PCB));
    strcpy(p->name, name);
    p->pid = pid;
    p->burstTime = burst;
    p->remainingBurst = burst;

    p->ioStart = ioStart;
    p->ioDuration = ioDur;
    p->ioRemaining = 0;

    p->executedTime = 0;
    p->startTime = 0;
    p->completionTime = 0;

    p->state = READY;
    return p;
}

typedef struct QNode
{
    PCB *pcb;
    struct QNode *next;
} QNode;

typedef struct Queue
{
    QNode *front;
    QNode *rear;
} Queue;

void initQueue(Queue *q)
{
    q->front = q->rear = NULL;
}

void enqueue(Queue *q, PCB *p)
{
    QNode *temp = (QNode *)malloc(sizeof(QNode));
    temp->pcb = p;
    temp->next = NULL;

    if (q->rear == NULL)
        q->front = q->rear = temp;
    else
    {
        q->rear->next = temp;
        q->rear = temp;
    }
}

PCB *dequeue(Queue *q)
{
    if (q->front == NULL)
        return NULL;

    QNode *temp = q->front;
    PCB *p = temp->pcb;

    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
    return p;
}

int isEmpty(Queue *q)
{
    return q->front == NULL;
}

#define TABLE_SIZE 1009

typedef struct
{
    int used;
    int pid;
    PCB *pcb;
} HashEntry;

HashEntry table[TABLE_SIZE];

void initHashmap()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table[i].used = 0;
        table[i].pid = -1;
        table[i].pcb = NULL;
    }
}

int hashValue(int pid)
{
    return pid % TABLE_SIZE;
}

void insertPcb(PCB *p)
{
    int idx = hashValue(p->pid);

    while (table[idx].used == 1)
    {
        idx = (idx + 1) % TABLE_SIZE;
    }

    table[idx].used = 1;
    table[idx].pid = p->pid;
    table[idx].pcb = p;
}

PCB *getPcb(int pid)
{
    int idx = hashValue(pid);
    while (table[idx].used != 0)
    {
        if (table[idx].pid == pid)
            return table[idx].pcb;
        idx = (idx + 1) % TABLE_SIZE;
    }
    return NULL;
}

void deletePcb(int pid)
{
    int idx = hashValue(pid);
    while (table[idx].used != 0)
    {
        if (table[idx].pid == pid)
        {
            table[idx].used = 0;
            table[idx].pid = -1;
            table[idx].pcb = NULL;
            return;
        }
        idx = (idx + 1) % TABLE_SIZE;
    }
}

typedef struct
{
    int pid;
    int killTime;
} KillEvent;

KillEvent kills[100];
int killCount = 0;

int currentTime = 0;

Queue readyQueue, waitQueue, termQueue;

void updateWaitQueue()
{
    QNode *node = waitQueue.front;
    QNode *prev = NULL;

    while (node != NULL)
    {
        PCB *p = node->pcb;
        p->ioRemaining--;

        if (p->ioRemaining <= 0)
        {
            p->state = READY;

            if (prev == NULL)
                waitQueue.front = node->next;
            else
                prev->next = node->next;

            if (node == waitQueue.rear)
                waitQueue.rear = prev;

            QNode *tmp = node;
            node = node->next;

            enqueue(&readyQueue, tmp->pcb);
            free(tmp);
            continue;
        }

        prev = node;
        node = node->next;
    }
}

void applyKillEvents()
{
    for (int i = 0; i < killCount; i++)
    {
        if (kills[i].killTime == currentTime)
        {
            PCB *p = getPcb(kills[i].pid);
            if (p != NULL && p->state != TERMINATED)
            {
                p->state = TERMINATED;
                p->completionTime = currentTime;

                enqueue(&termQueue, p);
                deletePcb(p->pid);
            }
        }
    }
}

void scheduler()
{
    while (1)
    {
        applyKillEvents();

        if (!isEmpty(&readyQueue))
        {
            PCB *p = dequeue(&readyQueue);

            if (p->state == TERMINATED)
                continue;

            while (1)
            {
                applyKillEvents();
                if (p->state == TERMINATED)
                    break;

                p->state = RUNNING;
                p->executedTime++;
                p->remainingBurst--;

                sleep(1);
                currentTime++;

                if (p->executedTime == p->ioStart && p->ioDuration > 0)
                {
                    p->state = WAITING;
                    p->ioRemaining = p->ioDuration;
                    enqueue(&waitQueue, p);
                    break;
                }

                if (p->remainingBurst <= 0)
                {
                    p->state = TERMINATED;
                    p->completionTime = currentTime;
                    enqueue(&termQueue, p);
                    break;
                }

                updateWaitQueue();
            }
        }
        else
        {
            sleep(1);
            currentTime++;
        }

        updateWaitQueue();

        if (isEmpty(&readyQueue) && isEmpty(&waitQueue))
            break;
    }
}

int main()
{
    initQueue(&readyQueue);
    initQueue(&waitQueue);
    initQueue(&termQueue);
    initHashmap();

    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        char name[32];
        int pid, burst, ioStart, ioDur;
        scanf("%s %d %d %d %d", name, &pid, &burst, &ioStart, &ioDur);

        PCB *p = createPcb(name, pid, burst, ioStart, ioDur);
        insertPcb(p);
        enqueue(&readyQueue, p);
    }

    printf("Enter number of kill events: ");
    scanf("%d", &killCount);

    for (int i = 0; i < killCount; i++)
    {
        scanf("%d %d", &kills[i].pid, &kills[i].killTime);
    }

    scheduler();

    printf("\nFINAL RESULTS:\n");
    printf("PID\tName\tCPU\tIO\tTurnaround\tWaiting\n");

    QNode *node = termQueue.front;
    while (node != NULL)
    {
        PCB *p = node->pcb;

        int turnaround = p->completionTime;
        int waiting = turnaround - p->executedTime - p->ioDuration;

        printf("%d\t%s\t%d\t%d\t%d\t\t%d\n",
               p->pid, p->name, p->executedTime,
               p->ioDuration, turnaround, waiting);

        node = node->next;
    }

    return 0;
}
