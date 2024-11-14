#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define COLS 60
#define ROWS 30

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void moveCursor(int x, int y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void clearTail(int x, int y) {
    moveCursor(x, y);
    printf("·");
}

void drawApple(int x, int y) {
    moveCursor(x, y);
    printf("$");
}

void drawHead(int x, int y) {
    moveCursor(x, y);
    printf("▓");
}

void gameOverMessage() {
    moveCursor(COLS / 2 - 5, ROWS / 2);
    printf("Game Over!");
}

int main() {
    hideCursor();

    int x[1000], y[1000];
    int quit = 0;
    int head = 0, tail = 0;
    x[head] = COLS / 2;
    y[head] = ROWS / 2;
    int gameover = 0;
    int xdir = 1, ydir = 0;
    int applex = -1, appley = -1;

    // Render table borders
    moveCursor(0, 0);
    printf("┌");
    for (int i = 0; i < COLS; i++)
        printf("─");
    printf("┐\n");

    for (int j = 0; j < ROWS; j++) {
        printf("│");
        for (int i = 0; i < COLS; i++)
            printf("·");
        printf("│\n");
    }

    printf("└");
    for (int i = 0; i < COLS; i++)
        printf("─");
    printf("┘\n");

    // Move cursor back to the top of the playing field
    moveCursor(0, 1);

    while (!quit) {
        // Generate new apple if necessary
        if (applex < 0) {
            applex = rand() % COLS;
            appley = rand() % ROWS;

            // Ensure apple doesn't appear on the snake's body
            int isOnSnake = 0;
            for (int i = tail; i != head; i = (i + 1) % 1000) {
                if (x[i] == applex && y[i] == appley) {
                    isOnSnake = 1;
                    break;
                }
            }
            if (!isOnSnake) {
                drawApple(applex + 1, appley + 1);  // Corrected position for display
            } else {
                applex = -1; // Re-roll apple position if it overlaps with snake
            }
        }

        // Move the snake head to the new position
        int newhead = (head + 1) % 1000;
        x[newhead] = (x[head] + xdir + COLS) % COLS;
        y[newhead] = (y[head] + ydir + ROWS) % ROWS;
        head = newhead;

        // Check if the snake eats the apple
        if (x[head] == applex && y[head] == appley) {
            applex = -1; // Remove the apple to create a new one next time
            printf("\a"); // Sound to indicate apple consumption
            // Snake grows by not moving the tail
        } else {
            // Clear the tail if no apple was eaten
            clearTail(x[tail] + 1, y[tail] + 1);  // Corrected position for display
            tail = (tail + 1) % 1000;
        }

        // Draw the snake head
        drawHead(x[head] + 1, y[head] + 1);  // Corrected position for display

        // Check for collisions with itself
        for (int i = tail; i != head; i = (i + 1) % 1000) {
            if (x[i] == x[head] && y[i] == y[head]) {
                gameover = 1;
                break;
            }
        }

        if (gameover) {
            gameOverMessage();
            break;
        }

        // Delay for movement speed
        Sleep(100);

        // Handle user input for direction change
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 27 || ch == 'q') {  // ESC or 'q' to quit
                quit = 1;
            } else if (ch == 'a' && xdir != 1) {
                xdir = -1;
                ydir = 0;
            } else if (ch == 'd' && xdir != -1) {
                xdir = 1;
                ydir = 0;
            } else if (ch == 's' && ydir != -1) {
                xdir = 0;
                ydir = 1;
            } else if (ch == 'w' && ydir != 1) {
                xdir = 0;
                ydir = -1;
            }
        }
    }

    // Show cursor after the game ends
    showCursor();

    return 0;
}
