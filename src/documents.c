#include "documents.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cada vegada que el codi detecti un link a la part del 'body', l'estructura de
// links s'anirà omplint dinàmicament. Per tant, inicialitza i retorna una
// estructura Links
Links *LinksInit(void) {
  Links *links = (Links *)malloc(
      sizeof(Links)); // Reservem memòria per a una estructura del tipus link
  assert(links != NULL); // Si el malloc falla, retorna NULL
  links->count = 0;      // Inicialitza el contador de l'estructura a 0
  return links;          // Retorna el punter a l'estructura links
}

//'Desreialitzar'--> Convertir dades que estan guardades en un fitxer a una
// estructura de dades per a poder-les utilitzar

// Funció que llegeix un fitxer i ens permet omplir les dades de l'estructura
// Document
Document *document_desserialize(char *path) {
  FILE *f = fopen(path, "r"); // Llegim el fitxer
  assert(
      f !=
      NULL); // Si no pot obrir el fitxer (perquè no existeix...) retornarà NULL

  Document *document = (Document *)malloc(sizeof(
      Document)); // Reservem memòria dinàmica per a l'estructura Document
  // assert(document != NULL);

  char buffer[262144];
  int bufferSize = 262144;
  int bufferIdx = 0;
  char ch;

  // Llegeix/analitza l'id
  while ((ch = fgetc(f)) != '\n') {
    assert(bufferIdx < bufferSize);
    buffer[bufferIdx++] = ch;
  }
  assert(bufferIdx < bufferSize);
  buffer[bufferIdx++] = '\0';
  document->id = atoi(buffer);

  // Llegeix/analitza el title
  bufferIdx = 0;
  while ((ch = fgetc(f)) != '\n' && ch != EOF) {
    assert(bufferIdx < bufferSize);
    buffer[bufferIdx++] = ch;
  }
  assert(bufferIdx < bufferSize);
  buffer[bufferIdx] = '\0';
  document->title = (char *)malloc(sizeof(char) * (bufferIdx + 1));
  assert(document->title != NULL);
  strcpy(document->title, buffer);

  // Llegeix/analitza el body
  char linkBuffer[64];
  int linkBufferSize = 64;
  int linkBufferIdx = 0;
  bool parsingLink = false;
  Links *links = LinksInit();

  bufferIdx = 0;
  while ((ch = fgetc(f)) != EOF) {
    assert(bufferIdx < bufferSize);
    buffer[bufferIdx++] = ch;
    if (parsingLink) {
      if (ch == ')') {
        parsingLink = false;
        assert(linkBufferIdx < linkBufferSize);
        linkBuffer[linkBufferIdx++] = '\0';
        int linkId = atoi(linkBuffer);

        assert(links->count < MAX_LINKS);
        links->ids[links->count++] = linkId;

        linkBufferIdx = 0;
      } else if (ch != '(') {
        assert(linkBufferIdx < linkBufferSize);
        linkBuffer[linkBufferIdx++] = ch;
      }
    } else if (ch == ']') {
      parsingLink = true;
    }
  }
  assert(bufferIdx < bufferSize);
  buffer[bufferIdx++] = '\0';

  for (int i = 0; buffer[i] != '\0'; i++) {
    buffer[i] = tolower((unsigned char)buffer[i]);
  }

  document->body = (char *)malloc(sizeof(char) * (bufferIdx + 1));
  assert(document->body != NULL);
  strcpy(document->body, buffer);

  document->links = links;

  fclose(f);       // Tanca el fitxer
  return document; // Retorna el document
}

// Funció que carrega tots els documents d'una carpeta i retorna un array de
// punters a Document
Document **loadAllDocuments(void) {
  // char *folders[] = {"wikipedia12"};
  char *folders[] = {"wikipedia5400"};
  // int counts[] = {13}; //
  int counts[] = {5401};
  int numFolders = sizeof(counts) / sizeof(counts[0]);
  // int TOTAL_DOCS = 13;
  int TOTAL_DOCS = 5401;

  // Reservem l'array de punters
  Document **docs = malloc(sizeof(Document *) * TOTAL_DOCS);
  assert(docs != NULL && "No s'ha pogut reservar memòria pels docs");

  int idx = 0;
  char path[256];

  for (int f = 0; f < numFolders; ++f) {
    for (int j = 0; j < counts[f]; ++j) {
      sprintf(path, "datasets/%s/%d.txt", folders[f], j);

      Document *doc = document_desserialize(path);
      if (!doc) {
        fprintf(stderr, "Error carregant '%s'\n", path);
        for (int k = 0; k < idx; ++k)
          freeDocument(docs[k]);
        free(docs);
        return NULL;
      }
      docs[idx++] = doc;
    }
  }

  assert(idx == TOTAL_DOCS && "Carregats menys documents dels esperats");

  return docs;
}

// Funció que allibera tota la memòria associada a una estructura Document
void freeDocument(Document *doc) {
  free(doc->title);
  free(doc->body);
  free(doc->links);
  free(doc);
}

// Funció que ens permet alliberar tots els Documents d'un array i aquest array
// mateix
void freeAllDocuments(Document **allDocs, int totalDocs) {
  if (allDocs == NULL)
    return;

  for (int i = 0; i < totalDocs; ++i) {
    if (allDocs[i] != NULL) {
      freeDocument(allDocs[i]);
    }
  }
  free(allDocs);
}