#include <stdio.h>
#include "5sem1.h"

int main() {
    char s1[86], s2[68];
    printf("Enter the first string: ");
    scanf("%s", s1);
    printf("Enter the second string: ");
    scanf("%s", s2);
    
    int index = find_char(s1, s2);
    if(index != -1) {
        printf("Place of symbol: %d", index);
    }
    else {
        printf("-1\n");
    }
    return 0;
}