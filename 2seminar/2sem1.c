#include "2sem1.h"

void task1()
{
    printf("Enter the string: ");
    int a;
    a = getchar();
    while (a != '\n')
    {
        if (a!=' ' && a!='\t')
        {
            putchar(a);
        }
        a = getchar();
    }

}

/*  получаю строку символов, создаю цикл, в котором если элемент - это символ,
 то я его вывожу, если же это пробел или таб - пропускаю  */