#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "documents.h"

void createaleak() {
  char *foo = malloc(20 * sizeof(char));
  printf("Allocated leaking string: %s", foo);
}


int main() {
  
    for(int i=0; i<=12; ++i){
      char ruta[200];
      sprintf(ruta, "datasets/wikipedia12/%d.txt", i);    //Ens permet anar actualitzant la ruta a cada iteració

      Document *doc = document_desserialize(ruta);
      if (doc == NULL) {
          fprintf(stderr, "Error al carregar el document.\n");
          return 1;
      }

      printf("ID: %d\n", doc->id);
      printf("Titol: %s\n", doc->title);
      printf("Cos:\n%s\n", doc->body);

      if (doc->links != NULL) {
          printf("Links (%d):\n", doc->links->count);
          for (int i = 0; i < doc->links->count; i++) {
              printf("  Link %d: %d\n", i + 1, doc->links->ids[i]);
          }
          printf("\n");
      }
      freeDocument(doc);    //Cridem la funció que allibera el document (títol, cos, links i document)
    } 

  //printf("*****************\nWelcome to EDA 2!\n*****************\n");

  // how to import and call a function
  //printf("Factorial of 4 is %d\n", fact(4));

  // uncomment and run "make v" to see how valgrind detects memory leaks
  // createaleak();

  return 0;
}
