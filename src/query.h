#ifndef QUERY_H
#define QUERY_H

#include "docIdList.h"
#include "documents.h"
#include <stdbool.h>

typedef struct QueryNode {
  char *keyword;
  bool exclude; // Ens indicarà si la paraula s'ha d'excloure
  struct QueryNode *next;
} QueryNode;

typedef struct {
  QueryNode *head;
  int count;
} Query;

// char *normalize(const char *word);
Query *Query_init(const char *line);
bool document_matches(const Document *doc, const Query *q);
DocIdList *searchDocumentLineal(Document **allDocs, int totalDocs,
                                const Query *q);
void query_queue(Query *q);
void query_queue_clear(void);
void Query_free(Query *q);

#endif