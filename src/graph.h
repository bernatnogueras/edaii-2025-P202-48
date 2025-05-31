#ifndef GRAPH_H
#define GRAPH_H

#include "docIdList.h"
#include "documents.h"

// Graph en forma de matriu
typedef struct {
  int num_documents;
  int **matr;
  Document **docs;
} Graph;

typedef struct {
  int id;
  int relevance;
} Relevance;

Graph *crear_graph(Document **docs_array, int num_docs);
void omplir_matriu(Graph *g);
Relevance *relevance_score_filtered(Graph *g, DocIdList *result,
                                    int *num_resultats);
void print_relevance(Relevance *top, Document **docs_array, int n);
void select_document(Document **docs_array, Relevance *top, int n);
void free_graph(Graph *g);

#endif