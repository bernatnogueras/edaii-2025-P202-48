#include "documents.h"
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
  
  printf("******* recent searches ********\n\nSearch: ");

  freeAllDocuments(allDocs, 12);                  //freeAllDocuments(allDocs, 6222);

  printf("Done\n");

  // printf("*****************\nWelcome to EDA 2!\n*****************\n");

  // how to import and call a function
  // printf("Factorial of 4 is %d\n", fact(4));

  // uncomment and run "make v" to see how valgrind detects memory leaks
  // createaleak();

  return 0;
}
