#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

void tail(const char *name_file, int n) {
    FILE *file = fopen(name_file, "r"); 
    if (file == NULL) //если траблы с файлом 
    { 
        printf("Error opening file.\n");
        return;
    }
    char *buffer = malloc(n * MAX * sizeof(char)); // выделение памяти 
    int count = 0; 
    while (fgets(buffer + (count % n) * MAX, MAX, file) != NULL) // чтение строк и их сохранение
    { 
        count++;
    }
    fclose(file);
    int start;
    if (count > n)  // если строк больше чем надо
    {
        start = count % n; // первая строка для вывода будет считаться с конца 
    } 
    else 
    {
        start = 0; // если строк меньше, то вывод с начала
    }
    for (int i = start; i < start + n && i < count; i++)  // вывод n последних строк начиная со start
    {
        printf("%s", buffer + (i % n) * MAX);
    }
    free(buffer); //освобождение памяти
}

int main() {
    char name_file[MAX]; 
    int n;
    printf("Enter file name: ");
    scanf("%s", name_file); 
    printf("Enter number of lines: ");
    scanf("%d", &n); 

    tail(name_file, n); 

    return 0; 
}