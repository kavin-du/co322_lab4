#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]){
    char* restOfWord = "";
    char c = 'r';

    strncat(&restOfWord, &c, 1);

    printf("%s\n", restOfWord);

    return 0;
}
