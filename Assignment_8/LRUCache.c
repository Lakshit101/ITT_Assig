#include <stdio.h>
#include <stdlib.h>

#define MAX_KEY 1000

typedef struct Node
{
    int key;
    char data[50];
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct LRUCache
{
    int capacity;
    int size;
    Node *head;
    Node *tail;
    Node *map[MAX_KEY];
} LRUCache;

char *stringCopy(char *str1, char *str2)
{
    char *start = str1;
    while (*str2 != '\0')
    {
        *str1 = *str2;
        str1++;
        str2++;
    }
    *str1 = '\0';
    return start;
}

int toLowerCase(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + 32;
    return c;
}

int myStrcmp(char *s1, char *s2)
{
    while (*s1 != '\0' && *s2 != '\0')
    {
        char c1 = toLowerCase(*s1);
        char c2 = toLowerCase(*s2);

        if (c1 != c2)
            return c1 - c2;

        s1++;
        s2++;
    }
    return toLowerCase(*s1) - toLowerCase(*s2);
}

Node *createNode(int key, char *value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    stringCopy(node->data, value);
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void removeNode(LRUCache *cache, Node *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        cache->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        cache->tail = node->prev;
}

void addNodeAtHead(LRUCache *cache, Node *node)
{
    node->prev = NULL;
    node->next = cache->head;

    if (cache->head)
        cache->head->prev = node;

    cache->head = node;

    if (cache->tail == NULL)
        cache->tail = node;
}

void moveToHead(LRUCache *cache, Node *node)
{
    if (cache->head == node)
        return;

    removeNode(cache, node);
    addNodeAtHead(cache, node);
}

char *get(LRUCache *cache, int key)
{
    if (!cache)
        return NULL;

    if (key < 0 || key >= MAX_KEY)
        return NULL;

    Node *node = cache->map[key];
    if (node == NULL)
        return NULL;

    moveToHead(cache, node);
    return node->data;
}

LRUCache *createCache(int capacity)
{
    LRUCache *cache = (LRUCache *)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;

    for (int i = 0; i < MAX_KEY; i++)
        cache->map[i] = NULL;

    return cache;
}

void put(LRUCache *cache, int key, char *value)
{
    if (!cache)
        return;

    Node *node = cache->map[key];

    if (node != NULL)
    {
        stringCopy(node->data, value);
        moveToHead(cache, node);
        return;
    }

    Node *newNode = createNode(key, value);
    addNodeAtHead(cache, newNode);
    cache->map[key] = newNode;
    cache->size++;

    if (cache->size > cache->capacity)
    {
        Node *remove = cache->tail;
        cache->map[remove->key] = NULL;
        removeNode(cache, remove);
        free(remove);
        cache->size--;
    }
}

void handleCreateCache(LRUCache **cache)
{
    int size;
    if (scanf("%d", &size) != 1 || size < 1 || size > 1000)
    {
        printf("Invalid cache size. Must be 1-1000.\n");
        return;
    }
    *cache = createCache(size);
    printf("Cache of size %d created.\n", size);
}


void initializeProgram(){
   LRUCache *cache = NULL;

    int key;
    char command[20];
    char value[50];

    while (1)
    {
        printf("Enter Command:\n");
        scanf("%s", command);

        if (myStrcmp(command, "createcache") == 0)
        {
            handleCreateCache(&cache);
        }
        else if (myStrcmp(command, "put") == 0)
        {
            if (!cache)
            {
                printf("Error: Cache not created.\n");
                continue;
            }
            scanf("%d %s", &key, value);
            put(cache, key, value);
        }
        else if (myStrcmp(command, "get") == 0)
        {
            if (!cache)
            {
                printf("Error: Cache not created.\n");
                continue;
            }
            scanf("%d", &key);
            char *res = get(cache, key);
            if (res == NULL)
                printf("NULL\n");
            else
                printf("%s\n", res);
        }
        else if (myStrcmp(command, "exit") == 0)
        {
            return ;
        }
        else
        {
            printf("Invalid Command\n");
        }
    }
}

int main()
{
    initializeProgram();
    return 0;
}
