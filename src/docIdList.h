#ifndef DOCIDLIST_H
#define DOCIDLIST_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  int *doc_ids;
  size_t count;
  size_t capacity;
} DocIdList;

DocIdList *DocIdList_create(void);
void DocIdList_free(DocIdList *list);
bool DocIdList_contains(DocIdList *list, int doc_id);
int DocIdList_add(DocIdList *list, int doc_id);

#endif // DOCIDLIST_H
