#include <stdio.h>
#include <stdlib.h>

#define TRUE		1
#define FALSE		0

#define START		0
#define EASY		1
#define MEDIUM		2
#define HARD		3
#define FINISHED	4
#define QUIT		5

typedef int boolean;

// Function Declarations
void displayMenu();
void startGame();
void outputGrid();
void get_user_input();
void cp_play();
boolean check_grid();
boolean check_line(int, int, int);
void cleanup_game();
void shuffle_easy();
int search_winning_piece(int, char);
int close_piece(int, char);

// Global Variables
char game_state[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' '};
int easy_state[9] = {0,1,2,3,4,5,6,7,8};
int *ptrPlay;
int play_state;
int user_select;
boolean testing;
char test_turn[5];
boolean auto_play;
int turn;

int main(int argc, char *argv[]) {
	play_state = START;
	shuffle_easy();
	int c;
	c = 1;
	turn = 0;

	auto_play = FALSE;

	for (c = 1; c < argc; c++) {
		test_turn[0] = argv[c][0];
		test_turn[1] = argv[c][1];
		test_turn[2] = argv[c][2];
		test_turn[3] = argv[c][3];
		test_turn[4] = argv[c][4];
		auto_play = TRUE;
	}


	while (play_state != QUIT) {
		switch (play_state) {
		case START:
			displayMenu();
			break;

		case EASY:
		case MEDIUM:
			ptrPlay = &easy_state[0];
			startGame();
			break;

		case HARD:
			ptrPlay = NULL;
			startGame();
			break;

		case FINISHED:
			if (auto_play == TRUE)
				play_state = QUIT;
			else {
				cleanup_game();
				displayMenu();
			}
			break;

		case QUIT:

			break;
		}
	}
	if (auto_play == FALSE) {
		printf("Thank you for playing\n");
	}
	return (0);
}


void displayMenu() {
	int user_input;

	if (auto_play == TRUE) {
		user_input = 3;
	} else {
		printf(" ----   Welcome to noughts and crosses   ----\n");
		printf(" ---- Please select an option from below -----\n");
		printf("1. To start a new easy game\n");
		printf("2. To start a new medium game\n");
		printf("3. To start a new hard game\n");
		printf("4. To quit nought and crosses\n");
		scanf("%d", &user_input);
	}
	switch (user_input) {
	case 1:
		play_state = EASY;
		break;
	case 2:
		play_state = MEDIUM;
		break;
	case 3:
		play_state = HARD;
		break;
	case 4:
		play_state = QUIT;
		break;
	}
}

void startGame() {

	outputGrid();
	get_user_input();
	if (check_grid() == TRUE) {
		play_state = FINISHED;
	} else {
		cp_play();
		if (check_grid() == TRUE) {
			play_state = FINISHED;
		}
	}

}

void outputGrid() {

	if (auto_play == FALSE) {
		system("clear");
		printf("\n\n\n\n");
		printf("	 %c	  |	 %c	 |	 %c	 \n", game_state[0], game_state[1], game_state[2]);
		printf("---------------------------------------\n");
		printf("	 %c	  |	 %c	 |	 %c	 \n", game_state[3], game_state[4], game_state[5]);
		printf("---------------------------------------\n");
		printf("	 %c	  |	 %c	 |	 %c	 \n", game_state[6], game_state[7], game_state[8]);
	}
}

void get_user_input() {
	int c;
	int d;
	d = 0;
	if (auto_play == TRUE) {
		for (c = 0; c < 9; c++) {
			if (game_state[c] == ' ') {
				d++;
				if (test_turn[turn] == (char)(((int)'0')+d)) {
					game_state[c] = 'X';
					turn++;
					return;
				}
			}
		}
	} else {
		printf("Select a number from 1 - 9 based on the grid above, or 0 to quit\n");
		scanf("%d", &user_select);
	}
	if (user_select == 0) {
		play_state = FINISHED;
		return;
	}
	if (game_state[user_select - 1] == 'X' || game_state[user_select - 1] == 'O') {
	if (auto_play == FALSE) {
		printf("Invalid Option\n");
		get_user_input();
	} else {
		printf("Invalid option with %c%c%c%c%c\n", test_turn[0], test_turn[1], test_turn[2], test_turn[3], test_turn[4]);
		play_state = FINISHED;
	}
	} else {
		game_state[user_select - 1] = 'X';

	}

}

void cp_play() {
	int i;
	int j;
	int weight;
	int piece;
	int temp_weight;

	// First check to see if we have a pre defined move in the collection
	if (ptrPlay != NULL) {
		if (play_state == MEDIUM) {
			for (i = 0; i < 9; i++) {
				if (game_state[i] == ' ') {
					temp_weight = search_winning_piece(i, 'O');
					temp_weight = temp_weight + search_winning_piece(i, 'X');
					if (temp_weight > 0) {
						game_state[i] = 'O';
						return;
					}
				}
			}
		}
		// loop through our ptrPlay array and fill out the next available square
		for (i = 0; i < 9; i++) {
			if ( game_state[ptrPlay[i]] != 'X' && game_state[ptrPlay[i]] != 'O') {
				game_state[ptrPlay[i]] = 'O';
				return;
			}
		}

	} else {
		if (game_state[4] == 'X' && game_state[8] == 'X' && game_state[2] != 'O') {
			game_state[2] = 'O';
			return;
		}

		// Computer decides on it's turn by assigning a weighting to each value on the board
		// Whichever piece has the highest weighting will be selected
		weight = 0;
		piece = 0;
		for (i = 0; i < 9; i++) {
			if (game_state[i] != 'X' && game_state[i] != 'O') {
				temp_weight = 0;
				if (i == 4) {
					temp_weight = 10;
				}
				temp_weight = temp_weight + search_winning_piece(i, 'O');
				temp_weight = temp_weight + search_winning_piece(i, 'X');
				temp_weight = temp_weight + close_piece(i, 'O');
				temp_weight = temp_weight + close_piece(i, 'X');

				if (temp_weight > weight) {
					weight = temp_weight;
					piece = i;
				}
			}
		}
		if (weight > 0) {
			game_state[piece] = 'O';
		} else {
			for (j = 0; j < 9; j++) {
				if (game_state[j] == ' '){
					game_state[j] = 'O';
					return;
				}
			}
		}
	}

}

boolean check_grid() {
	int i;

	// Game Complete == 012 - 345 - 678 - 036 - 147 - 258 - 048 - 246
	if (check_line(0, 1, 2) > 0 || check_line(3, 4, 5) > 0 || check_line(6, 7, 8) > 0 || check_line(0, 3, 6) > 0 || check_line(1, 4, 7) > 0 || check_line(2, 5, 8) > 0 || check_line(0, 4, 8) > 0 || check_line(2, 4, 6)) {
		return TRUE;
	}


	for (i = 0; i < 9; i++) {
		if (game_state[i] == ' ') {
			return FALSE;
		}
	}
	outputGrid();

	if (auto_play == TRUE) {
		printf("Drew with %c%c%c%c%c\n", test_turn[0], test_turn[1], test_turn[2], test_turn[3], test_turn[4]);
	} else {
		printf("\n\nA Draw has been reached!\n\n");
	}
	return TRUE;

}

boolean check_line(int i, int j, int k) {
	if (game_state[i] == game_state[j] && game_state[j] == game_state[k]) {
			if (game_state[i] == 'X') {
				// Player X Won (person)
				outputGrid();
				if (auto_play == TRUE) {
					printf("Holy Moly you beat the computer with %c%c%c%c%c\n", test_turn[0], test_turn[1], test_turn[2], test_turn[3], test_turn[4]);
				} else {
					printf("Congratulations - You Won\n");
				}
				return TRUE;
			} else if(game_state[i] == 'O') {
				// Player O Won (computer)
				outputGrid();
				if (auto_play == TRUE) {
					printf("Failed with %c%c%c%c%c\n", test_turn[0], test_turn[1], test_turn[2], test_turn[3], test_turn[4]);
				} else {
					printf("Bad Luck - You Lost\n");
				}
				return TRUE;
			}
	}
	return FALSE;

}

void cleanup_game() {
	int i;

	ptrPlay = NULL;
	for (i = 0; i < 9; i++) {
		game_state[i] = ' ';
		easy_state[i] = i;
	}
}

void shuffle_easy() {
	int i;
	int rand_num;

	for (i = 0; i < 9; i++) {
		rand_num = rand() % 9;
		if (rand_num != i) {
			// This operation is completely unecessary and possibly even slower than using a temporary variable to swap the values
			// ... but it was fun to do
			easy_state[i] ^= easy_state[rand_num];
			easy_state[rand_num] ^= easy_state[i];
			easy_state[i] ^= easy_state[rand_num];
		}
	}
}

int search_winning_piece(int piece, char search) {
	int rtnValue;

	if (search == 'X') {
		rtnValue = 50;
	} else {
		rtnValue = 70;
	}

	//Check vertical first
	if (piece < 3) {
		if (game_state[piece + 3] == search && game_state[piece + 6] == search) {
			return rtnValue;
		}
	} else if (piece < 6) {
		if (game_state[piece - 3] == search && game_state[piece + 3] == search) {
			return rtnValue;
		}
	} else if (piece < 9) {
		if (game_state[piece - 3] == search && game_state[piece - 6] == search) {
			return rtnValue;
		}
	}

	// Check horizontal
	if (piece == 0 || piece == 3 || piece == 6) {
		if (game_state[piece + 1] == search && game_state[piece + 2] == search) {
			return rtnValue;
		}
	} else if (piece == 1 || piece == 4 || piece == 7) {
		if (game_state[piece - 1] == search && game_state[piece + 1] == search) {
			return rtnValue;
		}
	} else if (piece == 2 || piece == 5 || piece == 8) {
		if (game_state[piece - 1] == search && game_state[piece - 2] == search) {
			return rtnValue;
		}
	}

	// 4th square must appear in any diagonal win
	if (piece != 4 && game_state[4] == ' ') {
		return 0;
	} else if (piece == 1 || piece == 3 || piece == 5 || piece == 7) {
		return 0;
	}

	switch (piece) {
	case 1:
		if (game_state[4] == search && game_state[8] == search) {
			return rtnValue;
		}
		break;
	case 2:
		if (game_state[4] == search && game_state[6] == search) {
			return rtnValue;
		}
		break;
	case 4:
		if (game_state[0] == search && game_state[8] == search) {
			return rtnValue;
		}
		if (game_state[2] == search && game_state[6] == search) {
			return rtnValue;
		}
		break;
	case 6:
		if (game_state[2] == search && game_state[4] == search) {
			return rtnValue;
		}
		break;
	case 8:
		if (game_state[0] == search && game_state[4] == search) {
			return rtnValue;
		}
		break;
	}

	return 0;
}


int close_piece(int piece, char search) {
	int rtnValue;
	int sumReturn;
	int i;
	sumReturn = 0;

	if (search == 'X') {
		rtnValue = 3;
	} else {
		rtnValue = 1;
	}


	switch (piece) {
	case 0:
		for (i = 0; i < 9; i++){
			if (game_state[i] == search && (i == 1 || i == 4 || i == 3)) {
				sumReturn = sumReturn + rtnValue;
			}
		}
		break;
	case 1:
		for (i = 0; i < 9; i++){
			if (game_state[i] == search && i != 6 && i != 7 && i != 8) {
				sumReturn = sumReturn + rtnValue;
			}
		}
		break;
	case 2:
		for (i = 0; i < 9; i++){
			if (game_state[i] == search && (i == 1 || i == 4 || i == 5)) {
				sumReturn = sumReturn + rtnValue;
			}
		}
		break;
	case 3:
		for (i = 0; i < 9; i++){
			if (game_state[i] == search && i != 2 && i != 5 && i != 8) {
				sumReturn = sumReturn + rtnValue;
			}
		}
		break;
	case 4:
		for (i = 0; i < 9; i++){
			if (game_state[i] == search && i != 4) {
				sumReturn = sumReturn + rtnValue;
			}
		}
		break;
	case 5:
		for (i = 0; i < 9; i++){
			if (game_state[i] == search && i != 0 && i != 3 && i != 6) {
				sumReturn = sumReturn + rtnValue;
			}
		}
		break;
	case 6:
		for (i = 0; i < 9; i++){
			if (game_state[i] == search && (i == 3 || i == 4 || i == 7)) {
				sumReturn = sumReturn + rtnValue;
			}
		}
		break;
	case 7:
		for (i = 0; i < 9; i++){
			if (game_state[i] == search && i != 0 && i != 1 && i != 2) {
				sumReturn = sumReturn + rtnValue;
			}
		}
		break;
	case 8:
		for (i = 0; i < 9; i++){
			if (game_state[i] == search && (i == 4 || i == 5 || i == 7)) {
				sumReturn = sumReturn + rtnValue;
			}
		}
		break;
	}
	if ((piece == 7 || piece ==  5 || piece == 3 || piece == 1) && sumReturn > 2)
	{
		sumReturn = sumReturn - 2;
	}
	return sumReturn;
}

