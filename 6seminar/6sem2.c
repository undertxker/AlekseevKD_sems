#include <stdio.h>
#include <stdbool.h>

#define SWAP(type, a, b) type tmp = a; a = b; b = tmp;

void sort(int list[], int listLength)
{
	while(listLength--)
	{
		bool swapped = false;
		
		for(int i = 0; i < listLength; i++)
		{
			if(list[i] > list[i + 1])
			{
				SWAP(int, list[i], list[i + 1]);
				swapped = true;
			}
		}
		
		if(swapped == false)
			break;
	}
}


int bisearch(int value, int* mas, int min, int max)
{
    if ( max < min ) return -1;
    int midpoint = ( min + max ) / 2;  
    if ( mas[midpoint] > value ) return bisearch(value, mas, min, midpoint - 1 );
    if ( mas[midpoint] < value ) return bisearch(value, mas, midpoint + 1, max);
    if ( value == mas[midpoint] ) return 0;
    return -1;
}

int main(void)
{
    int mas[] = {20, 62, 34, 7, 96, 38, 87, 20, 42, 89, 7, 59, 45, 18, 5, 97, 43, 97, 37, 32}; //рандомный массив
    int s = (sizeof(mas) / sizeof(mas[0]));
    sort(mas, s);
    int search;
    printf("Enter searching element: ");
    scanf("%d", &search);
    if (bisearch(search, mas, 0, s) == 0) printf("%d was found\n", search);
    else printf("%d wasn't found\n", search);
    return 0;
}