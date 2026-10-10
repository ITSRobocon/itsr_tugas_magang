#include <iostream>
#include <string>

using namespace std;

string nama;
string profesi;
int hpbar;
int attack;
int defense;

class Player {
    public:
        string nama;
        int hpbar;
        int attack;
        int defense;

        Player(string namaInput){
            nama = namaInput;
            hpbar = 0;
            attack = 0;
            defense = 0;
        }

        void infoPlayer(){
            cout << "\nNama Player: " << nama << endl;
            cout << "Class: " << profesi << endl;
            cout << "HP Bar: " << hpbar << endl;
            cout << "Attack: " << attack << endl;
            cout << "Defense: " << defense << endl;
        }
};

class Warrior : public Player {
    public:
        Warrior(string namaInput) : Player(namaInput) {
            profesi = "Warrior";
            hpbar = 150;
            attack = 50;
            defense = 30;
        }
};

class Mage : public Player {
    public:
        Mage(string namaInput) : Player(namaInput) {
            profesi = "Mage";
            hpbar = 100;
            attack = 70;
            defense = 20;
        }
};

class Archer : public Player {
    public:
        Archer(string namaInput) : Player(namaInput) {
            profesi = "Archer";
            hpbar = 120;
            attack = 60;
            defense = 25;
        }
};

class Musuh {
    public:
        string jenis;
        int hpbar;
        int attack;
        int defense;

        Musuh(string jenisInput){
            jenis = jenisInput;
            hpbar = 0;
            attack = 0;
            defense = 0;
        }

        void infoMusuh(){
            cout << "\nJenis Musuh: " << jenis << endl;
            cout << "HP Bar: " << hpbar << endl;
            cout << "Attack: " << attack << endl;
            cout << "Defense: " << defense << endl;
        }
};

class Slime : public Musuh {
    public:
        Slime() : Musuh("Slime"){
            hpbar = 50;
            attack = 10;
            defense = 5;
        }
};

class Goblin : public Musuh {
    public:
        Goblin() : Musuh("Goblin"){
            hpbar = 80;
            attack = 20;
            defense = 10;
        }
};

class Ogre : public Musuh {
    public:
        Ogre() : Musuh("Ogre"){
            hpbar = 120;
            attack = 30;
            defense = 15;
        }
};

void playerMenyerang(Player& player, Musuh& musuh){
    int damage = player.attack - musuh.defense;
    if (damage < 0) {
        damage = 0;
    }
    musuh.hpbar -= damage;
    cout << player.nama << " menyerang " << musuh.jenis << " dan memberikan " << damage << " damage!" << endl;
    if (musuh.hpbar <= 0) {
        cout << musuh.jenis << " telah dikalahkan!" << endl;
    } else {
        cout << musuh.jenis << " memiliki sisa HP: " << musuh.hpbar << endl;
    }
}

void musuhMenyerang(Musuh& musuh, Player& player){
    int damage = musuh.attack - player.defense;
    if (damage < 0) {
        damage = 0;
    }
    player.hpbar -= damage;
    cout << musuh.jenis << " menyerang " << player.nama << " dan memberikan " << damage << " damage!" << endl;
    if (player.hpbar <= 0) {
        cout << player.nama << " telah dikalahkan!" << endl;
    } else {
        cout << player.nama << " memiliki sisa HP: " << player.hpbar << endl;
    }
}

void battle(Player& player, Musuh& musuh){
    cout << "\n=================================" << endl;
    cout << "         Battle Start XD          " << endl;
    cout << "=================================" << endl;

    while (player.hpbar > 0 && musuh.hpbar > 0) {
        playerMenyerang(player, musuh);
        if (musuh.hpbar > 0) {
            musuhMenyerang(musuh, player);
        } else {
            cout << musuh.jenis << " telah dikalahkan!" << endl;
        }
    }
}

int main() {

    string nama;
    int pilihan1, pilihan2;

    cout << "=================================" << endl;
    cout << "       SIMPLE RPG BATTLE         " << endl;
    cout << "=================================" << endl;

    cout << "Masukkan nama player: ";
    cin >> nama;

    cout << "\nPilih class:\n";
    cout << "1. Warrior\n";
    cout << "2. Mage\n";
    cout << "3. Archer\n";
    cout << "Pilihan: ";
    cin >> pilihan1;

    Player* player;
    Warrior warrior(nama);
    Mage mage(nama);
    Archer archer(nama);

    if (pilihan1 == 1) {
        player = &warrior;
        player->infoPlayer();
    }
    else if (pilihan1 == 2) {
        player = &mage;
        player->infoPlayer();
    }
    else if (pilihan1 == 3) {
        player = &archer;
        player->infoPlayer();
    }
    else {
        cout << "Pilihan tidak valid!" << endl;
    }

    cout << "\nPilih musuh:\n";
    cout << "1. Slime\n";
    cout << "2. Goblin\n";
    cout << "3. Ogre\n";
    cout << "Pilihan: ";
    cin >> pilihan2;

    Musuh *musuh;
    Slime Slime;
    Goblin Goblin;
    Ogre Ogre;

    if (pilihan2 == 1) {
        musuh = &Slime;
        musuh->infoMusuh();
    }
    else if (pilihan2 == 2) {
        musuh = &Goblin;
        musuh->infoMusuh();
    }
    else if (pilihan2 == 3) {
        musuh = &Ogre;
        musuh->infoMusuh();
    }
    else {
        cout << "Pilihan tidak valid!" << endl;
    }

    battle(*player, *musuh);
    return 0;
}

