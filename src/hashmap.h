#ifndef HASHMAP_H
#define HASHMAP_H
#include <stddef.h>
#include "docIdList.h"


// Chains to generic values map (void*)
typedef struct HashNode {
  char *key;
  void *value;
  struct HashNode *next;
} HashNode;
    
typedef struct HashMap {
  size_t bucket_count;
  HashNode **buckets;
} HashMap;

// Create a hashmap with bucket_count buckets
HashMap *HashMap_create(size_t bucket_count);
// Free hashmap, no keys/values
void HashMap_free(HashMap *map);

// Insert (key, value) in the map (chaining). Clones key.
int HashMap_put(HashMap *map, const char *key, void *value);
// Returns value associated to key, or NULL if it doesn't exist
void *HashMap_get(HashMap *map, const char *key);
void *HashMap_get(HashMap *map, const char *key);


#endif
