#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


// Hash function for strings
static uint64_t hash_fn(const char *s) {
  uint64_t h = 5381;
  unsigned char c;
  while ((c = (unsigned char)*s++)) {
    h = ((h << 5) + h) + c;
  }
  return h;
}


HashMap *HashMap_create(size_t bucket_count) {
  // Allocate map structure
  HashMap *map = malloc(sizeof *map);
  if (!map) return NULL;
  map->bucket_count = bucket_count;
  // Allocate buckets array
  map->buckets = calloc(bucket_count, sizeof *map->buckets);
  if (!map->buckets) {
    free(map);
    return NULL;
  }
  return map;
}


void HashMap_free(HashMap *map) {
  if (!map) return;
  for (size_t i = 0; i < map->bucket_count; ++i) {
    HashNode *node = map->buckets[i];
    while (node) {
      HashNode *tmp = node->next;
      free(node->key);
      // node->value is managed externally
      free(node);
      node = tmp;
    }
  }
  free(map->buckets);
  free(map);
}


int HashMap_put(HashMap *map, const char *key, void *value) {
  uint64_t i = hash_fn(key) % map->bucket_count;
  HashNode *node = map->buckets[i];
  // Search existing node
  while (node) {
    if (strcmp(node->key, key) == 0) {
      node->value = value; // replace value
      return 1;
    }
    node = node->next;
  }
  // create new node
  node = malloc(sizeof *node);
  if (!node) return 0;
  node->key = strdup(key);
  node->value = value;
  node->next = map->buckets[i];
  map->buckets[i] = node;
  return 1;
}


void *HashMap_get(HashMap *map, const char *key) {
  uint64_t i = hash_fn(key) % map->bucket_count;
  HashNode *node = map->buckets[i];
  while (node) {
    if (strcmp(node->key, key) == 0) {
      return node->value;
    }
    node = node->next;
  }
  return NULL;
}