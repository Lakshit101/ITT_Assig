#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum { READY, RUNNING, WAITING, TERMINATED } State;

typedef struct PCB {
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

PCB* createPcb(char *name, int pid, int burst, int ioStart, int ioDur) {
    PCB *p = (PCB*)malloc(sizeof(PCB));
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

typedef struct QNode {
    PCB *pcb;
    struct QNode *next;
} QNode;

typedef struct Queue {
    QNode *front;
    QNode *rear;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = NULL;
}

void enqueue(Queue *q, PCB *p) {
    QNode *temp = (QNode*)malloc(sizeof(QNode));
    temp->pcb = p;
    temp->next = NULL;
    if (q->rear == NULL)
        q->front = q->rear = temp;
    else {
        q->rear->next = temp;
        q->rear = temp;
    }
}

PCB* dequeue(Queue *q) {
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

int isEmpty(Queue *q) {
    return q->front == NULL;
}

#define TABLE_SIZE 1009

typedef struct {
    int used;
    int pid;
    PCB *pcb;
} HashEntry;

HashEntry table[TABLE_SIZE];

void initHashmap() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i].used = 0;
        table[i].pid = -1;
        table[i].pcb = NULL;
    }
}

int hashValue(int pid) {
    return pid % TABLE_SIZE;
}

void insertPcb(PCB *p) {
    int idx = hashValue(p->pid);
    while (table[idx].used)
        idx = (idx + 1) % TABLE_SIZE;
    table[idx].used = 1;
    table[idx].pid = p->pid;
    table[idx].pcb = p;
}

PCB* getPcb(int pid) {
    int idx = hashValue(pid);
    while (table[idx].used) {
        if (table[idx].pid == pid)
            return table[idx].pcb;
        idx = (idx + 1) % TABLE_SIZE;
    }
    return NULL;
}

void deletePcb(int pid) {
    int idx = hashValue(pid);
    while (table[idx].used) {
        if (table[idx].pid == pid) {
            table[idx].used = 0;
            table[idx].pid = -1;
            table[idx].pcb = NULL;
            return;
        }
        idx = (idx + 1) % TABLE_SIZE;
    }
}

PCB *finished[200];
int finishCount = 0;

typedef struct {
    int pid;
    int killTime;
} KillEvent;

KillEvent kills[100];
int killCount = 0;

int currentTime = 0;

Queue readyQueue, waitQueue, termQueue;

void updateWaitQueue() {
    QNode *node = waitQueue.front;
    QNode *prev = NULL;
    while (node) {
        PCB *p = node->pcb;
        p->ioRemaining--;
        if (p->ioRemaining <= 0) {
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

void applyKillEvents() {
    for (int i = 0; i < killCount; i++) {
        if (kills[i].killTime == currentTime) {
            PCB *p = getPcb(kills[i].pid);
            if (p && p->state != TERMINATED) {
                p->state = TERMINATED;
                p->completionTime = currentTime;
                finished[finishCount++] = p;
                deletePcb(p->pid);
            }
        }
    }
}

void scheduler() {
    while (1) {
        applyKillEvents();
        if (!isEmpty(&readyQueue)) {
            PCB *p = dequeue(&readyQueue);
            if (p->state == TERMINATED)
                continue;
            while (1) {
                applyKillEvents();
                if (p->state == TERMINATED)
                    break;
                p->state = RUNNING;
                p->executedTime++;
                p->remainingBurst--;
                sleep(1);
                currentTime++;
                if (p->executedTime == p->ioStart && p->ioDuration > 0) {
                    p->state = WAITING;
                    p->ioRemaining = p->ioDuration;
                    enqueue(&waitQueue, p);
                    break;
                }
                if (p->remainingBurst <= 0) {
                    p->state = TERMINATED;
                    p->completionTime = currentTime;
                    finished[finishCount++] = p;
                    break;
                }
                updateWaitQueue();
            }
        } else {
            sleep(1);
            currentTime++;
        }
        updateWaitQueue();
        if (isEmpty(&readyQueue) && isEmpty(&waitQueue))
            break;
    }
}

void loadProcesses() {
    int n;
    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("  Number of processes must be a positive integer.\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        char name[32];
        int pid, burst, ioStart, ioDur;

        if (scanf("%s %d %d %d %d", name, &pid, &burst, &ioStart, &ioDur) != 5) {
            printf("ERROR: Invalid process format. Expected: name pid burst ioStart ioDuration\n");
            exit(1);
        }

        if (pid < 0 || burst <= 0 || ioStart < 0 || ioDur < 0) {
            printf(" Process values must be non-negative integers.\n");
            exit(1);
        }

        if (ioStart > burst) {
            printf("  IO start cannot be greater than burst time.\n");
            exit(1);
        }

        PCB *p = createPcb(name, pid, burst, ioStart, ioDur);
        insertPcb(p);
        enqueue(&readyQueue, p);
    }
}

void loadKillEvents() {
    printf("Enter number of kill events: ");
    if (scanf("%d", &killCount) != 1 || killCount < 0) {
        printf("ERROR: Kill count must be a non-negative integer.\n");
        exit(1);
    }

    for (int i = 0; i < killCount; i++) {
        if (scanf("%d %d", &kills[i].pid, &kills[i].killTime) != 2) {
            printf("ERROR: Invalid kill event format. Expected: pid killTime\n");
            exit(1);
        }
        if (kills[i].pid < 0 || kills[i].killTime < 0) {
            printf("ERROR: PID and killTime must be non-negative integers.\n");
            exit(1);
        }
    }
}

void printResults() {
    for (int i = 0; i < finishCount - 1; i++) {
        for (int j = i + 1; j < finishCount; j++) {
            if (finished[i]->pid > finished[j]->pid) {
                PCB *tmp = finished[i];
                finished[i] = finished[j];
                finished[j] = tmp;
            }
        }
    }

    printf("\nFINAL RESULTS (Sorted by PID):\n");
    printf("PID\tName\tCPU\tIO\tTurnaround\tWaiting\n");

    for (int i = 0; i < finishCount; i++) {
        PCB *p = finished[i];
        int turnaround = p->completionTime;
        int waiting = turnaround - p->burstTime;

        printf("%d\t%s\t%d\t%d\t%d\t\t%d\n",
               p->pid, p->name, p->executedTime,
               p->ioDuration, turnaround, waiting);
    }
}

int main() {
    initQueue(&readyQueue);
    initQueue(&waitQueue);
    initQueue(&termQueue);
    initHashmap();

    loadProcesses();
    loadKillEvents();

    scheduler();
    printResults();

    return 0;
}
