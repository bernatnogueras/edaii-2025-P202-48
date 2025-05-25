#include "query.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Creem una funció que crea i inicialitza una estructura del tipus query
Query *Query_init(const char *line) {
  Query *q = malloc(sizeof(Query)); // Reservem memòria dinàmica
  if (!q)
    return NULL;
  q->head = NULL; // Inicialitzem l'inici i el contador
  q->count = 0;

  // Fem una còpia de la línia introduïda per a poder-la modificar
  char *copy = malloc(strlen(line) + 1);
  if (!copy) {
    free(q);
    return NULL;
  }
  strcpy(copy, line);

  char *p = copy;
  QueryNode **pp = &q->head;

  while (*p) {
    // Saltem els espais, salts de línia i tabuladors
    while (*p && (*p == ' ' || *p == '\t' || *p == '\n')) {
      p++;
    }
    if (!*p)
      break;

    // Detectem si és una paraula exclosa
    bool exclude = false;
    if (*p == '-') {
      exclude = true;
      p++; // Saltem el guió '-'
    }

    // Detecta on comença i acaba la paraula i la seva mida
    char *start = p;
    while (*p && *p != ' ' && *p != '\t' && *p != '\n') {
      p++;
    }
    size_t len = p - start;

    // Copiem la parula a la nova cadena
    char *word = malloc(len + 1);
    if (!word) {
      // Si dona error, alliberem memòria i sortim
      Query_free(q);
      free(copy);
      return NULL;
    }
    strncpy(word, start, len);
    word[len] = '\0';

    // Creem un QueryNode per guardar la paraula i l'enllaça a la llista
    QueryNode *node = malloc(sizeof(QueryNode));
    if (!node) {
      free(word);
      Query_free(q);
      free(copy);
      return NULL;
    }
    node->keyword = word;
    node->exclude = exclude;
    node->next = NULL;
    *pp = node;
    pp = &node->next;
    q->count++;
  }

  free(copy);
  return q;
}

// Fucnió que allibera tota la memòria d'una estrcutura Query
void Query_free(Query *q) {
  QueryNode *curr = q->head;
  while (curr) {
    QueryNode *tmp = curr->next;
    free(curr->keyword);
    free(curr);
    curr = tmp;
  }
  free(q);
}

// Funció que retorna true si conté les paraules obligatòries i no conté cap
// paraula exclosa
bool document_matches(const Document *doc, const Query *q) {
  if (!q || q->count == 0)
    return false;
  for (QueryNode *n = q->head; n; n = n->next) {
    bool actual =
        strstr(doc->body, n->keyword) || strstr(doc->title, n->keyword);

    // Si hi ha una paraula que ha d'estar exclosa, retornem fals
    if (n->exclude) {
      if (actual)
        return false;
    }
    // Si no apareix una paraula obligatòria, retornem fals
    else {
      if (!actual)
        return false;
    }
  }
  // Si no és una paraula exclosa i és obligatòria i apareix, retornem true
  return true;
}

// Inicialitzem la cua a NULL
static Query *queue[3] = {NULL, NULL, NULL};
static int count = 0;
// Show the last 3 queries using a queue
void query_queue(Query *q) {
  // Creem un bucle per desplaçar tots els elements de la cua una posició
  // endarrera
  if (count == 3) {
    Query_free(queue[2]);
    count = 2;
  }
  for (int i = count; i > 0; --i) {
    queue[i] = queue[i - 1];
  }
  // Introduïm el nou element al primer element de la cua, ja que és el més
  // recent
  queue[0] = q;
  count++; // Incrementem el contador

  printf("L'historial de les últimes consultes és:\n");
  for (int i = 0; i < count; ++i) {
    printf("·Query %d: ", i + 1);
    for (QueryNode *n = queue[i]->head; n; n = n->next) {
      if (n->exclude) {
        printf("-%s ", n->keyword);
      } else {
        printf("%s ", n->keyword);
      }
    }
    printf("\n");
  }
  printf("\n");
}

void query_queue_clear(void) {
  for (int i = 0; i < count; ++i) {
    Query_free(queue[i]);
    queue[i] = NULL;
  }
  count = 0;
}

void searchDocumentLineal(Document **allDocs, int totalDocs, const Query *q) {
  for (int i = 0; i < totalDocs; ++i) {
    if (document_matches(allDocs[i], q)) {
      printf("\"%s\" SÍ coincideix\n", allDocs[i]->title);
    } else {
      printf("\"%s\" NO coincideix\n", allDocs[i]->title);
    }
  }
}