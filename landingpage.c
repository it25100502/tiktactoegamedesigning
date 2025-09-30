#include <stdio.h>
#include <stdlib.h>

void showOpeningPage() {
    printf("====================================\n");
    printf("        TIK TAC TOK GAME             \n");
    printf("    Designed by IT25100502           \n");
    printf("====================================\n\n");
}

int selectGameMode() {
    int choice;
    printf("Please Select Game Mode:\n");
    printf("1. Player vs Player\n");
    printf("2. Player vs Computer\n");
    printf("3. Multiplayer\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

int selectBoardSize() {
    int size;
    printf("\nSelect Board Size (3 to 10): ");
    scanf("%d", &size);

    if (size < 3 || size > 10) {
        printf("Invalid choice! Defaulting to 3x3.\n");
        size = 3;
    }
    return size;
}

int main() {
    showOpeningPage();

    int mode = selectGameMode();
    int boardSize = selectBoardSize();

    printf("\nYou selected: ");
    switch(mode) {
        case 1: printf("Player vs Player"); break;
        case 2: printf("Player vs Computer"); break;
        case 3: printf("Multiplayer"); break;
        default: printf("Invalid mode, defaulting to Player vs Player"); mode = 1; break;
    }

    printf(" on a %dx%d board.\n", boardSize, boardSize);

    // TODO: Add game logic for selected mode and board size

    return 0;
}
