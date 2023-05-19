#include <stdio.h> 
#include <stdlib.h> 

#define MAX 100

int getline(char s[]) {
    int k, i;
    for(i = 0; i < MAX-2 && (k = getchar()) != '\n'; i++) { 
        s[i] = k;
    }
    s[i] = '\n';
    s[++i] = '\0';
    return i;
}   