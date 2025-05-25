#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  int *doc_ids;    // Array  IDs
  size_t count;    // Número de IDs guardats
  size_t capacity; // Capacitat actual array
} DocIdList;

// crear llista buida
DocIdList *DocIdList_create(void) {
  DocIdList *list = malloc(sizeof(DocIdList));
  if (!list)
    return NULL;
  list->capacity = 4;
  list->count = 0;
  list->doc_ids = malloc(sizeof(int) * list->capacity);
  if (!list->doc_ids) {
    free(list);
    return NULL;
  }
  return list;
}

// vuidar la llista
void DocIdList_free(DocIdList *list) {
  if (!list)
    return;
  free(list->doc_ids);
  free(list);
}

// Verificar si la lista ya conte un doc_id
bool DocIdList_contains(DocIdList *list, int doc_id) {
  for (size_t i = 0; i < list->count; ++i) {
    if (list->doc_ids[i] == doc_id)
      return true;
  }
  return false;
}

// Añadir un doc_id a la lista
int DocIdList_add(DocIdList *list, int doc_id) {
  if (DocIdList_contains(list, doc_id)) {
    return 1; // Ya estaba, no lo añade de nuevo
  }
  if (list->count == list->capacity) {
    size_t new_capacity = list->capacity * 2;
    int *new_arr = realloc(list->doc_ids, sizeof(int) * new_capacity);
    if (!new_arr)
      return 0; // realloc ha fallat
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
