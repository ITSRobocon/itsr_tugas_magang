
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <string>

using namespace std;

class ticTacToeGame {
private:
    int typeGame = 0;
    int pTurn = 1;

    char turn = 'X';
    char humanMark = 'X';
    char computerMark = 'O';

    char arr[9] = {
        '1', '2', '3',
        '4', '5', '6',
        '7', '8', '9'
    };

    // Membaca input angka dan memvalidasi rentangnya
    int getInteger(int minValue, int maxValue,
                   const string& prompt) {
        int value;

        while (true) {
            cout << prompt;

            if (!(cin >> value)) {
                if (cin.eof()) {
                    cout << "\nInput ended. Exiting program.\n";
                    exit(EXIT_SUCCESS);
                }

                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(
                    numeric_limits<streamsize>::max(), '\n'
                );
                continue;
            }

            if (value < minValue || value > maxValue) {
                cout << "Please enter a number between "
                     << minValue << " and "
                     << maxValue << ".\n";
                continue;
            }

            return value;
        }
    }

    // Memeriksa kotak kosong
    bool isEmpty(int index) const {
        return arr[index] >= '1' && arr[index] <= '9';
    }

    // Menampilkan papan permainan
    void display() const {
        cout << "\n========== TIC TAC TOE ==========\n";

        if (typeGame == 1) {
            cout << "You      = [" << humanMark << "]\n";
            cout << "Computer = [" << computerMark << "]\n";
        } else {
            cout << "Player 1 = [X]\n";
            cout << "Player 2 = [O]\n";
        }

        cout << "\n";
        for (int i = 0; i < 9; i++)
            { 
                if( i==0 || i==3 || i==6 || i==8){ cout<<"\t\t     |     |     "<<endl;} 
                else if( i==1 || i==4 || i==7){ cout<<"\t\t  "<<arr[i-1]<<"  |  "<<arr[i]<<"  |  "<<arr[i+1]<<"  "<<endl;}
                else{cout<<"\t\t_____|_____|_____"<<endl;}
            }
    }

    // Meminta pilihan kotak dari pemain/player
    void humanTurn() {
        string prompt;

        if (typeGame == 1) {
            prompt = "\nYour turn [" + string(1, turn)
                   + "]. Choose a cell (1-9): ";
        } else if (turn == 'X') {
            prompt = "\nPlayer 1 [X], choose a cell (1-9): ";
        } else {
            prompt = "\nPlayer 2 [O], choose a cell (1-9): ";
        }

        while (true) {
            int choice = getInteger(1, 9, prompt);
            int index = choice - 1;

            if (!isEmpty(index)) {
                cout << "This cell is already occupied! "
                     << "Please choose another cell.\n";
                continue;
            }

            arr[index] = turn;
            break;
        }
    }

    // Komputer memilih secara acak dari kotak yang masih kosong
    void computerTurn() {
        int emptyPositions[9];
        int count = 0;

        for (int i = 0; i < 9; i++) {
            if (isEmpty(i)) {
                emptyPositions[count] = i;
                count++;
            }
        }

        if (count == 0) {
            return;
        }

        int randomIndex = rand() % count;
        int selectedIndex = emptyPositions[randomIndex];

        arr[selectedIndex] = turn;

        cout << "\nComputer [" << turn
             << "] chose cell "
             << selectedIndex + 1 << ".\n";
    }

    // Mengatur giliran
    void changeTurn() {
        turn = (turn == 'X') ? 'O' : 'X';
    }

    // Memeriksa apakah ada pemain yang berhasil membuat pola
    bool winner() const {
        const int patterns[8][3] = {
            {0, 1, 2}, // Baris pertama
            {3, 4, 5}, // Baris kedua
            {6, 7, 8}, // Baris ketiga
            {0, 3, 6}, // Kolom pertama
            {1, 4, 7}, // Kolom kedua
            {2, 5, 8}, // Kolom ketiga
            {0, 4, 8}, // Diagonal utama
            {2, 4, 6}  // Diagonal lainnya
        };

        for (int i = 0; i < 8; i++) {
            int a = patterns[i][0];
            int b = patterns[i][1];
            int c = patterns[i][2];

            if ((arr[a] == 'X' || arr[a] == 'O') &&
                arr[a] == arr[b] &&
                arr[b] == arr[c]) {
                return true;
            }
        }

        return false;
    }

    // Memeriksa seri
    bool matchDraw() const {
        for (int i = 0; i < 9; i++) {
            if (isEmpty(i)) {
                return false;
            }
        }

        return true;
    }

    // Menampilkan hasil
    void gameOver() const {
        cout << "\n========== GAME OVER ==========\n";

        if (typeGame == 1) {
            if (turn == humanMark) {
                cout << "Congratulations! You won!\n";
            } else {
                cout << "The computer won!\n";
            }
        } else {
            cout << "Player [" << turn << "] wins!\n";
        }
    }

public:
    // Mengatur mode dan giliran
    void setup() {
        cout << "=================================\n";
        cout << "         TIC TAC TOE GAME        \n";
        cout << "=================================\n";

        typeGame = getInteger(
            1, 2,
            "\nSelect a game mode:\n"
            "1. 1 Player (vs Computer)\n"
            "2. 2 Players\n"
            "Choice: "
        );

        turn = 'X';

        if (typeGame == 1) {
            pTurn = getInteger(
                1, 2,
                "\nChoose your starting turn:\n"
                "1. Play as X (go first)\n"
                "2. Play as O (computer goes first)\n"
                "Choice: "
            );

            if (pTurn == 1) {
                humanMark = 'X';
                computerMark = 'O';
            } else {
                humanMark = 'O';
                computerMark = 'X';
            }
        }
    }

    // Fungsi menjalaknan permainan
    void play() {
        while (true) {
            display();

            // Menentukan siapa yang memulai
            if (typeGame == 1 && turn == computerMark) {
                computerTurn();
            } else {
                humanTurn();
            }

            // Periksa kondisi permainan
            if (winner()) {
                display();
                gameOver();
                break;
            }

            if (matchDraw()) {
                display();
                cout << "\nThe game ended in a draw!\n";
                break;
            }

            // Giliran berpindah
            changeTurn();
        }
    }
};

int main() {
    // Inisialisasi angka acak satu kali
    srand(static_cast<unsigned int>(time(nullptr)));

    ticTacToeGame Game;

    Game.setup();
    Game.play();

    return 0;
}
