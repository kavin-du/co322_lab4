#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strcat()
#include <ctype.h> // for tolowercase()

// size of the alphabet
#define CHAR_SET 26  // there are non aphabetical characters in the text file
#define SIZE_OF_SINGLE_WORD 35 
#define WORDS_IN_FILE 10000 // need to change this if the file name is changed


typedef struct trienode{ 
	int isWord; // to determine if the node is endpoint of a word
	struct trienode* character[CHAR_SET];
    char label[CHAR_SET][SIZE_OF_SINGLE_WORD]; 
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
char* copyFromGivenIndex(char* word, int length, int index){ // length is optional ? remove later
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
    
	// inserting word when prefix is already there and prefix is the endof word
	// do --> doing        done, doing --> do ????? try this also
	char temp[SIZE_OF_SINGLE_WORD];
	strcpy(temp, traveler -> label[*word - 'a']);
	// printf(".........%s\n", temp);
	int i=0;
	while(temp[i] != '\0' && word[i] != '\0'){
		if(temp[i] == word[i]) i++;
		else break;
	}
	// printf(".........%d\n", i);
	// i--;
	// word exists but not marked as a word
	if((int)strlen(word) == (i) && (int)strlen(temp) == (i) && ((traveler -> character[*word-'a'] -> isWord)) == 0){
		(traveler -> character[*word-'a'] -> isWord) = 1; // mark as a word
		return;
	}

	// have 'do' -> insert 'doing'
	if((int)strlen(word) > (i) && (int)strlen(temp) == (i)) {
		char wordPart[SIZE_OF_SINGLE_WORD]; // try to reduce size
		strcpy(wordPart, copyFromGivenIndex(word, (int)strlen(word), i));
		// printf(".........%s\n", wordPart);
		insertWord(traveler -> character[*word-'a'], wordPart);
		return; 
	}

	// have 'facebook' --> insert 'face'
	if((int)strlen(word) == (i) && (int)strlen(temp) > (i)) { // what if marked as word?????????
		char wordPart[SIZE_OF_SINGLE_WORD];
		strcpy(wordPart, copyFromGivenIndex(temp, (int)strlen(temp), i));
		strcpy(traveler -> label[*word-'a'], word); // truncate the existing word

		TrieNode* tempNode = traveler -> character[*word-'a'];

		(traveler -> character[*word-'a']) = createNode();
		traveler -> character[*word-'a'] -> isWord = 1; // mark as a word
		traveler -> character[*word-'a'] -> character[*wordPart-'a'] = tempNode; // copy all the children
		strcpy(traveler -> character[*word-'a'] -> label[*wordPart-'a'], wordPart); // update the word
		traveler -> character[*word-'a'] -> character[*wordPart-'a'] -> isWord = 1;// mark as a word

		return;
	}

	// have 'there' -> insert 'this'
	if((int)strlen(word) > (i) && (int)strlen(temp) > (i)) {
		char commonPrefix[SIZE_OF_SINGLE_WORD];
		char suffixOne[SIZE_OF_SINGLE_WORD]; // suffix of the internal node
		char suffixTwo[SIZE_OF_SINGLE_WORD]; // suffix of inserting word

		strncpy(commonPrefix, word, i);
		commonPrefix[i] = '\0';

		// printf(".........%d\n", i);
		strcpy(suffixOne, copyFromGivenIndex(temp, (int)strlen(temp), i));
		strcpy(suffixTwo, copyFromGivenIndex(word, (int)strlen(word), i));
		// printf("...%s...%s...%s...%s...%d\n", word, commonPrefix, suffixOne, suffixTwo, i);

		strcpy(traveler -> label[*word-'a'], commonPrefix); // truncate the existing word
		TrieNode* tempNode = traveler -> character[*word-'a']; // to move children

		traveler -> character[*word-'a'] = createNode();

		(traveler -> character[*word-'a'] -> character[*suffixOne - 'a']) = tempNode; // moving children
		strcpy(traveler -> character[*word-'a'] -> label[*suffixOne - 'a'], suffixOne);  

		insertWord(traveler -> character[*word-'a'], suffixTwo);
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
			// restOfWord[pos] = i+'a';
			// printNode(node -> character[i], restOfWord, pos+1);

			// restOfWord[pos] = i+'a';
			char temp[SIZE_OF_SINGLE_WORD];
			strcpy(temp, node -> label[i]);
			int iter = pos;
			for(int j=0; temp[j] != '\0'; j++){
				restOfWord[iter++] = temp[j];
			}

			printNode(node -> character[i], restOfWord, pos+(int)strlen(temp));
		} 
	}
}

// priting suggestions for a given part of a word
void printSuggestions(TrieNode* head, char* wordPart, char* concatPart, int concatenate){
	
	// head is null when tree does not exist
	if(head == NULL) {
		printf("No words found....\n");
		return;
	}
	
	// assigning head node to temporary node
	TrieNode* traveler = head;

	char temp[SIZE_OF_SINGLE_WORD];
	strcpy(temp, traveler -> label[*wordPart - 'a']);
	// printf(".........%s\n", temp);
	int i=0;
	while(temp[i] != '\0' && wordPart[i] != '\0'){
		if(temp[i] == wordPart[i]) i++;
		else break;
	}

	if((int)strlen(wordPart) <= (i) && (int)strlen(temp) <= (i)){
		goto print;
	}

	if((int)strlen(wordPart) > (i) && (int)strlen(temp) == (i)) {
		char tempwordPart[SIZE_OF_SINGLE_WORD]; // unmatched string part to be passed to next call
		strcpy(tempwordPart, copyFromGivenIndex(wordPart, (int)strlen(wordPart), i));

		char concatPartNew[SIZE_OF_SINGLE_WORD]; // passed the matched part of the string
		strcpy(concatPartNew, concatPart);
		strncat(concatPartNew, wordPart, i);
		concatPartNew[(int)strlen(concatPart) + i] = '\0';

		// printf("...%s....%s...%d\n",wordPart, tempwordPart, i);
		printSuggestions(traveler->character[*wordPart - 'a'], tempwordPart, concatPartNew, 1);
		return;
	}

	if((int)strlen(wordPart) > (i) && (int)strlen(temp) > (i)){
		printf("No words found..!\n");
		return;
	}
	
	// printf("testing..=...=...=.......=....=...=....=..=...=..=....\n"); // cannot come here

print: ;
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
	if(isEmpty) printf("%s\n", wordPart); // if the last node is empty, the given word is the matching word
	else{
		if(concatenate){
			strcpy(empty, concatPart);
			// strcat(empty, wordPart);
			strcat(empty, traveler->label[*wordPart - 'a']);
			// printf("concat......%s...%s\n", concatPart, wordPart);
		} else {
			strcpy(empty, wordPart);
			// printf("not_concat......%s\n", empty);
		}
		 
		int stringlength = strlen(empty); // length of the orignal word
		printNode(traveler->character[*wordPart - 'a'], empty, stringlength); // print the child nodes of the last node
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

// remove non-alphabetic charaters from a word
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
	
	// char words[][20] = {"for", "do", "while", "doing", "facebook", "face", "thereafter", "therein", "this"};

	// for(int i=0; i<9; i++){
	// 	insertWord(head, words[i]);
	// }

    

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
	char empty[100];
	// printNode(head, empty, 0);

	char userInput[SIZE_OF_SINGLE_WORD];

	printf("Compressed trie data structure.. \n");
	printf("Enter some text to find: ");
	scanf("%[^\n]%*c", userInput);
	sanitize(userInput);

	printf(":::::finding:::::\n\n");

	printSuggestions(head, userInput, empty, 0); // printing the suggestions for a given word
}
