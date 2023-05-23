#include <stdio.h>
#include <stdlib.h>

void print_mas(int *list)
{
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", list[i]);
    }
}

void shell_sort (int mas[], int n)
{
    int gap = n / 2;
    while (gap > 0) 
    {
        for (int i = gap; i < n; i++)
        {
            int temp = mas[i];
            int j;
            for (j = i; j >= gap && mas[j - gap] > temp; j -= gap) 
            {
                mas[j] = mas[j - gap];
            }
            mas[j] = temp;
        }
        gap /= 2;
    }
}

int main(void)
{
    int k;
    puts("Enter amount of numbers: ");
    scanf("%d", &k);
    int *mas = (int*) malloc(k * sizeof(int));
    for (int i = 0; i < k; i++)
    {
        scanf("%d", &mas[i]);
    }
    
    shell_sort(mas, k);

    print_mas(mas);
    free(mas);
    return 0;
}