#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <math.h>

#include "kek.h" 

#define MAXOP 100  

int main() { 
    int dig = 0, len_of_s; 
    double op2; 
    char s[MAXOP]; 
    len_of_s = getline(s);
    for(int i = 0; i < len_of_s; i++) {    
        switch (s[i]) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
             dig += s[i]-'0';
             while (isdigit(s[++i])) 
                dig = dig * 10 + s[i]-'0';
             push (dig);
             dig = 0; 
             break;
            case '+': 
             push (pop() + pop()); 
             break; 
            case '*': 
             push (pop() * pop()); 
             break; 
            case '-': 
             op2 = pop(); 
             push (pop() - op2); 
             break; 
            case '/' : 
             op2 = pop(); 
             if (op2 != 0.0) {
                push (pop() / op2);
            } else {
                printf("error: zero divisor\n"); 
                return -1; 
                }
            break;
            case '%' : 
             op2 = pop();
             if (op2 != 0.0) {
                push ((int)(pop() / op2));}
             else {
                printf("error: zero divisor\n"); 
                return -1; 
                }
                break;
            case 's' :
             push (sin(pop()));
             break;
            case 'c' :
             push (cos(pop()));
             break;
            case '^' :
             op2 = pop();
             push (pow(pop(), op2));
            case ' ' :
             break;
            case '\n' : 
             printf("\t%.8g\n", pop()); 
             break; 
            default: 
             printf("error: unknown command %s\n", s); 
             return -1; 
        } 
    } 
    return 0; 
}
