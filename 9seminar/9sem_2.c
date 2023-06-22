#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

void mix_files(const char *file1_name, const char *file2_name) 
{
    FILE *file1 = fopen(file1_name, "r");
    FILE *file2 = fopen(file2_name, "r");

    //если что то не так с файлом 
    if (file1 == NULL || file2 == NULL) 
    {
        printf("Error opening files.\n");
        return;
    }

    char line1[MAX];
    char line2[MAX];

    // Флаги окончания файлов
    int end1 = 0, end2 = 0;

    // Чтение строк и вывод 
    while (!end1 || !end2) 
    {
        // Чтение строки из первого файла
        if (fgets(line1, sizeof(line1), file1) != NULL && !end1) 
        {
            printf("%s", line1);
        } 
        else 
        {
            end1 = 1; // Установка флага окончания первого файла
        }
        
        // Чтение строки из второго файла
        if (fgets(line2, sizeof(line2), file2) != NULL && !end2) 
        {
            printf("%s", line2);
        } 
        else 
        {
            end2 = 1; // Установка флага окончания второго файла
        }
    }

    fclose(file1);
    fclose(file2);
}

int main() 
{
    char file1_name[MAX];
    char file2_name[MAX];

    printf("Enter first file name: ");
    scanf("%1023s", file1_name);

    printf("Enter second file name: ");
    scanf("%1023s", file2_name);

    mix_files(file1_name, file2_name);

    return 0;
}