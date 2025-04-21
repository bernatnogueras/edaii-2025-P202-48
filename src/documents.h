#ifndef DOCUMENTS_H
#define DOCUMENTS_H

#include <stdbool.h>

#define MAX_LINKS 1000

// #include <string.h>
// #define N

typedef struct {
  int ids[MAX_LINKS];
  int count;
} Links;

typedef struct {
  int id;
  char *title;
  char *body;
  Links *links;
} Document;

Document *document_desserialize(char *path);

Links *LinksInit(void);
//Document **load_docs(int num_docs);
void freeDocument(Document *doc);
#endif