#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ROWS 4
#define COLS 5

enum {
    EMPTY = 0,
    RED = 1,
    BLUE = 2,
};

typedef char board_t[ROWS][COLS];
typedef int player_t;

void init_board(board_t board)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            board[row][col] = EMPTY;
        }
    }
}

player_t other_player(player_t player)
{
    return (player == RED) ? BLUE : RED;
}

void printBoard(board_t board) {
    printf("\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            switch (board[i][j]) {
                case EMPTY: printf("|  "); break;
                case RED: printf("| R "); break;
                case BLUE: printf("| B "); break;
            }
        }
        printf("|\n");
    }
    printf("\n");
}



int has_won(board_t board, player_t player) {
    // Check horizontally
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS-3; j++) {
            if (board[i][j] == player && board[i][j + 1] == player &&
                board[i][j + 2] == player && board[i][j + 3] == player) {
                return 1;
            }
        }
    }

    // Check vertically
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS-3; j++) {
            if (board[j][i] == player && board[j + 1][i] == player &&
                board[j + 2][i] == player && board[j + 3][i] == player) {
                return 1;
            }
        }
    }

    // Check diagonally (top-left to bottom-right)
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == player && board[i + 1][j + 1] == player &&
                board[i + 2][j + 2] == player && board[i + 3][j + 3] == player) {
                return 1;
            }
        }
    }

    // Check diagonally (bottom-left to top-right)
    for (int i = 3; i < ROWS; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == player && board[i - 1][j + 1] == player &&
                board[i - 2][j + 2] == player && board[i - 3][j + 3] == player) {
                return 1;
            }
        }
    }

    return 0;
}

// Function to check if the board is full (a draw)
int isFull(board_t board) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == EMPTY) {
                return 0; // There's an empty cell, not full
            }
        }
    }
    return 1; // All cells are filled, it's a draw
}

int columnFull(board_t board, int col)
{
    return board[0][col] != EMPTY;
}

int token_gravity(board_t board, int col, player_t player)
{
    for (int row = 3; row >= 0; row--) {
        if (board[row][col] == EMPTY) {
            board[row][col] = player;
            return row;
        }
    }
    return -1;
}

typedef struct {
    int col;
    /* -1 for a loss, 0 for a draw, 1 for a win. */
    int score;
} move_t;

move_t best_move(board_t board, player_t player);

void print_key()
{
    for (int col = 0; col < COLS; ++col) {
        printf("%3d ", col);
    }
    printf("\n");
}

move_t best_move(board_t board, player_t player)
{
    move_t response;
    move_t candidate;
    int no_candidate = 1;

    for (int col = 0; col < 5; ++col) {
        if (!columnFull(board, col)) {
            int row = token_gravity(board, col, player);
            board[row][col] = player;
            if (has_won(board, player)) {
                board[row][col] = EMPTY;
                return (move_t){col, 1};
            }
            board[row][col] = EMPTY;
        }
    }

    player_t opponent = other_player(player);
    for (int col = 0; col < 5; ++col) {
        if (!columnFull(board, col)) {
            int row = token_gravity(board, col, opponent);
            if (has_won(board, opponent)) {
                board[row][col] = EMPTY;
                return (move_t){col, 1};
            }
            board[row][col] = EMPTY;
        }
    }

    for (int col = 0; col < 5; ++col) {
        if (!columnFull(board, col)) {
            int row = token_gravity(board, col, player);
            board[row][col] = player;
            if (isFull(board)) {
                board[row][col] = EMPTY;
                return (move_t){col, 0};
            }
            response = best_move(board, other_player(player));
            board[row][col] = EMPTY;
            if (response.score == -1) {
                return (move_t){col, 1};
            } else if (response.score == 0) {
                candidate = (move_t){col, 0};
                no_candidate = 0;
            } else {
                if (no_candidate) {
                    candidate = (move_t){col, -1};
                    no_candidate = 0;
                }
            }
        }
    }

    return candidate;
}

int main()
{
    /* Your game play logic. */
    /* The user should have the option to select Red or Blue player. */
    int move, col;
    board_t board;
    move_t response;

    int x;
    printf("Do you choose to be Player 1 or Player 2: ");
    scanf("%d", &x);
    player_t human, computer;

    if (x == 1) {
        human = RED;
        computer = BLUE;
    } else if (x == 2) {
        human = BLUE;
        computer = RED;
    } else {
        printf("\nInvalid choice");
        return 1; // Exit the program due to invalid choice
    }

    init_board(board);

    while (1) {
        printBoard(board);

        if (human == RED) {
            print_key();
            printf("\nEnter your move (0-4): ");
            scanf("%d", &move);
            col = move;

            if (col >= 0 && col < 5 && !columnFull(board, col)) {
                token_gravity(board, col, human);
            } else {
                printf("Invalid move. Try again.\n");
                continue;
            }
        }
        else {

            response = best_move(board, computer);
            col = response.col;

            if (col >= 0 && col < 5 && !columnFull(board, col)) {
                int row = token_gravity(board, col, human);
            }
        }



        if (has_won(board, human)) {
            printBoard(board);
            printf("Player 2 has won!\n");
            break;
        } else if (has_won(board, computer)) {
            printBoard(board);
            printf("Player 1 has won!\n");
            break;
        } else if (isFull(board)) {
            printf("It's a draw!\n");
            break;
        }

        human = other_player(human);
    }
    return 0;
}
