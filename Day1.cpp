#include <bits/stdc++.h>
using namespace std;

#define fi first
#define se second
bool vis[12][12];
bool vis2[12][12];

int maxTreasure, maxMonster;

char maze[12][12] = { 
{'#','#','#', '#','#','#','#','#','#','#','#','#'},
{'#','.','.','.','.','.','.','.','.','.','.','#'},
{'#','.','.','.','.','.','.','.','.','.','.','#'},
{'#','.','.','.','.','.','.','.','.','.','.','#'},
{'#','.','.','.','.','.','.','.','.','.','.','#'},
{'#','.','.','.','.','.','.','.','.','.','.','#'},
{'#','.','.','.','.','.','.','.','.','.','.','#'},
{'#','.','.','.','.','.','.','.','.','.','.','#'},
{'#','.','.','.','.','.','.','.','.','.','.','#'},
{'#','.','.','.','.','.','.','.','.','.','.','#'},
{'#','.','.','.','.','.','.','.','.','.','.','#'},
{'#','#','#', '#','#','#','#','#','#','#','#','#'}
};
 
vector<int> stRow = {1,2,3,4,5,6,7,8,9,10}, stCol = {1,2,3,4,5,6,7,8,9,10};

void generateMaze(){
	
	for (int i = 1; i <= 10 ; i++){
		for (int j = 1; j <= 10 ; j++){
			maze[i][j] = '.';
		}
	}
	maxTreasure = 99;
	vector<pair<int, int>> move = {{1, 0},{-1,0}, {0,-1}, {0,1}};
	
	random_shuffle(stRow.begin(), stRow.end());
	random_shuffle(stCol.begin(), stCol.end());
	
	stack<pair<int, int>> st;
	st.push({stRow[0], stCol[0]});
	vis[stRow[0]][stCol[0]] = 1;
	
	while (!st.empty()){
		int row = st.top().fi;
		int col = st.top().se;
		
		st.pop();
		
		random_shuffle(move.begin(), move.end());
		for (auto a : move){
			int nextRow = row+a.fi, nextCol = col+a.se;
			if(vis[nextRow][nextCol] || maze[nextRow][nextCol] == '#') continue;
			
			bool valid = true;
			for(auto b : move){
				if(nextRow+b.fi == row && nextCol+b.se == col)continue;
				if(vis[nextRow+b.fi][nextCol+b.se]) valid = false;
			}
			
			if(valid){
				vis[nextRow][nextCol] = true;
				st.push({nextRow, nextCol});
			}
		}
	}
	
	for (int i = 0; i < 12 ; i++){
		for (int j = 0 ; j < 12; j++){
			if(!vis[i][j]) {
				maze[i][j] = '#';
				maxTreasure--;
			}
		}
	}
	
}

void start(){
	cout << "Press W S A D to move\n" << "Press E to Exit\n";
	int treasure = rand()%(maxTreasure-5)+1, row, col;
	while(treasure > 0){
		random_shuffle(stRow.begin(), stRow.end());
		random_shuffle(stCol.begin(), stCol.end());
		row = stRow[0], col = stCol[0];
		if(maze[row][col] == '#')continue;
		maze[row][col] = 'T';
		treasure--;
	}
}

class Player{
	public:		
		Player (){
			int row = stRow[0], col = stCol[0];
			while(maze[row][col] == '#'){
				random_shuffle(stRow.begin(), stRow.end());
				random_shuffle(stCol.begin(), stCol.end());
				row = stRow[0], col = stCol[0];
			}
			
			this->row = row;
			this->col = col;	
		}
		
		int score = 0;
		int row;
		int col;
		
		void move(char playerInput){
			int nextRow = row, nextCol = col;
			
			if(playerInput == 'W') nextRow--;
			else if(playerInput == 'S') nextRow++;
			else if(playerInput == 'A') nextCol--;
			else if(playerInput == 'D') nextCol++;
			else{
				cout << "Invalid\n";
				return;
			}
			if(maze[nextRow][nextCol] == '#') cout << "Invalid\n";
			else row = nextRow, col = nextCol;
			
			if (maze[row][col] == 'T'){
				score+=10;
				maze[row][col] = '.';
			}
		}
};

class Monster{
	public:
		
		Monster(){
			int row = stRow[1], col = stRow[1];
			while(maze[row][col] == '#'){
				random_shuffle(stRow.begin(), stRow.end());
				random_shuffle(stCol.begin(), stCol.end());
				row = stRow[1], col = stCol[1];
			}
			this->row = row;
			this->col = col;
		}
		int row;
		int col;	
		
		void move(){
			vector<char> c = {'W', 'S', 'A', 'D'};
			random_shuffle(c.begin(), c.end());
			
			int nextRow = row, nextCol = col;
			if(c[0] == 'W') nextRow--;
			if(c[0] == 'S') nextRow++;
			if(c[0] == 'A') nextCol--;
			if(c[0] == 'D') nextCol++;
			
			while (maze[nextRow][nextCol] == '#'){
				random_shuffle(c.begin(), c.end());	
				nextRow = row, nextCol = col;
				if(c[0] == 'W') nextRow--;
				if(c[0] == 'S') nextRow++;
				if(c[0] == 'A') nextCol--;
				if(c[0] == 'D') nextCol++;				
			}
				row = nextRow;
				col = nextCol;
		}
};

bool monsterNearby(Player player, Monster mons){
	int pRow = player.row;
	int pCol = player.col;
	int mRow = mons.row;
	int mCol = mons.col;
	
	if ((pRow == mRow && pCol == mCol)||(pRow+1 == mRow && pCol == mCol)||(pRow-1 == mRow && pCol == mCol)||(pRow == mRow && pCol+1 == mCol)||(pRow == mRow && pCol-1 == mCol)) return true;
	else return false;
}

void printMaze(Player player, Monster monster){
	for (int i = 0; i < 12 ; i++){
		for (int j = 0 ; j < 12; j++){
			if(i == monster.row && j == monster.col){
				cout << "M ";
			}
			else if(i == player.row && j == player.col){
				cout << "P ";
			}
			else cout << maze[i][j] << " ";
		}
		cout << endl;
	}	
}
int main(){
	srand(time(NULL));
	
	replay:
	generateMaze();
	start();
	
	Player player;
	Monster monster;
	while (true){
		printMaze(player, monster);
	
		char playerInput;	
		cin >> playerInput;
		
		if(playerInput == 'E') break;
		
		player.move(playerInput);
		monster.move();
		
		if(monsterNearby(player,monster)) {
			printMaze(player, monster);
			cout << "Defeat\n";
			player.score = 0;
			break;	
		}
	}	
	
	cout << "Your score : " << player.score << endl;
	cout << "Play again? (Y for yes)" << endl;
	char c;
	cin >> c;
	if(c == 'Y') goto replay;
	
	return 0;
}