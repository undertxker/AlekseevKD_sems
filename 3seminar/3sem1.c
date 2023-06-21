#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 10

typedef struct Node node_t;
struct Node {
    node_t *next;
    char   *city;
    size_t p;
};

typedef struct {
    node_t **l;
    size_t msize;
} table_t;

size_t hash(char const *s)
{
    size_t h = 0;
    for (int i = 0; s[i]; i++) {
        h = h * 17 + s[i];
    }
    return h;
}

node_t *new_node(char *c, size_t p)
{
    node_t *n = malloc(sizeof(node_t));
    n->next = NULL;
    n->city = strdup(c);
    n->p = p;
    return n;
}

int add(table_t *t, char *c, size_t p)
{
    size_t h = hash(c) % t->msize;
    node_t *it = t->l[h];
    while (it) {
        if (strcmp(it->city, c) == 0) {
            printf("city has already been added\n");
            return 1;
        }
        it = it->next;
    }
    it = new_node(c, p);
    it->next = t->l[h];
    t->l[h] = it;
    return 0;
}

void erase(table_t *t, char const *s)
{
    size_t h = hash(s) % t->msize;
    _Bool found = 0;
    node_t *it = t->l[h];
    if (it && strcmp(it->city, s) == 0) {
        found = 1;
        free(it->city);
        t->l[h] = it->next;
        free(it);
    }
    else {
        while (it->next) {
            if (strcmp(it->next->city, s) == 0) {
                node_t *n = it->next;
                free(it->next->city);
                it->next = it->next->next;
                free(n);

                found = 1;
                break;
            }
            it = it->next;
        }
    }
    if (found == 0) {
        fprintf(stderr, "not found\n");
    }
}

void list(table_t const *t)
{
    for (size_t i = 0; i < t->msize; i++) {
        node_t *it = t->l[i];
        while (it) {
            printf("%s %zu\n", it->city, it->p);
            it = it->next;
        }
    }
}

void init(table_t *t)
{
    t->l = calloc(MAX_SIZE, sizeof(node_t *));
    t->msize = MAX_SIZE;
}

void destroy(table_t *t)
{
    for (size_t i = 0; i < t->msize; i++) {
        while (t->l[i]) {
            node_t *n = t->l[i];
            t->l[i] = t->l[i]->next;
            free(n->city);
            free(n);
        }
    }
    free(t->l);
    t->l = NULL;
    t->msize = 0;
}

int main(void)
{
    table_t t;
    init(&t);
    char l[128] = {0};
    size_t p = 0;
    for (;;) {
        char *m[] = {"exit", "add", "erase", "list"};
        for (size_t i = 0; i < sizeof(m) / sizeof(m[0]); i++) {
            printf("%zu - %s\n", i, m[i]);
        }
        int ch;
        if (scanf("%d", &ch) < 1) {
            break;
        }
        if (ch == 0) {
            break;
        }
        else if (ch == 1) {
            scanf("%s %zu", l, &p);
            add(&t, l, p);
        }
        else if (ch == 2) {
            scanf("%s", l);
            erase(&t, l);
        }
        else {
            list(&t);
