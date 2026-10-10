#include <iostream>
#include <limits>

class TicTacToe {

private:
    int map[3][3];
    char giliran;
    int mode;               
    const char AI = 'O';    
    const char MANUSIA = 'X';

public:

    TicTacToe(){
        giliran = 'X';
        mode = 2;
        int isi = '1';
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                map[i][j] = isi;
                isi++;
            }
        }
    }

    void displayPeta(){
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                std::cout << (char)map[i][j];
                if(j < 2){
                    std::cout << " | ";
                }
            }
            std::cout << std::endl;
            if(i < 2){
                std::cout << "--+---+--";
            }
            std::cout << std::endl;
        }
    }

    void move(int inputValue){
        int row = (inputValue - 1) / 3;
        int col = (inputValue - 1) % 3;
        map[row][col] = giliran;
    }

    void switchGiliran(){
        if(giliran == 'X'){
            giliran = 'O';
        } else if(giliran == 'O'){
            giliran = 'X';
        }
    }

    bool timpa(int inputValue){
        int row = (inputValue - 1) / 3;
        int col = (inputValue - 1) % 3;

        if(map[row][col] == 'X' || map[row][col] == 'O'){
            std::cout << "Gaboleh timpa lawan woi!!! ULANG!!\n";
            return true;
        }
        return false;
    }

    // 8 kemungkinan menang
    bool cekMenang(){
        for(int i = 0; i < 3; i++){                                     // Horizontal
            if(map[i][0] == map[i][1] && map[i][1] == map[i][2]){
                return true;
            }
        }
        for(int i = 0; i < 3; i++){                                     // Vertikal
            if(map[0][i] == map[1][i] && map[1][i] == map[2][i]){
                return true;
            }
        }
        if(map[0][0] == map[1][1] && map[1][1] == map[2][2]){           // Diagonal KANAN
            return true;
        }
        if(map[0][2] == map[1][1] && map[1][1] == map[2][0]){           // Diagonal KIRI
            return true;
        }
        return false;
    }

    bool cekSeri(){
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(map[i][j] != 'X' && map[i][j] != 'O'){
                    return false;
                }
            }
        }
        return true;
    }

    int minimax(char pemain, int depth){
        bool giliranAI = (pemain == AI);
        int terbaik = giliranAI ? -1000 : 1000;

        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(map[i][j] == 'X' || map[i][j] == 'O'){
                    continue;
                }

                int asal = map[i][j];   
                map[i][j] = pemain;

                int skor;
                if(cekMenang()){
                    
                    skor = giliranAI ? (10 - depth) : (depth - 10);
                } else if(cekSeri()){
                    skor = 0;
                } else {
                    skor = minimax(giliranAI ? MANUSIA : AI, depth + 1);
                }

                map[i][j] = asal;       

                if(giliranAI){
                    if(skor > terbaik) terbaik = skor;
                } else {
                    if(skor < terbaik) terbaik = skor;
                }
            }
        }
        return terbaik;
    }

    int langkahAI(){
        int skorTerbaik = -1000;
        int langkah = -1;

        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(map[i][j] == 'X' || map[i][j] == 'O'){
                    continue;
                }

                int asal = map[i][j];
                map[i][j] = AI;

                int skor;
                if(cekMenang()){
                    skor = 10;
                } else if(cekSeri()){
                    skor = 0;
                } else {
                    skor = minimax(MANUSIA, 1);
                }

                map[i][j] = asal;

                if(skor > skorTerbaik){
                    skorTerbaik = skor;
                    langkah = i * 3 + j + 1;
                }
            }
        }
        return langkah;
    }

    void pilihMode(){
        int pilihan = 0;
        while(true){
            std::cout << "===== TIC TAC TOE =====\n";
            std::cout << "1. Lawan AI\n";
            std::cout << "2. Main berdua (2 pemain)\n";
            std::cout << "Pilih mode (1/2): ";

            if(!(std::cin >> pilihan)){
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Input SALAH! masukkan angka 1 atau 2.\n\n";
                continue;
            }
            if(pilihan == 1 || pilihan == 2){
                mode = pilihan;
                break;
            }
            std::cout << "Input SALAH! pilih 1 atau 2.\n\n";
        }
        std::cout << std::endl;

        if(mode == 1){
            std::cout << "Mode: Lawan AI (kamu 'X', AI 'O')\n\n";
        } else {
            std::cout << "Mode: 2 Pemain\n\n";
        }
    }

    void game(){
        pilihMode();

        int inputValue;

        while(true){
            displayPeta();

            if(mode == 1 && giliran == AI){
                inputValue = langkahAI();
                std::cout << "AI ('" << giliran << "') memilih: " << inputValue << "\n\n";
            } else {
                std::cout << "Masukkan angka pemain '" << giliran << "' (1-9): ";
                if(!(std::cin >> inputValue)){
                    if(std::cin.eof()){
                        std::cout << "\nInput berakhir. Game dihentikan.\n";
                        break;
                    }
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Input SALAH! masukkan ANGKA 1-9!!\n";
                    continue;
                }

                if(inputValue < 1 || inputValue > 9){
                    std::cout << "Input SALAH! baca lagi MAPNYA!!\n";
                    continue;
                }

                if(timpa(inputValue)) {
                    continue;
                }
            }

            move(inputValue);

            if(cekMenang()) {
                if(mode == 1 && giliran == AI){
                    std::cout << "AI ('" << giliran << "') menang!!!\n\n";
                } else {
                    std::cout << "player " << giliran << " win!!!\n\n";
                }
                std::cout << "Hasil: \n";
                displayPeta();
                break;
            }

            if(cekSeri()) {
                std::cout << "\n===== SERI karena map sudah penuh =====\n";
                std::cout << "Hasil: \n\n";
                displayPeta();
                break;
            }

            switchGiliran();
        }
    }

};

int main() {

    TicTacToe mulaicoy;
    mulaicoy.game();

    return 0;
}