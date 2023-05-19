#include <stdio.h>

int find_char(char s1[], char s2[]) {
    int i, j;
    for(i=0; s1[i]!='\0'; i++) {
        for(j=0; s2[j]!='\0'; j++){
            if(s2[j] == s1[i]) {
            return i;
            }
        }
    }
    return -1;
}


    