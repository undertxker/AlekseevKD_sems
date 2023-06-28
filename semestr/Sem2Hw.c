#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dynamic_string.h"
#include "hash_map.h"
#include "csv.h"
#include <locale.h>
#include <errno.h>

#define MAX_CSV_LINE 512
#define BOOK_COLUMNS_COUNT 5
#define max(a,b) (((a) > (b)) ? (a) : (b))

char* book_columns[5] = 
{
    "isbn",
    "authors",
    "title",
    "count",
    "available_count"
};

typedef struct {
    dynamic_string isbn;
    dynamic_string authors;
    dynamic_string title;
    dynamic_string count;
    dynamic_string available_count;
} book;

void free_book(book* x)
{
    dynamic_string_free(&(x->isbn));
    dynamic_string_free(&(x->authors));
    dynamic_string_free(&(x->title));
    dynamic_string_free(&(x->count));
    dynamic_string_free(&(x->available_count));
    free(x);
}

hash_map books;

int try_read_books(char* path)
{
    FILE* f = fopen(path, "r");
    if (!f)
    {
        printf("read_books - failed to open a file '%s', error: %s\n", path, strerror(errno));
        return 1;
    }

    char line[MAX_CSV_LINE];
    while (fgets(line, MAX_CSV_LINE, f) != NULL)
    {
        char* unprocessed = line;
        book* b = malloc(sizeof(book));

        b->isbn = csv_read_next(&unprocessed);
        b->authors = csv_read_next(&unprocessed);
        b->title = csv_read_next(&unprocessed);
        b->count = csv_read_next(&unprocessed);
        b->available_count = csv_read_next(&unprocessed);

        hash_map_add(&books, dynamic_string_copy(&(b->isbn)), b);
    }

    fclose(f);
    return 0;
}

int read_command() 
{
    printf("Available commands:\n");
    printf("1 - Add book\n");
    printf("2 - Delete book\n");
    printf("3 - Print book info\n");
    printf("4 - Print all books info\n");
    printf("5 - Exit\n");

    int command;
    printf("Enter command number: ");
    scanf("%d", &command);
    getchar();

    return command;
}

dynamic_string read_string(char* text) 
{
    dynamic_string s;
    dynamic_string_init(&s);

    printf(text);

    int c;
    while ((c = getchar()) != EOF)
    {
        if (c == '\n')
            break;

        dynamic_string_push_back(&s, (char)c);
    }

    return s;
}

void add_book()
{
    book* b = malloc(sizeof(book));

    b->isbn = read_string("Enter ISBN: ");
    
    if (b->isbn.length == 0)
    {
        dynamic_string_free(&b->isbn);
        free(b);
        printf("ISBN have to be non empty\n");
        return;
    }

    hash_map_entry* entry = hash_map_find(&books, &b->isbn);
    if (entry->key != NULL) 
    {
        dynamic_string_free(&b->isbn);
        free(b);
        printf("A book with this ISBN already exist\n");
        return;
    }

    b->authors = read_string("Enter authors: ");
    b->title = read_string("Enter title: ");
    b->count = read_string("Enter count: ");
    b->available_count = read_string("Enter available count: ");

    hash_map_add(&books, dynamic_string_copy(&b->isbn), b);
}

void delete_book() 
{
    dynamic_string isbn = read_string("Enter ISBN: ");
    hash_map_entry* entry = hash_map_find(&books, &isbn);

    if (entry->key == NULL)
    {
        printf("A book with this ISBN is not found\n\n");
    }
    else
    {
        hash_map_delete(&books, entry);
    }

    dynamic_string_free(&isbn);
}

void find_and_print_book() 
{
    dynamic_string isbn = read_string("Enter ISBN: ");
    hash_map_entry* entry = hash_map_find(&books, &isbn);

    if (entry->key == NULL)
    {
        printf("A book with this ISBN is not found\n\n");
    }
    else
    {
        book* b = entry->value;
        printf("ISBN           : %s\n", b->isbn.buffer);
        printf("Authors        : %s\n", b->authors.buffer);
        printf("Title          : %s\n", b->title.buffer);
        printf("Total count    : %s\n", b->count.buffer);
        printf("Available count: %s\n\n", b->available_count.buffer);
    }

    dynamic_string_free(&isbn);
}

int compare_book(book** a, book** b) 
{
    return strcmp((*a)->isbn.buffer, (*b)->isbn.buffer);
}

void print_all_books() 
{
    book** tmp = malloc(books.length * sizeof(book*));

    int widths[BOOK_COLUMNS_COUNT];
    for (size_t i = 0; i < BOOK_COLUMNS_COUNT; ++i)
        widths[i] = strlen(book_columns[i]);

    for (size_t i = 0, tmpIdx = 0; i < books.capacity; ++i) 
    {
        if (books.entries[i].key != NULL) 
        {
            book* b = books.entries[i].value;
            widths[0] = max(widths[0], b->isbn.length);
            widths[1] = max(widths[1], b->authors.length);
            widths[2] = max(widths[2], b->title.length);
            widths[3] = max(widths[3], b->count.length);
            widths[4] = max(widths[4], b->available_count.length);
            tmp[tmpIdx++] = b;
        }
    }

    qsort(tmp, books.length, sizeof(book*), compare_book);
    
    // Выводим заголовок
    printf("+");
    for (size_t i = 0; i < BOOK_COLUMNS_COUNT; ++i)
    {
        for (size_t x = 0; x < widths[i]; ++x)
            printf("-");
        printf("+");
    }
    printf("\n|");
    for (size_t i = 0; i < BOOK_COLUMNS_COUNT; ++i)
    {
        printf("%*s%s", widths[i] - strlen(book_columns[i]), "", book_columns[i]);
        printf("|");
    }
    printf("\n+");
    for (size_t i = 0; i < BOOK_COLUMNS_COUNT; ++i)
    {
        for (size_t x = 0; x < widths[i]; ++x)
            printf("-");
        printf("+");
    }
    printf("\n");

    for (size_t i = 0; i < books.length; ++i) 
    {
        printf("|%*s%.*s", widths[0] - tmp[i]->isbn.length, "", tmp[i]->isbn.length, tmp[i]->isbn.buffer);
        printf("|%*s%.*s", widths[1] - tmp[i]->authors.length, "", tmp[i]->authors.length, tmp[i]->authors.buffer);
        printf("|%*s%.*s", widths[2] - tmp[i]->title.length, "", tmp[i]->title.length, tmp[i]->title.buffer);
        printf("|%*s%.*s", widths[3] - tmp[i]->count.length, "", tmp[i]->count.length, tmp[i]->count.buffer);
        printf("|%*s%.*s", widths[4] - tmp[i]->available_count.length, "", tmp[i]->available_count.length, tmp[i]->available_count.buffer);
        printf("|\n");
    }

    printf("+");
    for (size_t i = 0; i < BOOK_COLUMNS_COUNT; ++i)
    {
        for (size_t x = 0; x < widths[i]; ++x)
            printf("-");
        printf("+");
    }
    printf("\n\n");

    free(tmp);
}

void die_on_error(int error) 
{
    if (!error)
        return;

    printf("save - failed to write to a file, error: %s\n", strerror(errno));
    exit(1);
}

int save(char* path)
{
    FILE* f = fopen(path, "w");
    if (!f)
    {
        printf("save - failed to open a file '%s', error: %s\n", path, strerror(errno));
        return 1;
    }

    for (size_t i = 0; i < books.capacity; ++i)
    {
        if (books.entries[i].key != NULL)
        {
            book* b = books.entries[i].value;
            die_on_error(csv_write_entry(f, &b->isbn));
            die_on_error(csv_write_separator(f));
            die_on_error(csv_write_entry(f, &b->authors));
            die_on_error(csv_write_separator(f));
            die_on_error(csv_write_entry(f, &b->title));
            die_on_error(csv_write_separator(f));
            die_on_error(csv_write_entry(f, &b->count));
            die_on_error(csv_write_separator(f));
            die_on_error(csv_write_entry(f, &b->available_count));
            die_on_error(csv_write_new_line(f));
        }
    }

    fclose(f);
    return 0;
}

int main() 
{
    system("chcp 1251");
    setlocale(LC_ALL, "UTF8");

    hash_map_init(&books, &free_book);

    if (try_read_books("books.csv"))
        return 1;

    while (1) 
    {
        int command = read_command();

        switch (command)
        {
        case 1:
            add_book();
            break;
        case 2:
            delete_book();
            break;
        case 3:
            find_and_print_book();
            break;
        case 4:
            print_all_books();
            break;
        case 5:
            return save("books.csv");
        default:
            printf("Invalid command number\n");
            break;
        }
    }

    return 0;
}