#ifndef DOCIDLIST_H
#define DOCIDLIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int *doc_ids;
  size_t count;
  size_t capacity;
} DocIdList;

DocIdList *DocIdList_create(void);
bool DocIdList_contains(DocIdList *list, int doc_id);
int DocIdList_add(DocIdList *list, int doc_id);
void DocIdList_free(DocIdList *list);
bool DocIdList_is_empty(DocIdList *list);
DocIdList *DocIdList_intersect(DocIdList *a, DocIdList *b);
DocIdList *DocIdList_difference(DocIdList *a, DocIdList *b);
void DocIdList_print(DocIdList *list);

#endif // DOCIDLIST_H
