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

  Document *doc = document_desserialize("datasets/wikipedia12/0.txt");
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
    }

    free(doc->title);
    free(doc->body);
    free(doc->links);
    free(doc);

  //printf("*****************\nWelcome to EDA 2!\n*****************\n");

  // how to import and call a function
  //printf("Factorial of 4 is %d\n", fact(4));

  // uncomment and run "make v" to see how valgrind detects memory leaks
  // createaleak();

  return 0;
}
