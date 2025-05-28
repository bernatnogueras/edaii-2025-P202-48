#include "hashmap.h"
#include "docIdList.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funció que s'encarrega de transformar cada paraula en un índex dins del mapa
int hash_fn(const char *s) {
  int h = 10000;
  unsigned char c;
  while ((c = (unsigned char)*s++)) {
    h = ((h << 5) + h) + c;
  }
  return h;
}

// Funció que inicialitza un Hashmap amb un nombre definit de
// caselles/compartiments
HashMap *HashMap_create(size_t bucket_count) {
  // Reserva memòria per l'estructura del mapa
  HashMap *map = malloc(sizeof *map);
  if (!map)
    return NULL;
  map->bucket_count = bucket_count;
  // Reserva memòria per l'array de compartiments
  map->buckets = calloc(bucket_count, sizeof *map->buckets);
  if (!map->buckets) {
    free(map);
    return NULL;
  }
  return map;
}

// Funció que insereix una paraula (key) i la seva llista de documents (value) i
// clona la clau
int HashMap_put(HashMap *map, const char *key, void *value) {
  int i = hash_fn(key) % map->bucket_count;
  HashNode *node = map->buckets[i];
  // Cerca d'un node existent
  while (node) {
    if (strcmp(node->key, key) == 0) {
      node->value = value;
      return 1;
    }
    node = node->next;
  }
  // Creem un nou node
  node = malloc(sizeof *node);
  if (!node)
    return 0;
  node->key = strdup(key);
  node->value = value;
  node->next = map->buckets[i];
  map->buckets[i] = node;
  return 1;
}

// Retorna el valor associat a la clau o NULL si no existeix
void *HashMap_get(HashMap *map, const char *key) {
  int i = hash_fn(key) % map->bucket_count;
  HashNode *node = map->buckets[i];
  while (node) {
    if (strcmp(node->key, key) == 0) {
      return node->value;
    }
    node = node->next;
  }
  return NULL;
}

// Funció que transforma la cadena a minúscules
void normalize_word(char *word) {
  int len = strlen(word);
  int idx = 0;
  for (int i = 0; i < len; i++) {
    if (isalpha((unsigned char)
                    word[i])) { // Comprova que sigui una lletra alfabètica
                                // (tant majúscula com minúscula)
      word[idx++] =
          tolower((unsigned char)word[i]); // Convertim la lletra a minúscula
    }
  }
  word[idx] = '\0';
}

// Funció que insereix totes les paraules de tots els documents al Hashmap
// Utilitzem void **docs perquè docs és un array de punters a qualsevol tipus de
// dades
void add_words_to_reverse_index(HashMap *reverseIndex, void **docs,
                                int totalDocs) {
  for (int i = 0; i < totalDocs; i++) {
    typedef struct Document {
      int id;
      char *title;
      char *body;
      void *links;
    } Document;

    Document *doc = (Document *)docs[i];
    char *text = doc->body;

    char *copy = strdup(text); // Dupliquem la cadena de caràcters
    if (!copy)
      continue;

    char *token = strtok(
        copy,
        " \t\n\r.,;:!?\"()[]{}<>"); // Retornem aquella cadena sense aquests
                                    // caràcters especials (els "eliminem")

    while (token != NULL) {
      normalize_word(token);
      if (strlen(token) > 0) {
        DocIdList *list = (DocIdList *)HashMap_get(reverseIndex, token);

        if (!list) {
          list = DocIdList_create();
          if (!list) {
            free(copy);
            return;
          }
          HashMap_put(reverseIndex, token, list);
        }

        DocIdList_add(list, doc->id);
      }
      token = strtok(NULL, " \t\n\r.,;:!?\"()[]{}<>");
    }
    free(copy);
  }
}

// Funció que allibera la memòria del hashmap
// Allibera la clau amb el node->key
// Allibera el valor amb free_value(node->value)
void HashMap_free(HashMap *map, void (*free_value)(void *)) {
  if (!map)
    return;
  for (size_t i = 0; i < map->bucket_count; ++i) {
    HashNode *node = map->buckets[i];
    while (node) {
      HashNode *tmp = node->next;
      free(node->key);
      if (free_value) {
        free_value(node->value);
      }
      free(node);
      node = tmp;
    }
  }
  free(map->buckets);
  free(map);
}