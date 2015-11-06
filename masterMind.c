#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

int getGuess(char*);
int checkNums(char*, char*);
void dumpLine(FILE*);
int toIndex(int, int);
void playSelf(char*, int*);
void stillValid(int*, char*, int);
void printResult(int);

#define MAXSIZE  5
#define TOTAL 10000
#define OUTCOMES 14

int main(void){	
	char secretNum[MAXSIZE];
	char theGuess[MAXSIZE];
	int temp = rand() % TOTAL;
	sprintf(secretNum,"%d", temp);
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

void dumpLine(FILE* filePointer){
	printf("dumpline");
	int temp = '0';
	while((temp = fgetc(filePointer)) != EOF && temp != '\n'){
		/* 
 		* No body to loop	 
  		*/
	}
}

void playSelf(char* guess, int* remaining){
	int* allPossible[OUTCOMES];
	int j;
	for (j = 0; j < OUTCOMES; j ++){
		allPossible[j] = (int *)malloc(TOTAL * sizeof(int));
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
				sprintf(toString, "%d", j);
				char remainingStr[(MAXSIZE-1)];
				sprintf(remainingStr, "%d", i);
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
	printf("%d\n", nextGuess);
	sprintf(guess,"%d", nextGuess);
}


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

 
void stillValid(int* array, char* guess, int guessResult){
	int i;
	for (i =0; i < TOTAL; i++){
		char value[(MAXSIZE-1)];
		sprintf(value, "%d", i);
		bool inPlay = (array[i] == 1);
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
