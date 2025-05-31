#include "graph.h"
#include "docIdList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funció que crea un graf
Graph *crear_graph(Document **allDocs, int num_docs) {
  Graph *g = (Graph *)malloc(sizeof(Graph));
  g->num_documents = num_docs;
  g->docs = (Document **)malloc(num_docs * sizeof(Document *));

  // Copiem cada punter
  for (int i = 0; i < num_docs; i++) {
    g->docs[i] = allDocs[i];
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

// Funció que ens permet trobar la posició d'un document
static int buscar_index(Graph *g, int doc_id) {
  for (int i = 0; i < g->num_documents; i++) {
    if (g->docs[i]->id == doc_id) {
      return i;
    }
  }
  return -1;
}

// Funció que omple la matriu si té connexió
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

// Funció que calcula la rellevància de cada document
Relevance *relevance_score_filtered(Graph *g, DocIdList *result,
                                    int *num_resultats) {
  int n = result->count;
  Relevance *r = malloc(n * sizeof(Relevance));
  if (!r) {
    return NULL;
  }

  // Sumar els 1
  for (int i = 0; i < n; i++) {
    int sum = 0;
    int doc_id = result->doc_ids[i];
    int idx = buscar_index(g, doc_id);
    if (idx < 0) {
      r[i].id = doc_id;
      r[i].relevance = 0;
      continue;
    }
    for (int j = 0; j < g->num_documents; j++) {
      if (g->matr[j][idx] == 1) {
        sum++;
      }
    }
    r[i].id = doc_id;
    r[i].relevance = sum;
  }
  // Ara ordenem els 5 millors
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (r[j].relevance > r[i].relevance) {
        Relevance temp = r[i];
        r[i] = r[j];
        r[j] = temp;
      }
    }
  }
  if (n > 5) {
    n = 5;
  }

  *num_resultats = n;
  return r;
}

// Funció que ens imprimeix les 5 millors rellevàncies
void print_relevance(Relevance *top, Document **allDocs, int n) {
  int total = 0;
  printf("\n");
  for (int i = 0; i < n; ++i) {
    int id_doc = top[i].id;
    // Si hi ha menys de 5 elements rellevants, n'imprimim menys
    if (top[i].relevance == 0) {
      break;
    }

    printf("(%d) %s\n", i, allDocs[id_doc]->title);
    printf("----\n");
    char text[151];
    strncpy(text, allDocs[id_doc]->body, 150);
    text[150] = '\0'; // Assignem el caràcter NULL a la posició 150 perquè sinó
                      // dona error del valgrind
    printf("%s\n", text);
    printf("----\n");
    printf("Relevance score: %d\n", top[i].relevance);
    printf("\n");
    total++;
  }
  printf("[%d results]\n", total);
}

// Funció que imprimeix el document seleccionat per l'usuari
void select_document(Document **allDocs, Relevance *top, int n) {
  int c;
  printf("\033[1;34mSelecciona un document (0-%d):\033[0m ", n - 1);
  c = getchar();

  if (c == '\n') {
    return;
  }
  int num = c - '0';
  int buf;
  // Netegem el buffer d'entrada descartant tots els caràcters fins arribar a
  // un salt de línia o final de fitxer
  while ((buf = getchar()) != '\n' && buf != EOF)
    ;

  if (num < 0 || num >= n - 1) {
    printf("\033[1;31m\tAquest document no és rellevant!\033[0m\n");
    return;
  }

  int doc_id = top[num].id;

  printf("\n\033[0;32mID\033[0m\n%d\n", allDocs[doc_id]->id);
  printf("\033[0;32mTITLE\033[0m\n%s\n", allDocs[doc_id]->title);
  printf("\033[0;32mRELEVANCE SCORE\033[0m\n%d\n", top[num].relevance);
  printf("\033[0;32mBODY\033[0m\n%s\n", allDocs[doc_id]->body);
}

// Funció que allibera tota la memòria del graf
void free_graph(Graph *g) {
  for (int i = 0; i < g->num_documents; i++) {
    free(g->matr[i]);
  }
  free(g->matr);
  free(g->docs);
  free(g);
}