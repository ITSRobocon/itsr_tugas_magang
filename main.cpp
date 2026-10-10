#include <iostream>

using namespace std;


class game{
    public:
    game(){
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                board[i][j]='.';
            }
        }
    }

    void display(){
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                cout<<board[i][j]<<" ";
            }
            cout<<endl;
        }
    }

    void changeTile(int y, int x){
        if(numberOfMove%2==0){
            board[y][x]='X';
        }
        else{
            board[y][x]='O';
        }
        numberOfMove+=1;
    }

    void setTile(){
        int x=0 , y=0;
        cout<<"Silahkan masukkan koordinat x :";
        cin>>x;
        cout<<"Silahkan masukkan koordinat y :";
        cin>>y;
        if(board[y][x]=='.'){
            changeTile(y,x);
        }
        else{
        cout<<"Koordinat sudah dipenuhi!!\n";
        setTile();
        }
    }

    int startGame(){
        display();
        while(boardState==0){
            setTile();
            for(int i=0; i<3; i++){
                if((board[i][0]=='X'&&board[i][1]=='X'&&board[i][2]=='X') || (board[i][0]=='O'&&board[i][1]=='O'&&board[i][2]=='O')){
                    boardState=1;
                    return numberOfMove;
                }
                if((board[0][i]=='X'&&board[1][i]=='X'&&board[2][i]=='X') || (board[0][i]=='O'&&board[1][i]=='O'&&board[2][i]=='O')){
                    boardState=1;
                    return numberOfMove;
                }
            }
            if((board[0][0]=='X'&&board[1][1]=='X'&&board[2][2]=='X') || (board[0][0]=='O'&&board[1][1]=='O'&&board[2][2]=='O')){
                boardState=1;
                return numberOfMove;
            }
            int available=0;
            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    if(board[i][j]=='.'){
                        available+=1;
                    }
                }
            }
        if(available<1){
            boardState=1;
            return -1;
        }
        cout<<endl;
        display();
        }
        return numberOfMove;
    }

    private:
    char board[3][3];
    int numberOfMove=0;
    int boardState=0;
};

class player{
    public:
    string playerName;
    player(string name){
        playerName=name;
    }
};

int main(){
    int akhir;
    game board1;
    player player1("Andi");
    player player2("Budi");
    akhir=board1.startGame();
    cout<<endl;
    board1.display();
    if(akhir<0){
        cout<<"Permainan seri";
    }
    else if(akhir%2==0){
        cout<<player2.playerName<<" adalah pemenangnya!!";
    }
    else{
        cout<<player1.playerName<<" adalah pemenangnya!!";
    }
    return 0;
}
