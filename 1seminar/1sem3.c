#include "1sem3.h"

void Multiplication(int x)
{
    printf("\n");
    for (int i = 1; i < x+1; i++)
    {
        for (int j = 1; j < x+1; j++)
        {
            printf("%d\t", i*j);
        }
        printf("\n");
    }
}


/* создаю двумерный массив 123456789.... и перемножаю числа с одинаковыми координатами */ 


void task3()
{
    int size;
    printf("Enter the size of your table: ");
    scanf("%d", &size);
    Multiplication(size);

}


/* вывод */