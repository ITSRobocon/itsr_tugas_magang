#include <iostream>
#include <string>
using namespace std;

class Board {
public:
    Board() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                grid[i][j] = ' ';
            }
        }
    }

    void display() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cout << " " << grid[i][j] << " ";
                if (j < 2) cout << "|";
            }
            cout << "\n";
            if (i < 2) cout << "---+---+---\n";
        }
    }

    bool isEmpty(int row, int col) {
        return grid[row-1][col-1] == ' '; 
    }

    void mark(int row, int col, char symbol) {
        grid[row-1][col-1] = symbol;
        cout << "the grid: " << row << " " << col << " is marked w " << symbol << endl;
    }

    bool checkWin(char s) {
        for (int i = 0; i < 3; i++) {
            if (grid[i][0] == s && grid[i][1] == s && grid[i][2] == s) return true;  // row i
            if (grid[0][i] == s && grid[1][i] == s && grid[2][i] == s) return true;  // column i
        }
        if (grid[0][0] == s && grid[1][1] == s && grid[2][2] == s) return true;      // diagonal "\"
        if (grid[0][2] == s && grid[1][1] == s && grid[2][0] == s) return true;      // diagonal "/"
        return false;
    }

    bool isFull() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (grid[i][j] == ' ') return false;
            }
        }
        return true;
    }

private:
    char grid[3][3];
};

class Player {
public:
    Player(string n, char s) {
        name = n;
        symbol = s;
    }

    string getName() {
        return name;
    }

    char getSymbol() {
        return symbol;
    }

private:
    string name;
    char symbol;
};

class Game {
public:
    Game() : p1("Player 1", 'X'), p2("Player 2", 'O') {
        current = &p1;
    }

    void run() {
        while (true) {
            board.display();

            string r, c;
            cout << current->getName() << " (" << current->getSymbol() << "), enter row col: ";
            cin >> r >> c;

            if (r.size() != 1 || c.size() != 1 ||
                r[0] < '1' || r[0] > '3' || c[0] < '1' || c[0] > '3') {
                cout << "Invalid input, use 1-3.\n";
                continue;
            }

            int row = r[0] - '0';
            int col = c[0] - '0';

            if (!board.isEmpty(row, col)) {
                cout << "That cell is taken, try again.\n";
                continue;
            }

            board.mark(row, col, current->getSymbol());

            if (board.checkWin(current->getSymbol())) {
                board.display();
                cout << current->getName() << " wins!\n";
                return;
            }
            if (board.isFull()) {
                board.display();
                cout << "It's a draw!\n";
                return;
            }

            current = (current == &p1) ? &p2 : &p1;
        }
    }

private:
    Board board;
    Player p1;
    Player p2;
    Player* current;
};

int main() {
    cout << "type 'start' to play" << endl;
    string cmd;
    while (cin >> cmd) {
        if (cmd == "start") {
            Game game;
            game.run();
            break;
        } else {
            cout << "Type 'start' to play" << endl;
        }
    }
    return 0;
}