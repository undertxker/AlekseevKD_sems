#include "1sem2.h"

void task2()
{
    char name[100];
    printf("\nEnter your name: ");
    scanf("%s", name);
    Output(name);

}


/* получаем имя, которое в последствии будем "окружать" звездочками */


int Length(char line[])
{
    int len = 0;
    while (line[len] != '\0')
    {
        len++;
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