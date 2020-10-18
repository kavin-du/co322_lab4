#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strcat()
#include <ctype.h> // for tolowercase()

// size of the alphabet
#define CHAR_SET 27  // there are non aphabetical characters in the text file
#define SIZE_OF_SINGLE_WORD 25 // 25
#define WORDS_IN_FILE 10000 // need to change this if the file name is changed


typedef struct trienode{ 
	int isWord; // to determine if the node is endpoint of a word
	struct trienode* character[CHAR_SET];
}TrieNode;


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
	// copying head node to a temporary node
	TrieNode* traveler = head;

	while(*word){
		// if the target node is null, assign a new node
		if(traveler->character[*word -'a'] == NULL){
			traveler->character[*word -'a'] = createNode();
		}
		
		// move to next node
		traveler = traveler->character[*word-'a'];
		
		// move to next characer
		word++;
	}
	// mark last node as a word
	traveler->isWord = 1;
}

// this function can check if a given word is exist in the trie
// It was implemented for dubugging purposes
int searchWord(TrieNode* head, char* word){
	// if the tree is not exist, return
	if(head == NULL) return 0;

	// assign head node to temporary node
	TrieNode* traverler = head;

	while(*word){
		// move to next node
		traverler = traverler->character[*word-'a'];

		// if the current node is empty, there is no word
		if(traverler == NULL) return 0;

		// move to next character
		word++;
	}
	// came to the end character, check if the node is marked as a word
	return traverler->isWord;
}

// priting suggestions for a given part of a word
void printSuggestions(TrieNode* head, char* wordPart){
	
	// head is null when tree does not exist
	if(head == NULL) {
		printf("Tree is empty.\n");
		return;
	}
	
	// assigning head node to temporary node
	TrieNode* traveler = head;

	// saving original word, because we incrementing the pointer
	char* originalWord = wordPart;

	while(*wordPart){

		// if the character not exist in the trie, there is no word matching
		if(traveler->character[*wordPart-'a'] == NULL){
			printf("No words found!!\n");
			return;
		}
		
		// move to next node
		traveler = traveler->character[*wordPart-'a'];
		
		// move to next character
		wordPart++;
	}

	// checking if the last node has at least one child exist,
	// if threre is one child exist, it is not empty
	int isEmpty = 1;
	for(int i=0; i<CHAR_SET; i++){
		if(traveler->character[i] != NULL){
			isEmpty = 0;
			break;
		}
	}

	char empty[100];
	if(isEmpty) printf("%s\n", originalWord); // if the last node is empty, the given word is the matching word
	else{
		strcpy(empty, originalWord); // copying orignal word to pass to the function
		int stringlength = strlen(empty); // length of the orignal word
		printNode(traveler, empty, stringlength); // print the child nodes of the last node
	} 
	
}

// printing all the childs as seperate words for a given node
// Not necessary to be the head node
void printNode(TrieNode* node, char* restOfWord, int pos){
	if(node == NULL) return; // return to caller if the node is null

	// if the node is marked as a word, then print the parent nodes
	// restOfWord stores the characters of the parent nodes
	if(node->isWord){
		for(int i=0; i<pos; i++){
			printf("%c", restOfWord[i]);
		}
		printf("\n");
	}

	// checking all the child nodes, if they are not null
	// append the node character to the array and increment array position
	// then call the function recursively
	for(int i=0; i<CHAR_SET; i++){
		if(node->character[i] != NULL){
			restOfWord[pos] = i+'a';
			printNode(node -> character[i], restOfWord, pos+1);
		} 
	}
}

// function for reading the text file
void readFile(char* path, char** array){
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
		token = strtok(word, "\n"); // removing new line characters from the word
		char temp[SIZE_OF_SINGLE_WORD];
		strcpy(temp, token); // copy the sanitized word to new word
		toLowerCase(temp); // convert to lowercase
		strcpy(array[i], temp); // store in the array in the main function
		i++;
	}
	fclose(file); // close the file
}

// function for converting a given word into lowercase
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


	// dynamically allocated array of char pointers to store the words from file
	char** wordList = malloc(WORDS_IN_FILE*sizeof(char*));
	for(int i=0; i<WORDS_IN_FILE; i++){
		wordList[i] = malloc(SIZE_OF_SINGLE_WORD*sizeof(char));
	}

	char* filePath = "./wordlist/wordlist10000.txt"; // this path not work in windows
	// char* filePath = "./wordlist/test.txt"; // this path not work in windows
	readFile(filePath, wordList);

	for(int i=0; i<WORDS_IN_FILE; i++){
		insertWord(head, wordList[i]); // insert words to the trie
		// printf("%s\n", wordList[i]);
	}

	// print the whole dictionary 
	// char empty[100];
	// printNode(head, empty, 0);

	char* w = "fuc"; // need lowercase? 
	// printf("%s\n", searchWord(head, w) ? "found" : "not found");
	printSuggestions(head, w); // printing the suggestions for a given word
}
