#include <bits/stdc++.h>
#define vec vector
#define fs first
#define sc second
#define pii pair<int,int>
using namespace std;


class Game {
public:
    bool over = 0;
    int emptySlots = 9;
    vec<vec<char>> grid = {
        {' ',' ',' '},
        {' ',' ',' '},
        {' ',' ',' '}
    };


    int check(char c, int row, int col) {
        if(grid[row][0] == c && grid[row][1] == c && grid[row][2] == c) return 1;
        if(grid[0][col] == c && grid[1][col] == c && grid[2][col] == c) return 1;
        if(row == col && grid[0][0] == c && grid[1][1] == c && grid[2][2] == c) return 1;
        if(row + col == 2 && grid[0][2] == c && grid[1][1] == c && grid [2][0] == c) return 1;
        if(!emptySlots) return 2;
        return 0;
    }


    bool put(char p, int row, int col) {
        if(grid[row][col] == ' ') {grid[row][col] = p; emptySlots--; return 1;}
        return 0;
    }


    void print() {
        for(int i=0; i<3; i++) {
            cout<<"   |   |   \n";
            for(int j=0; j<3; j++) {
               
                cout<<" "<<grid[i][j]<<" ";
                if(j<2) cout<<"|";
            }
            if(i<2) cout<<endl<<"___|___|___\n";
            else cout<<"\n   |   |   \n";
        }
    }


    int menu(int num) {
        int in;
        if(num == 0) {
            cout<<"================\n";
            cout<<"TIC TAC TOE GAME\n";
            cout<<"================\n";
   
            cout<<"1. Play\n";
            cout<<"2. Exit\n";
            cin>>in;
        }
        if(num == 1) {
            cout<<"===================\n";
            cout<<"SELECT MODE TO PLAY\n";
            cout<<"===================\n";
           
            cout<<"1. Human vs Human\n";
            cout<<"2. Human vs Bot\n";
            cout<<"3. Bot vs Bot\n";
            cin>>in;
        }
        return in;
    }
};




class Player {
public:
    string playerName;
    Game* game;
    Player(string name, Game* g) : playerName(name), game(g) {}
    virtual void play(char p) = 0;
    bool playTurn(char p, int row, int col) {
        if(row >= 0 && row < 3 && col >= 0 && col < 3) {
            if(game->put(p,row,col))
            {
                game->print();
                if(int status = game->check(p, row, col)) {
                    (status == 1 ? cout<<playerName<<" has won!\n" : cout<<"It's a tie!\n" );
                    game->over = 1;
                }
                return 1;
            }
        }
        return 0;
    }
};


class Human : public Player {
public:
   
    Human(string name, Game* g) : Player(name, g) {}
    void play(char p) {
        cout<<playerName<<" is playing ("<<p<<")\n";
        while(1) {
            cout<<"Pick rows (1-3) and columns (1-3): ";
            int row,col; cin>>row>>col; row--; col--;
            if(playTurn(p, row, col)) break;
            else cout<<"Invalid play!\n";
        }
    };
};


class Bot : public Player {
public:


    Bot(string name, Game* g) : Player(name, g) {}
    void play(char p) {
        cout<<playerName<<" is playing ("<<p<<")\n";
        std::this_thread::sleep_for(1000ms);
        vec<pii> winningMoves;
        for(int i=0; i<3; i++) {
            int count = 0; pii emptyCell = {-1,-1};
            for(int j=0; j<3; j++) {
                if(game->grid[i][j] == p) count++;
                else if(game->grid[i][j] == ' ') emptyCell = {i,j};
            }
            if(emptyCell != make_pair(-1,-1) && count == 2) {
                winningMoves.push_back(emptyCell);
            }
        }
        for(int j=0; j<3; j++) {
            int count = 0; pii emptyCell = {-1,-1};
            for(int i=0; i<3; i++) {
                if(game->grid[i][j] == p) count++;
                else if(game->grid[i][j] == ' ') emptyCell = {i,j};
            }
            if(emptyCell != make_pair(-1,-1) && count == 2) {
                winningMoves.push_back(emptyCell);
            }
        }
        {
            int count = 0; pii emptyCell = {-1,-1};
            for(int i=0; i<3; i++) {
                if(game->grid[i][i] == p) count++;
                else if(game->grid[i][i] == ' ') emptyCell = {i,i};
            }
            if(emptyCell != make_pair(-1,-1) && count == 2) winningMoves.push_back(emptyCell);
        }


        {
            int count = 0; pii emptyCell = {-1,-1};
            for(int i=0; i<3; i++) {
                if(game->grid[i][2-i] == p) count++;
                else if(game->grid[i][2-i] == ' ') emptyCell = {i,2-i};
            }
            if(emptyCell != make_pair(-1,-1) && count == 2) winningMoves.push_back(emptyCell);
        }
       


        vec<pii> blockingMoves;
        char eP = (p == 'X' ? 'O' : 'X');
        for(int i=0; i<3; i++) {
            int count = 0; pii emptyCell = {-1,-1};
            for(int j=0; j<3; j++) {
                if(game->grid[i][j] == eP) count++;
                else if(game->grid[i][j] == ' ') emptyCell = {i,j};
            }
            if(emptyCell != make_pair(-1,-1) && count == 2) {
                blockingMoves.push_back(emptyCell);
            }
        }
        for(int j=0; j<3; j++) {
            int count = 0; pii emptyCell = {-1,-1};
            for(int i=0; i<3; i++) {
                if(game->grid[i][j] == eP) count++;
                else if(game->grid[i][j] == ' ') emptyCell = {i,j};
            }
            if(emptyCell != make_pair(-1,-1) && count == 2) {
                blockingMoves.push_back(emptyCell);
            }
        }
        {
            int count = 0; pii emptyCell = {-1,-1};
            for(int i=0; i<3; i++) {
                if(game->grid[i][i] == eP) count++;
                else if(game->grid[i][i] == ' ') emptyCell = {i,i};
            }
            if(emptyCell != make_pair(-1,-1) && count == 2) blockingMoves.push_back(emptyCell);
        }
        {
            int count = 0; pii emptyCell = {-1,-1};
            for(int i=0; i<3; i++) {
                if(game->grid[i][2-i] == eP) count++;
                else if(game->grid[i][2-i] == ' ') emptyCell = {i,2-i};
            }
            if(emptyCell != make_pair(-1,-1) && count == 2) blockingMoves.push_back(emptyCell);
        }


        vec<pii>* opts = nullptr;
        if(!winningMoves.empty()) opts = &winningMoves;
        else if(!blockingMoves.empty()) opts = &blockingMoves;


        if(opts) {
            pii m = (*opts)[rand() % opts->size()];
            playTurn(p, m.fs, m.sc);
            return;
        }
        vec<pii> emptyCells;
        for(int i=0; i<3; i++) {
            for(int j=0; j<3; j++) {
                if(game->grid[i][j] == ' ') emptyCells.push_back({i,j});
            }
        }
        pii m = emptyCells[rand() % emptyCells.size()];
        playTurn(p, m.fs, m.sc);
    }
};




int main() {
    srand(time(0));
    while(1) {        
        Game game;
        Human human1("Player 1", &game);
        Human human2("Player 2", &game);
        Bot bot1("Bot 1", &game);
        Bot bot2("Bot 2", &game);
        int opt;
        if(opt = game.menu(0) == 2) break;
        else{
            opt = game.menu(1);
        }
       
        vec<Player*> players = {&human1, &bot1};
        switch (opt){
        case 1: players = {&human1, &human2}; break;
        case 2: players = {&human1, &bot1}; break;
        case 3: players = {&bot1, &bot2}; break;
        default: players = {&human1, &human2}; break;
        }
       
        vec<char> pieces = {'X', 'O'};
        game.print();
        int turn = 0;
        while(!game.over) {
            turn%=2;
            players[turn]->play(pieces[turn]);
            turn++;
        }
    }
}
