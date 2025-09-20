#include <stdio.h>
#include <stdlib.h>
#include "main.h"
void trans_symbols(const char *str){
    int i = 0;
    while(str[i] != '\0'){
        scanf("%c", str[i]);
        i = rand() % 8;
        i++;
    }
}
void hex_coding(const char *str){
    int i = 0;
    while(str[i] != '\0'){
        scanf("%c", str[i]);
        i++;
    }
}
