#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <threads.h>

#define HASHMAP_INITIAL_SIZE 16
#define LOAD_FACTOR 0.75

typedef struct HashNode
{
    char *key;
    void *value;
    struct HashNode *next;
} HashNode;

typedef struct
{
    HashNode **buckets;
    size_t size;
    size_t count;
    mtx_t lock; // C11 mutex
} HashMap;

/* MurmurHash3 32-bit */
static uint32_t murmurhash3(const void *key, int len, uint32_t seed)
{
    uint32_t h = seed;
    const uint32_t c1 = 0xcc9e2d51, c2 = 0x1b873593;
    const int r1 = 15, r2 = 13;
    const uint32_t m = 5, n = 0xe6546b64;
    const uint8_t *data = (const uint8_t *)key;
    int i;

    for (i = 0; i + 4 <= len; i += 4)
    {
        uint32_t k;
        memcpy(&k, data + i, sizeof(uint32_t));
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        h ^= k;
        h = (h << r2) | (h >> (32 - r2));
        h = h * m + n;
    }

    uint32_t k = 0;
    switch (len & 3)
    {
    case 3:
        k ^= (uint32_t)(data[i + 2]) << 16;
        /* fallthrough */
    case 2:
        k ^= (uint32_t)(data[i + 1]) << 8;
        /* fallthrough */
    case 1:
        k ^= (uint32_t)(data[i]);
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        h ^= k;
        break;
    default:
        break;
    }

    h ^= (uint32_t)len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

/* Function prototypes */
HashMap *hashmap_create(size_t size);
void hashmap_put(HashMap *map, const char *key, void *value, size_t value_size);
void *hashmap_get(HashMap *map, const char *key);
void hashmap_resize(HashMap *map);
void hashmap_destroy(HashMap *map);
int benchmark_hashmap(void);

/* Create hashmap */
HashMap *hashmap_create(size_t size)
{
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    if (!map)
        return NULL;
    map->size = size;
    map->count = 0;
    map->buckets = (HashNode **)calloc(size, sizeof(HashNode *));
    if (!map->buckets)
    {
        free(map);
        return NULL;
    }
    if (mtx_init(&map->lock, mtx_plain) != thrd_success)
    {
        free(map->buckets);
        free(map);
        return NULL;
    }
    return map;
}

/* Put value into hashmap */
void hashmap_put(HashMap *map, const char *key, void *value, size_t value_size)
{
    mtx_lock(&map->lock);

    uint32_t hash = (uint32_t)(murmurhash3(key, (int)strlen(key), 0x9747b28c) % map->size);
    HashNode *node = map->buckets[hash];

    while (node)
    {
        if (strcmp(node->key, key) == 0)
        {
            free(node->value);
            node->value = malloc(value_size);
            if (!node->value)
            {
                mtx_unlock(&map->lock);
                return;
            }
            memcpy(node->value, value, value_size);
            mtx_unlock(&map->lock);
            return;
        }
        node = node->next;
    }

    node = (HashNode *)malloc(sizeof(HashNode));
    if (!node)
    {
        mtx_unlock(&map->lock);
        return;
    }

    node->key = (char *)malloc(strlen(key) + 1);
    if (!node->key)
    {
        free(node);
        mtx_unlock(&map->lock);
        return;
    }
    strcpy(node->key, key);
    node->value = malloc(value_size);
    if (!node->value)
    {
        free(node->key);
        free(node);
        mtx_unlock(&map->lock);
        return;
    }

    memcpy(node->value, value, value_size);
    node->next = map->buckets[hash];
    map->buckets[hash] = node;
    map->count++;

    if ((double)map->count / (double)map->size > LOAD_FACTOR)
    {
        hashmap_resize(map);
    }

    mtx_unlock(&map->lock);
}

/* Get value from hashmap */
void *hashmap_get(HashMap *map, const char *key)
{
    mtx_lock(&map->lock);

    uint32_t hash = (uint32_t)(murmurhash3(key, (int)strlen(key), 0x9747b28c) % map->size);
    HashNode *node = map->buckets[hash];

    while (node)
    {
        if (strcmp(node->key, key) == 0)
        {
            mtx_unlock(&map->lock);
            return node->value;
        }
        node = node->next;
    }

    mtx_unlock(&map->lock);
    return NULL;
}

/* Resize hashmap */
void hashmap_resize(HashMap *map)
{
    size_t new_size = map->size * 2;
    HashNode **new_buckets = (HashNode **)calloc(new_size, sizeof(HashNode *));
    if (!new_buckets)
        return;

    for (size_t i = 0; i < map->size; i++)
    {
        HashNode *node = map->buckets[i];
        while (node)
        {
            uint32_t hash = (uint32_t)(murmurhash3(node->key, (int)strlen(node->key), 0x9747b28c) % new_size);
            HashNode *next = node->next;
            node->next = new_buckets[hash];
            new_buckets[hash] = node;
            node = next;
        }
    }

    free(map->buckets);
    map->buckets = new_buckets;
    map->size = new_size;
}

/* Destroy hashmap */
void hashmap_destroy(HashMap *map)
{
    mtx_lock(&map->lock);

    for (size_t i = 0; i < map->size; i++)
    {
        HashNode *node = map->buckets[i];
        while (node)
        {
            HashNode *temp = node;
            node = node->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }

    free(map->buckets);
    mtx_unlock(&map->lock);
    mtx_destroy(&map->lock);
    free(map);
}

int benchmark_hashmap(void)
{
    const size_t NUM_ENTRIES = 1000000;
    HashMap *map = hashmap_create(HASHMAP_INITIAL_SIZE);
    if (!map)
    {
        fprintf(stderr, "Failed to create hashmap\n");
        return EXIT_FAILURE;
    }

    srand((unsigned int)time(NULL));

    clock_t start = clock();
    for (size_t i = 0; i < NUM_ENTRIES; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "key%zu", i);
        int *value = (int *)malloc(sizeof(int));
        if (!value)
        {
            fprintf(stderr, "Memory allocation failed\n");
            hashmap_destroy(map);
            return EXIT_FAILURE;
        }
        *value = (int)i;
        hashmap_put(map, key, value, sizeof(int));
        free(value);
    }
    clock_t end = clock();
    printf("Insertion time: %.4f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (size_t i = 0; i < NUM_ENTRIES; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "key%zu", i);
        int *retrieved = (int *)hashmap_get(map, key);
        if (!retrieved)
        {
            fprintf(stderr, "Key not found: %s\n", key);
        }
    }
    end = clock();
    printf("Lookup time: %.4f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    hashmap_destroy(map);
    return EXIT_SUCCESS;
}

#endif // HASHMAP_H
