#include <stdio.h>

#define SWAP(type, a, b) type tmp = a; a = b; b = tmp;

int main(void)
{
    int x, y;
    scanf("%d", &x);
    scanf("%d", &y);
    SWAP(int, x, y);
    printf("%d %d", x, y);
    return 0;
}