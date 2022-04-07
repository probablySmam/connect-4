#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
// Sam Schaeffler
/* bash
 cd /mnt/c/Users/cu/Documents/programs
 gcc FILENAME.c -o FILENAME
 ./FILENAME
*/


//initalizations
char player1[20], player2[20], currentPlayer[20];
char currentInp; // for user's choice in row
char status[7] = "playing"; //won't go in main loop if not "playing"
int turn = 1; // will keep track of who's turn it is, P1 or P2
int* board; //point to board space
int row = 6, column= 7; // board is 7w & 6h (A-G is 7) int board[5][6];


/*start method
	– gathers player names
	– makes space for the board
	– prepares the board by filling it
*/
void init() {
	printf("Setting up the game...\nPlease input your name...\nPlayer 1: ");
	scanf("%s", player1);
	printf("Player 2: ");
	scanf("%s", player2);
	
	board = (int*)malloc(row* column* sizeof(int)); // allocating space 6x7
	
	//since malloc doesn't give default value, i'm starting it off with 0
	for(int l = 0; l<row; l++) { //row
		for(int m = 0; m<column; m++){ //collumn
			*(board + l*column + m) = 0; //board[l][m]?
		}
	}
	
}


/* ending method
	–states the game is ending and frees the board
*/
void teardown() {
	printf("Destroying the game\n");
	free(board); // freeing allocated space so no memory leak
}


/* ACCEPTING INPUT
– accept a letter for which column of the game we are going to drop a disc into.
– Valid letters are A-G (7 columns).
– You should designate a letter for “Quit the game”.
– Input must be returned from this function.
*/
char getInput() { //"should not accept numbers" -5 ._.
	printf("Please input a letter A-G or Q to quit.\n");
	scanf("%s", &currentInp);
	return currentInp;
}


//converts the given column letter to an int to put directly into chosen array
int columnToArr(char currentInput) {
	int column;
	switch(currentInput) {
		case 'a' :
			column = 0;
			break;
		case 'b' :
			column = 1;
			break;
		case 'c' :
			column = 2;
			break;
		case 'd' :
			column = 3;
			break;
		case 'e' :
			column = 4;
			break;
		case 'f' :
			column = 5;
			break;
		case 'g' :
			column = 6;
			break;
		case 'q' :
			strcpy(status, "quit"); //quits game
			column = -1; //to indicate it isn't a column
			break;
	}
	return column;
}


/* DESCRIPTION
 this function will look for an empty space in the chosen collumn,
 closest to the bottom of the board where it would fall and
 place the player's piece there.. if there is space...
*/
void findAndPlacePiece(int chosenColumn, int playerPiece) {
	
	if (chosenColumn != -1) { // when the player didn't choose to quit
		if(*(board + 0*row + chosenColumn) != 0) { //board[0][chosenColumn] if the top of that collumn is full
			strcpy(status, "quit"); // the game quits when the chosen row is full!
		}
		else { //column is chosen, checking to find available place to drop piece
			for(int k = 5; k>-1; k--) { //start from bottom of row since it falls
				if(*(board + k*column + chosenColumn) == 0) { //check if spot empty // board[k][columnNum]
					*(board + k*column + chosenColumn) = playerPiece; //put currentplayer piece in
					k = -1; // get out of loop once bottom most spot found
				}
			
			}
		}
		
	}
	
}


//go through board and check to see different combos for 4 in a row
void checkWin() {
	int inaRow = 1;
	int previousPiece = 0; //no piece at first
	int currentPiece;
	
		
	// checking each row
	for(int l = 0; l<row; l++) { //row
		for(int m = 0; m<column; m++){ //collumn
		
			currentPiece = *(board + l*column + m); //board[l][m] (r,c)
			
			if((currentPiece == previousPiece) && (currentPiece != 0)) {
				inaRow++; //first time in a row is 2
				if(inaRow == 4) {
					strcpy(status, "win");
				}
			} else {
				inaRow = 1; //reset count; 1 for single piece
			}
			
			previousPiece = currentPiece; //piece used is now the one compared to
		}
		inaRow = 1; //once the collumn is finished without being 4, it resets for the next
	}
	inaRow = 1; //if there is no 4 in a row in the collumn, reset
	previousPiece = 0; //reset for new search

	
	// checking each column
	for(int l = 0; l<column; l++) { //row
		for(int m = 0; m<row; m++){ //collumn
		
			currentPiece = *(board + m*column + l); //board[l][m] (r,c)
			
			if((currentPiece == previousPiece) && (currentPiece != 0)) {
				inaRow++; //first time in a row is 2
				if(inaRow == 4) {
					strcpy(status, "win");
				}
			} else {
				inaRow = 1; //reset count; 1 for single piece
			}
			
			previousPiece = currentPiece; //piece used is now the one compared to
		}
		inaRow = 1; //once the collumn is finished without being 4, it resets for the next
	}
	
	
	// Diagonal check (top left to bottom right)
	int rightColumnStart[] = {0,0,0,1,2,3}; //starting points
	int rightRowStart[] = {2,1,0,0,0,0}; // upper left
	previousPiece = 0;
	int p;
	
	for(int k = 0;k<6;k++){ // loop through different starting points of the board (left side)
		p = 0;
		
		while(((rightColumnStart[k] + p) < column) && ((rightRowStart[k] + p) < row)) { //so p doesn't go off board
			currentPiece = *(board + (rightRowStart[k] + p)*column + (rightColumnStart[k] + p));
			//*(board + (rightRowStart[k] + p)*column + (rightColumnStart[k] +p)) = currentPiece+5; //TEST
				
			if((currentPiece == previousPiece) && (currentPiece != 0)) {
				inaRow++; //first time in a row is 2
				if(inaRow == 4) {
					strcpy(status, "win");
				}
			} else {
				inaRow = 1; //reset count; 1 for single piece
			}
			previousPiece = currentPiece; //piece used is now the one compared to
			p++;
		}
		inaRow = 1; //reset if nothing found in first diagonal
	}
	
	
	// Diagonal check (top right to bottom left)
	int leftColumnStart[] = {3,4,5,6,6,6};
	int leftRowStart[] = {0,0,0,0,1,2};
	previousPiece = 0;
	inaRow = 1;
	int r, q;
	
	for(int k=0;k<6;k++){ // loop through different starting points of the board (right side)
		r = 0; // column and row going different ways
		q = 0; // top right is positive
		
		//*(board + (leftRowStart[k])*column + (leftColumnStart[k])) = 3; //TESTTTTTTTTTTTTT
		
		while(((leftColumnStart[k] + q) > -1) && ((leftRowStart[k] + r) < row)) { //so p doesn't go off board
			currentPiece = *(board + (leftRowStart[k] + r)*column + (leftColumnStart[k] + q));
			//*(board + (leftRowStart[k] + r)*column + (leftColumnStart[k] + q)) = currentPiece + 5; //TESTTTTTT
				
			if((currentPiece == previousPiece) && (currentPiece != 0)) {
				inaRow++; //first time in a row is 2
				if(inaRow == 4) {
					strcpy(status, "win");
				}
			} else {
				inaRow = 1; //reset count; 1 for single piece
			}
			previousPiece = currentPiece; //piece used is now the one compared to
			r++; //when you go down row, you add
			q--; //going to left of column is subtracting
		}
		inaRow = 1; //reset if nothing found in first diagonal
	}
	
	
}

	

/*
in turn function, it changes turn after stating who goes,
so this function return the correct turn to use as a
player piece on the board
*/
int getPlayerPiece() { 
	switch(turn) { 
		case 1:
			return 2; 
			break;
		case 2:
			return 1;
			break;
	}
}


/* UPDATE WORLD STATE
This method must not print anything.
– pass the input data to this function.
– must place player piece in correct place nearest to bottom
– signals game end when a column is too full
– check to see if there is 4 in a row
*/
void updateWorld(char currentInp) {
	char lowerInp = tolower(currentInp); //convert all to lower before going into next func
	int columnNum = columnToArr(lowerInp); // board[?][columnNum]
	int playerPiece = getPlayerPiece(); //doing first so loop doesn't loop
	
	findAndPlacePiece(columnNum, playerPiece);
	checkWin();
}


/*
Display the state of the world –
-Print the result calculated in the state of the world.
-print the board.
*/
void displayWorld() {
	printf("Current Status: %s\n\n", status);
	//print board!!
	for(int l = 0; l<row; l++) { //row
		for(int m = 0; m<column; m++){ //collumn
			printf("%d ", *(board + l*column + m));
		}
		printf("\n");
	}
}

void prettyBoard() { // h ehe c o l o r s
	printf("Current Status: \033[1;32m%s\033[0m\n\n", status);
	
	printf("\n  --\033[1;36mA\033[0m---\033[1;36mB\033[0m---\033[1;36mC\033[0m");
	printf("---\033[1;36mD\033[0m---\033[1;36mE\033[0m---\033[1;36mF\033[0m");
	printf("---\033[1;36mG\033[0m---\n");
	for(int i=0; i<row;i++) {
		printf("%d |", i);
		for(int j=0; j<column; j++) {
			printf("(");
			switch(*(board + i*column + j)) {
				case 0:
					printf(" ");
					break;
				case 1:
					printf("\033[1;31m%d\033[0m", *(board + i*column + j));
					break;
				case 2:
					printf("\033[1;33m%d\033[0m", *(board + i*column + j));
					break;
			}
			printf(") ");
		}	
		printf("|\n");
	}
	printf("  |\\--------------------------/|\n");
	printf("  |_|                        |_|\n");
}


//keeps track of the turn of which player goes
void playerTurn() {
	switch(turn) {
		case 1: //starts off P1 turn
			printf("\033[1;31m%s\033[0m's turn! ", player1);
			strcpy(currentPlayer, player1);
			turn++; //progress to P2 turn
			break;
		case 2:
			printf("\033[1;33m%s\033[0ms's turn! ", player2);
			strcpy(currentPlayer, player2);
			turn--; //go back to P1 turn
			break;
	}
}


/* ending message
  – this states the reason of the games end, whether the
	player quit or if someone won.
*/
void endText() {
	switch(status[0]) {
		case 'w': //starts off P1 turn
			printf("\033[1;35m%s\033[0m has won!\n", currentPlayer);
			break;
		case 'q':
			printf("Come play again next time!\n");
			break;
	}
}


/* MAIN
– call initialization, then loop until a flag is set (game over or a player quit),
– calling accept input, update and display.
– Outside of the loop, call teardown
*/
int main(int argc, char const *argv[]) {
	
	init();
	while(status[0] == 'p') {
		playerTurn();
		char userInput = getInput();
		updateWorld(userInput);
		//displayWorld();
		prettyBoard();
	}
	
	endText();
	teardown();
}