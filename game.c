#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ANSI COLOR CODES */
#define RESET   "\033[0m"
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
    if (size < 3 || size > 4) size = 3;

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
            clearScreen();
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
                clearScreen();
                printBoard();
                printf(NEON_G "\n🎉 %s wins!\n" RESET,
                       (turn == 'X') ? playerX : playerO);
                (turn == 'X') ? scoreX++ : scoreO++;
                break;
            }

            if (isDraw()) {
                clearScreen();
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
    printf(NEON_C);
    printf("=================================\n");
    printf("       NEON TIC TAC TOE           \n");
    printf("=================================\n\n");
    printf(RESET);
}

/* Initialize board */
void initBoard() {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = ' ';
}

/* Print board */
void printBoard() {
    neonTitle();

    printf("   ");
    for (int j = 0; j < size; j++)
        printf(" %d  ", j + 1);
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf(" %d ", i + 1);
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 'X')
                printf(NEON_G " X " RESET);
            else if (board[i][j] == 'O')
                printf(NEON_M " O " RESET);
            else
                printf(" _ ");

            if (j < size - 1) printf("|");
        }
        printf("\n");
        if (i < size - 1) {
            printf("   ");
            for (int j = 0; j < size; j++) {
                printf("---");
                if (j < size - 1) printf("+");
            }
            printf("\n");
        }
    }
}

/* Player move */
void playerMove(char sym) {
    int r, c;
    time_t start, end;

    time(&start);
    printf("\nPlayer %c enter row & column: ", sym);

    while (1) {
        scanf("%d %d", &r, &c);
        time(&end);

        if (difftime(end, start) > timeLimit) {
            printf("⏰ Time over! Try again: ");
            time(&start);
            continue;
        }

        r--; c--;
        if (r >= 0 && c >= 0 && r < size && c < size && board[r][c] == ' ') {
            board[r][c] = sym;
            break;
        }
        printf("Invalid move! Try again: ");
    }
}

/* Computer move */
void computerMove(char sym) {
    int r = -1, c = -1;

    if (level == 1) {
        do {
            r = rand() % size;
            c = rand() % size;
        } while (board[r][c] != ' ');
    }
    else if (level == 2) {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    if (checkWin('X')) {
                        board[i][j] = sym;
                        return;
                    }
                    board[i][j] = ' ';
                }
        do {
            r = rand() % size;
            c = rand() % size;
        } while (board[r][c] != ' ');
    }
    else {
        int best = -1000;
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (board[i][j] == ' ') {
                    board[i][j] = sym;
                    int score = minimax('X');
                    board[i][j] = ' ';
                    if (score > best) {
                        best = score;
                        r = i; c = j;
                    }
                }
    }

    if (r != -1 && c != -1)
        board[r][c] = sym;
}

/* Minimax */
int minimax(char sym) {
    if (checkWin('O')) return 10;
    if (checkWin('X')) return -10;
    if (isDraw()) return 0;

    int best = (sym == 'O') ? -1000 : 1000;

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == ' ') {
                board[i][j] = sym;
                int score = minimax((sym == 'O') ? 'X' : 'O');
                board[i][j] = ' ';
                if (sym == 'O' && score > best) best = score;
                if (sym == 'X' && score < best) best = score;
            }

    return best;
}

/* Check win */
int checkWin(char sym) {
    for (int i = 0; i < size; i++) {
        int row = 1, col = 1;
        for (int j = 0; j < size; j++) {
            if (board[i][j] != sym) row = 0;
            if (board[j][i] != sym) col = 0;
        }
        if (row || col) return 1;
    }

    int d1 = 1, d2 = 1;
    for (int i = 0; i < size; i++) {
        if (board[i][i] != sym) d1 = 0;
        if (board[i][size - i - 1] != sym) d2 = 0;
    }
    return d1 || d2;
}

/* Draw */
int isDraw() {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
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
