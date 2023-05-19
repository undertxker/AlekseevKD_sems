#include <stdio.h>
#include "4sem1.h"

int main() {
    char a[137];
    printf("Enter digit in 16 cc: ");
    scanf("%s", a);
    printf("10 cc: %d\n", trans(a));
    printf("count 1: %d", transsqr(trans(a)));
    return 0;
}