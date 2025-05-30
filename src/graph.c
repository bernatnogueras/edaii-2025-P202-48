#include <stdlib.h>
#include <string.h>
#include "graph.h"

Graph *crear_graph(Document **docs_array, int num_docs) {
  Graph *g = (Graph *)malloc(sizeof(Graph));
  g->num_documents = num_docs;
  g->docs = (Document **)malloc(num_docs * sizeof(Document*));

  // Copiem cada punter
  for (int i = 0; i < num_docs; i++) {
    g->docs[i] = docs_array[i];
  }
  g->matr = (int **)malloc(num_docs * sizeof(int *));

  // Inicialitzem matriu
  for (int i = 0; i < num_docs; i++) {
    g->matr[i] = (int *)malloc(num_docs * sizeof(int));

    // Posem matriu a 0
    for (int j = 0; j < num_docs; j++) {
      g->matr[i][j] = 0;
    }
  }
  return g;
}


static int buscar_index(Graph *g, int doc_id) {
  for (int i = 0; i < g->num_documents; i++) {
    if (g->docs[i]->id == doc_id) {
      return i;
    }
  }
  return -1;
}


void omplir_matriu(Graph *g) {
  for (int i = 0; i < g->num_documents; i++) {
    Document *doc = g->docs[i];
    Links *link = doc->links;
    for (int k = 0; k < link->count; k++) {
      int id = link->ids[k];
      int j = buscar_index(g, id);
      if (j >= 0) {
        g->matr[i][j] = 1;
      }
    }
  }
}


int *relevance_score(Graph *g) {
  int n = g->num_documents;
  int *link_count = malloc(n * sizeof(int));

  // Sumar els 1
  for (int i = 0; i < n; i++) {
    int sum = 0;
    for (int j = 0; j < n; j++) {
      sum += g->matr[i][j];
    }
    link_count[i] = sum;
  }
  return link_count;
}


void free_graph(Graph *g) {
  for (int i = 0; i < g->num_documents; i++) {
    free(g->matr[i]);
  }
  free(g->matr);
  free(g->docs);
  free(g);
}