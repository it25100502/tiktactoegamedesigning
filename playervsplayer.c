#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char** initBoard(int N) {
    char** board = (char**)malloc(N * sizeof(char*));
    if (board == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    for (int i = 0; i < N; i++) {
        board[i] = (char*)malloc(N * sizeof(char));
        if (board[i] == NULL) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
        for (int j = 0; j < N; j++) {
            board[i][j] = ' ';
        }
    }
    return board;
}

void displayBoard(char** board, int N) {
    // Print column headers
    printf("  ");
    for (int j = 1; j <= N; j++) {
        printf("%2d", j);
    }
    printf("\n");

    // Print the board
    for (int i = 1; i <= N; i++) {
        printf("%2d", i);
        for (int j = 0; j < N; j++) {
            printf("%2c", board[i - 1][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void getMove(int* row, int* col, char** board, int N, char player) {
    int r, c;
    do {
        printf("Player %c, enter row (1-%d): ", player, N);
        if (scanf("%d", &r) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        if (r < 1 || r > N) {
            printf("Row must be between 1 and %d.\n", N);
            continue;
        }

        printf("Enter column (1-%d): ", N);
        if (scanf("%d", &c) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        if (c < 1 || c > N) {
            printf("Column must be between 1 and %d.\n", N);
            continue;
        }

        if (board[r - 1][c - 1] != ' ') {
            printf("That cell is already occupied. Choose another position.\n");
            continue;
        }

        break;
    } while (1);

    *row = r;
    *col = c;
}

bool isValidMove(int row, int col, char** board, int N) {
    if (row < 1 || row > N || col < 1 || col > N) {
        return false;
    }
    if (board[row - 1][col - 1] != ' ') {
        return false;
    }
    return true;
}

void makeMove(char** board, int row, int col, char player) {
    board[row - 1][col - 1] = player;
}

bool checkWin(char** board, int N, char player) {
    // Check rows
    for (int i = 0; i < N; i++) {
        bool rowWin = true;
        for (int j = 0; j < N; j++) {
            if (board[i][j] != player) {
                rowWin = false;
                break;
            }
        }
        if (rowWin) return true;
    }

    // Check columns
    for (int j = 0; j < N; j++) {
        bool colWin = true;
        for (int i = 0; i < N; i++) {
            if (board[i][j] != player) {
                colWin = false;
                break;
            }
        }
        if (colWin) return true;
    }

    // Check main diagonal
    bool diag1Win = true;
    for (int i = 0; i < N; i++) {
        if (board[i][i] != player) {
            diag1Win = false;
            break;
        }
    }
    if (diag1Win) return true;

    // Check anti-diagonal
    bool diag2Win = true;
    for (int i = 0; i < N; i++) {
        if (board[i][N - 1 - i] != player) {
            diag2Win = false;
            break;
        }
    }
    if (diag2Win) return true;

    return false;
}

bool isDraw(char** board, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}

void logBoard(char** board, int N, FILE* logf, int moveNum, char player) {
    if (logf == NULL) return;
    fprintf(logf, "After move %d by player %c:\n", moveNum, player);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(logf, "%c ", board[i][j]);
        }
        fprintf(logf, "\n");
    }
    fprintf(logf, "------------------------\n\n");
}

int main() {
    int N;
    printf("Welcome to N x N Tic-Tac-Toe!\n");
    printf("Enter grid size N (3 <= N <= 10): ");
    if (scanf("%d", &N) != 1 || N < 3 || N > 10) {
        printf("Invalid grid size. Exiting.\n");
        return 1;
    }

    char** board = initBoard(N);
    FILE* logf = fopen("game.log", "w");
    if (logf == NULL) {
        printf("Cannot open log file 'game.log' for writing.\n");
        // Free board before exit
        for (int i = 0; i < N; i++) free(board[i]);
        free(board);
        return 1;
    }

    // Log initial board
    fprintf(logf, "Initial board (empty):\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(logf, "%c ", board[i][j]);
        }
        fprintf(logf, "\n");
    }
    fprintf(logf, "------------------------\n\n");

    bool gameOver = false;
    int moveNum = 0;
    char currentPlayer = 'X';

    printf("Player X goes first. 'X' and 'O' take turns.\n");
    printf("Win by completing a full row, column, or diagonal.\n\n");

    while (!gameOver) {
        displayBoard(board, N);

        int row, col;
        getMove(&row, &col, board, N, currentPlayer);

        // Since getMove already validates, we can directly make the move
        makeMove(board, row, col, currentPlayer);
        moveNum++;

        logBoard(board, N, logf, moveNum, currentPlayer);

        if (checkWin(board, N, currentPlayer)) {
            displayBoard(board, N);
            printf("Player %c wins!\n", currentPlayer);
            gameOver = true;
            break;
        }

        if (isDraw(board, N)) {
            displayBoard(board, N);
            printf("The board is full. It's a draw!\n");
            gameOver = true;
            break;
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    printf("Game over. Check 'game.log' for the move history.\n");

    fclose(logf);

    // Free allocated memory
    for (int i = 0; i < N; i++) {
        free(board[i]);
    }
    free(board);

    return 0;
}
