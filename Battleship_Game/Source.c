/* Assignment: 3
Author: Amit Ovadia, ID: 207634791
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Board size, min and max index
#define BOARD_SIZE 10
#define MAX_INDEX_BOARD 9
#define MIN_INDEX_BOARD 0

// The number of submarines
#define NUM_SUBMARINE 5

typedef enum { FALSE, TRUE } boolean;
// The represation of the submarines on board that the user can't see
enum submarinesReprestionOnBoard { SUBMARINE_1 = '1', SUBMARINE_2 = '2', SUBMARINE_3 = '3', SUBMARINE_4 = '4', SUBMARINE_5 = '5' };
// The directions the ship can be set
enum shipDirection { HORIZONTAL = 0, VERTICAL = 1 };
// The sizes of the submarines
enum sizeOfSubmarines { SUBMARINE_SIZE_1 = 2, SUBMARINE_SIZE_2 = 3, SUBMARINE_SIZE_3 = 3, SUBMARINE_SIZE_4 = 4, SUBMARINE_SIZE_5 = 5 };
// Symobls that the user will see when he will see the board
enum signs { YET_HIT = ' ', MISS = '~', HIT = 'X' };

// Functions declaration
int initializeBoardWithYetHitSigns(char board[][BOARD_SIZE], int rows);
int setRandomlySubmarines(char board[][BOARD_SIZE], int numOfSubmarines, int* submarinesSizes);
boolean isValidLocation(char board[][BOARD_SIZE], int row, int col, int direction, int submarineSize);
boolean isAttachedToOtherSubmarine(char board[][BOARD_SIZE], int row, int col, int direction, int submarineSize);
boolean setSubmarineOnBoard(char board[][BOARD_SIZE], int direction, int row, int col, int* submarineSize, int numberOfSubmarine);
int getRandomNumber(int min, int max);
int printBoard(char board[][BOARD_SIZE]);
int play(char board[][BOARD_SIZE], int* counterSubmarinesHits, int* submarinesSizes, int numberOfSubmarines, int* numberOfSinkedShips);
int checkHit(char board[][BOARD_SIZE], int row, int col, int* counterSubmarinesHits, int* numberOfSinkedShips, int* submarinesSizes);
boolean isWin(int numberOfSubmarines, int* numberOfSinkedShips);

int main() {
	// Set board
	char board[BOARD_SIZE][BOARD_SIZE];
	// Create array with submarines sizes
	int submarinesSizes[NUM_SUBMARINE] = { SUBMARINE_SIZE_1, SUBMARINE_SIZE_2, SUBMARINE_SIZE_3, SUBMARINE_SIZE_4, SUBMARINE_SIZE_5 };
	// Create array counter for each submarine hits
	int counterSubmarinesHits[NUM_SUBMARINE] = { 0 };
	// Counter of sinked ships
	int numberOfSinkedShips = 0;

	// Set random
	srand((unsigned int)time(NULL));
	// Initialize the board with only YET_HIT signs
	initializeBoardWithYetHitSigns(board, BOARD_SIZE);
	// Set randomly on the board the submarines (by the rules of the game)
	setRandomlySubmarines(board, NUM_SUBMARINE, submarinesSizes);
	// Start the game
	play(board, counterSubmarinesHits, submarinesSizes, NUM_SUBMARINE, &numberOfSinkedShips);

	return 0;
}

// This function initialize the recieved board with YET_HIT signs
int initializeBoardWithYetHitSigns(char board[][BOARD_SIZE], int rows) {
	int i, j;

	for (i = 0; i < rows; ++i)
		for (j = 0; j < BOARD_SIZE; ++j)
			board[i][j] = YET_HIT;

	return 0;
}

// This function set on the recieved board submarines 
int setRandomlySubmarines(char board[][BOARD_SIZE], int numOfSubmarines, int* submarinesSizes) {
	int i, row, col, direction;

	// For to create each submarine
	for (i = 0; i < numOfSubmarines; ++i) {
		// Get a valid location for the submarine that isn't going of the board and isn't attached to another submarine
		do {
			// Get random starting coordinate and a direction (HORIZONTAL = 0, VERTICAL = 1)
			col = getRandomNumber(MIN_INDEX_BOARD, MAX_INDEX_BOARD);
			row = getRandomNumber(MIN_INDEX_BOARD, MAX_INDEX_BOARD);
			direction = getRandomNumber(0, 1);
		} while ((!isValidLocation(board, row, col, direction, *(submarinesSizes + i))) || (!isAttachedToOtherSubmarine(board, row, col, direction, *(submarinesSizes + i))));
		// If the location is valid than set the submarine on the board
		setSubmarineOnBoard(board, direction, row, col, submarinesSizes + i, i + 1);
	}


	return 0;
}

// This function check if the submarine can be set on the board without going off the board
boolean isValidLocation(char board[][BOARD_SIZE], int row, int col, int direction, int submarineSize) {
	int i;

	switch (direction) {
	case HORIZONTAL:
		// Check if the submarine isn't going off the matrix
		if ((col + submarineSize) > BOARD_SIZE)
			return FALSE;

		// Check if the submarine doesnt go over other submarines
		for (i = 0; i < submarineSize; ++i) {
			if (board[row][col + i] != YET_HIT)
				return FALSE;
		}
		break;

	case VERTICAL:
		// Check if the submarine isn't going off the matrix
		if ((row + submarineSize) > BOARD_SIZE)
			return FALSE;

		// Check if the submarine doesnt go over other submarines
		for (i = 0; i < submarineSize; ++i)
			if (board[row + i][col] != YET_HIT)
				return FALSE;
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

// This function check if the submarine is attached to other submarine that already been set on the board
boolean isAttachedToOtherSubmarine(char board[][BOARD_SIZE], int row, int col, int direction, int submarineSize) {
	int i;

	switch (direction) {
	case HORIZONTAL:
		// Check if on the sides (right and left - if the check is still on the board) of the submarine there is already another submarine
		for (i = 0; i < submarineSize; ++i) {
			if ((row > 0) && (board[row - 1][col + i] != YET_HIT))
				return FALSE;
			if ((row < 9) && (board[row + 1][col + i] != YET_HIT))
				return FALSE;
		}
		// Check above and below the submarine (if the check is still on the board) if there isalready another submarine
		for (i = 0; i < 3; ++i) {
			if ((col - 1 >= 0) && (row - 1 + i >= 0) && (board[row - 1 + i][col - 1] != YET_HIT))
				return FALSE;
			if ((row - 1 + i >= 0) && (col + submarineSize <= 9) && (board[row - 1 + i][col + submarineSize] != YET_HIT))
				return FALSE;
		}
		break;

	case VERTICAL:
		// Check if on the sides (right and left - if the check is still on the board) of the submarine there is a submarine
		for (i = 0; i < submarineSize; ++i) {
			if ((col > 0) && (board[row + i][col - 1] != YET_HIT))
				return FALSE;
			if ((col < 9) && (board[row + i][col + 1] != YET_HIT))
				return FALSE;
		}
		// Check above and below the submarine (if the check is still on the board) if there isalready another submarine
		for (i = 0; i < 3; ++i) {
			if ((row - 1 >= 0) && (col - 1 + i >= 0) && (col - 1 + i <= 9) && (board[row - 1][col - 1 + i] != YET_HIT))
				return FALSE;
			if ((col - 1 + i >= 0) && (col - 1 + i <= 9) && (row + submarineSize <= 9) && (board[row + submarineSize][col - 1 + i] != YET_HIT))
				return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

// This function set submarine on the board
boolean setSubmarineOnBoard(char board[][BOARD_SIZE], int direction, int row, int col, int* submarineSize, int numberOfSubmarine) {
	int i;

	switch (direction) {
	case HORIZONTAL:
		// Set submarine on board with represation of the number of the submarine
		for (i = 0; i < *submarineSize; ++i)
			board[row][col + i] = numberOfSubmarine + '0';
		break;

	case VERTICAL:
		// Set submarine on board with represation of the number of the submarine
		for (i = 0; i < *submarineSize; ++i)
			board[row + i][col] = numberOfSubmarine + '0';
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

// This function return a random number
int getRandomNumber(int min, int max) {
	return ((rand() % (max - min + 1)) + min);
}

// This function prints the board properly
int printBoard(char board[][BOARD_SIZE]) {
	int i, j;

	for (i = 0; i < BOARD_SIZE; ++i) {
		for (j = 0; j < BOARD_SIZE; ++j) {
			// If the coordinate yet been hit - print YET_HIT sign. Else print the sign there is in this coordinate
			if (board[i][j] != YET_HIT && board[i][j] != MISS && board[i][j] != HIT)
				printf("%c", YET_HIT);
			else
				printf("%c", board[i][j]);
		}
		printf("\n");
	}

	return 0;
}

// This function starts and runs the game
int play(char board[][BOARD_SIZE], int* counterSubmarinesHits, int* submarinesSizes, int numberOfSubmarines, int* numberOfSinkedShips) {
	int row, col, win = 0;

	printf("Welcome to Battleship, Let's play!\nThe board is 10x10 and is already sets with submarines!\nIn order to exit the game enter 0 for a row and for a column\nGood luck!\n");

	// Get a valid coordinate from the user
	do {
		printf("Please enter the number of row you would like to hit (1-10) : \n");
		scanf("%d", &row);
		row -= 1;
		printf("Please enter the number of column you would like to hit (1-10) : \n");
		scanf("%d", &col);
		col -= 1;
		// If the coordinate is valid than check the user hit and print the board appropriately
		if ((row >= MIN_INDEX_BOARD) && (row <= MAX_INDEX_BOARD) && (col >= MIN_INDEX_BOARD) && (col <= MAX_INDEX_BOARD))
		{
			checkHit(board, row, col, counterSubmarinesHits, numberOfSinkedShips, submarinesSizes);
			printBoard(board);
		}
		// If the coordinate isn't valid than print message accordingly
		else if ((row != -1) && (col != -1))
			printf("Illegal values, please try again.\n");

		// Get coordinate from the user till the user win or want to exit (-1, -1)
	} while ((row != -1) && (col != -1) && (!(win = isWin(numberOfSubmarines, numberOfSinkedShips))));

	// If the user win print message accordingly
	if (win)
		printf("Congratulations you win!\n");
	// If not - than the user wanted to exit - print message accordingly
	else
		printf("See you next time !\n");

	return 0;
}

// Check the user hit, set board sign accordingly and print message accordingly
int checkHit(char board[][BOARD_SIZE], int row, int col, int* counterSubmarinesHits, int* numberOfSinkedShips, int* submarinesSizes) {
	int numSubmarine, sizeOfSubmarine;

	switch (board[row][col]) {
		// If the user hit a submarine
	case SUBMARINE_1:
	case SUBMARINE_2:
	case SUBMARINE_3:
	case SUBMARINE_4:
	case SUBMARINE_5:
		// Print a message accordingly
		printf("It's a hit! \n");
		// Get the submarine number
		numSubmarine = board[row][col] - '0';
		// Get the size of the submarine
		sizeOfSubmarine = *(submarinesSizes + numSubmarine - 1);
		// Add to the submarine counter 1
		++* (counterSubmarinesHits + (numSubmarine - 1));
		// If the number of hits eqauls to the size of the submarine than print a message accordingly
		if ((*(counterSubmarinesHits + (numSubmarine - 1))) == (sizeOfSubmarine)) {
			// Print a message that the user sinked a submarine, and the number of cells the submarine had
			printf("Congratulations you just sink a submarine with %d cells.\n", sizeOfSubmarine);
			// Add to the sinked ships counter 1
			++* (numberOfSinkedShips);
		}
		// Set board sign accordingly
		board[row][col] = HIT;
		break;

	case HIT:
	case MISS:
		// Print a message accordingly
		printf("You have already hit that spot \n");
		break;

	case YET_HIT:
		// Print a message accordingly
		printf("It's a miss! \n");
		// Set board sign accordingly
		board[row][col] = MISS;
		break;
	}

	return 0;
}

// This function checks if the user sinked all ships and won - and returns TRUE or FALSE accordingly
boolean isWin(int numberOfSubmarines, int* numberOfSinkedShips) {
	return (*(numberOfSinkedShips) == numberOfSubmarines);
}