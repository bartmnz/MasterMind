#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int getGuess(char*);
int checkNums(char*, char*);
void dumpLine(FILE*);
int toIndex(int, int);
void playSelf(char*, int*);
void stillValid(int*, char*, int);
void printResult(int);
void asChars(char*, int);

#define MAXSIZE  5
#define TOTAL 10000
#define OUTCOMES 14


/*
 *Program to play the Master Mind game. A random 4 digit number is chosen, and the user is asked to input a guess. Feedback is given in the form of # Red # White where red is the number of characters in the correct spot, and white is the number of characters in the incorrect spot. 
 *As implemented the program also implements Knuth's algorithm to solve in the least number of guesses. The suggested guess for is printed to the b ottom of the screen. The user is free to choose their own guess. 
 */


int main(void){
	srand(time(NULL));	
	char secretNum[MAXSIZE];
	char theGuess[MAXSIZE];
	int temp = rand() % TOTAL;
	asChars(secretNum, temp);
	int numGuess=0;
	int* remaining = malloc(TOTAL * sizeof(int));
	int i;
	for(i =0;i < TOTAL; i++){
		remaining[i] = 1;
	}
	while(true){
		while(getGuess(theGuess)){
			fprintf(stderr, "Input %d digits\n", MAXSIZE-1);
		}
		numGuess++;
		int result = checkNums(secretNum, theGuess);
		printResult(result);		
		if(result == (OUTCOMES -1)){
			fprintf(stdout, "You win!  It took you %d guesses.\n", numGuess);
			break;
		}
		stillValid(remaining, theGuess, result);
		playSelf(theGuess, remaining);
	}	
	free(remaining);	
}

/*
 *getGuess returns an int that is the error status of the function. 0 = no error. 1 = too much or too little data input. 2 = non numeric input.
 *Function gets input from the user and verify's that it is a series of 4 digits.
 *
 */

int getGuess(char* userGuess){
	printf("Guess a number:");
	char temp[MAXSIZE+1];
	int i;
	int returnValue = 0;
	fgets(temp,(MAXSIZE+1), stdin);
	if(temp[MAXSIZE -1] == '\n'){
		strncpy(userGuess, temp, MAXSIZE);
		for(i = 0; i < (MAXSIZE-1); i++){
			if(!isdigit(userGuess[i])){
				returnValue = 2;
			}
		}	
	}
	else{
		returnValue = 1;
		if(((int)strlen(temp)) == MAXSIZE ){	
			dumpLine(stdin);
		}	
	}	
	return returnValue;
	}
/*
 *checkNums returns an int corelating to the red / white value of theGuess compared to the Secret. 
 *
 *
 */


int checkNums(char* theSecret, char* theGuess){
	int numRed = 0;
	int numWhite = 0;
	int i;
	char tempSecret[MAXSIZE];
	char tempGuess[MAXSIZE];	
	strncpy(tempSecret, theSecret, MAXSIZE);
	strncpy(tempGuess, theGuess, MAXSIZE);
	for(i = 0; i < (MAXSIZE-1); i++){
                if(tempSecret[i] == tempGuess[i]){
                        numRed++;
			tempGuess[i]= 'x';
			tempSecret[i] = 'y';
                }

        }
	int j;
	for(i = 0; i < (MAXSIZE-1); i++){
		for(j = 0; j < (MAXSIZE -1); j++){
			if(tempGuess[i] == tempSecret[j]){
				tempGuess[i] = 'x';
				tempSecret[j] = 'y';
				numWhite++;
			}
		}
	}

	return toIndex(numRed, numWhite);
}

/*
 *dumpLine is used to clear the stdin buffer when/if the user inputs too much data.
 *
 */

void dumpLine(FILE* filePointer){
	printf("dumpline");
	int temp = '0';
	while((temp = fgetc(filePointer)) != EOF && temp != '\n'){
		/* 
 		* No body to loop	 
  		*/
	}
}

/*
 *playSelf implements Knuth's algorithm to solve the Mastermind game. Function takes pointers to arrays for the remaining valid numbers and the last guess.
 *The algorithm allocates an array large enough to hold an integer for every possible outcome of every possible guess.
 *The algorithm compares every possible guess (numbers 0-9999) to every remaining possible value. In doing so an array is created which details the possible outcomes of every guess. The data for each outcome details how many numbers of those remaining will still be valid if that is the return value. 
 *The algorithm uses that information to choose the next guess based on what number will guarantee to remove the MOST values from the current list of numbers.
 * see also wikipedia -- Mastermind
 * 
 */

void playSelf(char* guess, int* remaining){
	int* allPossible[OUTCOMES];
	int j;
	for (j = 0; j < OUTCOMES; j ++){
		allPossible[j] = malloc(TOTAL * sizeof(int));
	}
	int i;
	int results;
	char toString[MAXSIZE];
	for(i = 0; i < OUTCOMES; i++){
                for(j = 0; j < TOTAL; j++){ 
			allPossible[i][j] = 0;
		}
        }	
	for(i = 0; i < TOTAL; i++){
		if(remaining[i] == 1){
			for(j = 0; j < TOTAL; j++){
				asChars(toString, j);
				char remainingStr[(MAXSIZE-1)];
				asChars(remainingStr, i);
				results = checkNums(remainingStr,toString);
				allPossible[results][j]++;
			}
		}
	}
	int minMax = TOTAL;
	int nextGuess;
	for (j = 0; j < TOTAL; j++){
		int maxValue = 0;
		int index= 0;
		for (i = 0; i < OUTCOMES; i++){
			if (allPossible[i][j] >= maxValue){
				maxValue = allPossible[i][j];
				index = j;	
			}
		}
		if(maxValue < minMax || (maxValue == minMax && allPossible[(OUTCOMES -1)][index] == 1)){
			minMax = maxValue;
			nextGuess = index;
		} 
	}
	for(j =0; j < OUTCOMES; j++){
		free(allPossible[j]);
	}
	asChars(guess, nextGuess);
	printf("Suggested Guess: %s\n", guess);
}
/*
 *returns a unique value for every possible value of red and white. 
 *inverse of printResult()
 */

int toIndex(int red, int white){
	if (red == 1 || red == 2){
		return (4*red + white + 1);
	}
	else if (red == (MAXSIZE-2)){
		return (OUTCOMES-2);
	}
	else if (red == (MAXSIZE-1)){
		return (OUTCOMES-1);
	}
	else{
		return white;
	}
}

/*
 *takes a unique value corresponding to a red/ white number and prints the red / white value to the screen.
 *inverse of toIndex()
 *
 */

void printResult(int index){
	if ( index < MAXSIZE ) {
		printf("%d white\n", index);
	}
	else if ( index < 9 ) {
		printf("1 red %d white\n", (index-5));
	}
	else if ( index < 12) {
		printf("2 red %d white\n", (index-9));
	}
	else if (index == (OUTCOMES-2)){
		printf("3 red\n");
	}
	else {
		printf("4 red\n");
	}
}
/*
 *Array[] is an array of numbers where the index correspons to a guess value. The value stored at the index is either 0 or 1. 1 corresponds to a 
 *number that is still a possibility in Knuth's Algorithm. the string guess and the int guessResult correspond to the last guess and it's unique index value as determined by toIndex()
 *For every number that is currently still valid, the function checks to see if it is a possible value for the secret number if not it is marked as a 0
 */
 
void stillValid(int* array, char* guess, int guessResult){
	int i;
	for (i =0; i < TOTAL; i++){
		char value[(MAXSIZE-1)];
		asChars(value, i);
		bool inPlay = (array[i] == 1);
		if(value == guess){
			inPlay = false;
		}
		bool possible = false;
		if (inPlay){
			int checkVal = checkNums(value, guess);
			possible = (checkVal == guessResult);
		}
		if(!(inPlay && possible)){
			array[i] = 0;
		}
	}
}
/*
 *
 *Function takes a string pointer and a number, and changes the string to correspond to a 4 digit value of the number e.x. 7 is "0007" 
 */

void asChars(char* asString, int number){
	if(number < 10){
		sprintf(asString, "000%d", number);	
	}
	else if(number < 100){
		sprintf(asString, "00%d", number);
	}
	else if(number < 1000){
		sprintf(asString, "0%d", number);
	}
	else {
		sprintf(asString, "%d", number);
	}
}	
