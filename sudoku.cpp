#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <sstream>
#include <string>

using namespace std;

class Board {
public:
    static const int SIZE = 9;   
    static const int BOX  = 3;   

    Board() { clear(); }

    void clear() {
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                grid[r][c]  = 0;       
                fixed[r][c] = false;   
            }
        }
    }

    int  get(int r, int c) const     { return grid[r][c]; }
    void set(int r, int c, int v)    { grid[r][c] = v; }
    bool isFixed(int r, int c) const { return fixed[r][c]; }

    void lockGivenCells() {
        for (int r = 0; r < SIZE; r++)
            for (int c = 0; c < SIZE; c++)
                fixed[r][c] = (grid[r][c] != 0);
    }

    bool isSafe(int r, int c, int v) const {
        for (int i = 0; i < SIZE; i++) {
            if (i != c && grid[r][i] == v) return false;   
            if (i != r && grid[i][c] == v) return false;   
        }
        int startR = (r / BOX) * BOX;   
        int startC = (c / BOX) * BOX;   
        for (int i = 0; i < BOX; i++) {
            for (int j = 0; j < BOX; j++) {
                int rr = startR + i;
                int cc = startC + j;
                if ((rr != r || cc != c) && grid[rr][cc] == v) return false;
            }
        }
        return true;
    }

    bool isFull() const {
        for (int r = 0; r < SIZE; r++)
            for (int c = 0; c < SIZE; c++)
                if (grid[r][c] == 0) return false;
        return true;
    }

    void print() const {
        cout << "\n      1 2 3   4 5 6   7 8 9\n";
        for (int r = 0; r < SIZE; r++) {
            if (r % BOX == 0) cout << "    +-------+-------+-------+\n";
            cout << "  " << (r + 1) << " |";
            for (int c = 0; c < SIZE; c++) {
                cout << ' ';
                if (grid[r][c] == 0)       cout << '.';
                else if (fixed[r][c])      cout << grid[r][c];                     // angka soal
                else                       cout << "\033[36m" << grid[r][c] << "\033[0m"; // angka pemain (cyan)
                if (c % BOX == BOX - 1) cout << " |";
            }
            cout << '\n';
        }
        cout << "    +-------+-------+-------+\n\n";
    }

private:
    int  grid[SIZE][SIZE];    
    bool fixed[SIZE][SIZE];   
};

class Generator {
public:
    Generator() : rng(random_device{}()) {}
    Board createPuzzle(int removeCount, Board &solution) {
        Board b;
        fillBoard(b, 0);        
        solution = b;          

        vector<int> cells;      
        for (int i = 0; i < Board::SIZE * Board::SIZE; i++) cells.push_back(i);
        shuffle(cells.begin(), cells.end(), rng);
        for (int i = 0; i < removeCount; i++) {
            b.set(cells[i] / Board::SIZE, cells[i] % Board::SIZE, 0);
        }

        b.lockGivenCells();     
        return b;
    }

private:
    mt19937 rng;   

    bool fillBoard(Board &b, int pos) {
        if (pos == Board::SIZE * Board::SIZE) return true;   

        int r = pos / Board::SIZE;
        int c = pos % Board::SIZE;

        vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        shuffle(nums.begin(), nums.end(), rng);              

        for (int n : nums) {
            if (b.isSafe(r, c, n)) {
                b.set(r, c, n);                             
                if (fillBoard(b, pos + 1)) return true;      
                b.set(r, c, 0);                              
            }
        }
        return false;  
    }
};

class Game {
public:
    void run() {
        int removeCount = chooseDifficulty();
        puzzle = generator.createPuzzle(removeCount, solution);
        showHelp();

        string line;
        while (true) {
            puzzle.print();
            cout << "Perintah > ";
            if (!getline(cin, line)) break;     

            istringstream iss(line);
            string cmd;
            iss >> cmd;
            if (cmd.empty()) continue;           

            if (cmd == "q") {
                cout << "Sampai jumpa!\n";
                break;
            } else if (cmd == "?") {
                showHelp();
            } else if (cmd == "h") {
                giveHint();
            } else if (cmd == "s") {
                autoSolve();
                break;
            } else {
                int r, c, v;
                istringstream nums(line);
                if (!(nums >> r >> c >> v)) {
                    cout << "Format salah. Ketik ? untuk bantuan.\n";
                    continue;
                }
                placeNumber(r - 1, c - 1, v);    
            }

            if (puzzle.isFull()) {
                puzzle.print();
                cout << "SELAMAT! Sudoku berhasil diselesaikan!\n";
                break;
            }
        }
    }

private:
    Board puzzle;      
    Board solution;    
    Generator generator;   

    int chooseDifficulty() {
        cout << "=== SUDOKU TERMINAL ===\n";
        cout << "Pilih level: 1) Mudah  2) Sedang  3) Sulit\n> ";
        string line;
        getline(cin, line);
        int level = 2;                           
        try { level = stoi(line); } catch (...) {}
        if (level == 1) return 35;
        if (level == 3) return 52;
        return 45;
    }

    void showHelp() const {
        cout << "\nCARA MAIN:\n"
             << "  baris kolom angka   -> isi sel, contoh: 3 5 7\n"
             << "  baris kolom 0       -> hapus isi sel\n"
             << "  h                   -> minta 1 petunjuk\n"
             << "  s                   -> tampilkan jawaban (menyerah)\n"
             << "  ?                   -> tampilkan bantuan\n"
             << "  q                   -> keluar\n";
    }

    void placeNumber(int r, int c, int v) {
        if (r < 0 || r >= 9 || c < 0 || c >= 9 || v < 0 || v > 9) {
            cout << "Baris/kolom harus 1-9 dan angka 0-9.\n";
            return;
        }
        if (puzzle.isFixed(r, c)) {
            cout << "Sel itu adalah angka soal, tidak bisa diubah.\n";
            return;
        }
        if (v == 0) {                            
            puzzle.set(r, c, 0);
            return;
        }
        if (!puzzle.isSafe(r, c, v)) {
            cout << "Angka " << v << " bentrok dengan baris/kolom/kotak!\n";
            return;
        }
        puzzle.set(r, c, v);
    }

    void giveHint() {
        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                if (puzzle.get(r, c) == 0) {
                    int v = solution.get(r, c);
                    if (!puzzle.isSafe(r, c, v)) {
                        cout << "Ada angka isian salah yang bentrok. "
                             << "Coba hapus (isi 0) dulu.\n";
                        return;
                    }
                    puzzle.set(r, c, v);
                    cout << "Petunjuk: baris " << r + 1 << ", kolom "
                         << c + 1 << " = " << v << "\n";
                    return;
                }
            }
        }
    }

    void autoSolve() {
        for (int r = 0; r < 9; r++)
            for (int c = 0; c < 9; c++)
                if (!puzzle.isFixed(r, c))
                    puzzle.set(r, c, solution.get(r, c));
        puzzle.print();
        cout << "Ini jawabannya. Coba lagi lain kali!\n";
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
