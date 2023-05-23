#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SWAP(type, a, b) type tmp = a; a = b; b = tmp;

char *input_string(char *string) 
{
    char input;
    for (int i = 0; (input = getchar()) != '\n'; ++i) 
    {
        string = (char *) realloc(string, sizeof(char) * (i + 1));
        string[i] = input;
        string[i + 1] = '\0';
    }
    return string;
}

void shell_sort(char **mas, int amount) 
{

    int distance = amount / 2;

    while (distance >= 1) 
    {
        for (int i = 0; i < (amount - distance); i++) 
        {
            int j = i;
            while (j >= 0) 
            {
                if (strlen(mas[j]) > strlen(mas[j + distance]))
                    {
                        SWAP(char*, mas[j + distance], mas[j]);
                    }
                j--;
            }
        }
        distance /= 2;
    }
}

int main()
{
    int mas_len;
    char** mas = (char**) malloc (sizeof(char*) * mas_len);

    puts("Enter the length of the mas: ");
    scanf("%d", &mas_len);

    puts("enter the mas:");
    for (int i = 0; i < mas_len; ++i) 
    {
        mas[i] = (char*) malloc(sizeof(char));
        mas[i] = input_string(mas[i]);
    }

    shell_sort(mas, mas_len);

    puts("\nSorted by length:");

    for (int i = 0; i < mas_len; i++) 
    {
        printf("%s\n", mas[i]);
    }
    free(mas);
    return 0;
}