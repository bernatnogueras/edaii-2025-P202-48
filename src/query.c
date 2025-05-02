#include "query.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Query *Query_init(const char *line) {
    Query *q = malloc(sizeof(Query));
    if (!q) return NULL;
    q->head = NULL;
    q->count = 0;

    // Hacemos copia mutable de la línea
    char *copy = malloc(strlen(line) + 1);
    if (!copy) {
        free(q);
        return NULL;
    }
    strcpy(copy, line);

    char *p = copy;
    QueryNode **pp = &q->head;

    while (*p) {
        // Saltar espacios, tabuladores y saltos de línea
        while (*p && (*p == ' ' || *p == '\t' || *p == '\n')) {
            p++;
        }
        if (!*p) break;

        // Inicio de la palabra
        char *start = p;
        while (*p && *p != ' ' && *p != '\t' && *p != '\n') {
            p++;
        }
        size_t len = p - start;

        // Copiamos la palabra en una nueva cadena
        char *word = malloc(len + 1);
        if (!word) {
            // En caso de error, liberamos y salimos
            Query_free(q);
            free(copy);
            return NULL;
        }
        strncpy(word, start, len);
        word[len] = '\0';

        // Creamos nodo y lo enlazamos
        QueryNode *node = malloc(sizeof(QueryNode));
        if (!node) {
            free(word);
            Query_free(q);
            free(copy);
            return NULL;
        }
        node->keyword = word;
        node->next = NULL;
        *pp = node;
        pp = &node->next;
        q->count++;
    }

    free(copy);
    return q;
}


void Query_free(Query *q) {
    QueryNode *curr = q->head;
    while (curr) {
        QueryNode *tmp = curr->next;
        free(curr->keyword);
        free(curr);
        curr = tmp;
    }
    free(q);
}


bool document_matches(const Document *doc, const Query *q) {
    if (!q || q->count == 0) return false;
    for (QueryNode *n = q->head; n; n = n->next) {
        if (!strstr(doc->body, n->keyword) && !strstr(doc->title, n->keyword)) {
            return false;
        }
    }
    return true;
}