#include <stdio.h>

void qsort (int *mas, int a, int b) {
    int left = a, right = b;
    int pivot = mas[(left + right)/2];
	do {
		while (mas[left] < pivot) {
			left++;
		}
		while (mas[right] > pivot) {
			right--;
		}
		if (left <= right) {
			if (mas[left] > mas[right]) {
				int tmp = mas[left];
				mas[left] = mas[right];
				mas[right] = tmp;
			}
			left++;
			if (right > 0) {
				right--;
			}
		}
	} while (left <= right);

	if (left < b) {
		qsort(mas, left, b);
	}
	if (right > a) {
		qsort(mas, a, right);
	}
}

int main() {
    int array[] = {5, 3, 9, 7, 1, 8, 6, 4, 2, 0}; // исходный массив для сортировки
    int n = sizeof(array)/sizeof(array[0]); //число элементов массива
    qsort(array, 0, n - 1);

    for (int i = 0; i < n; i++)
    {
        printf("%d ", array[i]); 
    }
    return 0;
}