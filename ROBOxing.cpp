#include <iostream>
#include <string>

using namespace std;

class Player {
private:
    string name;
    string specialAttackName;
    string ultimateAttackName;
    
    int hp, maxHp;
    int stamina, maxStamina;
    int attackPower;
    
    int specialCount;
    int dotTurnsLeft;
    int healingLeft;

public:
    Player() {
        hp = 200;
        maxHp = 200;
        specialCount = 0;
        dotTurnsLeft = 0;
        healingLeft = 3;
    }

    void setupCharacter(int playerNum) {
        cout << "\n=== REGISTRASI PLAYER " << playerNum << " ===\n";
        cout << "Masukkan Nama Karakter: ";
        getline(cin, name);
        
        bool valid = false;
        while (!valid) {
            cout << "Atur Max Stamina (Max 100): ";
            cin >> maxStamina;
            cout << "Atur Attack Power (Max 100): ";
            cin >> attackPower;
            
            if (maxStamina > 100 || attackPower > 100) {
                cout << "[!] Stamina dan Attack Power maksimal masing-masing adalah 100!\n";
            } else if (maxStamina + attackPower > 150) {
                cout << "[!] Total Stamina + Attack Power tidak boleh lebih dari 150!\n";
            } else {
                valid = true;
            }
        }
        stamina = maxStamina;
        
        cin.ignore();
        cout << "Nama Jurus Special Attack: ";
        getline(cin, specialAttackName);
        cout << "Nama Jurus Ultimate: ";
        getline(cin, ultimateAttackName);
    }

    void takeDamage(int damage) {
        hp -= damage;
        if (hp < 0) hp = 0;
    }

    void basicAttack(Player &target) {
        cout << name << " melancarkan Basic Attack! (+10 Damage)\n";
        target.takeDamage(10);
    }

    bool specialAttack(Player &target) {
        int staminaCost = attackPower * 0.4;
        int damage = attackPower * 0.6;

        if (stamina >= staminaCost) {
            stamina -= staminaCost;
            specialCount++;
            cout << name << " menggunakan [" << specialAttackName << "]! (+" << damage << " Damage, -" << staminaCost << " Stamina)\n";
            target.takeDamage(damage);
            return true;
        } else {
            cout << "[!] Stamina tidak cukup untuk Special Attack! (Butuh " << staminaCost << ")\n";
            return false;
        }
    }

    bool ultimateAttack(Player &target) {
        if (specialCount >= 2) {
            cout << "\n>>> " << name << " MENGELUARKAN ULTIMATE: [" << ultimateAttackName << "]! <<<\n";
            cout << "Target terkena 50 Damage Instan dan Damage Berkelanjutan (-5 HP selama 4 Turn)!\n";
            
            target.takeDamage(50);
            target.dotTurnsLeft = 4;
            
            specialCount = 0;
            return true;
        } else {
            cout << "[!] Ultimate belum siap! (Butuh " << 2 - specialCount << " kali Special Attack lagi)\n";
            return false;
        }
    }

    bool heal() {
        if (healingLeft > 0) {
            hp += 50;
            if (hp > maxHp) hp = maxHp;
            
            stamina += 10;
            if (stamina > maxStamina) stamina = maxStamina;
            
            healingLeft--;
            cout << name << " melakukan Heal! (+50 HP, +10 Stamina). Sisa Healing Potion: " << healingLeft << "x\n";
            return true;
        } else {
            cout << "[!] Healing Potion sudah habis!\n";
            return false;
        }
    }

    void applyDoT() {
        if (dotTurnsLeft > 0) {
            cout << "[EFEK ULTIMATE] " << name << " terkena damage berkelanjutan! (-5 HP)\n";
            takeDamage(5);
            dotTurnsLeft--;
        }
    }

    void recoverStamina() {
        stamina += 10;
        if (stamina > maxStamina) stamina = maxStamina;
    }

    void printStatus() {
        cout << "[" << name << "] HP: " << hp << "/" << maxHp 
             << " | Stamina: " << stamina << "/" << maxStamina 
             << " | Special Attack Count: " << specialCount << "/2\n";
    }

    bool isAlive() { return hp > 0; }
    string getName() { return name; }
};

class Game {
private:
    Player p1, p2;

    void showGuide() {
        cout << "==========================================\n";
        cout << "           WELCOME TO ROBOXING\n";
        cout << "==========================================\n";
        cout << "GUIDES:\n";
        cout << "> Max HP 200, Max Stamina 100, Max Attack Power 100.\n";
        cout << "> Total Stamina + Attack Power maksimal 150.\n";
        cout << "> Basic Attack memberikan 10 Damage.\n";
        cout << "> Special Attack membutuhkan stamina & menghasilkan damage berdasarkan Attack Power.\n";
        cout << "> Ultimate akan terbuka setelah melakukan 2x Special Attack.\n";
        cout << "> Ultimate memberikan damage instan + damage berkelanjutan (-5 HP/turn).\n";
        cout << "> Healing Potion (+50 HP, +10 Stamina) maksimal 3x per game.\n";
        cout << "> Stamina pulih 10 poin di setiap akhir ronde.\n";
        cout << "==========================================\n\n";
    }

    void playTurn(Player &attacker, Player &defender) {
        attacker.applyDoT();
        if (!attacker.isAlive()) return;

        bool validAction = false;
        while (!validAction) {
            cout << "\nGiliran " << attacker.getName() << "!\n";
            cout << "1. Basic Attack\n";
            cout << "2. Special Attack\n";
            cout << "3. Ultimate Attack\n";
            cout << "4. Healing Potion\n";
            cout << "Pilih aksi (1-4): ";
            
            int choice;
            cin >> choice;

            if (choice == 1) {
                attacker.basicAttack(defender);
                validAction = true;
            } else if (choice == 2) {
                validAction = attacker.specialAttack(defender);
            } else if (choice == 3) {
                validAction = attacker.ultimateAttack(defender);
            } else if (choice == 4) {
                validAction = attacker.heal();
            } else {
                cout << "[!] Pilihan tidak valid!\n";
            }
        }
    }

public:
    void start() {
        showGuide();
        
        p1.setupCharacter(1);
        p2.setupCharacter(2);

        cout << "\n==========================================\n";
        cout << "               FIGHT BEGIN!\n";
        cout << "==========================================\n";

        int round = 1;
        while (p1.isAlive() && p2.isAlive()) {
            cout << "\n--- RONDE " << round << " ---\n";
            p1.printStatus();
            p2.printStatus();
            cout << "-----------------------\n";

            // Giliran P1
            playTurn(p1, p2);
            if (!p2.isAlive()) break;

            // Giliran P2
            playTurn(p2, p1);
            if (!p1.isAlive()) break;

            // Akhir Ronde: Pemulihan Stamina
            p1.recoverStamina();
            p2.recoverStamina();
            
            round++;
        }

        cout << "\n==========================================\n";
        cout << "                    K.O!\n";
        if (p1.isAlive()) cout << "       PEMENANGNYA ADALAH: " << p1.getName() << "!\n";
        else cout << "       PEMENANGNYA ADALAH: " << p2.getName() << "!\n";
        cout << "==========================================\n";
    }
};

int main() {
    Game roboxing;
    roboxing.start();
    return 0;
}