#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

#define MISSING_ARG_MSG "nety consolnogo argumenta"
#define NOT_FOUND_FILE_MSG "ne mogu open file"

typedef struct Node node_t;
struct Node 
{
    node_t *left;
    node_t *right;
    char   *str;
    int     repeats;
};

node_t* new_node(char *str) 
{
    node_t *new_node = malloc(sizeof(node_t));
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->str = str;
    new_node->repeats = 1;
    return new_node;
}

node_t* insert(node_t *r, char *str) 
{
    if (r == NULL) 
    {
        return new_node(str);
    }
    node_t *p = r;
    for (;;) 
    {
        int res = strcmp(p->str, str);
        if (res == 0) 
        {
            p->repeats++;
            free(str);
            return r;
        }
        if (res < 0) 
        {
            if (p->left == NULL) 
            {
                p->left = new_node(str);
                return r;
            }
            p = p->left;
        } 
        else 
        {
            if (p->right == NULL) 
            {
                p->right = new_node(str);
                return r;
            }
            p = p->right;
        }
    }
}

void clear(node_t *n) 
{
    if (n) 
    {
        clear(n->left);
        clear(n->right);
        free(n->str);
        free(n);
    }
}

int parse_arg(int argc, char **argv, int *order) 
{
    int r;
    while ((r = getopt(argc, argv, "o:")) != -1) 
    {
        switch (r) 
        {
            case 'o':
                if (strcmp(optarg, "DESC") == 0) 
                {
                    *order = 1;
                } 
                else if (strcmp(optarg, "ASC") == 0) 
                {
                    *order = 2;
                } 
                else 
                {
                    fprintf(stderr, "ne correct option\n");
                    return 1;
                }
                break;
        }
    }
    return 0;
}

char* read_line(FILE *file) 
{
    char *str = malloc(sizeof(char) * 11);
    int n = 0;
    while (fscanf(file, "%10[^\n]", str + 10*n) == 1) 
    {
        n++;
        str = realloc(str, sizeof(char) * (10*n + 11));
    }
    fscanf(file, "%*c");
    if (n == 0) 
    {
        free(str);
        return NULL;
    }
    n--;
    str = realloc(str, sizeof(char) * (10*n + strlen(str + 10 * n) + 1));     
    return str;
}

void inorder(node_t *n) 
{
    if (n) 
    {
        inorder(n->left);
        printf("%s %d\n", n->str, n->repeats);
        inorder(n->right);
    }
}

void rinorder(node_t *n) 
{
    if (n) 
    {
        inorder(n->right);
        printf("%s %d\n", n->str, n->repeats);
        inorder(n->left);
    }
}

void task(FILE *f, int order) 
{
    node_t *n = NULL;
    while (!feof(f)) 
    {
        char *line = read_line(f);
        if (line) 
        {
            n = insert(n, line);
        }
    }
    if (order == 1) 
    {
        inorder(n);
    } 
    else 
    {
        rinorder(n);
    }
    clear(n);
}

int main(int argc, char **argv) 
{
    if (argc < 2) 
    {
        fprintf(stderr, "%s\n", MISSING_ARG_MSG);
        return 1;
    }
    char *fname = argv[1];
    FILE *f = fopen(fname, "r");
    if (f == NULL) 
    {
        fprintf(stderr, "%s — %s\n", fname, NOT_FOUND_FILE_MSG);
        return 3;
    }
    int order = 1;
    if (parse_arg(argc, argv, &order)) 
    {
        fclose(f);
        return 1;
    }
    task(f, order);
    fclose(f);
    return 0;
}