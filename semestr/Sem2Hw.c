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

// Функция для чтения списка книг из csv файла
int try_read_books(char* path)
{
    FILE* f = fopen(path, "r"); // Открываем файл для чтения
    if (!f)
    {
        printf("read_books - failed to open a file '%s', error: %s\n", path, strerror(errno));
        return 1;
    }

// Для каждой строки файла производим чтение и добавляем книгу в хеш-таблицу
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

// Закрываем файл и возвращаем 0, если все прошло успешно
    fclose(f);
    return 0;
}

// Функция для чтения команды для пользователя
int read_command() 
{
    printf("Available commands:\n");
    printf("1 - Add book\n");
    printf("2 - Delete book\n");
    printf("3 - Print book info\n");
    printf("4 - Print all books info\n");
    printf("5 - Exit\n");

// Считываем номер команды и возвращаем его
    int command;
    printf("Enter command number: ");
    scanf("%d", &command);
    getchar();

    return command;
}

// Функция для считывания динамических строк с клавиатуры
dynamic_string read_string(char* text) 
{
    dynamic_string s;
    dynamic_string_init(&s);

    printf(text);

// Читаем посимвольно до тех пор, пока не встретим конец строки или конец файла
    int c;
    while ((c = getchar()) != EOF)
    {
        if (c == '\n')
            break;

        dynamic_string_push_back(&s, (char)c);
    }

    return s;
}

// Функция для добавления новой книги в хеш-таблицу
void add_book()
{
    book* b = malloc(sizeof(book));

// Считываем ISBN книги
    b->isbn = read_string("Enter ISBN: ");

// Проверяем, что ISBN не пустой
    if (b->isbn.length == 0)
    {
        dynamic_string_free(&b->isbn);
        free(b);
        printf("ISBN have to be non empty\n");
        return;
    }
// Проверяем, что книга с таким ISBN еще не существует
    hash_map_entry* entry = hash_map_find(&books, &b->isbn);
    if (entry->key != NULL) 
    {
        dynamic_string_free(&b->isbn);
        free(b);
        printf("A book with this ISBN already exist\n");
        return;
    }

// Считываем остальные поля и добавляем книгу в хеш-таблицу
    b->authors = read_string("Enter authors: ");
    b->title = read_string("Enter title: ");
    b->count = read_string("Enter count: ");
    b->available_count = read_string("Enter available count: ");

    hash_map_add(&books, dynamic_string_copy(&b->isbn), b);
}

// Функция для удаления книги по ее ISBN
void delete_book() 
{

//Считываем ISBN книги и ищем ее в таблице
    dynamic_string isbn = read_string("Enter ISBN: ");
    hash_map_entry* entry = hash_map_find(&books, &isbn);

// Если книга не найдена, выводим соответствующее сообщение
    if (entry->key == NULL)
    {
        printf("A book with this ISBN is not found\n\n");
    }
    else     // Иначе удаляем книгу из таблицы
    {
        hash_map_delete(&books, entry);
    }

    dynamic_string_free(&isbn);
}

// функция поиска и вывода информации по ее ISBN
void find_and_print_book() 
{
    dynamic_string isbn = read_string("Enter ISBN: ");
    hash_map_entry* entry = hash_map_find(&books, &isbn);

// Если книга не найдена, выводим соответствующее сообщение
    if (entry->key == NULL)
    {
        printf("A book with this ISBN is not found\n\n");
    }
    else     // Иначе выводим информацию о книге
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

// Функция для сравнения двух книг по их ISBN
int compare_book(book** a, book** b) 
{
    return strcmp((*a)->isbn.buffer, (*b)->isbn.buffer);
}

void print_all_books() 
{
    // Выделяем память под временный массив указателей на книги размером books.length
book** tmp = malloc(books.length * sizeof(book*));

int widths[BOOK_COLUMNS_COUNT];
// Инициализируем массив ширин каждого столбца таблицы
for (size_t i = 0; i < BOOK_COLUMNS_COUNT; ++i)
    widths[i] = strlen(book_columns[i]);

// Проходим по всем элементам хэш-таблицы books
for (size_t i = 0, tmpIdx = 0; i < books.capacity; ++i) 
{
    // Если текущий элемент не пустой
    if (books.entries[i].key != NULL) 
    {
        // Получаем указатель на книгу
        book* b = books.entries[i].value;
        
        // Обновляем ширины столбцов, если необходимо
        widths[0] = max(widths[0], b->isbn.length);
        widths[1] = max(widths[1], b->authors.length);
        widths[2] = max(widths[2], b->title.length);
        widths[3] = max(widths[3], b->count.length);
        widths[4] = max(widths[4], b->available_count.length);
        
        // Добавляем указатель на книгу во временный массив
        tmp[tmpIdx++] = b;
    }
}

// Сортируем временный массив книг
qsort(tmp, books.length, sizeof(book*), compare_book);

// Выводим верхнюю границу таблицы
printf("+");
for (size_t i = 0; i < BOOK_COLUMNS_COUNT; ++i)
{
    for (size_t x = 0; x < widths[i]; ++x)
        printf("-");
    printf("+");
}
printf("\n|");

// Выводим названия столбцов таблицы
for (size_t i = 0; i < BOOK_COLUMNS_COUNT; ++i)
{
    // Выравниваем по ширине столбца при необходимости
    printf("%*s%s", widths[i] - strlen(book_columns[i]), "", book_columns[i]);
    printf("|");
}

// Выводим нижнюю границу таблицы
printf("\n+");
for (size_t i = 0; i < BOOK_COLUMNS_COUNT; ++i)
{
    for (size_t x = 0; x < widths[i]; ++x)
        printf("-");
    printf("+");
}
printf("\n");

// Проходим по всем книгам в временном массиве и выводим их данные в таблицу
for (size_t i = 0; i < books.length; ++i) 
{
    printf("|%*s%.*s", widths[0] - tmp[i]->isbn.length, "", tmp[i]->isbn.length, tmp[i]->isbn.buffer);
    printf("|%*s%.*s", widths[1] - tmp[i]->authors.length, "", tmp[i]->authors.length, tmp[i]->authors.buffer);
    printf("|%*s%.*s", widths[2] - tmp[i]->title.length, "", tmp[i]->title.length, tmp[i]->title.buffer);
    printf("|%*s%.*s", widths[3] - tmp[i]->count.length, "", tmp[i]->count.length, tmp[i]->count.buffer);
    printf("|%*s%.*s", widths[4] - tmp[i]->available_count.length, "", tmp[i]->available_count.length, tmp[i]->available_count.buffer);
    printf("|\n");
}

// Выводим нижнюю границу таблицы
printf("+");
for (size_t i = 0; i < BOOK_COLUMNS_COUNT; ++i)
{
    for (size_t x = 0; x < widths[i]; ++x)
        printf("-");
    printf("+");
}
printf("\n\n");

// Освобождаем выделенную память для временного массива
free(tmp);
}

// Функция для обработки ошибок с выводом сообщения и завершением программы
void die_on_error(int error) 
{
    // Если ошибки нет, то мы ничего не делаем и выходим из функции через return 
    if (!error)
        return;
// Завершаем работу программы с кодом ошибки 1
    printf("save - failed to write to a file, error: %s\n", strerror(errno));
    exit(1);
}

// Функция для сохранения книг в файл
int save(char* path)
{
    // Открываем файл на запись 
    FILE* f = fopen(path, "w");
    if (!f)
    {
        // Если не удалось открыть файл, выводим сообщение об ошибке и возвращаем код ошибки 1
        printf("save - failed to open a file '%s', error: %s\n", path, strerror(errno));
        return 1;
    }

// Записываем информацию о каждой книге в файл, используя функцию csv_write_entry и проверяя ошибки через функцию die_on_error
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

    fclose(f); // Заркываем файл 
    return 0; // Возвращаем код успешного выполнения
}

int main() 
{
    system("chcp 1251"); // Устанавливаем кодировку консоли
    setlocale(LC_ALL, "UTF8"); // Устанавливаем локаль программы

    hash_map_init(&books, &free_book); // Инициализируем хэш-таблицу книг

    if (try_read_books("books.csv")) // Пытаемся прочитать информацию о книгах из файла "books.csv"
        return 1; // Если что-то пошло не так, выходим из программы с ошибкой

    while (1) // Бесконечный цикл для работы программы
    {
        int command = read_command(); // Получаем от пользователя номер желаемого действия

        switch (command) // Обрабатываем выбранное пользователем действие
        {
        case 1:
            add_book(); // Добавляем книгу
            break;
        case 2:
            delete_book(); // Удаляем книгу
            break;
        case 3:
            find_and_print_book(); // Находим и выводим информацию о книге
            break;
        case 4:
            print_all_books(); // Выводим информацию обо всех книгах
            break;
        case 5:
            return save("books.csv"); // Сохраняем информацию о книгах в файл и выходим из программы
        default:
            printf("Invalid command number\n"); // Если выбрано неверное действие, сообщаем об ошибке
            break;
        }
    }

    return 0; // Возвращаем 0, если программа завершена успешно
}
