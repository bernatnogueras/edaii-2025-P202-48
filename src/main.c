#include "docIdList.h"
#include "documents.h"
#include "hashmap.h"
#include "query.h"
#include "sample_lib.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main() {
  Document **allDocs = loadAllDocuments();
  if (allDocs == NULL) {
    fprintf(stderr, "Error carregant documents\n");
    return 1;
  }

  const int totalDocs = 13;
  char input[1024];

  /*
  /////////// BÚSQUEDA LINEAL (versió lenta) ///////////

  while (1) {
    printf("Search (lineal): ");
    if (!fgets(input, sizeof(input), stdin)) {
      fprintf(stderr, "Error llegint l'entrada\n");
      break;
    }

    if (input[0] == '\n') {
      break;
    }

    int len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
      input[len - 1] = '\0';
      --len;
    }

    // Inicialitzem la Query
    Query *q = Query_init(input);
    if (!q) {
      fprintf(stderr, "Error inicialitzant la query\n");
      break;
    }

    searchDocumentLineal(allDocs, totalDocs, q);
    printf("\n");
    query_queue(q);
  }

  /////////// ACABA VERSIÓ LINEAL ///////////
  */

  /////////// HASHMAP (versió ràpida) ///////////
  ///*
  HashMap *reverseIndex = HashMap_create(10000);
  if (!reverseIndex) {
    fprintf(stderr, "Error creant el hashmap\n");
    freeAllDocuments(allDocs, totalDocs);
    return 1;
  }
  add_words_to_reverse_index(reverseIndex, (void **)allDocs, totalDocs);

  while (1) {
    printf("Search (hashmap): ");
    if (!fgets(input, sizeof(input), stdin)) {
      fprintf(stderr, "Error llegint l'entrada\n");
      break;
    }

    if (input[0] == '\n') {
      break;
    }
    int len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
      input[len - 1] = '\0';
      --len;
    }
    Query *q = Query_init(input);
    if (!q) {
      fprintf(stderr, "Error inicialitzant la query\n");
      break;
    }
    DocIdList *result = NULL;

    //  Analitzem les paraules que hem de buscar (les que estan incloses)
    QueryNode *actual = q->head;

    while (actual) {
      if (!actual->exclude) {
        DocIdList *llista = HashMap_get(reverseIndex, actual->keyword);
        if (llista) {
          if (!result) {
            result = DocIdList_create();
            for (size_t i = 0; i < llista->count; ++i) {
              DocIdList_add(result, llista->doc_ids[i]);
            }
          } else {
            DocIdList *tmp = DocIdList_intersect(result, llista);
            DocIdList_free(result);
            result = tmp;
          }
        }
      }
      actual = actual->next;
    }

    //  Analitzem les paraules que hem d'excloure (les que NO estan incloses)

    actual = q->head;
    while (actual && result) {
      if (actual->exclude) {
        DocIdList *exclosos = HashMap_get(reverseIndex, actual->keyword);
        if (exclosos) {
          DocIdList *tmp = DocIdList_difference(result, exclosos);
          DocIdList_free(result);
          result = tmp;
        }
      }
      actual = actual->next;
    }

    // Imprimim el resultat
    if (!result || DocIdList_is_empty(result)) {
      printf("\tNo s'ha trobat cap document per la consulta\n");
    } else {
      printf("Documents que coincideixen amb la consulta:\n");
      DocIdList_print(result);
    }
    printf("\n");

    DocIdList_free(result);
    query_queue(q);
  }
  HashMap_free(reverseIndex, (void (*)(void *))DocIdList_free);

  /////////// ACABA VERSIÓ HASHMAP ///////////
  //*/

  query_queue_clear();
  freeAllDocuments(allDocs, totalDocs);
  return 0;
}
