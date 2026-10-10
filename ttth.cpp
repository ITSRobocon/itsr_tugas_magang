    #include <iostream>
#include <string>
#include <vector>

// I honestly never use these two
// GODSPEED STACK OVERFLOW
#include <cstdlib>
#include <ctime>

#include "coinflip.h"

using namespace std;

// Kita perlu class Game sbg parent class
// Class player sbg child class
// Class player ada yang AI/Bot dan User/Client
class Game {
public:
    bool gameRun = true;
    int boardSize;
    vector<vector<string>> board;

    Game(int boardSizeInput) {
        boardSize = boardSizeInput;
        board.assign(boardSize, vector<string>(boardSize, "="));
    }

    void setGameRun(bool inputGameRun) {
        gameRun = inputGameRun;
    }

    void displayBoard() {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }

    bool applyMove(int row, int col, string symbol) {
        if (row < 0 || row >= boardSize || col < 0 || col >= boardSize) {
            cout << "Move cannot be made." << endl;
            return false;
        }

        if (board[row][col] != "=") {
            cout << "Space is occupied by " << board[row][col] << endl;
            return false;
        }

        board[row][col] = symbol;

        return true;
    }

    // Heuristic "AI" (its not intelligence at all)
    string checkWin() {
        // GOD, ITS CALLED A CHECK NEIGHBOR!

        // PS: It does not suffice, thank you for the 1 hour.
        // P.PS: This algorithm sucks. But it works. It sucks though.

        // HORIZONTAL CHECKING
        for (int i = 0; i < boardSize; i++) {
            if (board[i][0] == "=") {
                continue;
            }
            string firstSymbol = board[i][0];
            bool isWin = true;
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] != firstSymbol) {
                    isWin = false;
                    break;
                }
            }

            if (isWin)
                return firstSymbol;
        }

        // VERTICAL CHECKING
        for (int i = 0; i < boardSize; i++) {
            if (board[0][i] == "=") {
                continue;
            }

            string firstSymbol = board[0][i];
            bool isWin = true;

            for (int j = 0; j < boardSize; j++) {
                if (board[j][i] != firstSymbol) {
                    isWin = false;
                    break;
                }
            }

            if (isWin)
                return firstSymbol;
        }

        // DIAGONAL CHECKING
        // PS. I THOUGHT THIS IS ANOTHER NEIGHBOR CHECK BUT NO NO NO.
        // IT WAS NOT.
        if (board[0][0] != "=") {
            string firstSymbol = board[0][0];
            bool isWin = true;

            for (int k = 0; k < boardSize; k++) {
                if (board[k][k] != firstSymbol) {
                    isWin = false;
                    break;
                }
            }

            if (isWin)
                return firstSymbol;
        }

        // ANTI-DIAGONAL CHECKING
        if (board[0][boardSize - 1] != "=") {
            string firstSymbol = board[0][boardSize - 1];
            bool isWin = true;

            for (int k = 0; k < boardSize; k++) {
                if (board[k][(boardSize - 1) - k] != firstSymbol) {
                    isWin = false;
                    break;
                }
            }

            if (isWin)
                return firstSymbol;
        }

        return "=";
    }

    void checkTie() {
        bool isTie = true;
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] == "=") {
                    isTie = false;
                    break;
                }
            }
        }
        if (isTie) {
            cout << "The game is a tie! \n";
            gameRun = false;
            return;
        }
    }
};

class Player {
public:
    int score = 0;
    string role;
    bool bot;

    Player(bool playerBot) {
        bot = playerBot;
    }

    void setRole(string roleInput) {
        role = roleInput;
    }

    void botAlgo(Game& activeGame) {
        // while (true) {
        //     int randomRow = rand() % activeGame.boardSize;
        //     int randomCol = rand() % activeGame.boardSize;

        //     if (activeGame.board[randomRow][randomCol] != "=") {
        //         continue;
        //     }

        //     if (activeGame.applyMove(randomRow, randomCol, role)) {
        //         cout << "Bot placed " << role << " at [" << randomRow + 1 << ", " << randomCol + 1 << "]" << endl;
        //         break;
        //     }
        // }

        // ATTACK PHASE
        for (int i = 0; i < activeGame.boardSize; i++) {
            for (int j = 0; j < activeGame.boardSize; j++) {
                if (activeGame.board[i][j] == "=") {
                    activeGame.board[i][j] = role;
                    if (activeGame.checkWin() == role) {
                        cout << "Bot placed " << role << " at [" << i + 1 << ", " << j + 1 << "]\n";
                        return;
                    }
                    activeGame.board[i][j] = "=";
                }
            }
        }

        // DEFENSE PHASE
        for (int i = 0; i < activeGame.boardSize; i++) {
            for (int j = 0; j < activeGame.boardSize; j++) {
                if (activeGame.board[i][j] == "=") {
                    string playerRole = (role == "X") ? "O" : "X";
                    activeGame.board[i][j] = playerRole;
                    if (activeGame.checkWin() == playerRole) {
                        activeGame.board[i][j] = role;
                        cout << "Bot placed " << role << " at [" << i + 1 << ", " << j + 1 << "]\n";
                        return;
                    }
                    activeGame.board[i][j] = "=";
                }
            }
        }

        // CENTERING
        int center = activeGame.boardSize / 2;
        if (activeGame.board[center][center] == "=") {
            activeGame.applyMove(center, center, role);
            cout << "Bot placed " << role << " at [" << center + 1 << ", " << center + 1 << "]\n";
            return;
        }

        // POSITION CONTROL
        for (int i = 0; i < activeGame.boardSize; i++) {
            for (int j = 0; j < activeGame.boardSize; j++) {
                if (activeGame.board[i][j] == "=") {
                    activeGame.applyMove(i, j, role);
                    cout << "Bot placed " << role << " at [" << i + 1 << ", " << j + 1 << "]\n";
                    return;
                }
            }
        }
    }
};

int main() {
    srand(time(0));
    string userRole = "-";
    int userRow, userCol;
    int boardSize = 0;

    // ============ SnA Players ============ //
    cout << "====== [GAME INITIALIZATION] ======" << endl;

    Player bot(true);
    Player user(false);

    while (userRole != "X" && userRole != "O") {
        cout << "Input your role (X or O): ";
        cin >> userRole;

        if (userRole == "X") {
            bot.setRole("O");
        } else if (userRole == "O") {
            bot.setRole("X");
        } else {
            cout << "Invalid sign. Input X or O" << endl;
        }
    }

    user.setRole(userRole);

    cout << "User: " << user.role << endl;
    cout << "Bot: " << bot.role << endl;

    // Ai slop starts here
    cout << "====== [COIN FLIP] ======" << endl;

    string guess = "-";
    while (guess != "H" && guess != "T") {
        cout << "Pick a side (H = Heads, T = Tails): ";
        cin >> guess;

        if (guess != "H" && guess != "T") {
            cout << "Invalid side. Input H or T" << endl;
        }
    }

    bool resultHeads = (rand() % 2 == 0);
    playCoinFlipAnimation(resultHeads);

    bool guessedHeads = (guess == "H");
    bool userTurn = (guessedHeads == resultHeads);

    cout << "It landed on " << (resultHeads ? "HEADS" : "TAILS") << "! ";
    cout << (userTurn ? "User goes first." : "Bot goes first.") << endl;
    // Ai slop ends here

    // ============ SnA Game ============ //

    while (boardSize == 0 || boardSize < 3) {
        cout << "Set preferred board size (min. 3): ";
        cin >> boardSize;
    }

    Game ttt(boardSize);

    cout << "Tic Tac Toe Board: \n";
    ttt.displayBoard();

    // ============ Game Flow ============ //

    cout << "====== [GAME RULES AND INFO] ======" << endl;
    cout << "- Game board uses basic matrix mapping." << endl;
    cout << "- Move input uses format as follows: row column (ex: 2 2)." << endl;
    cout << "- Move input can only be in between 1 and " << ttt.boardSize << "." << endl;

    cout << "====== [GAME START] ======" << endl;

    while (ttt.gameRun) {
        if (userTurn) {
            // User moves
            while (true) {
                cout << "User" << ", make your move: ";
                cin >> userRow >> userCol;
                if (ttt.applyMove(userRow - 1, userCol - 1, user.role)) {
                    break;
                }
            }
            cout << "User" << " placed " << user.role << " at [" << userRow << ", " << userCol << "]\n";
            ttt.displayBoard();
        } else {
            // Bot moves
            bot.botAlgo(ttt);
            ttt.displayBoard();
        }

        string winner = ttt.checkWin();
        if (winner != "=") {
            cout << (winner == user.role ? "User" : "Bot") << " wins!" << endl;
            ttt.setGameRun(false);
        } else {
            ttt.checkTie();
        }

        userTurn = !userTurn;
    }
}

/*
    To-Do
    - Less than 3 matrix ordo is invalid for game
    - Make rock paper scissors for playing order
*/