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

/*
// LA PODEM BORRAR, NO L'ESTEM UTILITZANT
void createaleak() {
  char *memoria = malloc(20 * sizeof(char));
  if(!memoria){
    fprintf(stderr, " Error allocating memòria\n");
    return;
    // Inicialitzem la cadena amb una cadena buida
    memoria[0] = '\0';
  }
  printf("Allocated leaking string: %s", memoria);
  free(memoria);
}
*/

int main() {
  Document **allDocs = loadAllDocuments();
  if (allDocs == NULL) {
    fprintf(stderr, "Error carregant documents\n");
    return 1;
  }

  const int totalDocs = 13;
  char input[1024];

  /*
      //BÚSQUEDA LINEAL (versió lenta)

      while (1) {
      printf("Search (lineal): ");
      if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error llegint l'entrada\n");
        break;
      }

      if (input[0] == '\n') {
        break;
      }

      size_t len = strlen(input);
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

    query_queue_clear();
  */

  // HASHMAP (versió ràpida)

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
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
      input[len - 1] = '\0';
      --len;
    }

    normalize_word(input);
    if (strlen(input) == 0) {
      printf("Entrada no vàlida\n");
      continue;
    }

    DocIdList *list = (DocIdList *)HashMap_get(reverseIndex, input);
    if (list == NULL || DocIdList_is_empty(list)) {
      printf("\tNo s'ha trobat cap document amb la paraula '%s'\n", input);
    } else {
      printf("Documents amb la paraula '%s':\n", input);
      DocIdList_print(list);
    }
    printf("\n");
    
    Query *q = Query_init(input);
    if (!q) {
      fprintf(stderr, "Error inicialitzant la query\n");
      break;
    }
    query_queue(q);
  }
  
  HashMap_free(reverseIndex);
  query_queue_clear();
  freeAllDocuments(allDocs, totalDocs);
  return 0;
}
