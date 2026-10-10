#include<iostream>
#include<vector>
#include<cstdlib>

using namespace std;

class Player{
private:
    string nama;
    char tanda;
    bool AI;

public:
    Player(){
        nama = "Player";
        tanda = 'X';
        AI = false;
    }
    void setNama(string n){
        nama = n;
    }

    void setTanda(char t){
        tanda = t;
    }

    void setAI(bool ai){
        AI = ai;
    }

    Player(string n, char t, bool ai){
        setNama(n);
        setTanda(t);
        setAI(ai);
    }

    string getNama() {
        return nama;
    }

    char getTanda(){
        return tanda;
    }

    bool getAI(){
        return AI;
    }
};

class Game{
private:
    char board[3][3];
    int giliran;
    Player player1;
    Player player2;

    void inisialisasiBoard(){
        int angka = 1;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                board[i][j] = '0' + angka++;
            }
        }
    }

    void visualisasiBoard(){
        cout << endl;
        cout << " " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << endl;
        cout << "---+---+---\n";
        cout << " " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << endl;
        cout << "---+---+---\n";
        cout << " " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << endl << endl;
    }

    bool validMove(int pilihan){
        if(pilihan < 1 || pilihan > 9) return false;
        int row = (pilihan-1)/3;
        int col = (pilihan-1)%3;
        return (board[row][col] != 'X' && board[row][col] != 'O');
    }

    void isi(int pilihan, char tanda){
        int row = (pilihan-1)/3;
        int col = (pilihan-1)%3;
        board[row][col] = tanda;
    }

    bool cekMenang(char s){
        for(int i = 0; i < 3; i++){
            if(board[i][0] == s && board[i][1] == s && board[i][2] == s) return true;
            if(board[0][i] == s && board[1][i] == s && board[2][i] == s) return true;
        }

        if(board[0][0] == s && board[1][1] == s && board[2][2] == s) return true;
        if(board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;

        return false;
    }

    int cariLangkah(char simbol){
        for(int i = 1; i <= 9; i++){
            if(validMove(i)){
                int row = (i - 1)/3;
                int col = (i - 1)%3;
                char temp = board[row][col];
                board[row][col] = simbol;

                bool menang = cekMenang(simbol);
                board[row][col] = temp;

                if(menang) return i;
            }
        }
        return -1;
    }

    int langkahAI(char simbolai, char simbol1){
        int langkah = cariLangkah(simbolai);
        if(langkah != -1) return langkah;

        langkah = cariLangkah(simbol1);
        if(langkah != -1) return langkah;

        if(validMove(5)) return 5;

        vector<int> kosong;
        for(int i = 1; i <= 9; i++){
            if(validMove(i)) kosong.push_back(i);
        }
        return kosong[rand()%kosong.size()];
    }

    bool seri(){
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(board[i][j] != 'X' && board[i][j] != 'O') return false;
            }
        }
        return true;
    }

public:
    Game(Player p1, Player p2){
        this->player1 = p1;
        this->player2 = p2;
        this->giliran = 0;
        inisialisasiBoard();
    }

    void play(){
        cout << "=========================" << endl;
        cout << "       TIC TAC TOE       " << endl;
        cout << "=========================" << endl;

        while(true){
            visualisasiBoard();
            Player& p_now = (giliran == 0) ? player1 : player2;
            Player& p_enemy = (giliran == 0) ? player2 : player1;

            int pilihan;
            if(p_now.getAI()){
                pilihan = langkahAI(p_now.getTanda(), p_enemy.getTanda());
                cout << "[AI] memilih nomor: " << pilihan << endl;
            } else {
                cout << "[" << p_now.getNama() << "] (" << p_now.getTanda() << "), pilih nomor (1-9): ";
                cin >> pilihan;

                while(!validMove(pilihan)){
                    cout << "[Tidak Valid] Silakan pilih nomor petak yang kosong." << endl;
                    cout << "[" << p_now.getNama() << "] (" << p_now.getTanda() << "), pilih nomor (1-9): ";
                    cin >> pilihan;
                }
            }

            isi(pilihan, p_now.getTanda());

            if(cekMenang(p_now.getTanda())){
                visualisasiBoard();
                cout << "SELAMAT! [" << p_now.getNama() << "] (" << p_now.getTanda() << ") MENANG!" << endl;
                break;
            }

            if(seri()){
                visualisasiBoard();
                cout << "PERMAINAN SERI" << endl;
                break;
            }

            giliran = 1 - giliran;
        }
    }    
};

int main (){
    int mode;
    cout << "=== PERMAINAN TIC TAC TOE ===" << endl;
    cout << "1. Player 1 vs Player 2" << endl;
    cout << "2. Player vs AI" << endl;
    cout << "Pilih mode permainan: ";
    cin >> mode;

    string nama1, nama2;
    cout << "Masukkan nama Player 1 (X): ";
    cin >> nama1;

    Player p1(nama1, 'X', false);
    Player p2;

    if(mode == 2){
        p2 = Player("AI", 'O', true);
    } else {
        cout << "Masukkan nama Player 2 (O): ";
        cin >> nama2;
        p2 = Player(nama2, 'O', false);
    }

    Game permainan(p1, p2);
    permainan.play();

    return 0;
}