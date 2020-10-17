#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strcat()
#include <ctype.h> // for tolowercase()

// size of the alphabet
#define CHAR_SET 27  // ??????????????? // there is an apostrophe
#define SIZE_OF_SINGLE_WORD 25 // 25
#define WORDS_IN_FILE 10000


typedef struct trienode{  // change structure according to the assignment
	//char data;
	int isWord; // this is is leaf, check if can reduce for loops
	struct trienode* character[CHAR_SET];
}TrieNode;

// 
TrieNode* createNode(){
	// allocating memory for new node
	TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
	
	// making all children null
	for(int i=0; i<CHAR_SET; i++){
		newNode->character[i] = NULL;
	}
	
	// intially not a word
	newNode->isWord = 0;
	
	return newNode;
}

void insertWord(TrieNode* head, char* word){
	TrieNode* traveler = head;
	while(*word){
		if(traveler->character[*word -'a'] == NULL){
			traveler->character[*word -'a'] = createNode();
			//traveler->character[*word -'a']->data = *word;
		}
		
		// move to next node
		traveler = traveler->character[*word-'a'];
		
		// move to next characer
		word++;
	}
	// mark last node , need to add data ????????
	traveler->isWord = 1;
}

// not need, not necessasy but good to have
int searchWord(TrieNode* head, char* word){
	if(head == NULL) return 0;

	TrieNode* traverler = head;

	while(*word){

		traverler = traverler->character[*word-'a'];

		if(traverler == NULL) return 0;

		word++;
	}
	return traverler->isWord;
}

// return type differnt in the assignment
void printSuggestions(TrieNode* head, char* wordPart){
	
	if(head == NULL) {
		printf("Head pointer is null, no words found");
	}
	
	TrieNode* traveler = head;

	char* originalWord = wordPart; // because travel changes

	while(*wordPart){
		if(traveler->character[*wordPart-'a'] == NULL){
			printf("No words found!!\n");
			return;
		}
		
		// move to next node
		traveler = traveler->character[*wordPart-'a'];
		
		// move to next character
		wordPart++;
	}

	int isEmpty = 1;
	for(int i=0; i<CHAR_SET; i++){
		if(traveler->character[i] != NULL){
			isEmpty = 0;
			break;
		}
	}

	char empty[100];
	if(isEmpty) printf("%s\n", originalWord);
	else{
		strcpy(empty, originalWord);
		int stringlength = strlen(empty);
		//printf("%s ----- length=%d\n", empty, stringlength);
		printNode(traveler, empty, stringlength);
	} 
	
}

void printNode(TrieNode* node, char* restOfWord, int pos){
	if(node == NULL) return;
	if(node->isWord){
		// restOfWord[pos] = '\0';
		// printf("%s", restOfWord);
		for(int i=0; i<pos; i++){
			printf("%c", restOfWord[i]);
		}
		printf("\n");
	}

	for(int i=0; i<CHAR_SET; i++){
		if(node->character[i] != NULL){
			restOfWord[pos] = i+'a';
			printNode(node -> character[i], restOfWord, pos+1);
		} 
	}
}

void readFile(char* path, char** array){
	FILE* file;
	char word[SIZE_OF_SINGLE_WORD];

	file = fopen(path, "r");
	if(file == NULL) {
		printf("Error reading the file. Check if the path is correct.\n");
		return;
	}
	int i=0;
	while(fgets(word, SIZE_OF_SINGLE_WORD, file) != NULL){
		char* token;
		token = strtok(word, "\n");
		char temp[SIZE_OF_SINGLE_WORD];
		strcpy(temp, token);
		toLowerCase(temp);
		strcpy(array[i], temp); // removing new line character
		i++;
		// if(i==WORDS_IN_FILE) break;
		//printf("%s", word);
	}
	fclose(file);
}

void toLowerCase(char* word){
    for(int i=0; word[i] != '\0'; i++){
        word[i] = tolower(word[i]);
    }
}

int main(){
	
	TrieNode* head = createNode();
	/*
	char words[][10] = {"hello", "who", "are", "helloww", "you"};

	for(int i=0; i<5; i++){
		insertWord(head, words[i]);
	}*/

	//printf("%s\n", searchWord(head, "hwll") ? "found" : "not found");
	


	int numOfWords = WORDS_IN_FILE; 

	char** wordList = malloc(2*numOfWords*sizeof(char*));
	for(int i=0; i<2*numOfWords; i++){
		wordList[i] = malloc(SIZE_OF_SINGLE_WORD*sizeof(char));
	}

	char* filePath = "./wordlist/wordlist10000.txt"; // this path not work in windows
	// char* filePath = "./wordlist/wordlist500.txt";
	// char* filePath = "./wordlist/test.txt"; // this path not work in windows
	readFile(filePath, wordList);

	for(int i=0; i<numOfWords; i++){
		insertWord(head, wordList[i]);
		// printf("%s\n", wordList[i]);
	}
	// char empty[100];
	// printNode(head, empty, 0);

	char* w = "be"; // need lowercase? 
	// printf("%s\n", searchWord(head, w) ? "found" : "not found");
	printSuggestions(head, w);
}
