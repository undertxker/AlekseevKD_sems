#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 10

typedef struct Node node_t;

struct Node {
    node_t *next;
    char *city;
    size_t p;
};

typedef struct {
    node_t **l;
    size_t msize;
} table_t;

size_t hash(char const *string) {
    size_t result = 0;
    for (int i = 0; string[i]; i++) {
        result = result * 17 + string[i];
    }
    return result;
}

node_t* new_node(char *city, size_t p) {
    node_t *node = malloc(sizeof(node_t));
    node->next = NULL;
    node->city = strdup(city);
    node->p = p;
    return node;
}

int add(table_t *t, char *city, size_t p) {
    size_t h = hash(city) % t->msize;
    node_t *node = t->l[h];
    while (node) {
        if (strcmp(node->city, city) == 0) {
            printf("sity already exist\n");
            return 1;
        }
        node = node->next;
    }
    node = new_node(city, p);
    node->next = t->l[h];
    t->l[h] = node;
    return 0;
}

void erase(table_t *t, char const *city) {
    size_t h = hash(city) % t->msize;
    _Bool found = 0;
    node_t *node = t->l[h];
    if (node && strcmp(node->city, city) == 0) {
        found = 1;
        free(node->city);
        t->l[h] = node->next;
        free(node);
    } else {
        while (node && node->next) {
            if (strcmp(node->next->city, city) == 0) {
                node_t *n = node->next;
                free(n->city);
                node->next = node->next->next;
                free(n);
                found = 1;
                break;
            }
            node = node->next;
        }
    }
    if (found == 0) {
        fprintf(stderr, "sity ne naiden\n");
    }
}

void list(table_t const *t) {
    for (size_t i = 0; i < t->msize; i++) {
        node_t *node = t->l[i];
        while (node) {
            printf("%s %zu\n", node->city, node->p);
            node = node->next;
        }
    }
}

void init(table_t *t) {
    t->l = calloc(sizeof(node_t*), MAX_SIZE);
    t->msize = MAX_SIZE;
}

void destroy(table_t *t) {
    for (size_t i = 0; i < t->msize; i++) {
        while (t->l[i]) {
            node_t *n = t->l[i];
            t->l[i] = t->l[i]->next;
            free(n->city);
            free(n);
        }
    }
    free(t->l);
}

int main(void) {
    table_t t;
    init(&t);
    char city[128] = {0};
    size_t p = 0;
    for (;;) {
        char *menu[] = {"exit", "add", "erase", "list"};
        size_t menu_size = sizeof(menu) / sizeof(menu[0]);
        for (size_t i = 0; i < menu_size; i++) {
            printf("%zu - %s\n", i, menu[i]);
        }
        int choice;
        if (scanf("%d", &choice) < 1) {
            break;
        }
        if (choice == 0) {
            break;
        } else if (choice == 1) {
            scanf("%s %zu", city, &p);
            add(&t, city, p);
        } else if (choice == 2) {
            scanf("%s", city);
            erase(&t, city);
        } else {
            list(&t);
        }
    }
    destroy(&t);
    return 0;
}