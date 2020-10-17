#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strcat()

// size of the alphabet
#define CHAR_SET 26


struct TrieNode{
	char data;
	int isWord; // this is is leaf, check if can reduce for loops
	struct TrieNode* character[CHAR_SET];
};

// 
struct TrieNode* createNode(){
	// allocating memory for new node
	struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
	
	// making all children null
	for(int i=0; i<CHAR_SET; i++){
		newNode->character[i] = NULL;
	}
	
	// intially not a word
	newNode->isWord = 0;
	
	return newNode;
}

void insertWord(struct TrieNode* head, char* word){
	struct TrieNode* traveler = head;
	while(*word){
		if(traveler->character[*word -'a'] == NULL){
			traveler->character[*word -'a'] = createNode();
			traveler->character[*word -'a']->data = *word;
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
int searchWord(struct TrieNode* head, char* word){
	if(head == NULL) return 0;

	struct TrieNode* traverler = head;

	while(*word){

		traverler = traverler->character[*word-'a'];

		if(traverler == NULL) return 0;

		word++;
	}
	return traverler->isWord;
}


void printSuggestions(struct TrieNode* head,char* wordPart){
	
	if(head == NULL) {
		printf("Head pointer is null, no words found");
	}
	
	struct TrieNode* traveler = head;

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

void printNode(struct TrieNode* node, char* restOfWord, int pos){

	if(node->isWord){
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
int main(){
	
	struct TrieNode* head = createNode();

	char words[][10] = {"hello", "who", "are", "helloww", "you"};

	for(int i=0; i<5; i++){
		insertWord(head, words[i]);
	}

	//printf("%s\n", searchWord(head, "hwll") ? "found" : "not found");
	//char empty[CHAR_SET] = "";
	// printNode(head, empty, 0);
	printSuggestions(head, "hellowww");
}
