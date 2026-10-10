#include <iostream>
#include <string>
using namespace std;

class Character {
    private:
    protected:
    
    
    public:
    int hitpoint;
    string name;
    Character(string nam, int hp) {
        hitpoint = hp;
        name = nam;
    }
    
    virtual  void takedmg(int n) {
        
        hitpoint -= n;
    }
    virtual  void attack(Character &n) {
        n.takedmg(10);
    }
    void heal(int n) {
        hitpoint += n;
    }
};
class mage : public Character {
    
    public:
    using Character :: Character;
    void attack(Character &n) override {
        n.takedmg(n.hitpoint / 2 + ((10 - n.hitpoint / 2) < 0 ? 0 : (10 - n.hitpoint / 2)));
        heal(n.hitpoint / 4);
    }
};
class tank : public Character {
    public:
    using Character :: Character;
    void takedmg(int n) override {
        hitpoint = hitpoint - n + 5;
    }
};
class rouge : public Character {
    private:
    bool dodge = 0;
    public:
    using Character :: Character;

    void takedmg(int n) override {
        
        if(dodge == 1) {
            
            dodge = !dodge ;
        } else {
            hitpoint -= n;
            dodge = !dodge;
        }
    }
};

class player {
    private:
    int maxhp = 30;
    Character &cplayer;
    int lives = 3;
    int exp = 0;
    int level = 0;
    public:
    player(Character* n) : cplayer(*n) {}
    
    void battle(Character &enemy) {
        while(cplayer.hitpoint > 0 && enemy.hitpoint > 0) {
            cplayer.attack(enemy);
            enemy.attack(cplayer);
            cout << cplayer.name<< " Hp: " << cplayer.hitpoint << endl;
            cout << enemy.name << " Hp: " << enemy.hitpoint << endl;
        }
        if (cplayer.hitpoint <= 0) {
            lives -= 1;
            cout << "You Died Lives -1 Lives remaining: " << lives << endl;
            
        } else {
            cout << "you won" << endl;
        }
        healhp(maxhp - cplayer.hitpoint);
    }
    int getlive() {
        int x;
        x = lives;
        return x;
    }
    int gethp() {
        int x;
        x = cplayer.hitpoint;
        return x;
    }int healhp(int x) {
        cplayer.heal(x);
        return x;
    }
    void checkexp() {
        if(exp / 10 > level) {
            level += 1;
            maxhp += 10;
        }
    }
    void expup(int x) {
        exp += x;
        checkexp();
    }
    void status() {
        healhp(maxhp - cplayer.hitpoint);
        cout << "exp: " << exp << " level: " << level << endl;
        cout << "Current lives = " << lives << "Current hp = " << cplayer.hitpoint << endl;
        if(lives == 0) {
            cout << "game over" << endl;
        }
    }
};

void battle(Character &n, Character&m) {
    while (n.hitpoint > 0 && m.hitpoint > 0) {
        n.attack(m);
        m.attack(n);
        cout << n.name<< " Hp: " << n.hitpoint << endl;
        cout << m.name << " Hp: " << m.hitpoint << endl;
    }
    
}
Character* chooseclass(string n) {
    Character* c;
    int cc = 4;
    while(true) {

    
    cout << "choose your starting class" << endl;
    cout << "type 1 for mage to deal high damage against high hp enemy" << endl;
    cout << "type 2 for tank to reduce incoming damage" << endl;
    cout << "type 3 for rouge to dodge every other attack" << endl;
    cin >> cc;
    if(cc == 1) {
        
        c = new mage(n, 30);
        
        return c;
    }
    if(cc == 2) {
        c = new tank(n, 30);
        return c;
    }
    if(cc == 3) {
        c = new rouge(n, 30);
        return c;
    }
    cout << "Invalid choice try again." << endl;
    }
}
class stage {
    public:
    stage(player &p) {
        
    }
    protected:
    int choice;
};
class train : public stage {
    public:
    train(player& p) : stage(p) {
        cout << "while adventuring you get a little break should you train?" << endl;
        cout << "type 1 for yes" << endl;
        cin >> choice;
        if (choice == 1) {
        cout << "you decided to train before continuing your adventure exp up" << endl;
        p.expup(10);
        p.status();
        } else {
            cout << "you lie down and doomscroll for a while" << endl;
        }
    }

};
class Battle : public stage {
    public:
    Battle(player& p, Character& e) : stage(p) {
        cout << "you see a strange person from a far, should you get closer?" << endl;
        cout << "type 1 for yes" << endl;
        cin >> choice;
        if (choice == 1) {
        cout << "you met an enemy while adventuring battle commences" << endl;
        p.battle(e);
        p.expup(5);
        p.status();
         } else {
            cout << "you decided to avoid that person" << endl;
        }
    }

};
int main() {
    string name;
    cout << "Type in your name : " ;
    cin >> name;
    Character* c; 
    c = chooseclass(name);
    player p1(c);
    Character weakmonster("kalkulus", 20);
    tank wowo("wowo", 50);
    train dojo(p1);
    Battle box(p1, weakmonster);
    train belajar(p1);
    Battle bossbattle(p1, wowo);
    cout << "game over" << endl;
    delete c;
}