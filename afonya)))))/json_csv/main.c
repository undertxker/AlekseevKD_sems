#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

#define DELIMITER ";" // Разделитель, используемый с csv файлах

// Структура, описывающая элемент данных
typedef struct {
    cJSON *json;  // Указатель на cJSON объект
} DataItem;

// Рекурсивная функция записи заголовков
void rec_write_header(cJSON *cjson, FILE *file, int end) {
    int flag = 1;

// Обходим всех потомков текущего cJSON объекта
    if (cjson->child != NULL) {
        flag = 0;
        rec_write_header(cjson->child, file, 0);
    }
    if (cjson->next != NULL) {
        rec_write_header(cjson->next, file, 0);
    }

// Если потомков нет, выводим название текущего cJSON объекта
    if (flag) {
        if (end)
            fprintf(file, "%s", cjson->string);
        else fprintf(file, "%s%s", cjson->string, DELIMITER);
    }
}

// Рекурсивная функция записи основных данных
void rec_write_data(cJSON *cjson, FILE *file, int end) {
    int flag = 1;

// Обходим всех потомков текущего cJSON объекта
    if (cjson->child != NULL) {
        flag = 0;
        rec_write_data(cjson->child, file, 0);
    }
    if (cjson->next != NULL) {
        rec_write_data(cjson->next, file, 0);
    }

    // Если потомков нет, записываем значение текущего cJSON объекта
    if (flag) {
        if (cjson->type == cJSON_String) {
            fprintf(file, "%s", cjson->valuestring);
        } else if (cjson->type == cJSON_Number) {
            double value = cjson->valuedouble;
            fprintf(file, "%g", value);
        }
        if (!end)
            fputs(DELIMITER, file);
    }
}

int main(int argc, char *argv[]) {
    // Открываем файл, переданный пользователем
    FILE *inputFile = fopen(argv[1], "r");

    // Проверяем открылся ли
    if (inputFile == NULL) {
        printf("Error opening %s file.\nUse Parser <filename>", argv[1]);
        return 1;
    }

    // Определяем размер файла
    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

// Выделяем память для строки, содержащей данные файла
    char *jsonString = (char *) malloc((fileSize + 1) * sizeof(char));
    
// Считываем файл в строку
    fread(jsonString, sizeof(char), fileSize, inputFile);
    jsonString[fileSize] = '\0';

    fclose(inputFile);

    // Парсим строку в cJSON объект
    cJSON *root = cJSON_Parse(jsonString);

    // Проверяем cпарсилась ли
    if (root == NULL) {
        printf("Error parsing JSON string.\n");
        free(jsonString);
        return 1;
    }

    // Проверяем соответствует ли json стандарту
    if (!cJSON_IsArray(root)) {
        printf("Error: Root JSON structure should be an array.\n");
        cJSON_Delete(root);
        free(jsonString);
        return 1;
    }

    // Разбиваем на массив элементов
    int numItems = cJSON_GetArraySize(root);
    DataItem *items = (DataItem *) malloc(numItems * sizeof(DataItem));

    for (int i = 0; i < numItems; i++) {
        cJSON *item = cJSON_GetArrayItem(root, i);
        items[i].json = item;
    }

    // Открываем выходной файл на запись
    FILE *csvFile = fopen("output.csv", "w");

    // Проверяем открылся ли
    if (csvFile == NULL) {
        printf("Error opening the output.csv file.\n");
        cJSON_Delete(root);
        free(items);
        free(jsonString);
        return 1;
    }

    // Записываем заголовки
    rec_write_header(items[0].json->child, csvFile, 1);
    fprintf(csvFile, "\n");

    // Записываем основные данные
    for (int i = 0; i < numItems; i++) {
        rec_write_data(items[i].json->child, csvFile, 1);
        fprintf(csvFile, "\n");
    }

    //Закрываем файл
    fclose(csvFile);

    printf("Data written to output.csv successfully.\n");

    // Очищаем память
    cJSON_Delete(root);
    free(items);
    free(jsonString);

    return 0;
}