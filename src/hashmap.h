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

// Crea un hashmap amb bucket_count compartiments
HashMap *HashMap_create(size_t bucket_count);

// Allibera el hashmap, però no les claus ni els valors
void HashMap_free(HashMap *map);

// Insereix (clau,valor) al hashmap i clona la clau
int HashMap_put(HashMap *map, const char *key, void *value);

// Retorna el valor associat a la clau o NULL si no existeix
void *HashMap_get(HashMap *map, const char *key);
void *HashMap_get(HashMap *map, const char *key);

#endif
