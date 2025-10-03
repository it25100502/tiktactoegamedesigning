
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 50

// Initialize the board
char **initializeBoard(int size) {
    char **board = (char **)malloc(size * sizeof(char *));
    for (int i = 0; i < size; i++) {
        board[i] = (char *)malloc(size * sizeof(char));
        for (int j = 0; j < size; j++) {
            board[i][j] = ' ';
        }
    }
    return board;
}

// Display the board
void displayBoard(char **board, int size) {
    printf("\n    ");
    for (int j = 0; j < size; j++) {
        printf(" %2d ", j + 1);
    }
    printf("\n    ");
    for (int j = 0; j < size; j++) {
        printf("+---");
    }
    printf("+\n");

    for (int i = 0; i < size; i++) {
        printf(" %2d ", i + 1);
        for (int j = 0; j < size; j++) {
            printf("| %c ", board[i][j]);
        }
        printf("|\n    ");
        for (int j = 0; j < size; j++) {
            printf("+---");
        }
        printf("+\n");
    }
    printf("\n");
}

// Validate move
int isValidMove(char **board, int row, int col, int size) {
    if (row < 0 || row >= size || col < 0 || col >= size) return 0;
    if (board[row][col] != ' ') return 0;
    return 1;
}

// Check win condition
int checkWin(char **board, int size, char symbol) {
    int win;

    // Rows
    for (int i = 0; i < size; i++) {
        win = 1;
        for (int j = 0; j < size; j++) {
            if (board[i][j] != symbol) {
                win = 0;
                break;
            }
        }
        if (win) return 1;
    }

    // Columns
    for (int j = 0; j < size; j++) {
        win = 1;
        for (int i = 0; i < size; i++) {
            if (board[i][j] != symbol) {
                win = 0;
                break;
            }
        }
        if (win) return 1;
    }

    // Main diagonal
    win = 1;
    for (int i = 0; i < size; i++) {
        if (board[i][i] != symbol) {
            win = 0;
            break;
        }
    }
    if (win) return 1;

    // Anti-diagonal
    win = 1;
    for (int i = 0; i < size; i++) {
        if (board[i][size - i - 1] != symbol) {
            win = 0;
            break;
        }
    }
    if (win) return 1;

    return 0;
}

// Check draw
int checkDraw(char **board, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1;
}

// Computer move (random)
void computerMove(char **board, int size, int *row, int *col) {
    do {
        *row = rand() % size;
        *col = rand() % size;
    } while (board[*row][*col] != ' ');
}

// Free memory
void freeBoard(char **board, int size) {
    for (int i = 0; i < size; i++) {
        free(board[i]);
    }
    free(board);
}

// Save game result
void saveGameResult(FILE *fp, const char *winnerName, int boardSize, int mode, char playerNames[][MAX_NAME]) {
    fprintf(fp, "Game Mode: %d\n", mode);
    fprintf(fp, "Board Size: %d x %d\n", boardSize, boardSize);
    fprintf(fp, "Players: ");
    int playerCount = (mode == 3) ? 3 : 2;
    for (int i = 0; i < playerCount; i++) {
        fprintf(fp, "%s (%c)%s", playerNames[i], "XOZ"[i], (i < playerCount - 1) ? ", " : "\n");
    }

    if (winnerName != NULL)
        fprintf(fp, "Winner: %s\n", winnerName);
    else
        fprintf(fp, "Result: Draw\n");

    fprintf(fp, "-----------------------------\n");
}

// Main function
int main() {
    int size, mode;
    srand(time(NULL));

    printf("=================================\n");
    printf("      TIC-TAC-TOE GAME\n");
    printf("      Designed by IT25100502\n");
    printf("=================================\n");

    printf("\nSelect Game Mode:\n");
    printf("1. Player vs Player\n");
    printf("2. Player vs Computer\n");
    printf("3. Three Player Mode\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &mode);

    printf("Enter board size (3 to 10): ");
    scanf("%d", &size);
    if (size < 3 || size > 10) {
        printf("Invalid size.\n");
        return 1;
    }

    FILE *fp = fopen("game_results.txt", "a");
    if (fp == NULL) {
        printf("Error opening game_results.txt\n");
        return 1;
    }

    char playerNames[3][MAX_NAME];
    char symbols[3] = {'X', 'O', 'Z'};

    if (mode == 1) {
        printf("Enter name for Player 1 (X): ");
        scanf("%s", playerNames[0]);
        printf("Enter name for Player 2 (O): ");
        scanf("%s", playerNames[1]);
    } else if (mode == 2) {
        printf("Enter your name (X): ");
        scanf("%s", playerNames[0]);
        strcpy(playerNames[1], "Computer");
    } else if (mode == 3) {
        printf("Enter name for Player 1 (X): ");
        scanf("%s", playerNames[0]);
        printf("Enter name for Player 2 (O): ");
        scanf("%s", playerNames[1]);
        printf("Enter name for Player 3 (Z): ");
        scanf("%s", playerNames[2]);
    }

    char **board = initializeBoard(size);
    int row, col, turn = 0;

    while (1) {
        displayBoard(board, size);
        int currentPlayer = turn % ((mode == 3) ? 3 : 2);
        char currentSymbol = symbols[currentPlayer];

        if (mode == 2 && currentPlayer == 1) {
            printf("%s's turn (%c)...\n", playerNames[currentPlayer], currentSymbol);
            computerMove(board, size, &row, &col);
        } else {
            printf("%s's turn (%c). Enter row and column (1 to %d): ", playerNames[currentPlayer], currentSymbol, size);
            scanf("%d %d", &row, &col);
            row -= 1;
            col -= 1;
        }

        if (!isValidMove(board, row, col, size)) {
            printf("Invalid move! Try again.\n");
            continue;
        }

        board[row][col] = currentSymbol;
        fprintf(fp, "Move %d: %s (%c) -> Row %d, Col %d\n", turn + 1, playerNames[currentPlayer], currentSymbol, row + 1, col + 1);

        if (checkWin(board, size, currentSymbol)) {
            displayBoard(board, size);
            printf("%s wins!\n", playerNames[currentPlayer]);
            saveGameResult(fp, playerNames[currentPlayer], size, mode, playerNames);
            break;
        } else if (checkDraw(board, size)) {
            displayBoard(board, size);
            printf("It's a draw!\n");
            saveGameResult(fp, NULL, size, mode, playerNames);
            break;
        }

        turn++;
    }

    fclose(fp);
    freeBoard(board, size);
    return 0;
}
