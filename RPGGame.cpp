#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
const int ukuranpapan = 5;
class mesinpermainan;
struct pahlawan {
    int x, y;
    int kekuatan;
};
class objekpermainan {
protected:
    char tipeobjek;
    int nilaiobjek;
    int idobjek;
public:
    objekpermainan(char tipeawal, int nilaiawal, int idawal) : tipeobjek(tipeawal), nilaiobjek(nilaiawal), idobjek(idawal) {}
    virtual ~objekpermainan() {}
    char ambiltipeobjek() const { return tipeobjek; }
    int ambilnilaiobjek() const { return nilaiobjek; }
    int ambilidobjek() const { return idobjek; }
    virtual bool urusbenturan(pahlawan& pemain, mesinpermainan& mesin) = 0;
};
class monster : public objekpermainan {
public:
    monster(int tingkatancaman, int idmonster) : objekpermainan('e', tingkatancaman, idmonster) {}
    bool urusbenturan(pahlawan& pemain, mesinpermainan& mesin) override;
};
class mesinpermainan {
public:
    objekpermainan* petakobjek[ukuranpapan][ukuranpapan];
    pahlawan pemain;
    int sisamonster;
    bool sudahkalah;
    bool sudahmenang;
    mesinpermainan() {
        pemain = {0, 0, 3};
        sisamonster = 3;
        sudahkalah = false;
        sudahmenang = false;
        for (int baris = 0; baris < ukuranpapan; baris++) {
            for (int kolom = 0; kolom < ukuranpapan; kolom++) {
                petakobjek[baris][kolom] = nullptr;
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
    system("cls");
    cout << "---    rpg matriks    ---\n\n";
    for (int baris = 0; baris < ukuranpapan; baris++) {
        for (int kolom = 0; kolom < ukuranpapan; kolom++) {
            if (pemain.x == kolom && pemain.y == baris) {
                cout << "[ p  ] ";
            } else if (petakobjek[baris][kolom] != nullptr) {
                cout << "[ e" << petakobjek[baris][kolom]->ambilidobjek() << " ] ";
            } else {
                cout << "[    ] ";
            }
        }
        cout << "\n\n";
    }
    cout << "keterangan:\n";
    cout << "p = player (kekuatan: " << pemain.kekuatan << ")\n";
    cout << "e = enemy\n";
    for (int baris = 0; baris < ukuranpapan; baris++) {
        for (int kolom = 0; kolom < ukuranpapan; kolom++) {
            if (petakobjek[baris][kolom] != nullptr) {
                cout << "e" << petakobjek[baris][kolom]->ambilidobjek() << " = " << petakobjek[baris][kolom]->ambilnilaiobjek() << "\n";
            }
        }
    }
    cout << "\n";
    if (sudahmenang) cout << "YOU WIN!!!\n";
    else if (sudahkalah) cout << "YOU LOSE:((\n";
    else cout << "gerak (w/a/s/d): ";
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
    objekpermainan* objektujuan = petakobjek[ylanjut][xlanjut];
    if (objektujuan == nullptr || objektujuan->urusbenturan(pemain, *this)) {
        if (objektujuan != nullptr) {
            delete objektujuan;
            petakobjek[ylanjut][xlanjut] = nullptr;
        }
        pemain.x = xlanjut;
        pemain.y = ylanjut;
    }
}
int main() {
    mesinpermainan mesin;
    mesin.petakobjek[1][2] = new monster(2, 1);
    mesin.petakobjek[3][1] = new monster(4, 2);
    mesin.petakobjek[4][4] = new monster(8, 3);
    char tombol;
    while (!mesin.sudahkalah && !mesin.sudahmenang) {
        mesin.gambarlayar();
        cin >> tombol;
        mesin.gerakkanpemain(tombol);
    }
    mesin.gambarlayar();
    
    cin.ignore(256, '\n'); 
    
    cout << "Tekan tombol Enter untuk keluar...";
    cin.get(); 
    
    return 0;
}