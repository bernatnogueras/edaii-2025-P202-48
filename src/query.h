#ifndef QUERY_H
#define QUERY_H

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

Query *Query_init(const char *line);
void Query_free(Query *q);
bool document_matches(const Document *doc, const Query *q);
void query_queue(Query *q);
void query_queue_clear(void);
void searchDocumentLineal(Document **allDocs, int totalDocs, const Query *q);

#endif