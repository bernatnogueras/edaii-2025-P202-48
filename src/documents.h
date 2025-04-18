#ifndef DOCUMENTS_H
#define DOCUMENTS_H

#include <stdbool.h>

#define MAX_LINKS 1000

//#include <string.h>
//#define N

/*
    1. Create document.h
    2. Define the Document Struct
    3. Function to load a document from path
*/

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

void freeDocument(Document *doc);
#endif