#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 50  // Maximum length for player names

// Initialize the board - creates a 2D array for the game board
char **initializeBoard(int size) {
    // Allocate memory for rows (array of pointers)
    char **board = (char **)malloc(size * sizeof(char *));
    
    // Allocate memory for each row and initialize with empty spaces
    for (int i = 0; i < size; i++) {
        board[i] = (char *)malloc(size * sizeof(char));
        for (int j = 0; j < size; j++) {
            board[i][j] = ' ';  // Set each cell to empty space
        }
    }
    return board;  // Return the created board
}

// Display the board - shows the current state of the game
void displayBoard(char **board, int size) {
    printf("\n    ");
    // Print column numbers at the top (1, 2, 3, ...)
    for (int j = 0; j < size; j++) {
        printf(" %2d ", j + 1);
    }
    
    // Print the top border of the board
    printf("\n    ");
    for (int j = 0; j < size; j++) {
        printf("+---");
    }
    printf("+\n");

    // Print each row with row numbers and cell contents
    for (int i = 0; i < size; i++) {
        printf(" %2d ", i + 1);  // Row number on the left
        for (int j = 0; j < size; j++) {
            printf("| %c ", board[i][j]);  // Cell content with borders
        }
        printf("|\n    ");
        
        // Print separator line between rows
        for (int j = 0; j < size; j++) {
            printf("+---");
        }
        printf("+\n");
    }
    printf("\n");
}

// Validate move - checks if the chosen position is valid
int isValidMove(char **board, int row, int col, int size) {
    // Check if the position is within board boundaries
    if (row < 0 || row >= size || col < 0 || col >= size) return 0;
    
    // Check if the cell is already occupied
    if (board[row][col] != ' ') return 0;
    
    return 1;  // Return 1 if move is valid
}

// Check win condition - determines if a player has won
int checkWin(char **board, int size, char symbol) {
    int win;  // Flag to track if we found a winning line

    // Check all rows for a win
    for (int i = 0; i < size; i++) {
        win = 1;  // Start by assuming this row is a win
        for (int j = 0; j < size; j++) {
            if (board[i][j] != symbol) {
                win = 0;  // Found a cell without the symbol, so not a win
                break;
            }
        }
        if (win) return 1;  // If all cells in row have the symbol, return win
    }

    // Check all columns for a win
    for (int j = 0; j < size; j++) {
        win = 1;
        for (int i = 0; i < size; i++) {
            if (board[i][j] != symbol) {
                win = 0;
                break;
            }
        }
        if (win) return 1;  // If all cells in column have the symbol, return win
    }

    // Check main diagonal (top-left to bottom-right)
    win = 1;
    for (int i = 0; i < size; i++) {
        if (board[i][i] != symbol) {
            win = 0;
            break;
        }
    }
    if (win) return 1;  // If main diagonal has all symbols, return win

    // Check anti-diagonal (top-right to bottom-left)
    win = 1;
    for (int i = 0; i < size; i++) {
        if (board[i][size - i - 1] != symbol) {
            win = 0;
            break;
        }
    }
    if (win) return 1;  // If anti-diagonal has all symbols, return win

    return 0;  // No winning condition found
}

// Check draw - determines if the game is a draw (no empty spaces left)
int checkDraw(char **board, int size) {
    // Check every cell on the board
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == ' ')  // If any cell is empty
                return 0;  // Game is not a draw
    
    return 1;  // All cells are filled, so it's a draw
}

// Computer move (random) - generates a random valid move for computer
void computerMove(char **board, int size, int *row, int *col) {
    // Keep generating random positions until we find an empty one
    do {
        *row = rand() % size;  // Random row between 0 and size-1
        *col = rand() % size;  // Random column between 0 and size-1
    } while (board[*row][*col] != ' ');  // Repeat if cell is occupied
}

// Free memory - releases all allocated memory for the board
void freeBoard(char **board, int size) {
    // Free each row individually
    for (int i = 0; i < size; i++) {
        free(board[i]);
    }
    // Free the array of pointers
    free(board);
}

// Save game result - writes game outcome to a file
void saveGameResult(FILE *fp, const char *winnerName, int boardSize, int mode, char playerNames[][MAX_NAME]) {
    // Write game mode (1: PvP, 2: PvC, 3: 3 Players)
    fprintf(fp, "Game Mode: %d\n", mode);
    
    // Write board size
    fprintf(fp, "Board Size: %d x %d\n", boardSize, boardSize);
    
    // Write player names and their symbols
    fprintf(fp, "Players: ");
    int playerCount = (mode == 3) ? 3 : 2;  // 3 players for mode 3, otherwise 2
    for (int i = 0; i < playerCount; i++) {
        // "XOZ" string provides symbols: X for player 0, O for player 1, Z for player 2
        fprintf(fp, "%s (%c)%s", playerNames[i], "XOZ"[i], (i < playerCount - 1) ? ", " : "\n");
    }

    // Write the winner or draw result
    if (winnerName != NULL)
        fprintf(fp, "Winner: %s\n", winnerName);
    else
        fprintf(fp, "Result: Draw\n");

    // Add separator line for readability in the file
    fprintf(fp, "-----------------------------\n");
}

// Main function - program entry point
int main() {
    int size, mode;  // Board size and game mode
    srand(time(NULL));  // Seed random number generator for computer moves

    // Display game header
    printf("=================================\n");
    printf("      TIC-TAC-TOE GAME\n");
    printf("      By: IT25100502\n");
    printf("=================================\n");

    // Get game mode from user
    printf("\nGame Modes:\n");
    printf("1. Two Players\n");
    printf("2. Play vs Computer\n");
    printf("3. Three Players\n");
    printf("Enter choice (1-3): ");
    scanf("%d", &mode);

    // Get board size from user
    printf("Enter board size (3 to 10): ");
    scanf("%d", &size);
    // Validate board size
    if (size < 3 || size > 10) {
        printf("Wrong size.\n");
        return 1;  // Exit program with error code
    }

    // Open file for appending game results
    FILE *fp = fopen("game_results.txt", "a");
    if (fp == NULL) {
        printf("Cannot open file.\n");
        return 1;  // Exit if file cannot be opened
    }

    char playerNames[3][MAX_NAME];  // Array to store player names
    char symbols[3] = {'X', 'O', 'Z'};  // Symbols for players

    // Get player names based on selected game mode
    if (mode == 1) {
        // Two Player mode
        printf("Enter Player 1 name (X): ");
        scanf("%s", playerNames[0]);
        printf("Enter Player 2 name (O): ");
        scanf("%s", playerNames[1]);
    } else if (mode == 2) {
        // Player vs Computer mode
        printf("Enter your name (X): ");
        scanf("%s", playerNames[0]);
        strcpy(playerNames[1], "Computer");  // Set computer name
    } else if (mode == 3) {
        // Three Player mode
        printf("Enter Player 1 name (X): ");
        scanf("%s", playerNames[0]);
        printf("Enter Player 2 name (O): ");
        scanf("%s", playerNames[1]);
        printf("Enter Player 3 name (Z): ");
        scanf("%s", playerNames[2]);
    }

    // Initialize the game board
    char **board = initializeBoard(size);
    int row, col, turn = 0;  // turn counter to track current player

    // Main game loop - continues until win or draw
    while (1) {
        displayBoard(board, size);  // Show current board state
        
        // Determine current player and their symbol
        int currentPlayer = turn % ((mode == 3) ? 3 : 2);  // Cycle through players
        char currentSymbol = symbols[currentPlayer];  // Get symbol for current player

        // Computer's turn (only in mode 2 when it's computer's turn)
        if (mode == 2 && currentPlayer == 1) {
            printf("%s's turn (%c)...\n", playerNames[currentPlayer], currentSymbol);
            computerMove(board, size, &row, &col);  // Computer makes random move
        } else {
            // Human player's turn - get input from user
            printf("%s's turn (%c). Enter row and column (1 to %d): ", 
                   playerNames[currentPlayer], currentSymbol, size);
            scanf("%d %d", &row, &col);
            row -= 1;  // Convert from 1-based to 0-based indexing
            col -= 1;
        }

        // Validate the move
        if (!isValidMove(board, row, col, size)) {
            printf("Bad move! Try again.\n");
            continue;  // Skip rest of loop and try again
        }

        // Execute the valid move
        board[row][col] = currentSymbol;
        
        // Log the move to file
        fprintf(fp, "Move %d: %s (%c) -> Row %d, Col %d\n", 
                turn + 1, playerNames[currentPlayer], currentSymbol, row + 1, col + 1);

        // Check if current player has won
        if (checkWin(board, size, currentSymbol)) {
            displayBoard(board, size);  // Show final board
            printf("%s wins!\n", playerNames[currentPlayer]);
            saveGameResult(fp, playerNames[currentPlayer], size, mode, playerNames);
            break;  // Exit game loop
        } 
        // Check if game is a draw
        else if (checkDraw(board, size)) {
            displayBoard(board, size);  // Show final board
            printf("Game draw!\n");
            saveGameResult(fp, NULL, size, mode, playerNames);
            break;  // Exit game loop
        }

        turn++;  // Move to next player
    }

    // Cleanup before program exit
    fclose(fp);           // Close the results file
    freeBoard(board, size);  // Free allocated memory for board
    
    return 0;  // Program ended successfully
}
