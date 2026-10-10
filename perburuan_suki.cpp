#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <memory>
using namespace std;

//Perburuan Suki Liar

//modul random
static mt19937& gen(){
    static mt19937 rng(random_device{}());
    return rng;
}
int randint(int min, int max){
    uniform_int_distribution<int> dis(min,max);
    return dis(gen());
}

//fungsi weighted pick
int weighted_pick(vector<int> w){
    int total = 0;
    for (int x:w) total += x;
    int r = randint(1,total);
    for (int i=0;i<w.size();i++){
        r-=w[i];
        if (r<=0) return i;
    }
    return 0;
}

//template buat entity-nya~
class entity{
    protected:
    string name;
    int current_hp, max_hp, atk, def;
    bool defend = false;

    public:
    entity(string n, int a, int d, int h){
        name = n;
        atk = a;
        def = d;
        current_hp = h;
        max_hp = h;
    }
    virtual ~entity(){}

    //fungsi getter
    bool is_alive() const{
        return current_hp > 0;
    }
    bool set_defend(bool d){
        return defend = d;
    }
    int get_hp() const{
        return current_hp;
    }
    int get_max_hp() const{
        return max_hp;
    }
    string get_name() const{
        return name;
    }

    void terima_doksli(int dmg){
        dmg = max(1,dmg-def);
        if(defend){dmg/=2;}
        current_hp -= dmg;
        current_hp = max(0,current_hp);
        cout<<' '<<name<<" menerima doksli sebesar: "<<dmg<<endl;
    }

    void cuci_mata(int heal_amount){
        current_hp = min(max_hp,current_hp+heal_amount);
        cout<<' '<<name<<" memulihkan "<<heal_amount<<" hp"<<endl;
    }

    virtual void kasi_doksli(entity& target){
        int damage = atk + randint(-2,3);
        cout<<' '<<name<<" ngasi doksli sebanyak: "<<damage<<" doksli";
        cout<<((damage>35)?" nguawor banyak banget lho cik😅":" woilah😅")<<endl;
        target.terima_doksli(damage);
    }

    virtual void end_turn(){
        defend = false;
    }
};

class prajurit_rongawi:public entity{
    protected:
    int obat_cuci_mata = 3;
    int cooldown = 0;
    public:
    prajurit_rongawi(string n):entity(n,35,10,200){}

    bool pake_obat(){
        if(obat_cuci_mata == 0){
            cout<<"Wah obatnya habis wok, kuat-kuatin dah 😅"<<endl;
            return false;
        }
        if(cooldown > 0){
            cout<<"Matamu masih perih wir, tunggu "<<cooldown<<" giliran lagi😖"<<endl;
            return false;
        }
        obat_cuci_mata -= 1;
        cuci_mata(30);
        cooldown = 2;
        return true;
    }
    int get_obat() const{
        return obat_cuci_mata;
    }

    void end_turn() override{
        entity::end_turn();
        if(cooldown > 0) cooldown--;
    }
};

enum act {doksliin_wir, bertahan};

class suki_liar:public entity{
    public:
    suki_liar(string n, int a, int d, int h):entity(n,a,d,h){}

    virtual act decide(const entity& player){
        double myHP_percentage = current_hp*1.0/max_hp;
        double enemyHP_percentage = player.get_hp()*1.0/player.get_max_hp();
        int wA = 50, wD = 10;
        if (myHP_percentage < 0.3) wD += 50;
        if (enemyHP_percentage < 0.25) wA += 20;
        return (act)weighted_pick({wA,wD});
    }

    void action(entity& p){
        act aku_nak_aksi = decide(p);
        if(aku_nak_aksi == doksliin_wir) kasi_doksli(p);
        else if (aku_nak_aksi == bertahan){
            defend = true;
            cout<<' '<<name<<" bertahan dari doksli"<<endl;
        }
    }
};

class game{
    unique_ptr<prajurit_rongawi> player;

    void character_setup(){
        cout<<">>> PERBURUAN SUKI LIAR <<<"<<endl;
        cout<<"Kasi nama jagoanlu wir 😝: ";
        string s;getline(cin,s);

        player = make_unique<prajurit_rongawi>(s);
    }

    bool player_turn(suki_liar& suki){
        cout<<"1. Kasi doksli\n2. Bertahan dari doksli\n3. Cuci mata\n> ";
        int c;
        if(!(cin>>c)){
            cin.clear();
            cin.ignore(10000,'\n');
            cout<<"Pilih yang bener wir😤"<<endl;
            return false;
        }
        switch(c){
            case 1: player->kasi_doksli(suki); return true;
            case 2: player->set_defend(true); cout<<"Kamu bertahan, bersiaplah😨"<<endl; return true;
            case 3: return player->pake_obat();
            default: cout<<"Pilih yang bener wir😤"<<endl; return false;
        }
    }

    void doksing(const suki_liar& suki){
        cout<<"\n---------------------------\n";
        cout<<player->get_name()<<" HP: "<<player->get_hp()<<'/'<<player->get_max_hp()<<" | obat mata: "<< player->get_obat()<<endl;
        cout<<suki.get_name()<<" HP: "<<suki.get_hp()<<'/'<<suki.get_max_hp()<<endl;
        cout<<"---------------------------\n";
    }

    bool battle(suki_liar& suki){
        cout<<endl<<"Welahdalah, seekor suki liar bernama "<<suki.get_name()<<" muncul cik😨, takutnyoo😨"<<endl;
        while(player->is_alive() && suki.is_alive()){
            doksing(suki);
            if(!player_turn(suki)){
                continue;
            }
            if(!suki.is_alive()) break;
            suki.end_turn();
            suki.action(*player);
            player->end_turn();
        }
        return player->is_alive();
    }
    
    public:
    void start(){
        character_setup();
        vector<unique_ptr<suki_liar>> stage;
        stage.push_back(make_unique<suki_liar>("Koboi_Fesnuk",50,5,80));
        stage.push_back(make_unique<suki_liar>("Admin_Suki",30,20,120));

        for(auto& s : stage){
            if(!battle(*s)){
                cout<<"\n"<<player->get_name()<<" tewas dihitamkan, GAME OVER, woilah NT wir😭"<<endl;
                return;
            }
            cout<<"\n"<<s->get_name()<<" suki liar berhasil dihitamkan, mantap wir😎"<<endl;
            player->cuci_mata(40);
        }
        cout<<"\nSELAMAT! "<<player->get_name()<<" menamatkan Perburuan Suki Liar🎉 cihuyyyy"<<endl;
    }
};

int main(){
    game g;
    g.start();
    return 0;
}
