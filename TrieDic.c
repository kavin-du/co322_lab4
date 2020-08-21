#include <stdio.h>
#include <stdlib.h>

#define CHAR_SET 26


struct TrieNode{
	char data;
	int isWord;
	struct TrieNode* character[CHAR_SET];
	
};

struct TrieNode* createNode(){
	struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
	for(int i=0; i<CHAR_SET; i++){
		newNode->character[i] = NULL;
	}
	newNode->isWord = 0;
	
	return newNode;
}

void insertWord(struct TrieNode* head,char* word){
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

int printSuggestions(struct TrieNode* head,char* wordPart){
	struct TrieNode* traveler = head;
	while(*wordPart){
		if(traveler->character[*wordPart-'a'] == NULL){
			//printf("No words found!!");
			return 0;
			break;
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
		}
	}
	
}

void printNode(struct TrieNode* node){
	if(node == NULL){
		return;
	}
	for(int i=0; i<CHAR_SET; i++){
		printf("%c", node -> data);
		printNode(node -> character[i]);
	}
}
int main(){
	
	
	
	
}