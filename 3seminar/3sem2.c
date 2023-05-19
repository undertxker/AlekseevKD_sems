#include <stdio.h>
#include <stdlib.h>

void output(char s[]) {
    for(int i = 0; s[i] != '\0'; ++i) {
        printf("%c", s[i]);
    }
    printf("\n");
}

void string(char *string) {
    char input;  
// Переменная для считывания посимвольно
    for (int i = 0; (input = getchar()) != '\n'; ++i) {
        string = (char *) realloc(string, sizeof(char) * (i + 1));
        string[i] = input;
        string[i + 1] = '\0';
    }
}

void delete_s2_from_s1(int length_s2, char *s1, int j) {
// передвигаем значения на длину s2
    int l;
    for (l = j - length_s2 + 1; s1[l+length_s2] != '\0'; ++l) {
        s1[l] = s1[l+length_s2];
    }
    s1[l] = '\0';
}

int task2() {
    char *s1, *s2;
    s1 = (char *) malloc(sizeof(char) * 1);
    s2 = (char *) malloc(sizeof(char) * 1);
    puts("Input s1.");
    string(s1);
    puts("Input s2.");
    string(s2);
    int length_s2;

// узнаем длину
    for(int i = 0; s2[i] != '\0'; ++i)
        length_s2 = i + 1;

    int k = 0; 
// сколько символов подряд совпало с s2
    for(int j = 0; s1[j] != '\0'; ++j) {
// перебираем строку s1
        if (s1[j] != s2[k]) {
            k = 0;
            continue;
        }
        ++k;
        if (k == length_s2) {
            delete_s2_from_s1(length_s2, s1, j);
            k = 0;
            j = j - length_s2;
        }
    }

    puts("Changed s1.");
    output(s1);
    puts("s2.");
    output(s2);

    return 0;
}