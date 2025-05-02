#ifndef QUERY_H
#define QUERY_H

#include <stdbool.h>
#include "documents.h"


typedef struct QueryNode {
    char *keyword;
    struct QueryNode *next;
} QueryNode;


typedef struct {
    QueryNode *head;
    int count;
} Query;


Query *Query_init(const char *line);
void Query_free(Query *q);
bool document_matches(const Document *doc, const Query *q);

#endif