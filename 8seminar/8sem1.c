#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *input_string(char *string) {
    char input;
    for (int i = 0; (input = getchar()) != '\n'; ++i) {
        string = (char *) realloc(string, sizeof(char) * (i + 1));
        string[i] = input;
        string[i + 1] = '\0';
    }
    return string;
}

int strend(char *s, char *t) 
{
    int length_s = strlen(s); //строка
    int length_t = strlen(t); //подстрока

    int j = 0;
    for (int i = length_s - length_t; i <= length_s; ++i) 
    {
        if (s[i] != t[j]) return 0;
        ++j;
    }
    return 1;
}


int main()
{
    char* s = (char*) malloc (sizeof(char));
    char* t = (char*) malloc (sizeof(char));

    puts("Enter s: ");
    s = input_string(s);
    puts("Enter t: ");
    t = input_string(t);

    printf("The answer is: %d", strend(s, t));
}