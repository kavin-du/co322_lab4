#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE_OF_SINGLE_WORD 25 

void toLowerCase(char* word){
    for(int i=0; word[i] != '\0'; i++){
        word[i] = tolower(word[i]);
    }
}

char* copyFromGivenIndex(char* word, int length, int index){
    //printf("sizeOf word: %d\n", strlen(word));
    static char newWord[SIZE_OF_SINGLE_WORD];
    int i;
    for(i=index; i<length; i++){
        newWord[i-index] = word[i];
    }
    newWord[length-index] = '\0';
    return newWord;
}

char* test(char* word){
    char* temp = "fuckyou";
    word = temp;
    return word;
}
int main(){
    char new[SIZE_OF_SINGLE_WORD];
    char* word = "fuck fuck";

    strcpy(new, copyFromGivenIndex(word, strlen(word), 3));
    printf("%s\n", new);

    return 0;
}
