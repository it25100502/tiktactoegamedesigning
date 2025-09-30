#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// This function creates an empty N x N game board using dynamic memory.
// It allocates space for the board and fills it with spaces (' ').
char** initBoard(int N) {
    // Allocate memory for rows (an array of pointers)
    char** board = (char**)malloc(N * sizeof(char*));
    if (board == NULL) {  // Check if memory allocation failed
        printf("Error: Could not allocate memory for the board.\n");
        exit(1);  // Stop the program if there's an error
    }
    
    // For each row, allocate memory for columns and fill with spaces
    for (int i = 0; i < N; i++) {
        board[i] = (char*)malloc(N * sizeof(char));
        if (board[i] == NULL) {  // Check for allocation error
            printf("Error: Could not allocate memory for row %d.\n", i);
            exit(1);
        }
        // Initialize each cell to empty space
        for (int j = 0; j < N; j++) {
            board[i][j] = ' ';  // ' ' means empty cell
        }
    }
    return board;  // Return the created board
}

// This function prints the current state of the board to the screen.
// It shows row and column numbers for easy input (1-based indexing).
void displayBoard(char** board, int N) {
    // Print column numbers at the top (1 to N)
    printf("  ");  // Two spaces for alignment
    for (int j = 1; j <= N; j++) {
        printf("%2d", j);  // Print each column number with 2 spaces width
    }
    printf("\n");  // New line after headers

    // Print each row of the board
    for (int i = 1; i <= N; i++) {
        printf("%2d", i);  // Print row number (1-based)
        for (int j = 0; j < N; j++) {
            printf("%2c", board[i - 1][j]);  // Print cell content (0-based index)
        }
        printf("\n");  // New line after each row
    }
    printf("\n");  // Extra space after the board
}

// This function asks a player for their move (row and column).
// It keeps asking until a valid move is entered.
// Parameters: row and col will be updated with the player's choice.
void getMove(int* row, int* col, char** board, int N, char player) {
    int r, c;  // Temporary variables for row and column input
    printf("It's your turn, Player %c!\n", player);
    
    // Keep looping until a valid move is made
    do {
        // Ask for row
        printf("Enter row number (1 to %d): ", N);
        if (scanf("%d", &r) != 1) {  // Check if input is a valid number
            printf("Error: Please enter a valid number for the row.\n");
            while (getchar() != '\n');  // Clear any extra characters from input
            continue;  // Ask again
        }
        if (r < 1 || r > N) {  // Check if row is in range
            printf("Row must be between 1 and %d. Try again.\n", N);
            continue;
        }

        // Ask for column
        printf("Enter column number (1 to %d): ", N);
        if (scanf("%d", &c) != 1) {  // Check if input is a valid number
            printf("Error: Please enter a valid number for the column.\n");
            while (getchar() != '\n');  // Clear input buffer
            continue;
        }
        if (c < 1 || c > N) {  // Check if column is in range
            printf("Column must be between 1 and %d. Try again.\n", N);
            continue;
        }

        // Check if the chosen cell is already taken
        if (board[r - 1][c - 1] != ' ') {
            printf("Sorry, that cell is already taken! Choose an empty cell.\n");
            continue;
        }

        // If we reach here, the move is valid
        break;  // Exit the loop
    } while (true);  // Infinite loop until valid input

    // Update the pointers with the valid row and column
    *row = r;
    *col = c;
}

// This function checks if a move is valid (in bounds and empty cell).
// It returns true if valid, false otherwise.
bool isValidMove(int row, int col, char** board, int N) {
    // Check if row and column are within 1 to N
    if (row < 1 || row > N || col < 1 || col > N) {
        return false;
    }
    // Check if the cell is empty
    if (board[row - 1][col - 1] != ' ') {
        return false;
    }
    return true;  // Valid if both checks pass
}

// This function places the player's mark ('X' or 'O') on the board.
void makeMove(char** board, int row, int col, char player) {
    board[row - 1][col - 1] = player;  // Place mark (convert to 0-based index)
    printf("You placed %c at row %d, column %d.\n", player, row, col);
}

// This function checks if the current player has won.
// It checks all rows, columns, and both diagonals for a full line of the player's mark.
bool checkWin(char** board, int N, char player) {
    // Step 1: Check all rows
    for (int i = 0; i < N; i++) {  // For each row
        bool rowFull = true;  // Assume the row is full of player's marks
        for (int j = 0; j < N; j++) {  // Check each cell in the row
            if (board[i][j] != player) {
                rowFull = false;  // Not full if any cell doesn't match
                break;  // No need to check further
            }
        }
        if (rowFull) {
            return true;  // Win found!
        }
    }

    // Step 2: Check all columns
    for (int j = 0; j < N; j++) {  // For each column
        bool colFull = true;  // Assume the column is full
        for (int i = 0; i < N; i++) {  // Check each cell in the column
            if (board[i][j] != player) {
                colFull = false;
                break;
            }
        }
        if (colFull) {
            return true;
        }
    }

    // Step 3: Check main diagonal (top-left to bottom-right)
    bool mainDiagFull = true;
    for (int i = 0; i < N; i++) {
        if (board[i][i] != player) {
            mainDiagFull = false;
            break;
        }
    }
    if (mainDiagFull) {
        return true;
    }

    // Step 4: Check anti-diagonal (top-right to bottom-left)
    bool antiDiagFull = true;
    for (int i = 0; i < N; i++) {
        if (board[i][N - 1 - i] != player) {
            antiDiagFull = false;
            break;
        }
    }
    if (antiDiagFull) {
        return true;
    }

    // No win found
    return false;
}

// This function checks if the game is a draw (board full with no winner).
// It returns true if every cell is filled (no empty spaces).
bool isDraw(char** board, int N) {
    // Loop through all cells
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == ' ') {  // If any empty cell found
                return false;  // Not a draw yet
            }
        }
    }
    return true;  // All cells filled = draw
}

// This function writes the current board state to a log file.
// It logs after each move for history.
void logBoard(char** board, int N, FILE* logf, int moveNum, char player) {
    if (logf == NULL) {  // If file can't be opened, skip logging
        return;
    }
    // Write header for this move
    fprintf(logf, "After move #%d by Player %c:\n", moveNum, player);
    // Write the board row by row
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(logf, "%c ", board[i][j]);  // Print cell with space
        }
        fprintf(logf, "\n");  // New line after each row
    }
    fprintf(logf, "------------------------\n\n");  // Separator for next move
}

// Main function: Sets up the game and runs the main loop.
int main() {
    int N;  // Grid size
    
    // Welcome message and get grid size
    printf("Welcome to N x N Tic-Tac-Toe Game!\n");
    printf("This is a two-player game where you take turns placing 'X' or 'O'.\n");
    printf("The goal is to get a full row, column, or diagonal of your marks.\n\n");
    
    printf("Enter the grid size N (must be 3 to 10): ");
    if (scanf("%d", &N) != 1 || N < 3 || N > 10) {
        printf("Invalid size! It must be between 3 and 10. Game ending.\n");
        return 1;  // Exit with error
    }
    printf("Great! We'll play on a %d x %d board.\n\n", N, N);

    // Create the empty board
    char** board = initBoard(N);
    
    // Open the log file for writing (creates 'game.log' if it doesn't exist)
    FILE* logf = fopen("game.log", "w");  // "w" means write mode
    if (logf == NULL) {
        printf("Warning: Could not create log file 'game.log'. Continuing without logging.\n");
    } else {
        // Log the initial empty board
        fprintf(logf, "=== TIC-TAC-TOE GAME LOG ===\n");
        fprintf(logf, "Grid size: %d x %d\n\n", N, N);
        fprintf(logf, "Initial empty board:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                fprintf(logf, "%c ", board[i][j]);
            }
            fprintf(logf, "\n");
        }
        fprintf(logf, "------------------------\n\n");
    }

    // Game variables
    bool gameOver = false;  // True when game ends
    int moveNum = 0;  // Count of moves made
    char currentPlayer = 'X';  // X starts first

    printf("Player 'X' goes first. Players alternate turns.\n");
    printf("Enter row and column numbers (1-based) to place your mark.\n\n");

    // Main game loop: Continues until win or draw
    while (!gameOver) {
        // Show the current board
        displayBoard(board, N);

        // Get the move from the current player
        int row, col;
        getMove(&row, &col, board, N, currentPlayer);

        // Place the mark on the board
        makeMove(board, row, col, currentPlayer);
        moveNum++;  // Increment move count

        // Log this move to file
        logBoard(board, N, logf, moveNum, currentPlayer);

        // Check if current player won
        if (checkWin(board, N, currentPlayer)) {
            displayBoard(board, N);  // Show final board
            printf("Congratulations! Player %c wins the game!\n", currentPlayer);
            gameOver = true;
            break;  // End the game
        }

        // Check if it's a draw (board full)
        if (isDraw(board, N)) {
            displayBoard(board, N);  // Show final board
            printf("The board is full with no winner. It's a draw!\n");
            gameOver = true;
            break;
        }

        // Switch to the other player
        if (currentPlayer == 'X') {
            currentPlayer = 'O';
        } else {
            currentPlayer = 'X';
        }
    }

    // Game end message
    printf("\nGame Over! Thanks for playing.\n");
    if (logf != NULL) {
        printf("Check the file 'game.log' to see the full move history.\n");
        fclose(logf);  // Close the log file
    }

    // Clean up: Free all allocated memory to avoid memory leaks
    for (int i = 0; i < N; i++) {
        free(board[i]);  // Free each row
    }
    free(board);  // Free the array of row pointers

    return 0;  // Successful program end
}
