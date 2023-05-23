#include <stdio.h>
#include <stdlib.h>

char* read_string() {
    char* str = NULL;
    
    int size = 0;
    int c;

    while (c = getchar(), c != '\n') {
        str = realloc(str, ++size * sizeof(char));

        if (str == NULL) 
            return str;

        str[size-1] = c;
    }

    str = realloc(str, ++size * sizeof(char));
    str[size-1] = '\0';

    return str;
}

int main() {
    puts("enter string");

    char* str = read_string();

    printf("String:\n%s", str);
    free(str);
        
    return 0;
}