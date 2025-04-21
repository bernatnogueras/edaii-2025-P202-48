#include "documents.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cada vegada que el codi detecti un link a la part del 'body', l'estructura de
// links s'anirà omplint dinàmicament
Links *LinksInit(void) {
  Links *links = (Links *)malloc(
      sizeof(Links)); // Reservem memòria per a una estructura del tipus link
  assert(links != NULL); // Si el malloc falla, retorna NULL
  links->count = 0;      // Inicialitza el contador de l'estructura a 0
  return links;          // Retorna el punter a l'estructura links
}

//'Desreialitzar'--> Convertir dades que estan guardades en un fitxer a una
//estructura de dades per a poder-les utilitzar
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
      if (ch == ')') { // end of link
        parsingLink = false;
        assert(linkBufferIdx < linkBufferSize);
        linkBuffer[linkBufferIdx++] = '\0';
        int linkId = atoi(linkBuffer);

        // add to links
        assert(links->count < MAX_LINKS);
        links->ids[links->count++] = linkId;

        linkBufferIdx = 0;
      } else if (ch != '(') { // skip first parenthesis of the link
        assert(linkBufferIdx < linkBufferSize);
        linkBuffer[linkBufferIdx++] = ch;
      }
    } else if (ch ==
               ']') { // found beginning of link id, e.g.: [my link text](123)
      parsingLink = true;
    }
  }
  assert(bufferIdx < bufferSize);
  buffer[bufferIdx++] = '\0';

  char *body = (char *)malloc(sizeof(char) * bufferIdx);
  strcpy(body, buffer);

  document->body = (char *)malloc(sizeof(char) * (bufferIdx + 1));
  assert(document->body != NULL);
  strcpy(document->body, buffer);

  document->links = links;

  fclose(f);       // Tanca el fitxer
  return document; // Retorna el document
}

void freeDocument(
    Document *doc) { // Creem una funció que permet alliberar el document
  free(doc->title);
  free(doc->body);
  free(doc->links);
  free(doc);
}