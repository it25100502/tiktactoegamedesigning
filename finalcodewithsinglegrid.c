#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Platform-specific includes for screen clearing
#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN() system("cls")  // Clear screen command for Windows
#else
#include <unistd.h>
#define CLEAR_SCREEN() system("clear") // Clear screen command for Linux/Mac
#endif

#define MAX_NAME 50  // Maximum length for player names


char **initializeBoard(int size) {
    // Step 1: Allocate memory for the rows (array of pointers)
    char **board = (char **)malloc(size * sizeof(char *));
    
    // Step 2: For each row, allocate memory for columns and initialize with spaces
    for (int i = 0; i < size; i++) {
        board[i] = (char *)malloc(size * sizeof(char));
        // Step 3: Initialize each cell with empty space ' '
        for (int j = 0; j < size; j++) {
            board[i][j] = ' ';  // Empty cell indicates no move made yet
        }
    }
    return board;  // Return the fully initialized board
}

void displayBoard(char **board, int size) {
    CLEAR_SCREEN();  // Clear the terminal screen for clean display
    
    // Display game header
    printf("=================================\n");
    printf("      TIC-TAC-TOE GAME\n");
    printf("      By: IT25100502\n");
    printf("=================================\n\n");
    
    // Display column numbers (1, 2, 3, ...)
    printf("    ");
    for (int j = 0; j < size; j++) {
        printf(" %2d ", j + 1);  // Print column numbers with spacing
    }
    
    // Draw the top border of the board
    printf("\n    ");
    for (int j = 0; j < size; j++) {
        printf("+---");  // Top border segments
    }
    printf("+\n");  // Complete the top border

    // Display each row with row numbers and cell contents
    for (int i = 0; i < size; i++) {
        printf(" %2d ", i + 1);  // Print row number on the left
        // Display each cell in the current row
        for (int j = 0; j < size; j++) {
            printf("| %c ", board[i][j]);  // Cell content with borders
        }
        printf("|\n    ");  // End of row
        
        // Draw separator line between rows
        for (int j = 0; j < size; j++) {
            printf("+---");  // Row separator segments
        }
        printf("+\n");  // Complete the row separator
    }
    printf("\n");  // Extra line for better readability
}


int isValidMove(char **board, int row, int col, int size) {
    // Check 1: Are the coordinates within the board boundaries?
    if (row < 0 || row >= size || col < 0 || col >= size) 
        return 0;  // Invalid - out of bounds
    
    // Check 2: Is the selected cell empty?
    if (board[row][col] != ' ') 
        return 0;  // Invalid - cell already occupied
    
    return 1;  // Valid move - within bounds and empty cell
}


int checkWin(char **board, int size, char symbol) {
    int win;  // Flag to track if we found a winning line

    // Check 1: Check all ROWS for a complete line
    for (int i = 0; i < size; i++) {
        win = 1;  // Assume this row is a win until proven otherwise
        for (int j = 0; j < size; j++) {
            if (board[i][j] != symbol) {
                win = 0;  // Found a cell without the symbol - not a winning row
                break;     // No need to check rest of this row
            }
        }
        if (win) return 1;  // If all cells in row have the symbol, player wins!
    }

    // Check 2: Check all COLUMNS for a complete line
    for (int j = 0; j < size; j++) {
        win = 1;  // Assume this column is a win
        for (int i = 0; i < size; i++) {
            if (board[i][j] != symbol) {
                win = 0;  // Found a cell without the symbol
                break;     // Stop checking this column
            }
        }
        if (win) return 1;  // Winning column found
    }

    // Check 3: Check MAIN DIAGONAL (top-left to bottom-right)
    win = 1;  // Assume diagonal is a win
    for (int i = 0; i < size; i++) {
        if (board[i][i] != symbol) {  // Check cells where row = column
            win = 0;  // Diagonal broken
            break;
        }
    }
    if (win) return 1;  // Main diagonal win

    // Check 4: Check ANTI-DIAGONAL (top-right to bottom-left)
    win = 1;  // Assume anti-diagonal is a win
    for (int i = 0; i < size; i++) {
        // Check cells where column = (size - 1 - row)
        if (board[i][size - i - 1] != symbol) {
            win = 0;  // Anti-diagonal broken
            break;
        }
    }
    if (win) return 1;  // Anti-diagonal win

    return 0;  // No winning condition found - game continues
}

int checkDraw(char **board, int size) {
    // Scan every cell on the board
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == ' ')  // If any cell is empty
                return 0;  // Game is not a draw - empty space found
    
    return 1;  // All cells are filled - game is a draw
}

void computerMove(char **board, int size, int *row, int *col) {
    // Keep generating random positions until we find an empty cell
    do {
        *row = rand() % size;  // Generate random row (0 to size-1)
        *col = rand() % size;  // Generate random column (0 to size-1)
    } while (board[*row][*col] != ' ');  // Repeat if cell is occupied
    
    // When loop exits, *row and *col point to an empty cell
}

void freeBoard(char **board, int size) {
    // Step 1: Free each individual row
    for (int i = 0; i < size; i++) {
        free(board[i]);  // Free memory for this row
    }
    // Step 2: Free the array of row pointers
    free(board);  // Free the main board structure
}


void saveGameResult(FILE *fp, const char *winnerName, int boardSize, int mode, char playerNames[][MAX_NAME]) {
    // Write game configuration details
    fprintf(fp, "Game Mode: %d\n", mode);           // 1=PvP, 2=PvC, 3=3Players
    fprintf(fp, "Board Size: %d x %d\n", boardSize, boardSize);
    
    // Write player information
    fprintf(fp, "Players: ");
    int playerCount = (mode == 3) ? 3 : 2;  // Determine number of players based on mode
    for (int i = 0; i < playerCount; i++) {
        // Format: "Name (Symbol), Name (Symbol)" 
        // "XOZ"[i] gets the symbol for player i: X for 0, O for 1, Z for 2
        fprintf(fp, "%s (%c)%s", playerNames[i], "XOZ"[i], (i < playerCount - 1) ? ", " : "\n");
    }

    // Write game outcome
    if (winnerName != NULL)
        fprintf(fp, "Winner: %s\n", winnerName);  // There was a winner
    else
        fprintf(fp, "Result: Draw\n");  // Game ended in draw

    // Add separator for readability in the file
    fprintf(fp, "-----------------------------\n");
}

// Function: main
// Purpose: Program entry point - controls entire game flow
// Returns: 0 on successful execution, 1 on error
int main() {
    int size, mode;  // Board size and game mode
    srand(time(NULL));  // Seed random number generator with current time

    // Clear screen and display game header
    CLEAR_SCREEN();
    printf("=================================\n");
    printf("      TIC-TAC-TOE GAME\n");
    printf("      By: IT25100502\n");
    printf("=================================\n");

    // Step 1: Get game mode from user
    printf("\nGame Modes:\n");
    printf("1. Two Players\n");
    printf("2. Play vs Computer\n");
    printf("3. Three Players\n");
    printf("Enter choice (1-3): ");
    scanf("%d", &mode);

    // Step 2: Get board size from user
    printf("Enter board size (3 to 10): ");
    scanf("%d", &size);
    // Validate board size input
    if (size < 3 || size > 10) {
        printf("Wrong size.\n");
        return 1;  // Exit with error code
    }

    // Step 3: Open file for saving game results
    FILE *fp = fopen("singlegrid.txt", "a");  // "a" mode appends to existing file
    if (fp == NULL) {
        printf("Cannot open file.\n");
        return 1;  // Exit if file cannot be opened
    }

    // Step 4: Set up player information
    char playerNames[3][MAX_NAME];  // Array to store player names
    char symbols[3] = {'X', 'O', 'Z'};  // Player symbols

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
        strcpy(playerNames[1], "Computer");  // Set computer's name
    } else if (mode == 3) {
        // Three Player mode
        printf("Enter Player 1 name (X): ");
        scanf("%s", playerNames[0]);
        printf("Enter Player 2 name (O): ");
        scanf("%s", playerNames[1]);
        printf("Enter Player 3 name (Z): ");
        scanf("%s", playerNames[2]);
    }

    // Step 5: Initialize the game board
    char **board = initializeBoard(size);
    int row, col, turn = 0;  // turn counter tracks current player

    // Display the initial empty board
    displayBoard(board, size);

    // Step 6: MAIN GAME LOOP - continues until win or draw
    while (1) {
        // Determine current player and their symbol
        int currentPlayer = turn % ((mode == 3) ? 3 : 2);  // Cycle through players
        char currentSymbol = symbols[currentPlayer];  // Get symbol (X/O/Z)

        // Get move from current player
        if (mode == 2 && currentPlayer == 1) {
            // Computer's turn (only in mode 2 when player 1 is computer)
            printf("%s's turn (%c)...\n", playerNames[currentPlayer], currentSymbol);
            computerMove(board, size, &row, &col);  // Generate computer move
            printf("Computer chose: %d %d\n", row + 1, col + 1);  // Show computer's choice
        } else {
            // Human player's turn
            printf("%s's turn (%c). Enter row and column (1 to %d): ", 
                   playerNames[currentPlayer], currentSymbol, size);
            scanf("%d %d", &row, &col);
            row -= 1;  // Convert from 1-based to 0-based indexing
            col -= 1;
        }

        // Validate the move
        if (!isValidMove(board, row, col, size)) {
            printf("Bad move! Try again.\n");
            continue;  // Skip to next iteration without updating turn
        }

        // Execute the valid move
        board[row][col] = currentSymbol;
        
        // Log the move to file
        fprintf(fp, "Move %d: %s (%c) -> Row %d, Col %d\n", 
                turn + 1, playerNames[currentPlayer], currentSymbol, row + 1, col + 1);

        // Update the display with the new board state
        displayBoard(board, size);

        // Check for win condition
        if (checkWin(board, size, currentSymbol)) {
            printf("%s wins!\n", playerNames[currentPlayer]);
            saveGameResult(fp, playerNames[currentPlayer], size, mode, playerNames);
            break;  // Exit game loop
        } 
        // Check for draw condition
        else if (checkDraw(board, size)) {
            printf("Game draw!\n");
            saveGameResult(fp, NULL, size, mode, playerNames);
            break;  // Exit game loop
        }

        turn++;  // Move to next player
    }

    // Step 7: Cleanup before program exit
    fclose(fp);           // Close the results file
    freeBoard(board, size);  // Free allocated memory for board
    
    // Wait for user input before exiting (so they can see final result)
    printf("\nPress Enter to exit...");
    getchar();  // Clear any leftover newline from previous input
    getchar();  // Wait for user to press Enter
    
    return 0;  // Program ended successfully
}
