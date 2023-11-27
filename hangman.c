#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void addLetters(char* alp, int* alpIndex, char choice);
int alreadyInArray(char* guess, char choice, int len);
void attemptsLeft(int tries);
int checkArrays(char* correct, char* guess, char choice, int len);
int checkLetters(char* alp, int len, char choice);
void clearBuffer();
int countLetters(char* word);
int countLine(FILE* fp);
void drawCharacter(int tries);
void drawCharacterStart();
int finalCheck(char* correct, char* guess, int len);
char* getGuessStr(int len);
char* getWord(int* len);
void hangman(char* correct, char* guess, int len);
void makeArray(FILE* fp, char* str, int lineCount, int len, int random);
void makeGuessStr(char* str, int len);
void outputLetters(char* alp, int len);
int playerChoice(int* choice);
void printArr(char* str, int len, int includeSpaces);
int randomNumGenerator(int lineCount);
void takeOutNewLine(char* str);
void takeOutNull(char* strWithN, char* correct, int lenWithoutNull);
void userInput(char* choice, char* guess, int len);
void userOutput(int number, char* correct, int len);

int main(int argc, char* argv[]) {
	int keepPlaying = 1;
	while (playerChoice(&keepPlaying)) {
        int length;
        char* correctWord = getWord(&length);
		char* guessStr = getGuessStr(length);
		hangman(correctWord, guessStr, length);
		free(guessStr);
		free(correctWord);
	}

	printf("\nThank you for playing!!!!\n");
	return 0;
}

void hangman(char* correct, char* guess, int len) {
	int triesLeft = 5;
	char choice;
	char alp[26];
	int alpIndex = 0;
	printf("\nYou are now playing hangman\n\nYou have %d wrong guesses\n\n", triesLeft);
	drawCharacter(triesLeft);
	while (!finalCheck(correct, guess, len)) {
		userInput(&choice, guess, len);
		if (alreadyInArray(alp, choice, alpIndex)) {
            printf("You have already guessed %c\n\n", choice);
		} else if (checkArrays(correct, guess, choice, len)) {
			printf("%c was correct!!!\n\n", choice);
		} else {
			printf("Wrong choice\n\n");
			triesLeft--;
		}
	int final = finalCheck(correct, guess, len);
	if (!final) {
		attemptsLeft(triesLeft);
		drawCharacter(triesLeft);
	}
		if (triesLeft > 0 && !final) {
			addLetters(alp, &alpIndex, choice);
		}

		if (triesLeft == 0) {
			userOutput(0, correct, len);
			return;
		}

		printf("\n");
	}

	userOutput(1, correct, len);
}

char* getWord(int* len) {
	FILE* fp = fopen("words.txt", "r");
	int length = 25;
	char str[length];
	if (fp == NULL) {
		fprintf(stderr, "File cannot open");
		exit(1);
	}

	int lineCount = countLine(fp);
	rewind(fp);
	int random = randomNumGenerator(lineCount);
	makeArray(fp, str, lineCount, length, random);
	int numberOfLetters = countLetters(str);
	char* correctWord = malloc(numberOfLetters * sizeof(char));
	takeOutNull(str, correctWord, numberOfLetters);
	fclose(fp);
	*len = numberOfLetters;
	return correctWord;
}

int randomNumGenerator(int lineCount) {
	srand((unsigned int)time(NULL));
	int random = rand() % (lineCount - 1) + 1;
	return random;
}

void takeOutNewLine(char* str) {
	int i = 0;
	while (str[i] != '\n') {
		i++;
	}

	str[i] = '\0';
}

int countLine(FILE* fp) {
	char ch;
	int lineCount = 0;
	do {
		ch = fgetc(fp);
		if (ch == '\n') {
			lineCount++;
		}

	} while (ch != EOF);

	return lineCount;
}

void makeArray(FILE* fp, char* str, int lineCount, int len, int random) {
	int whichLine = 1;
	while (whichLine < lineCount) {
		fgets(str, len, fp);
		takeOutNewLine(str);
		if (random == whichLine) {
			break;
		}

		whichLine++;
	}
}

int countLetters(char* word) {
	int i = 0;
	while (word[i] != '\0') {
		i++;
	}

	return i;
}

void takeOutNull(char* strWithN, char* correct, int lenWithoutNull) {
	for (int i = 0; i < lenWithoutNull; i++) {
		correct[i] = strWithN[i];
	}
}

void printArr(char* str, int len, int includeSpaces) {
    for (int i = 0; i < len; i++) {
		printf("%c", str[i]);
        if (includeSpaces) {
            printf(" ");
        }
    }

	printf("\n");
}

void makeGuessStr(char* str, int len) {
	for (int i = 0; i < len; i++) {
		str[i] = '_';
	}
}

char* getGuessStr(int len) {
	char* guessStr = malloc(len * sizeof(char));
	makeGuessStr(guessStr, len);
	return guessStr;
}

void clearBuffer() {
	char ch;
	do {
		scanf("%c", &ch);
	} while (ch != '\n');
}

int finalCheck(char* correct, char* guess, int len) {
	int count = 0;
	for (int i = 0; i < len; i++) {
		if (correct[i] == guess[i]) {
			count++;
		}
	}

	if (count == len) {
		return 1;
	}

	return 0;
}

int checkArrays(char* correct, char* guess, char choice, int len) {
	int isElementCorrect = 0;
	for (int i = 0; i < len; i++) {
		if (correct[i] == choice) {
			guess[i] = choice;
			isElementCorrect++;
		}
	}

	return isElementCorrect;
}

int alreadyInArray(char* guess, char choice, int len) {
	for (int i = 0; i < len; i++) {
		if (guess[i] == choice) {
			return 1;
		}
	}

	return 0;
}

void userInput(char* choice, char* guess, int len) {
	char temp;
	printArr(guess, len, 1);
	printf("Pick a letter please: ");
	scanf("%c", &temp);
	clearBuffer();
	while (!isalpha(temp)) {
		printf("That is not a letter, pick a letter please: ");
		scanf("%c", &temp);
		clearBuffer();
	}
	*choice = tolower(temp);
}

void userOutput(int number, char* correct, int len) {
	if (!number) {
		printf("The correct word was ");
        printArr(correct, len, 0);
		printf("You lost\n");
	} else {
		printf("                               _  \n");
		printf("                             /. .\\ \n");
		printf("                             \\ U / \n");
		printf("                               |  \n");
		printf("                              \\|/  \n");
		printf("                               |   \n");
		printf("                              / \\   \n");
		printf("You win!!!\nThe correct word was ");
        printArr(correct, len, 0);
        printf("\n");
	}
}

void addLetters(char* alp, int* alpIndex, char choice) {
	if (!checkLetters(alp, *alpIndex, choice)) {
		alp[*alpIndex] = choice;
		outputLetters(alp, *alpIndex);
		++*alpIndex;
	} else {
		--*alpIndex;
		outputLetters(alp, *alpIndex);
		++*alpIndex;
	}
}

void outputLetters(char* alp, int len) {
	printf("Used letters are: ");
	for (int i = 0; i <= len; i++) {
		printf("%c ", alp[i]);
	}

	printf("\n");
}

int checkLetters(char* alp, int len, char choice) {
	for (int i = 0; i <= len; i++) {
		if (alp[i] == choice) {
			return 1;
		}
	}

	return 0;
}

void attemptsLeft(int tries) {
	if (tries == 1) {
		printf("You have 1 try left\n");
	} else if (tries == 0) {
		;
	} else {
		printf("You have %d tries left\n", tries);
	}
}

int playerChoice(int* choice) {
	char playChoice;
	if (*choice) {
		drawCharacterStart();
		printf("Would you like to play Hangman, (y)es or (n)o: ");
		*choice = 0;
	} else {
		printf("Would you like to play again, (y)es or (n)o: ");
	}

	while (1) {
		scanf(" %c", &playChoice);
		clearBuffer();
		switch (tolower(playChoice)) {
			case 'y':
				return 1;
			case 'n':
				return 0;
		}
		printf("Wrong answer, please type (y) or (n): ");
	}
}

void drawCharacterStart() {
	printf("                                        |\n");
	printf("                                        |\n");
	printf("                                        |\n");
	printf("                                        |\n");
	printf("                                       _|_\n");
	printf("                                      /. .\\\n");
	printf("                                      \\ - /\n");
	printf("                                        |\n");
	printf("                                      / | \\\n");
	printf("                                        |\n");
	printf("                                       / \\\n");
}

void drawCharacter(int tries) {
	switch (tries) {
		case 5:
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                        |\n");
			break;

		case 4:
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                       _|_\n");
			break;

		case 3:
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                       _|_\n");
			printf("                                      /. .\\\n");
			printf("                                      \\ - /\n");
			break;
		case 2:
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                       _|_\n");
			printf("                                      /. .\\\n");
			printf("                                      \\ - /\n");
			printf("                                        |\n");
			printf("                                      / | \\\n");
			break;
		case 1:
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                        |\n");
			printf("                                       _|_\n");
			printf("                                      /. .\\\n");
			printf("                                      \\ - /\n");
			printf("                                        |\n");
			printf("                                      / | \\\n");
			printf("                                        |\n");
			break;
		case 0:
			drawCharacterStart();
			break;
	}

}

