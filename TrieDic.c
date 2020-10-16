#include <stdio.h>
#include <stdlib.h>

// size of the alphabet
#define CHAR_SET 26


struct TrieNode{
	char data;
	int isWord;
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
			printf("No words found!!");
			return;
			//break; // no break after return
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
	if(isEmpty) printf("%s\n", originalWord);
	else printNode(traveler);
	
}

void printNode(struct TrieNode* node){
	if(node == NULL){ // skip empty nodes
		return;
	}
	printf("%c", node -> data);

	if(node->isWord) printf("\n");
	
	for(int i=0; i<CHAR_SET; i++){
		printNode(node -> character[i]);
	}
}
int main(){
	
	struct TrieNode* head = createNode();

	char words[][10] = {"hello", "who", "are", "helloww", "you"};

	for(int i=0; i<5; i++){
		insertWord(head, words[i]);
	}

	//printf("%s\n", searchWord(head, "hwll") ? "found" : "not found");
	
	printNode(head);
	//printSuggestions(head, "hell");
}
