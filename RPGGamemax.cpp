#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;
#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif
const int ukuranpapan = 6;
constexpr const char* warnanormal = "\033[0m";
constexpr const char* warnapemain = "\033[1;36m";
constexpr const char* warnamusuh = "\033[1;31m";
constexpr const char* warnatambahan = "\033[1;32m";
constexpr const char* warnajebakan = "\033[1;35m";
constexpr const char* warnalantairapuh = "\033[1;33m";
constexpr const char* warnatembok = "\033[1;30m";
class mesinpermainan;
struct pahlawan {
    int x, y;
    int kekuatan;
    int stamina;
};
class objekpermainan {
protected:
    char tipeobjek;
    int nilaiobjek;
public:
    objekpermainan(char tipeawal, int nilaiawal) : tipeobjek(tipeawal), nilaiobjek(nilaiawal) {}
    virtual ~objekpermainan() {}
    char ambiltipeobjek() const { return tipeobjek; }
    int ambilnilaiobjek() const { return nilaiobjek; }
    virtual bool urusbenturan(pahlawan& pemain, mesinpermainan& mesin) = 0;
};
class tambahkekuatan : public objekpermainan {
public:
    tambahkekuatan(int nilaitambahan) : objekpermainan('+', nilaitambahan) {}
    bool urusbenturan(pahlawan& pemain, mesinpermainan& mesin) override {
        pemain.kekuatan += nilaiobjek;
        return true;
    }
};
class jebakantersembunyi : public objekpermainan {
public:
    jebakantersembunyi(int nilaikerusakan) : objekpermainan('-', nilaikerusakan) {}
    bool urusbenturan(pahlawan& pemain, mesinpermainan& mesin) override {
        pemain.kekuatan -= nilaiobjek;
        if (pemain.kekuatan < 0) pemain.kekuatan = 0;
        return true;
    }
};
class monster : public objekpermainan {
public:
    monster(int tingkatancaman) : objekpermainan('e', tingkatancaman) {}
    bool urusbenturan(pahlawan& pemain, mesinpermainan& mesin) override;
};
class mesinpermainan {
public:
    objekpermainan* petakobjek[ukuranpapan][ukuranpapan];
    char petaklantai[ukuranpapan][ukuranpapan];
    pahlawan pemain;
    int sisamonster;
    bool sudahkalah;
    bool sudahmenang;
    mesinpermainan() {
        pemain = {0, 0, 0, 15};
        sisamonster = 3;
        sudahkalah = false;
        sudahmenang = false;
        for (int baris = 0; baris < ukuranpapan; baris++) {
            for (int kolom = 0; kolom < ukuranpapan; kolom++) {
                petakobjek[baris][kolom] = nullptr;
                petaklantai[baris][kolom] = '.';
            }
        }
    }
    ~mesinpermainan() {
        for (int baris = 0; baris < ukuranpapan; baris++) {
            for (int kolom = 0; kolom < ukuranpapan; kolom++) {
                if (petakobjek[baris][kolom] != nullptr) {
                    delete petakobjek[baris][kolom];
                }
            }
        }
    }
    void gambarlayar();
    void gerakkanpemain(char arah);
};
bool monster::urusbenturan(pahlawan& pemain, mesinpermainan& mesin) {
    if (pemain.kekuatan > nilaiobjek) {
        pemain.kekuatan += nilaiobjek;
        mesin.sisamonster--;
        if (mesin.sisamonster == 0) mesin.sudahmenang = true;
        return true;
    } else {
        mesin.sudahkalah = true;
        return false;
    }
}
void mesinpermainan::gambarlayar() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    cout << "==================================\n";
    cout << "      " << warnapemain << "rpg kekuatan matriks" << warnanormal << " c++\n";
    cout << "==================================\n";
    if (sudahmenang) cout << " status : " << warnatambahan << "YOU WIN!!!" << warnanormal << "\n";
    else if (sudahkalah) cout << " status : " << warnamusuh << "YOU LOSE:((" << warnanormal << "\n";
    else cout << " status : bermain\n";
    cout << " musuh  : " << sisamonster << " tersisa\n";
    cout << " kekuatan : " << warnapemain << setfill('0') << setw(2) << pemain.kekuatan << warnanormal << "\n";
    const char* warnastamina = (pemain.stamina <= 5) ? warnamusuh : warnatambahan;
    cout << " stamina: " << warnastamina << setfill('0') << setw(2) << pemain.stamina << warnanormal << " langkah\n";
    cout << "----------------------------------\n\n";
    for (int baris = 0; baris < ukuranpapan; baris++) {
        for (int kolom = 0; kolom < ukuranpapan; kolom++) {
            const char* warnaaktif = warnanormal;
            string teksisian = "..";
            if (pemain.x == kolom && pemain.y == baris) {
                warnaaktif = warnapemain;
                ostringstream aliranurai;
                aliranurai << setfill('0') << setw(2) << pemain.kekuatan;
                teksisian = aliranurai.str();
            } else if (petaklantai[baris][kolom] == '#') {
                warnaaktif = warnatembok;
                teksisian = "##";
            } else if (petakobjek[baris][kolom] != nullptr) {
                objekpermainan* objeksaatini = petakobjek[baris][kolom];
                if (objeksaatini->ambiltipeobjek() == '+') warnaaktif = warnatambahan;
                else if (objeksaatini->ambiltipeobjek() == '-') warnaaktif = warnajebakan;
                else if (objeksaatini->ambiltipeobjek() == 'e') warnaaktif = warnamusuh;
                ostringstream aliranurai;
                aliranurai << setfill('0') << setw(2) << objeksaatini->ambilnilaiobjek();
                teksisian = aliranurai.str();
            }
            if (petaklantai[baris][kolom] == '~' && teksisian != "##") {
                cout << warnalantairapuh << "{" << warnanormal << " " << warnaaktif << teksisian << warnanormal << " " << warnalantairapuh << "}" << warnanormal << " ";
            } else if (teksisian == "##") {
                cout << "  " << warnatembok << "##" << warnanormal << "  ";
            } else {
                cout << "[ " << warnaaktif << teksisian << warnanormal << " ] ";
            }
        }
        cout << "\n\n";
    }
    cout << "\npanduan: [+] hijau  [-] magenta (jebakan)  [e] merah  { } rapuh\n";
    if (!sudahkalah && !sudahmenang) cout << "gerak (w/a/s/d): ";
}
void mesinpermainan::gerakkanpemain(char arah) {
    int pindahx = 0, pindahy = 0;
    if (arah == 'w') pindahy = -1;
    else if (arah == 's') pindahy = 1;
    else if (arah == 'a') pindahx = -1;
    else if (arah == 'd') pindahx = 1;
    else return;
    int xlanjut = pemain.x + pindahx;
    int ylanjut = pemain.y + pindahy;
    if (xlanjut < 0 || xlanjut >= ukuranpapan || ylanjut < 0 || ylanjut >= ukuranpapan) return;
    if (petaklantai[ylanjut][xlanjut] == '#') return;
    objekpermainan* objektujuan = petakobjek[ylanjut][xlanjut];
    if (objektujuan == nullptr || objektujuan->urusbenturan(pemain, *this)) {
        if (objektujuan != nullptr) {
            delete objektujuan;
            petakobjek[ylanjut][xlanjut] = nullptr;
        }
        if (petaklantai[pemain.y][pemain.x] == '~') {
            petaklantai[pemain.y][pemain.x] = '#';
        }
        pemain.x = xlanjut;
        pemain.y = ylanjut;
        pemain.stamina--;
        if (pemain.stamina <= 0 && !sudahmenang) {
            sudahkalah = true;
        }
    }
}
int main() {
#ifdef _WIN32
    HANDLE pengendali = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modepengendali = 0;
    GetConsoleMode(pengendali, &modepengendali);
    modepengendali |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(pengendali, modepengendali);
#endif
    mesinpermainan mesin;
    mesin.petaklantai[1][2] = '~';
    mesin.petaklantai[2][2] = '~';
    mesin.petaklantai[3][2] = '~';
    mesin.petaklantai[4][0] = '#';
    mesin.petaklantai[4][1] = '#';
    mesin.petakobjek[0][2] = new tambahkekuatan(3);
    mesin.petakobjek[1][2] = new jebakantersembunyi(2);
    mesin.petakobjek[2][4] = new tambahkekuatan(6);
    mesin.petakobjek[3][0] = new monster(2);
    mesin.petakobjek[3][4] = new monster(6);
    mesin.petakobjek[5][5] = new monster(12);
    char tombolmasukan;
    while (!mesin.sudahkalah && !mesin.sudahmenang) {
        mesin.gambarlayar();
        cin >> tombolmasukan;
        mesin.gerakkanpemain(tombolmasukan);
    }
    mesin.gambarlayar();
    cin.ignore(256, '\n');
    cout << "tekan tombol enter untuk keluar...";
    cin.get();
    return 0;
}