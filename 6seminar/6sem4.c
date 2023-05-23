#include <stdio.h>
#include <stdlib.h>


void print_mas(int *list)
{
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", list[i]);
    }
}



int *count_sort(int *mas, int amount) 
{
    int max = mas[0], min = mas[0];
    int size_count;
    int *count_mas;
    int *sorted_mas;

    for (int i = 1; i < amount; i++) {
        if (mas[i] < min)
            min = mas[i];
        else if (mas[i] > max)
            max = mas[i];
    }

    size_count = max - min + 1;

    count_mas = (int *) malloc(sizeof(int) * size_count);
    sorted_mas = (int *) malloc(sizeof(int) * amount);

    for (int i = 0; i < size_count; i++)
        count_mas[i] = 0;
    for (int i = 0; i < amount; i++)
        sorted_mas[i] = 0;

    for (int i = 0; i < amount; ++i)
        count_mas[mas[i] - min] += 1;

    int j = 0;
    for (int i = 0; i < size_count; i++) {
        while (count_mas[i] > 0) {
            sorted_mas[j] = min;
            j++;
            count_mas[i]--;
        }
        min++;
    }

    return sorted_mas;
}
// ----------------------------------------------------------

void merge(int mas[], int left, int mid, int right) { 
    int i, j; 
    int len_left = mid - left + 1; 
    int len_right = right - mid; 

    int left_arr[len_left], right_arr[len_right]; 
  
    for (i = 0; i < len_left; i++) 
        left_arr[i] = mas[left + i]; 
    for (j = 0; j < len_right; j++) 
        right_arr[j] = mas[mid + 1 + j]; 
  
    i = j = 0; 
    while (i < len_left && j < len_right) { 
        if (left_arr[i] <= right_arr[j])
            mas[left++] = left_arr[i++]; 
        else
            mas[left++] = right_arr[j++];
    } 
  
    while (i < len_left)
        mas[left++] = left_arr[i++]; 

    while (j < len_right)
        mas[left++] = right_arr[j++]; 
} 

void merge_sort(int mas[], int left, int right) { 
    if (left < right) { 
        int mid = left + (right - left) / 2; 
        merge_sort(mas, left, mid); 
        merge_sort(mas, mid + 1, right); 
        merge(mas, left, mid, right); 
    } 
} 

// ---------------------------------------------------------
int main(void)
{
    int mas[] =  {0, 3, 2, 1, 4};
    merge_sort(mas, 0, 5-1);
    print_mas(mas);
    // print_mas(count_sort(mas, 5));

    return 0;
}