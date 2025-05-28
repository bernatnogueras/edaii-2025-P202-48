#ifndef HASHMAP_H
#define HASHMAP_H
#include "docIdList.h"
#include <stddef.h>

// Cadena d'elements a un hashmap de valors genèrics (void*)
typedef struct HashNode {
  char *key;
  void *value;
  struct HashNode *next;
} HashNode;

typedef struct HashMap {
  size_t bucket_count;
  HashNode **buckets;
} HashMap;

int hash_fn(const char *s);
HashMap *HashMap_create(size_t bucket_count);
int HashMap_put(HashMap *map, const char *key, void *value);
void *HashMap_get(HashMap *map, const char *key);
void normalize_word(char *word);
void add_words_to_reverse_index(HashMap *reverseIndex, void **docs,
                                int totalDocs);
void HashMap_free(HashMap *map, void (*free_value)(void *));

#endif
