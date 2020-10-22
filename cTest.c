#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE_OF_SINGLE_WORD 31 

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

void sanitize(char* word){
    char temp[SIZE_OF_SINGLE_WORD];
    int j=0;
    for(int i=0; i<strlen(word); i++){
        if(isalpha(word[i])){
            temp[j++] = word[i];
        }
    }
    strcpy(word, temp);
}

void readFile(char* path){
    int max = 0;
    int max2 = 0;
	FILE* file;
	char word[SIZE_OF_SINGLE_WORD]; // store readed words from the file

	file = fopen(path, "r");
	if(file == NULL) {
		printf("Error reading the file. Check if the path is correct.\n");
		return;
	}
	int i=0;
	while(fgets(word, SIZE_OF_SINGLE_WORD, file) != NULL){
		char* token;
        if((int)strlen(word) > max){
            max = (int) strlen(word);
        }
		token = strtok(word, "\n"); // removing new line characters from the word
		char temp[SIZE_OF_SINGLE_WORD];
		strcpy(temp, token); // copy the sanitized word to new word
		toLowerCase(temp); // convert to lowercase
		sanitize(temp); // remove non-alphabetic characters

        if((int)strlen(temp) > max2){
            max2 = (int) strlen(temp);
        }
		i++;
	}
    printf("max length: %d %d\n", max, max2);
	fclose(file); // close the file
}

int main(){
    // char new[SIZE_OF_SINGLE_WORD];
    // // char* word;

    // scanf("%[^\n]%*c", new);
    // sanitize(new);
    // printf("%s\n", new);

    char* filePath = "./wordlist/wordlist70000.txt"; // this path not work in windows

    readFile(filePath);

    return 0;
}
