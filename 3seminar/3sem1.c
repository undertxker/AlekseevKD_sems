#include <stdio.h>
#include <stdlib.h>

// Функция для ввода строки, в которой реализовано изменение размера выделенной памяти
void input_string(char *string) {
    char input;  
// Переменная для считывания посимвольно
    for (int i = 0; (input = getchar()) != '\n'; ++i) {
        string = (char *) realloc(string, sizeof(char) * (i + 1));
        string[i] = input;
        string[i + 1] = '\0';
    }
}

// Функция записывает значение в expanded_string, перед этим расширяет выделенную память. Для этого передается счетчик k
void input_value(int k, char value, char *expanded_string) {
    expanded_string = (char *) realloc(expanded_string, sizeof(char) * (k + 1));
    expanded_string[k] = value;
    expanded_string[k + 1] = '\0';
}

// Функция для развертки строки формата 'a-z'
void expand(char *string, char *expanded_string) {
    char lower_value, upper_value = 0; 
// Переменные "от", "до"
    int count = 0; 
// Отвечает за количество элементов, которые необходимы для развертывания
    int k = 0; 
// Счетчик
    for (int j = 0; string[j] != '\0'; ++j) {

// '-' перед
        if (string[0] == '-' && j == 0) {
            for (j = 0; string[j] == '-'; ++j) {
                input_value(k, string[j], expanded_string);
                ++k;
            }
        }

/* Если находим символ != '-', то записываем его в переменную upper_value,
* предварительно присвоив lower_value старый upper_value */
        if (string[j] != '-') {
            lower_value = upper_value;
            upper_value = string[j];
            count += 1;
        }

// Если есть 2 элемента, то в expanded_string[k] записываются значения
        if (count == 2) {
// Если нижнее значение имеет меньшее значение, чем верхнее
            if (lower_value < upper_value) {
                for (char value = lower_value; value <= upper_value; ++value) {
                    input_value(k, value, expanded_string);
                    ++k;
                }
                count = 0;
            }
// Если нижнее значение имеет большее значение, чем верхнее
            else {
                for (char value = lower_value; value >= upper_value; --value) {
                    input_value(k, value, expanded_string);
                    ++k;
                }
                count = 0;
            }

/* После записи значений в expanded_string проверяем 2 следующих символа в string.
* Если оба '-', то выходим из цикла. Перед выходом дополняем строку символами '-' */
            if (string[j + 1] == '-' && (string[j + 2] == '-' || string[j + 2] == '\0')) {
                for (int i = j + 1; string[i] != '\0'; ++i) {
                    input_value(k, '-', expanded_string);
                    ++k;
                }
                break;
            }

/* После того, как мы записали значения в expanded_string, проверили 2 следующих символа в string.
* Если '-', то 1) отнимаем от j единицу: таким образом впоследствии мы находим новое
* нижнее значение. 2) отнимаем от k единицу, чтобы лишний раз <C> (нижнее значение) не выводить:
* (-a-c-f- -> abc<C>def) */
            if (string[j + 1] == '-') {
                j -= 1;
                k -= 1;
            }
        }
    }
}

int task1() {

    // Создание указателей на динамические массивы, выделение памяти
    char *string1;
    string1 = (char *) malloc(sizeof(char) * 1);
    char *string2;
    string2 = (char *) malloc(sizeof(char) * 1);


    puts("Input string. (such as '-a-z-'/'a-z')");

    input_string(string1);

    expand(string1, string2);

    printf("%s", string2);


    // Освобождаем память
    free(string1);
    free(string2);

    return 0;
}