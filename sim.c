#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "test.h"

#define BOARD_SIZE(15)
/*
 * The status of a line.
 */
enum {
    NO = 0, /* No line */
    RED = 1,
    BLUE = 2
};

/*
 * The board records the colors of the lines.
 * board[0] = color of 12
 * board[1] = color of 13
 * ...
 * board[14] = color of 56
 */
typedef char board_t[15];
typedef char player_t; /* A player should be RED or BLUE. */

player_t other_player(player_t player){
    switch(player){
        case 'R' : return 'B';
        case 'B' : return 'R';
    }
}

int has_won(board_t board, player_t player)
{
    player = other_player(player);
    if((board[0] == player && board[5] == player && board[1] == player) || 
    (board[0] == player && board[6] == player && board[2] == player) ||
    (board[0] == player && board[7] == player && board[3] == player) ||
    (board[0] == player && board[8] == player && board[4] == player) ||
    (board[1] == player && board[9] == player && board[2] == player) ||
    (board[1] == player && board[10] == player && board[3] == player) ||
    (board[1] == player && board[11] == player && board[4] == player) ||
    (board[2] == player && board[12] == player && board[3] == player) ||
    (board[2] == player && board[13] == player && board[4] == player) ||
    (board[3] == player && board[14] == player && board[4] == player) ||
    (board[5] == player && board[9] == player && board[6] == player) ||
    (board[5] == player && board[10] == player && board[7] == player) ||
    (board[5] == player && board[11] == player && board[8] == player) ||
    (board[6] == player && board[12] == player && board[7] == player) ||
    (board[6] == player && board[13] == player && board[8] == player) ||
    (board[7] == player && board[14] == player && board[8] == player) ||
    (board[9] == player && board[12] == player && board[10] == player) ||
    (board[9] == player && board[13] == player && board[11] == player) ||
    (board[10] == player && board[14] == player && board[11] == player) ||
    (board[12] == player && board[14] == player && board[13] == player)
    ){
        return 1;
    }
    return 0;
}

void init_board(board_t board){
    for(int k = 0;k<BOARD_SIZE;k++){
        board[k] = '.';
    }
}

int is_full(board_t board)
{
    for(int u = 0;u<BOARD_SIZE;u++){
        if(board[u] == '.'){
            return 0;
        }
    }
    return 1;
}

typedef struct {
    int line; /* 0 for 12, 1 for 13, ..., 14 for 56. */
    int score; /* -1 for loss, 0 for draw, 1 for win. */
} move_t;

#define MAX_ORD (14348907)

uint8_t computed_moves[MAX_ORD + 1];

move_t decode_move(uint8_t b){
    move_t ans;

    ans.line = b & 0x457;
    if(b & 0x10) ans.score = 1;
    if(b & 0x20) ans.score = 0;
    if(b & 0x40) ans.score = -1;
    return ans;
}

uint8_t encode_move(move_t m){
    uint8_t out = 0;
    
    assert(0<= m.line && m.line <= BOARD_SIZE);
    out |= m.line;

    switch(m.score)
    {
        case -1:out |= 1 << 6;break;
        case 0:out |= 1 << 5;break;
        case 1:out |= 1 << 4;break;
    }
    return out;
}

int ord(board_t board){
    int p = 1;
    int i = 0;
    int d;
    for(int index = 0;index < BOARD_SIZE;index++){
        switch(board[index]){
            case 'R':d = RED;break;
            case 'B':d = BLUE;break;
            case '.':d = NO;break;
        }
        i += d*p;
        p *= 3;
    }
    return i;
}

move_t best_move(board_t board, player_t player)
{
    move_t response;
    move_t candidate;
    int no_candidate = 1;

    assert(!is_full(board));
    assert(!has_won(board,player));
    assert(!has_won(board,other_player(player)));

    int o = ord(board);

    if(computed_moves[o]){
        return decode_move(computed_moves[o]);
    }

    for(int ind = 0; ind < BOARD_SIZE;ind++){
        if(board[ind] == '.'){
            board[ind] = player;
            if(has_won(board,player)){
                board[ind] = '.';
                computed_moves[o] = encode_move(candidate = (move_t){
                    .line = ind,
                    .score = 1
                });
                return candidate;
            }
            board[ind] = '.';
        }
    }

    for(int ind = 0;ind<BOARD_SIZE;ind++){
        if(board[ind] == '.'){
            board[ind] = player;
            if(is_full(board)){
                board[ind] = '.';
                computed_moves[o] = encode_move(candidate = (move_t){
                    .line = ind,
                    .score = 0
                    });
                return candidate;
            }
            response = best_move(board,other_player(player));
            board[ind] = '.';
            if(response.score == -1){
                computed_moves[o] = encode_move(candidate = (move_t){
                    .line = ind,
                    .score = 1
                    });
                return candidate;
            }
            else if(response.score == 0){
                candidate = (move_t){
                    .line = ind,
                    .score = 0
                };
                no_candidate = 0;
            }
            else{
                if(no_candidate){
                    candidate = (move_t){
                        .line = ind,
                        .score = -1
                    };
                    no_candidate = 0;
                }
            }
        }
    }
    computed_moves[o] = encode_move(candidate);
    return candidate;
}

void print_board(board_t board)
{
    for (int i = 0; i < 15; i++) {
        switch (board[i]) {
        case 'R': printf("R  "); break;
        case 'B': printf("B  "); break;
        case '.': printf(". ");break;
        }
    }
    printf("\n");
}

int main()
{
    int move;
    board_t board;
    move_t response;
    player_t current;
    char player_choice;
    printf("Choose your color (R for Red, B for Blue): ");
    scanf(" %c", &player_choice);
    init_board(board);
    if(player_choice == 'R') {
        current = 'R';
    }else {
        current = 'B';
        srand(time(NULL));

        move = rand() % BOARD_SIZE;
        board[move] = current;
    }
    init_board(board);
    while(1){
        if(current == 'R'){
            print_board(board);
            printf("Enter your move: ");
            scanf("%d",&move);
            assert(board[move] == '.');
            board[move] = current;
        }
        else{
            response = best_move(board,current);
            board[response.line] = current;
        }
        if(has_won(board,current)){
            print_board(board);
            printf("Player %c has won!\n",current);
            break;
        }
        current = other_player(current);
    }
    /* Your game play logic. */
    /* The user should have the option to select red or blue player. */
    return 0;
}
