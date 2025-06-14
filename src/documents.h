#ifndef DOCUMENTS_H
#define DOCUMENTS_H

#include <stdbool.h>

#define MAX_LINKS 1000

#define TOTAL_DOCUMENTS 12 // #define TOTAL_DOCUMENTS 6222

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

Links *LinksInit(void);
Document *document_desserialize(char *path);
Document **loadAllDocuments(void);
void freeDocument(Document *doc);
void freeAllDocuments(Document **allDocs, int totalDocs);

#endif