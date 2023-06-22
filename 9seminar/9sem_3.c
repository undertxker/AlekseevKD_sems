#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

void print_line(int line_num, const char *line) 
{
    printf("%d: %s", line_num, line);
}

void print_page_number(int page_num) 
{
    printf(" - Page %d -\n", page_num);
}

//разбиения текста на стр
void paginate(const char *filename, int lines_per_page, int max_line_length) 
{
    FILE *file = fopen(filename, "r");
    // если траблс с файлами
    if (file == NULL) 
    {
        printf("Error opening file.\n");
        return;
    }
    // Создание массива 
    char line[MAX];
    int line_num = 0, lines_on_page = 0, curr_page = 1;
    // читаю файл по строкам
    while (fgets(line, sizeof(line), file) != NULL) 
    {
        line_num++;
        // сколько строчек на страницк
        lines_on_page = (line_num - 1) % lines_per_page + 1;
        // Печать строки с ее №
        print_line(lines_on_page, line);
        // если это последняя строка на странице, печатаем номер страницы и увеличиваем счетчик страниц
        if (lines_on_page == lines_per_page) 
        {
            print_page_number(curr_page);
            curr_page++;
        }
    }
    fclose(file);
}

// тут кароче пользовательские параметры можно ввести 
int main() 
{
    int lines_per_page, max_line_length;
    char filename[MAX];
    printf("Enter filename: ");
    scanf("%1023s", filename);    
    printf("Enter number of lines per page: ");
    scanf("%d", &lines_per_page);
    printf("Enter max line length: ");
    scanf("%d", &max_line_length);

    paginate(filename, lines_per_page, max_line_length);

    return 0;
}