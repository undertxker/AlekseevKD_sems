#include <stdio.h>

void horiz(int digits[], int length) {
    puts("Horizontal gystigram.");
    for(int i = 0; i < 10; ++i) {
        printf("%d: ", i);
        for(int j = 0; j < length; ++j) {
            if (digits[j] == i) {
                printf("#");
            }
        }
        printf("\n");
    }
}

void vertical(int digits[], int length) {
    puts("Vertical gystigram.");
    int count = 0;
    int max_height = 0;
    for (int j = 0; j < 10; ++j) {             
//ищем высоту масимального столбика
        for (int i = 0; i < length; ++i) {
            if (j == digits[i]) {
                count += 1;
            }
        }
        if (count > max_height) {
            max_height = count;
        }
        count = 0;
    }
    count = 0;
    for (int i = max_height; i > 0; --i) {
        for (int k = 0; k < 10; ++k) {
            for (int j = 0; j < length; ++j) { 
//считаем количество символов '0' , '1' и тд по отдельности
                if (digits[j] == k) {
                    count += 1;
                }
            }
            if (i>count) { 
//если строка, которую мы заполняем, имеет больший номер, чем количество элементов, которое мы посчитали, то выводим ' ',' '
                putchar(' ');
                putchar(' ');
            }
            else {
                putchar('#');
                putchar(' ');
            }
            count = 0;
        }
        putchar('\n');
    }
    puts("0 1 2 3 4 5 6 7 8 9");
}

int task3() {
    int digits[100];
    int length = 0;
    char input;
    int i = 0;
    puts("Input string with letters and nums.");
    while ((input = getchar()) != '\n' && i < 100) { 
// ввод строки из букв и цифр, остаются только цифры
        if (input >= '0' && input <= '9') {
            digits[i++] = input - '0';
            length = i;
        }
    }
    if (i>=100) {
        puts("ERROR. TOO many symbols.");
        return 0;
    }
//горизонтальная
    horiz(digits, length);
//вертикальная
    vertical(digits, length);
    return 0;
}