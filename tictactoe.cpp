#include <iostream>
#include "aiOpponent.cpp"
using namespace std;

const char playerX = 'X';
const char playerO = 'O';
//char board[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
//Apperently if you include the aiOpponent.cpp file, it will also include the board array. shet
int win = 0; //0==no winner, 1==X wins, 2==O wins

void wincon(){
    // Check rows for winner
    for(int i = 0; i < 3; i++){
        if(board[i][0] == playerX && board[i][1] == playerX && board[i][2] == playerX){
            win = 1;
            return;
        }
        if(board[i][0] == playerO && board[i][1] == playerO && board[i][2] == playerO){
            win = 2;
            return;
        }
    }
    // Check columns for winner
    for(int i = 0; i < 3; i++){
        if(board[0][i] == playerX && board[1][i] == playerX && board[2][i] == playerX){
            win = 1;
            return;
        }
        if(board[0][i] == playerO && board[1][i] == playerO && board[2][i] == playerO){
            win = 2;
            return;
        }
    }
    // Check diagonals for winner
    if(board[0][0] == playerX && board[1][1] == playerX && board[2][2] == playerX){
        win = 1;
        return;
    }
    if(board[0][0] == playerO && board[1][1] == playerO && board[2][2] == playerO){
        win = 2;
        return;
    }
    if(board[0][2] == playerX && board[1][1] == playerX && board[2][0] == playerX){
        win = 1;
        return;
    }
    if(board[0][2] == playerO && board[1][1] == playerO && board[2][0] == playerO){
        win = 2;
        return;
    }
}

int noOverwrite(int row, int col){
    if(board[row][col] == ' '){
        return 0; // Move successful
    } else {
        cout << "Cell already occupied! Try again." << endl;
        return 1; // Move failed
    }
}

int mode(){
    cout << "Please select mode: type '1' for Player vs Player, type '2' for Player vs AI" << endl;
    int choice;
    cin >> choice;
    return choice;
}

int main(){
    cout<<"Welcome to Tic Tac Toe!"<<endl;
    int gamemode = mode();
    if(gamemode == 1){
        cout<<"How to play: Enter the row (a-c) and column (1-3) to place your mark."<<endl;
        int turnOrder = 0; //0==X, 1==O
        char input[4];//first char is row represented with alphabet, second char is column represented with number
        while(win == 0){
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                cout<< board[i][j];
            }
            cout << endl;
        }
        if(turnOrder == 0){
            cout << "Player X's turn" << endl;
            cin >> input; // Example input for player X
            if(input[0] < 'a' || input[0] > 'c' || input[1] < '1' || input[1] > '3'){
                cout << "Invalid input! Please enter a row (a-c) and column (1-3)." << endl;
                continue; // Skip to the next iteration if input is invalid
            }
            if(noOverwrite(input[0]-'a', input[1]-'1')){continue;} // Update the board with player X's move
            board[input[0]-'a'][input[1]-'1'] = playerX; // Update the board with player X's move
            turnOrder = 1; // Flip to player O
        } else {
            cout << "Player O's turn" << endl;
            cin >> input; // Example input for player O
            if(input[0] < 'a' || input[0] > 'c' || input[1] < '1' || input[1] > '3'){
                cout << "Invalid input! Please enter a row (a-c) and column (1-3)." << endl;
                continue; // Skip to the next iteration if input is invalid
            }
            if(noOverwrite(input[0]-'a', input[1]-'1')){continue;} // Update the board with player O's move
            board[input[0]-'a'][input[1]-'1'] = playerO; // Update the board with player O's move 
            turnOrder = 0; //Flip to player X
        }
        wincon(); // Check for a winner after each move
        }
        if(win == 1){
            cout << "Player X wins!" << endl;
        } else if(win == 2){
            cout << "Player O wins!" << endl;
        } else {
            cout << "It's a draw!" << endl;
        }
    }
    else if(gamemode == 2){
        cout<<"How to play: Enter the row (a-c) and column (1-3) to place your mark."<<endl;
        int turnOrder = 0; //0==X, 1==O
        char input[4];//first char is row represented with alphabet, second char is column represented with number
        while(win == 0){
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                cout<< board[i][j];
            }
            cout << endl;
        }
        if(turnOrder == 0){
            cout << "Player X's turn" << endl;
            cin >> input; // Example input for player X
            if(input[0] < 'a' || input[0] > 'c' || input[1] < '1' || input[1] > '3'){
                cout << "Invalid input! Please enter a row (a-c) and column (1-3)." << endl;
                continue; // Skip to the next iteration if input is invalid
            }
            if(noOverwrite(input[0]-'a', input[1]-'1')){continue;} // Update the board with player X's move
            board[input[0]-'a'][input[1]-'1'] = playerX; // Update the board with player X's move
            turnOrder = 1; // Flip to AI
        } else {
            cout << "AI's turn" << endl;
            aiMove(); // Call the AI move function from aiOpponent.cpp
            turnOrder = 0; //Flip to player X
        }
        wincon(); // Check for a winner after each move
        }
        if(win == 1){
            cout << "You wins!" << endl;
        } else if(win == 2){
            cout << "AI wins!" << endl;
        } else {
            cout << "It's a draw!" << endl;
        }
    }
    return 0;
}