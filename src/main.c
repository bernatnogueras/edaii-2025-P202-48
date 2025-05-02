#include "documents.h"
#include "query.h"
#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void createaleak() {
  char *foo = malloc(20 * sizeof(char));
  printf("Allocated leaking string: %s", foo);
}

int main() {
  Document **allDocs = loadAllDocuments();
  if (allDocs == NULL) {
    return 1;
  }
  
  //printf("******* recent searches ********\n\nSearch: ");


  const int totalDocs = 13;
  // 2. Leemos las keywords de la entrada estándar
  char input[1024];
  printf("Introdueix paraules clau separades por espais: ");
  if (scanf(" %[^\n]", input) != 1) {
    fprintf(stderr, "Error llegint l'entrada\n");
    freeAllDocuments(allDocs, totalDocs);
    return 1;
  }

  // 3. Inicializamos la Query
  Query *q = Query_init(input);
  if (!q) {
    fprintf(stderr, "Error inicialitzant la query\n");
    freeAllDocuments(allDocs, totalDocs);
    return 1;
  }

  // 4. Recorremos y comprobamos cada documento
  for (int i = 0; i < totalDocs; ++i) {
    if (document_matches(allDocs[i], q)) {
      printf("\"%s\" SÍ coincideix\n", allDocs[i]->title);
    } else {
      printf("\"%s\" NO coincideix\n", allDocs[i]->title);
    }
  }

  // 5. Liberamos memoria
  Query_free(q);
  freeAllDocuments(allDocs, totalDocs);


  //freeAllDocuments(allDocs, 12);                  //freeAllDocuments(allDocs, 6222);

  //printf("Done\n");

  // printf("*****************\nWelcome to EDA 2!\n*****************\n");

  // how to import and call a function
  // printf("Factorial of 4 is %d\n", fact(4));

  // uncomment and run "make v" to see how valgrind detects memory leaks
  // createaleak();

  return 0;
}
