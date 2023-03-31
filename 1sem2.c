#include <stdio.h>

int Length(char line[]);
void Output(char line[]);
void Starline(char line[]);

int main(void)
{
    char name[100];
    printf("Enter your name: ");
    scanf("%s", name);
    Output(name);
    return 0;
}


/* получаем имя, которое в последствии будем "окружать" звездочками */


int Length(char line[])
{
    int len = 0;
    if (line != '\0')
    {
        while (line[len] != '\0')
        {
            len++;
        }
    }
    return len;
}


/* считаем длину имени */


void Output(char line[])
{
    Starline(line);
    printf("* %s *\n", line);
    Starline(line);
}


/* функция вывода имени в рамке из звездочек */


void Starline(char line[])
{
    int len = Length(line);
    int i = 0;
    while (i<(len+4))
    {
        printf("*");
        i++;
    }
    printf("\n");
}


/* создание и вывод строчки звездочек */