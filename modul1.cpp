#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class board{
    private:
        char tic[3][3];

        public:
    board() {
        for(int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                tic[i][j] = ' ';
            }
        }
    }

    ~board(){
        cout << "\nthanks for playing\n";
    }

    void display(){
        cout << "   0 1 2\n";   
        cout << "  -------\n";
        for(int i = 0; i < 3; i++){
            cout << i << " |" << tic[i][0] << "|" << tic[i][1] << "|" << tic[i][2] << "|\n";
        cout << "  -------\n";
        }
    }

    bool isEmpty (int x, int y){
       return tic[x][y] == ' '; 
    }

    void move(int x, int y, char symbol){
        tic[x][y] = symbol;
    }

    char checkWin(){
        for(int i = 0; i < 3; i++){
            if(tic[i][0] == tic[i][1] && tic[i][1] == tic[i][2] && tic[i][0] != ' ') return tic[i][0];
            if(tic[0][i] == tic[1][i] && tic[1][i] == tic[2][i] && tic[0][i] != ' ') return tic[0][i];
            }
            if(tic[0][0] == tic[1][1] && tic[1][1] == tic[2][2] && tic[0][0] != ' ') return tic[0][0];
            if(tic[0][2] == tic[1][1] && tic[1][1] == tic[2][0] && tic[0][2] != ' ') return tic[0][2];

            return ' ';
        }

    bool isFull(){
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(tic[i][j] == ' ') 
                    return false;
                }    
        }     
     return true; 
    }
};

class player{
    protected:
        char symbol;
        int totalMoves = 0;

    public:
    player(char sym):symbol(sym) {}

    virtual ~player() = default;

    virtual void move(board &board) = 0;

    char getSymbol() const {return symbol;}

    void incrementMoves() {totalMoves++;}

    int getTotalMoves() const {return totalMoves;}

}; 

class user : public player {
    private : 
    string name;

    public : 
    user(char sym): player (sym) {
        cout << "Enter your name: ";
        cin >> name;
    }

    ~user () override{
        cout << "\n good game " << name << "\n";
        cout << "\n your total moves : " << getTotalMoves() << "\n";
    }

    void move(board &board) override {
        int x, y;
        cout << "\n" << name << "'s turn.";

        while (!(cin >> x >> y) || x < 0 || x > 2 || y < 0 || y > 2 || !board.isEmpty(x, y)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid move. Try again: ";
        }

        board.move(x, y, symbol);
        incrementMoves();
    }

    string getName() const {return name;}

};

class computer : public player {
    public:
    computer(char sym): player(sym) {}

    ~computer() override{
        cout << "\ncomputer's total  moves: " << player::getTotalMoves() << "\n";
    }

    void move(board &board) override {
       cout << "\nComputer's turn.\n";
       for(int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if(board.isEmpty(i, j)) {
                board.move(i, j, symbol);
            if (board.checkWin() == symbol){
                player::incrementMoves();
                return;
            }
            board.move(i, j, ' ');  
            }
        }
    }

    int x, y;
    do {
        x = rand() % 3;
        y = rand() % 3;
    } while (!board.isEmpty(x, y));

    board.move(x, y, symbol);
    player::incrementMoves();
    cout << "\nComputer placed " << symbol << " at (" << x << ", " << y << ").\n";
    }
};

class game {
    private:
        board gameBoard;
        user *player1;
        computer *player2;
        player *currentPlayer;

    public:
        game(){
            cout << "Welcome to Tic Tac Toe!\n";
            player1 = new user('X');
            player2 = new computer('O');
            currentPlayer = player1;
        }

        ~game(){
            delete player1;
            delete player2;
            cout << "\ngame over\n";
        }

        void play() {
            while (true) {
                gameBoard.display();
                currentPlayer->move(gameBoard);

                char winner = gameBoard.checkWin();
                if (winner != ' ') {
                    gameBoard.display();
                    cout << "\n winner : " << (winner == player1->getSymbol() ? player1->getName() : "Computer") << "!\n";
                    break;
                }
                if (gameBoard.isFull()) {
                    gameBoard.display();
                    cout << "\n It's a draw!\n";
                    break;
                }

                if (currentPlayer == player1) currentPlayer = player2;
                else currentPlayer = player1;   
            }
            
        }
};

int main() {
    srand(time(0));{
    game newGame;
    newGame.play();
    }
    cout << "\npress enter to exit...";
    cin.ignore(10000, '\n');   
    cin.get();                 
    return 0;
}