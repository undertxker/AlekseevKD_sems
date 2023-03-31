#include <stdio.h>

int main(void)
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
    return 0;
}

/*  получаю строку символов, создаю цикл, в котором если элемент - это символ,
 то я его вывожу, если же это пробел или таб - пропускаю  */