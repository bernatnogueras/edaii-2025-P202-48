#ifndef GRAPH_H
#define GRAPH_H

#include "documents.h"

// Graph en forma de matriu
typedef struct {
  int num_documents;
  int **matr;
  Document **docs;
} Graph;


Graph *crear_graph(Document **docs_array, int num_docs);
void omplir_matriu(Graph *g);
int *relevance_score(Graph *g);
void free_graph(Graph *g);


#endif