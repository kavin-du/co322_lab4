#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strcat()
#include <ctype.h> // for tolowercase()

#define CHAR_SET 26  // size of the alphabet
#define SIZE_OF_SINGLE_WORD 35 
#define WORDS_IN_FILE 69903 // need to change this if the file name is changed

/**
 * CO322 LAB 04 Part II
 * 
 * Radix-tree data structure for text auto-complete
 * 
 * E/16/057 - Chamith UKDK
 * 
 * 22/10/2020
 * 
 * Creted and tested on a linux environment - Ubuntu 20.04
 * 
 */


typedef struct trienode{ 
	int isWord; // to determine if the node is endpoint of a word
	struct trienode* character[CHAR_SET]; // store the children
    char label[CHAR_SET][SIZE_OF_SINGLE_WORD]; // store the each string in the chil
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

// function for copy a string from given index to end of string
char* copyFromGivenIndex(char* word, int length, int index){ 
    static char newWord[SIZE_OF_SINGLE_WORD];
    int i;
    for(i=index; i<length; i++){
        newWord[i-index] = word[i];
    }
    newWord[length-index] = '\0';
    return newWord;
}

void insertWord(TrieNode* head, char* word){
	// copying head node to a temporary node
	TrieNode* traveler = head;
	
    // if there is no word corresponding to the start letter
    if(traveler -> character[*word -'a'] == NULL){
        traveler -> character[*word -'a'] = createNode();
        strcpy(traveler -> label[*word-'a'], word); // copy the whole word
        traveler -> character[*word -'a'] -> isWord = 1;  // ???? inner node? 
        return;
    }
    
	char temp[SIZE_OF_SINGLE_WORD];
	strcpy(temp, traveler -> label[*word - 'a']); // copy the string in the node

	// comparing how much characters equal between the given word and string in the node
	int i=0;
	while(temp[i] != '\0' && word[i] != '\0'){
		if(temp[i] == word[i]) i++;
		else break;
	}
	
	// word exists but not marked as a word
	if((int)strlen(word) == (i) && (int)strlen(temp) == (i) && ((traveler -> character[*word-'a'] -> isWord)) == 0){
		(traveler -> character[*word-'a'] -> isWord) = 1; // mark as a word
		return;
	}

	// have 'do' -> insert 'doing'
	if((int)strlen(word) > (i) && (int)strlen(temp) == (i)) {
		char wordPart[SIZE_OF_SINGLE_WORD]; 
		
		// copy the rest of word
		strcpy(wordPart, copyFromGivenIndex(word, (int)strlen(word), i)); 
		insertWord(traveler -> character[*word-'a'], wordPart); // insert the rest of word
		return; 
	}

	// have 'facebook' --> insert 'face'
	if((int)strlen(word) == (i) && (int)strlen(temp) > (i)) { 
		char wordPart[SIZE_OF_SINGLE_WORD];
		strcpy(wordPart, copyFromGivenIndex(temp, (int)strlen(temp), i));
		strcpy(traveler -> label[*word-'a'], word); // truncate the existing word

		TrieNode* tempNode = traveler -> character[*word-'a'];

		// moving the child nodes to new node
		(traveler -> character[*word-'a']) = createNode();
		traveler -> character[*word-'a'] -> isWord = 1; // mark as a word
		traveler -> character[*word-'a'] -> character[*wordPart-'a'] = tempNode; // copy all the children
		strcpy(traveler -> character[*word-'a'] -> label[*wordPart-'a'], wordPart); // update the word
		traveler -> character[*word-'a'] -> character[*wordPart-'a'] -> isWord = 1;// mark as a word

		return;
	}

	// have 'there' -> insert 'this'
	if((int)strlen(word) > (i) && (int)strlen(temp) > (i)) {
		char commonPrefix[SIZE_OF_SINGLE_WORD]; // to store common part for both words
		char suffixOne[SIZE_OF_SINGLE_WORD]; // suffix of the internal node
		char suffixTwo[SIZE_OF_SINGLE_WORD]; // suffix of inserting word

		// storing the common part of both words
		strncpy(commonPrefix, word, i);
		commonPrefix[i] = '\0';

		// storing suffix one and suffix two
		strcpy(suffixOne, copyFromGivenIndex(temp, (int)strlen(temp), i));
		strcpy(suffixTwo, copyFromGivenIndex(word, (int)strlen(word), i));


		strcpy(traveler -> label[*word-'a'], commonPrefix); // truncate the existing word
		TrieNode* tempNode = traveler -> character[*word-'a']; // to move children

		traveler -> character[*word-'a'] = createNode();

		(traveler -> character[*word-'a'] -> character[*suffixOne - 'a']) = tempNode; // moving children
		strcpy(traveler -> character[*word-'a'] -> label[*suffixOne - 'a'], suffixOne);  // update the string in child node

		insertWord(traveler -> character[*word-'a'], suffixTwo); // insert the remaining suffix
		return;
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

			char temp[SIZE_OF_SINGLE_WORD];
			strcpy(temp, node -> label[i]); // copy the string in the node
			int iter = pos;
			for(int j=0; temp[j] != '\0'; j++){
				restOfWord[iter++] = temp[j]; // updating the rest of word
			}

			printNode(node -> character[i], restOfWord, pos+(int)strlen(temp)); // printing the childs
		} 
	}
}

// priting suggestions for a given part of a word
void printSuggestions(TrieNode* head, char* wordPart, char* concatPart, int concatenate){
	
	if(head == NULL) {
		printf("No words found....\n");
		return;
	}
	
	// assigning head node to temporary node
	TrieNode* traveler = head;

	// store the string in the node
	char temp[SIZE_OF_SINGLE_WORD];
	strcpy(temp, traveler -> label[*wordPart - 'a']);
	
	// comparing how much characters are equal between words
	int i=0;
	while(temp[i] != '\0' && wordPart[i] != '\0'){
		if(temp[i] == wordPart[i]) i++;
		else break;
	}

	// if the provided string length is larger than word in the node
	if((int)strlen(wordPart) > (i) && (int)strlen(temp) == (i)) {
		char tempwordPart[SIZE_OF_SINGLE_WORD]; // unmatched string part to be passed to next call
		strcpy(tempwordPart, copyFromGivenIndex(wordPart, (int)strlen(wordPart), i));

		char concatPartNew[SIZE_OF_SINGLE_WORD]; // to pass the matched part of the string
		strcpy(concatPartNew, concatPart); // copy the provided prefix
		strncat(concatPartNew, wordPart, i); // copy the matched part
		concatPartNew[(int)strlen(concatPart) + i] = '\0'; // add null terminator

		// passed to child nodes and print
		printSuggestions(traveler->character[*wordPart - 'a'], tempwordPart, concatPartNew, 1);

	} else if((int)strlen(wordPart) > (i) && (int)strlen(temp) > (i)){

		printf("No words found..!\n"); // if there are unmatched characters left

	} else if((int)strlen(wordPart) <= (i) && (int)strlen(temp) <= (i)) { // match found

		// checking if the last node has at least one child exist,
		// if threre is one child exist, it is not empty
		int isEmpty = 1;
		for(int i=0; i<CHAR_SET; i++){
			if(traveler->character[i] != NULL){
				isEmpty = 0;
				break;
			}
		}

		char empty[100]; // to passed for the child nodes

		if(isEmpty) printf("%s\n", wordPart); // if the last node is empty, the given word is the matching word
		else{
			if(concatenate){ // if the concatenate signal enabled, concatenate parent strings 

				strcpy(empty, concatPart);
				strcat(empty, traveler->label[*wordPart - 'a']);
				
			} else {
				strcpy(empty, wordPart);
			}
			
			int stringlength = strlen(empty); // length of the orignal word
			printNode(traveler->character[*wordPart - 'a'], empty, stringlength); // print the child nodes of the last node
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
		strcpy(temp, token); // copy the '\n' removed word to new word
		toLowerCase(temp); // convert to lowercase
		sanitize(temp); // remove non-alphabetic characters
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

// function remove non-alphabetic charaters from a word
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
int main(){
	
	TrieNode* head = createNode();

	// dynamically allocated array of char pointers to store the words from file
	char** wordList = malloc(WORDS_IN_FILE*sizeof(char*));
	for(int i=0; i<WORDS_IN_FILE; i++){
		wordList[i] = malloc(SIZE_OF_SINGLE_WORD*sizeof(char));
	}

	char* filePath = "./wordlist/wordlist70000.txt"; // paths for text files, un-comment only one at a time
	// char* filePath = "./wordlist/wordlist10000.txt";
	// char* filePath = "./wordlist/wordlist1000.txt";
	
	readFile(filePath, wordList);

	for(int i=0; i<WORDS_IN_FILE; i++){
		insertWord(head, wordList[i]); // insert words to the trie
	}
    
	char empty[100]; // empty char array to pass strings of parent nodes

	/* print the whole dictionary */

	// printNode(head, empty, 0);

	char userInput[SIZE_OF_SINGLE_WORD]; // store the user input

	printf("...Compressed trie data structure... \n");

	printf("Enter some text to find: ");
	scanf("%[^\n]%*c", userInput); // taking user input

	toLowerCase(userInput); // convert to lower case
	sanitize(userInput); // remove non-alphabetic characters

	printf(":::::finding:::::\n\n");

	printSuggestions(head, userInput, empty, 0); // printing the suggestions for a given word
}
