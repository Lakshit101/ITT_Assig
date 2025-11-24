#include <stdio.h>
#include <stdlib.h>

#define HASH_TABLE_SIZE 2048
#define MAX_VALUE_LENGTH 64
#define COMMAND_MAX_LEN 32

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

typedef struct QueueNode
{
    int key;
    char value[MAX_VALUE_LENGTH];
    struct QueueNode *prev;
    struct QueueNode *next;
    struct QueueNode *hashNext;
} QueueNode;

typedef struct LruCache
{
    int capacity;
    int count;
    QueueNode *head;
    QueueNode *tail;
    QueueNode *hashTable[HASH_TABLE_SIZE];
} LruCache;

unsigned int getHashIndex(int key)
{
    return (unsigned int)key % HASH_TABLE_SIZE;
}

void detachNode(LruCache *cache, QueueNode *node)
{
    if (node->prev != NULL)
        node->prev->next = node->next;
    else
        cache->head = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;
    else
        cache->tail = node->prev;
}

void insertAtHead(LruCache *cache, QueueNode *node)
{
    node->next = cache->head;
    node->prev = NULL;
    if (cache->head != NULL)
        cache->head->prev = node;
    cache->head = node;
    if (cache->tail == NULL)
        cache->tail = node;
}

void removeFromHashTable(LruCache *cache, QueueNode *targetNode)
{
    unsigned int hashIndex = getHashIndex(targetNode->key);
    QueueNode *current = cache->hashTable[hashIndex];
    QueueNode *prev = NULL;

    while (current != NULL)
    {
        if (current == targetNode)
        {
            if (prev == NULL)
                cache->hashTable[hashIndex] = current->hashNext;
            else
                prev->hashNext = current->hashNext;
            return;
        }
        prev = current;
        current = current->hashNext;
    }
}

void removeLruNode(LruCache *cache)
{
    if (cache->tail == NULL)
        return;

    QueueNode *lruNode = cache->tail;
    detachNode(cache, lruNode);
    removeFromHashTable(cache, lruNode);
    free(lruNode);
    cache->count--;
}

void freeCache(LruCache *cache)
{
    QueueNode *current = cache->head;
    while (current != NULL)
    {
        QueueNode *next = current->next;
        free(current);
        current = next;
    }
    free(cache);
}

LruCache *createCache(int capacity)
{
    if (capacity <= 0)
        return NULL;

    LruCache *newCache = (LruCache *)malloc(sizeof(LruCache));
    if (!newCache)
        exit(1);

    newCache->capacity = capacity;
    newCache->count = 0;
    newCache->head = NULL;
    newCache->tail = NULL;

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
        newCache->hashTable[i] = NULL;

    return newCache;
}

char *get(LruCache *cache, int key)
{
    if (key < 0)
        return NULL;

    unsigned int hashIndex = getHashIndex(key);
    QueueNode *node = cache->hashTable[hashIndex];

    while (node != NULL)
    {
        if (node->key == key)
        {
            detachNode(cache, node);
            insertAtHead(cache, node);
            return node->value;
        }
        node = node->hashNext;
    }
    return NULL;
}

void put(LruCache *cache, int key, char *value)
{
    if (key < 0)
        return;

    unsigned int hashIndex = getHashIndex(key);
    QueueNode *node = cache->hashTable[hashIndex];

    while (node != NULL)
    {
        if (node->key == key)
        {
            stringCopy(node->value, value);
            detachNode(cache, node);
            insertAtHead(cache, node);
            return;
        }
        node = node->hashNext;
    }

    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (!newNode)
        exit(1);

    newNode->key = key;
    stringCopy(newNode->value, value);
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->hashNext = NULL;

    if (cache->count >= cache->capacity)
        removeLruNode(cache);

    insertAtHead(cache, newNode);

    newNode->hashNext = cache->hashTable[hashIndex];
    cache->hashTable[hashIndex] = newNode;

    cache->count++;
}

void initializeProgram()
{
    char command[COMMAND_MAX_LEN];
    int key;
    char value[MAX_VALUE_LENGTH];
    int capacity;
    LruCache *cache = NULL;

    printf("Input:\n");

    while (scanf("%s", command) != EOF)
    {
        if (myStrcmp(command, "createCache") == 0)
        {
            scanf("%d", &capacity);
            if (cache != NULL)
                freeCache(cache);
            cache = createCache(capacity);
        }
        else if (myStrcmp(command, "put") == 0)
        {
            if (cache == NULL)
                continue;

            scanf("%d", &key);
            getchar();
            scanf("%63[^\n]", value);

            if (key < 0)
            {
                printf("Invalid key\n");
                continue;
            }

            put(cache, key, value);
        }
        else if (myStrcmp(command, "get") == 0)
        {
            if (cache == NULL)
                continue;

            scanf("%d", &key);

            if (key < 0)
            {
                printf("NULL\n");
                continue;
            }

            char *result = get(cache, key);

            if (result != NULL)
                printf("%s\n", result);
            else
                printf("NULL\n");
        }
        else if (myStrcmp(command, "exit") == 0)
        {
            break;
        }
        else
        {
            printf("Please enter a valid command\n");
        }
    }

    if (cache != NULL)
        freeCache(cache);
}

int main()
{
    initializeProgram();
    return 0;
}
