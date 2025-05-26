#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int *doc_ids;    // Array  IDs
  size_t count;    // Número de IDs guardats
  size_t capacity; // Capacitat actual array
} DocIdList;

// Creem una llista buida
DocIdList *DocIdList_create(void) {
  DocIdList *list = malloc(sizeof(DocIdList));
  if (!list) {
    return NULL;
  }
  list->capacity = 4;
  list->count = 0;
  list->doc_ids = malloc(sizeof(int) * list->capacity);
  if (!list->doc_ids) {
    free(list);
    return NULL;
  }
  return list;
}

// Verifiquem si la llista ja conté un doc_id
bool DocIdList_contains(DocIdList *list, int doc_id) {
  for (size_t i = 0; i < list->count; ++i) {
    if (list->doc_ids[i] == doc_id)
      return true;
  }
  return false;
}

// Afegim un doc_id a la lista
int DocIdList_add(DocIdList *list, int doc_id) {
  if (DocIdList_contains(list, doc_id)) {
    return 1; // Ya estaba, no lo añade de nuevo
  }
  if (list->count == list->capacity) {
    size_t new_capacity = list->capacity * 2;
    int *new_arr = realloc(list->doc_ids, sizeof(int) * new_capacity);
    if (!new_arr) {
      return 0; // realloc ha fallat
    }
    // Si realloc ha fallat, no esborrem l'array original
    // i no actualitzem la capacitat
    // Si realloc ha anat bé, actualitzem la capacitat
    // i assignem el nou array
    list->doc_ids = new_arr;
    list->capacity = new_capacity;
  }
  list->doc_ids[list->count++] = doc_id;
  return 1;
}

bool DocIdList_is_empty(DocIdList *list) { return list->count == 0; }

// Funció que ens permet trobar els elements comuns a dues llistes
DocIdList *DocIdList_intersect(DocIdList *a, DocIdList *b) {
  DocIdList *result = DocIdList_create();
  if (!result) {
    return NULL;
  }
  for (size_t i = 0; i < a->count; ++i) {
    if (DocIdList_contains(b, a->doc_ids[i])) {
      DocIdList_add(result, a->doc_ids[i]);
    }
  }
  return result;
}

// Funció que retorna una nova llista amb els doc_ids de la 'a' que no estan a
// 'b'
DocIdList *DocIdList_difference(DocIdList *a, DocIdList *b) {
  DocIdList *result = DocIdList_create();
  if (!result) {
    return NULL;
  }
  for (size_t i = 0; i < a->count; ++i) {
    if (!DocIdList_contains(b, a->doc_ids[i])) {
      DocIdList_add(result, a->doc_ids[i]);
    }
  }
  return result;
}

void DocIdList_print(DocIdList *list) {
  if (!list || list->count == 0) {
    printf("No s'ha trobat cap document\n");
    return;
  }
  printf("\t");
  for (size_t i = 0; i < list->count; ++i) {
    printf("%d", list->doc_ids[i]);
    if (i < list->count - 1) {
      printf(", ");
    }
  }
  printf("\n");
}

// Buidem la llista
void DocIdList_free(DocIdList *list) {
  if (!list)
    return;
  free(list->doc_ids);
  free(list);
}