#include <stdio.h>
#include <string.h>
#include <ctype.h>

void toLowerCase(char* word){
    for(int i=0; word[i] != '\0'; i++){
        word[i] = tolower(word[i]);
    }
}

int main(){
    char* w = "FuckYou"; // fuck you this is read only fucker
    // char w[20] = "FuckYou";
    char temp[50];
    strcpy(temp, w);

    toLowerCase(temp);
    
    printf("%s\n", temp);

    return 0;
}
