#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ANSI COLOR CODES */
#define RESET   "\033[0m"
#define BLACKBG "\033[40m"
#define NEON_G  "\033[92m"
#define NEON_C  "\033[96m"
#define NEON_M  "\033[95m"
#define NEON_Y  "\033[93m"

char board[4][4];
int size, level, timeLimit;
char playerX[30], playerO[30];
int scoreX = 0, scoreO = 0, draws = 0;

/* Function declarations */
void clearScreen();
void neonTitle();
void initBoard();
void printBoard();
int checkWin(char);
int isDraw();
void playerMove(char);
void computerMove(char);
int minimax(char);
int playAgain();

/* MAIN FUNCTION */
int main() {
    int mode;
    char turn;

    srand(time(NULL));

    printf("Enter board size (3 or 4): ");
    scanf("%d", &size);

    printf("\n1. Player vs Player");
    printf("\n2. Player vs Computer");
    printf("\nChoose mode: ");
    scanf("%d", &mode);

    printf("Enter Player X name: ");
    scanf("%s", playerX);

    if (mode == 1) {
        printf("Enter Player O name: ");
        scanf("%s", playerO);
    } else {
        strcpy(playerO, "Computer");
        printf("\nSelect Difficulty Level:");
        printf("\n1. Easy\n2. Medium\n3. Hard\nEnter choice: ");
        scanf("%d", &level);
    }

    printf("Enter time limit per move (seconds): ");
    scanf("%d", &timeLimit);

    do {
        initBoard();
        turn = 'X';

        while (1) {
            printBoard();

            if (turn == 'X')
                playerMove('X');
            else {
                if (mode == 1)
                    playerMove('O');
                else
                    computerMove('O');
            }

            if (checkWin(turn)) {
                printBoard();
                printf(NEON_G "\n🎉 %s wins!\n" RESET,
                       (turn == 'X') ? playerX : playerO);
                if (turn == 'X') scoreX++;
                else scoreO++;
                break;
            }

            if (isDraw()) {
                printBoard();
                printf(NEON_Y "\n😐 Match Draw!\n" RESET);
                draws++;
                break;
            }

            turn = (turn == 'X') ? 'O' : 'X';
        }

        printf("\n--- SCOREBOARD ---\n");
        printf("%s (X): %d\n", playerX, scoreX);
        printf("%s (O): %d\n", playerO, scoreO);
        printf("Draws   : %d\n", draws);

    } while (playAgain());

    return 0;
}

/* Clear screen */
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* Neon title */
void neonTitle() {
    printf(BLACKBG NEON_C);
    printf("=================================\n");
    printf("       ✨ NEON TIC TAC TOE ✨      \n");
    printf("=================================\n\n");
    printf(RESET);
}

/* Initialize board */
void initBoard() {
    int i, j;
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            board[i][j] = ' ';
}

/* Print neon board */
void printBoard() {
    int i, j;
    clearScreen();
    neonTitle();

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == 'X')
                printf(BLACKBG NEON_G "  X  " RESET);
            else if (board[i][j] == 'O')
                printf(BLACKBG NEON_M "  O  " RESET);
            else
                printf(BLACKBG NEON_C "     " RESET);

            if (j < size - 1)
                printf(BLACKBG NEON_Y "|" RESET);
        }
        printf("\n");
        if (i < size - 1) {
            for (j = 0; j < size; j++)
                printf(BLACKBG NEON_Y "-----" RESET);
            printf("\n");
        }
    }
}

/* Player move with time bomb */
void playerMove(char sym) {
    int r, c;
    time_t start, end;

    time(&start);
    printf("\nPlayer %c enter row & column: ", sym);
    scanf("%d %d", &r, &c);
    time(&end);

    if (difftime(end, start) > timeLimit) {
        printf("💣 Time over! Turn skipped.\n");
        return;
    }

    r--; c--;
    if (r >= 0 && c >= 0 && r < size && c < size && board[r][c] == ' ')
        board[r][c] = sym;
    else {
        printf("Invalid move! Try again.\n");
        playerMove(sym);
    }
}

/* Computer move (level based) */
void computerMove(char sym) {
    int i, j, r = -1, c = -1;

    if (level == 1) {
        do {
            r = rand() % size;
            c = rand() % size;
        } while (board[r][c] != ' ');
    } else if (level == 2) {
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    if (checkWin('X')) {
                        board[i][j] = sym;
                        return;
                    }
                    board[i][j] = ' ';
                }
            }
        }
        do {
            r = rand() % size;
            c = rand() % size;
        } while (board[r][c] != ' ');
    } else {
        int best = -1000, score;
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = sym;
                    score = minimax('X');
                    board[i][j] = ' ';
                    if (score > best) {
                        best = score;
                        r = i;
                        c = j;
                    }
                }
            }
        }
    }

    board[r][c] = sym;
}

/* Minimax */
int minimax(char sym) {
    int i, j;
    if (checkWin('O')) return 10;
    if (checkWin('X')) return -10;
    if (isDraw()) return 0;

    int best = (sym == 'O') ? -1000 : 1000;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = sym;
                int score = minimax((sym == 'O') ? 'X' : 'O');
                board[i][j] = ' ';
                if (sym == 'O') {
                    if (score > best) best = score;
                } else {
                    if (score < best) best = score;
                }
            }
        }
    }
    return best;
}

/* Check win */
int checkWin(char sym) {
    int i, j, row, col, d1 = 1, d2 = 1;
    for (i = 0; i < size; i++) {
        row = col = 1;
        for (j = 0; j < size; j++) {
            if (board[i][j] != sym) row = 0;
            if (board[j][i] != sym) col = 0;
        }
        if (row || col) return 1;
    }
    for (i = 0; i < size; i++) {
        if (board[i][i] != sym) d1 = 0;
        if (board[i][size - i - 1] != sym) d2 = 0;
    }
    return d1 || d2;
}

/* Draw */
int isDraw() {
    int i, j;
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1;
}

/* Replay */
int playAgain() {
    char ch;
    printf("\nPlay again? (y/n): ");
    scanf(" %c", &ch);
    return (ch == 'y' || ch == 'Y');
}