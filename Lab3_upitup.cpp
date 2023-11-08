#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include <tuple>
#include <cassert>
#include<algorithm>

using namespace std;

struct Die {
    int top;
    int bottom;
    int front;
    int back;
    int left;
    int right;

    Die() : top(1), bottom(2), front(0), back(0), left(0), right(0) {}
};

struct Board {
    array<array<Die, 3>, 3> dice;

    Board() {
        for (auto &row : dice) {
            for (auto &die : row) {
                die = Die();
            }
        }
        dice[1][1].top = 0;
        dice[1][1].bottom = 0;
    }
};

int die_value(const Die& die) {
    if (die.top == 1) {
        return 1;
    } else if (die.bottom == 1) {
        return 2;
    } else if (die.front == 1) {
        return 3;
    } else if (die.back == 1) {
        return 4;
    } else if (die.left == 1) {
        return 5;
    } else if (die.right == 1) {
        return 6;
    } else {
        return 0;
    }
}

void displayDie(const Die& die) {
    cout << "Top: " << die.top << ", Bottom: " << die.bottom << ", Front: " << die.front
         << ", Back: " << die.back << ", Left: " << die.left << ", Right: " << die.right << endl;
}

int fact(int i) {
    int p = 1;
    for (int j = 2; j <= i; ++j) {
        p *= j;
    }
    return p;
}

int ord(const Board& board) {
    vector<int> permutation;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            permutation.push_back(die_value(board.dice[i][j]));
        }
    }
    int uniqueValue = 0;
    int factorial = 1;
    for (int i = 8; i >= 0; i--) {
        uniqueValue += permutation[i] * factorial;
        factorial *= 10;
    }

    return uniqueValue;
}

void initializeBoard1(Board& board) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            board.dice[row][col].top = 1;
            board.dice[row][col].bottom = 2;
            board.dice[row][col].front = 0;
            board.dice[row][col].back = 0;
            board.dice[row][col].left = 0;
            board.dice[row][col].right = 0;
        }
    }
    board.dice[1][1].top = 0;
    board.dice[1][1].bottom = 0;
}

void initializeBoard2(Board& board) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            board.dice[row][col].top = 2;
            board.dice[row][col].bottom = 1;
            board.dice[row][col].front = 0;
            board.dice[row][col].back = 0;
            board.dice[row][col].left = 0;
            board.dice[row][col].right = 0;
        }
    }
    board.dice[1][1].top = 0;
    board.dice[1][1].bottom = 0;
}

void printBoard(struct Board board) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            printf("Die at row %d, column %d:\n", row, col);
            printf("Top: %d, Bottom: %d, Front: %d, Back: %d, Left: %d, Right: %d\n",
                board.dice[row][col].top, board.dice[row][col].bottom, board.dice[row][col].front,
                board.dice[row][col].back, board.dice[row][col].left, board.dice[row][col].right);
            printf("\n");
        }
    }
}

void rollUp(Die& die) {
    int temp = die.front;
    die.front = die.top;
    die.top = die.back;
    die.back = die.bottom;
    die.bottom = temp;
}

void rollDown(Die& die) {
    int temp = die.front;
    die.front = die.bottom;
    die.bottom = die.back;
    die.back = die.top;
    die.top = temp;
}

void rollLeft(Die& die) {
    int temp = die.left;
    die.left = die.top;
    die.top = die.right;
    die.right = die.bottom;
    die.bottom = temp;
}

void rollRight(Die& die) {
    int temp = die.left;
    die.left = die.bottom;
    die.bottom = die.right;
    die.right = die.top;
    die.top = temp;
}

pair<int, int> find_space(const Board& b) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (die_value(b.dice[r][c]) == 0) {
                return make_pair(r, c);
            }
        }
    }
    cout << "Error in code" << endl;
    return make_pair(-1, -1);
}


Board up(const Board& b) {
    pair<int, int> space = find_space(b);
    int r = space.first;
    int c = space.second;
    Board o = b;
    rollUp(o.dice[r + 1][c]);
    swap(o.dice[r][c], o.dice[r + 1][c]);
    return o;
}

Board down(const Board& b) {
    pair<int, int> space = find_space(b);
    int r = space.first;
    int c = space.second;
    Board o = b;
    rollDown(o.dice[r - 1][c]);
    swap(o.dice[r - 1][c], o.dice[r][c]);
    return o;
}

Board left(const Board& b) {
    pair<int, int> space = find_space(b);
    int r = space.first;
    int c = space.second;
    Board o = b;
    rollLeft(o.dice[r][c + 1]);
    swap(o.dice[r][c + 1], o.dice[r][c]);
    return o;
}

Board right(const Board& b) {
    pair<int, int> space = find_space(b);
    int r = space.first;
    int c = space.second;
    Board o = b;
    rollRight(o.dice[r][c - 1]);
    swap(o.dice[r][c - 1], o.dice[r][c]);
    return o;
}


bool is_similar(const Board& a, const Board& b) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (die_value(a.dice[r][c]) != die_value(b.dice[r][c])) {
                return false;
            }
        }
    }
    return true;
}

enum move { L = 1, R = 2, U = 3, D = 4 };

vector<int> solve(const Board& src, const Board& dest) {
    int m = 24546739;
    queue<Board> q;
    vector<int> visited(24546739, 0);
    vector<Board> parent(24546739);
    q.push(src);
    visited[ord(src)] = L;

    while (!q.empty()) {
        Board u = q.front();
        q.pop();
        if (is_similar(u, dest)) {
            vector<int> moves;
            Board c = u;
            int o = ord(c);
            while (!is_similar(c, src)) {
                moves.push_back(visited[o]);
                c = parent[o];
                o = ord(c);
            }
            reverse(moves.begin(), moves.end());
            return moves;
        }

        Board a = up(u);
        Board b = down(u);
        Board c = left(u);
        Board d = right(u);

        int aord = ord(a);
        int bord = ord(b);
        int cord = ord(c);
        int dord = ord(d);

        if (!visited[aord]) {
            visited[aord] = U;
            parent[aord] = u;
            q.push(a);
        }
        if (!visited[bord]) {
            visited[bord] = D;
            parent[bord] = u;
            q.push(b);
        }
        if (!visited[cord]) {
            visited[cord] = L;
            parent[cord] = u;
            q.push(c);
        }
        if (!visited[dord]) {
            visited[dord] = R;
            parent[dord] = u;
            q.push(d);
        }
    }
    return vector<int>();
}

void print_moves(const vector<int>& moves) {
    for (auto m : moves) {
        switch (m) {
            case L: cout << "L "; break;
            case R: cout << "R "; break;
            case U: cout << "U "; break;
            case D: cout << "D "; break;
        }
    }
    cout << endl;
}

int main() {
    Board src, dest;
    initializeBoard1(src);
    initializeBoard2(dest);
    printBoard(src);
    printBoard(dest);
    // auto moves = solve(src, dest);
    // print_moves(moves);
    return 0;
}