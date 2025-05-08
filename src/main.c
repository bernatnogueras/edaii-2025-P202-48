#include "documents.h"
#include "query.h"
#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

void createaleak() {
  char *foo = malloc(20 * sizeof(char));
  printf("Allocated leaking string: %s", foo);
}

int main() {
  Document **allDocs = loadAllDocuments();
  if (allDocs == NULL) {
    return 1;
  }

  const int totalDocs = 13;
  char input[1024];

  while(1) {
    printf("Introdueix paraules clau separades por espais: ");
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

    for (int i = 0; i < totalDocs; ++i) {
      if (document_matches(allDocs[i], q)) {
        printf("\"%s\" SÍ coincideix\n", allDocs[i]->title);
      } else {
        printf("\"%s\" NO coincideix\n", allDocs[i]->title);
      }
    }

    printf("\n");
    query_queue(q);
  }

  query_queue_clear();
  freeAllDocuments(allDocs, totalDocs);
  return 0;
}
