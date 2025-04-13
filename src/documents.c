#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "documents.h"


Links *LinksInit(void) {
  Links *links = (Links *)malloc(sizeof(Links));
  assert(links != NULL);
  links->count = 0;
  return links;
}

Document *document_desserialize(char *path) {
    FILE *f = fopen(path, "r");
    assert(f != NULL);
    
    Document *document = (Document *)malloc(sizeof(Document));
    //assert(document != NULL);

    char buffer[262144];
    int bufferSize = 262144;
    int bufferIdx = 0;
    char ch;

    // parse id
    while ((ch = fgetc(f)) != '\n') {
        assert(bufferIdx < bufferSize);
        buffer[bufferIdx++] = ch;
    }
    assert(bufferIdx < bufferSize);
    buffer[bufferIdx++] = '\0';
    document->id = atoi(buffer);

    // parse title
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
    
    // parse body
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
        } else if (ch == ']') { // found beginning of link id, e.g.: [my link text](123) 
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

    fclose(f);
    return document;
}