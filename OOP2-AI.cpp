#include <iostream>
#include <string>
#include <unordered_set>
using namespace std;

class TicTacToe {
	private:
		string grid[3] = {"...", "...", "..."};
		bool gameStatus = false;
		bool winStatus;
		int filled_cells = 0;
	
	public:
		//Initialize the game, print the instructions
		void TutorialGame() {
			cout << "Welcome to the game TicTacToe" << endl;
			cout << "The grid would be represented like this: " << endl;
			cout << "|   0   |   1   |   2   |" << endl;
			cout << "|   3   |   4   |   5   |" << endl;
			cout << "|   6   |   7   |   8   |" << endl;
			cout << "The player's input would be a number between 0-8." << endl;
			cout << "This game will be playable for 2 players and an AI mode" << endl;
			cout << "Please enjoy the game !" << endl;
			cout << endl;
		}
		//Change the game status to true and reveal the players names
		void StartGame(string player1, string player2) {
		    cout << "Player 1 = " << player1 << "    Player 2 = " << player2 << endl;
		    cout << "The game begins now!" << endl;
		    cout << endl;
			gameStatus = true;
		}
		//Method to fill the grid, returns a true/false statement if cell is filled or not
		bool fillGrid(int row_coor, int col_coor, char content) {
			if (grid[row_coor][col_coor] == '.') {
			    grid[row_coor][col_coor] = content;
			    filled_cells++;
			    return true;
		    } return false;
		}
		//Print the grid, self-explanatory
		void gridPrint() {
			cout << "Current grid is as follows: " << endl;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					cout << " " << grid[i][j] << " ";
				}
				cout << endl;
			}
		}
		//Check if the grid is full or one player wins
		void gridCheck(int row, int col) {
			if (
			(
			((grid[row][0] == grid[row][1]) &&  (grid[row][1] == grid[row][2])) ||
			((grid[0][col] == grid[1][col]) &&  (grid[1][col] == grid[2][col])) &&
			(grid[row][col] != '.')
			) ||
			(
			((grid[0][0] == grid[1][1]) && (grid[1][1] == grid[2][2])) ||
			((grid[0][2] == grid[1][1]) && (grid[1][1] == grid[2][0])) &&
			(grid[1][1] != '.')
			)
			) {
			    gameStatus = false;
			    winStatus = true;
			}
			else if (filled_cells == 9) {
				gameStatus = false;
				winStatus = false;
			}
			else {
				gameStatus = true;
			}
		}
		bool statusOut() {return gameStatus;}
		bool winstatOut() {return winStatus;}
		char cell_out(int spot) const {
			return grid[spot/3][spot%3];
		}
		//Log messages when the game ends
		void endGame() {
			if (!gameStatus && winStatus) {
				cout << "Game has ended!" << endl;
			} else if (!gameStatus) {
				cout << "Game ended by draw, there are no more avaiable cells!" << endl;
			} else {
				cout << "Game has not ended yet!" << endl;
			}
		}
		
		int gamemode_choose() {
			int gamemode;
			if (!gameStatus) {
				cout << "You can now choose the gamemode, enter 0 for regular 2-Player mode, enter 1 for Player vs AI mode." << endl;
				cout << "Enter your number here (0-1) : ";
				cin >> gamemode;
				if (gamemode == 0) {
					cout << "You choose regular mode ! " << endl << endl;
				} else {
					cout << "You choose AI mode ! " << endl << endl;
				}
				return gamemode;
			} else {
				cout << "The gamemode cannot be changed as long as the game is still going!" << endl;
				return 2;
			}
		} 
};

class Player {
	protected:
		string player_name;
		int player_num;
		string symbol = "XO";
		int rawInput;
		
	public:
		//Constructor
		Player(string nameInput, int number) {
	        player_name = nameInput;
		    player_num = number;
		}
		string name_out() {return player_name;}
		//The symbol accessed from here
		char filler() {return symbol[player_num];}
		//Receive input (0-8)
		void receive_input(int intInput) {rawInput = intInput;}
		//Print the winner
		void winner(bool exist) {
			if (exist) {
				cout << "The winner is Player " << player_num+1;
			    cout << " with the name " << player_name << endl; 
			}
		}
		int row_convert() {return rawInput/3;}
		int col_convert() {return rawInput%3;}
};

class Bot : public Player {
	private :
		bool has_played;
		unordered_set<int> recorded_spots;
		
	public:
		using Player::Player;

		void record_move (int move) {
			recorded_spots.insert(move);
		}
		
		int decision_maker(const TicTacToe& game_scene) {
			int winning_lines[8][3] = {
				{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
				{0, 3, 6}, {1, 4, 7}, {2, 5, 8},
				{0, 4, 8}, {2, 4, 6}
			};
			char bot_symbol = 'O';
			char player_symbol = 'X';

			for (int i = 0; i < 8; i++) {
				int empty_spot = -1;
				int bot_spots = 0;
				for (int j = 0; j < 3; j++) {
					char cell = game_scene.cell_out(winning_lines[i][j]);
					if (cell == '.') {
						empty_spot = winning_lines[i][j];
					} else if (cell == bot_symbol) {
						bot_spots++;
					}
				}
				if (bot_spots == 2 && empty_spot != -1) {
					return empty_spot;
				}
			}

			for (int i = 0; i < 8; i++) {
				int empty_spot = -1;
				int player_spots = 0;
				for (int j = 0; j < 3; j++) {
					char cell = game_scene.cell_out(winning_lines[i][j]);
					if (cell == '.') {
						empty_spot = winning_lines[i][j];
					} else if (cell == player_symbol) {
						player_spots++;
					}
				}
				if (player_spots == 2 && empty_spot != -1) {
					return empty_spot;
				}
			}

			int preferred_spots[9] = {4, 0, 2, 6, 8, 1, 3, 5, 7};
			for (int i = 0; i < 9; i++) {
				if (game_scene.cell_out(preferred_spots[i]) == '.') {
					return preferred_spots[i];
				}
			}

			return -1;
		}
};

void gameplay_reg (TicTacToe game_scene) {
	int temp, turn = 0;
	string name;
	
	cout << "Please enter Player 1 (P1)" << endl;
	cin >> name;
	Player p1(name, 0);
	cout << "Please enter Player 2 (P2)" << endl;
	cin >> name;
	Player p2(name, 1);
	cout << endl;
	game_scene.StartGame(p1.name_out(), p2.name_out());
	
	bool cell_filled;
	while(game_scene.statusOut()) {
		cout << "Player " << turn%2 +1 << "'s Turn, please enter the grid (0-8)" << endl;
		cin >> temp;
		cout << endl;
		if (turn%2 == 0) {
			p1.receive_input(temp);
			cell_filled = game_scene.fillGrid(p1.row_convert(), p1.col_convert(), p1.filler());
			while (!cell_filled) {
				cout << "This cell is already filled!, please pick another cell!" << endl << endl;
				cin >> temp;
				p1.receive_input(temp);
				cell_filled = game_scene.fillGrid(p1.row_convert(), p1.col_convert(), p1.filler());
			}
		    game_scene.gridPrint();
		    game_scene.gridCheck(p1.row_convert(), p1.col_convert());
		} else {
			p2.receive_input(temp);
			cell_filled = game_scene.fillGrid(p2.row_convert(), p2.col_convert(), p2.filler());
		    while (!cell_filled) {
				cout << "This cell is already filled!, please pick another cell!" << endl << endl;
				cin >> temp;
				p2.receive_input(temp);
				cell_filled = game_scene.fillGrid(p2.row_convert(), p2.col_convert(), p2.filler());
			}
		    game_scene.gridPrint();
		    game_scene.gridCheck(p2.row_convert(), p2.col_convert());
		}
		turn++;
	}
	game_scene.endGame();
	if (turn%2) {
		p1.winner(game_scene.winstatOut());
	} else {
		p2.winner(game_scene.winstatOut());
	}
}

void gameplay_ai (TicTacToe game_scene) {
	int temp, turn = 0;
	string name;
	
	cout << "Please enter Player 1 (P1)" << endl;
	cin >> name;
	Player p1(name, 0);
	Bot bot("BotAI", 1);
	cout << endl;
	game_scene.StartGame(p1.name_out(), bot.name_out());
	
	bool cell_filled;
	while(game_scene.statusOut()) {
		cout << "Player " << turn%2 +1 << "'s Turn" << endl;
		if (turn%2 == 0) {
			cout << "Please enter the grid (0-8)" << endl;
			cin >> temp;
			cout << endl;
			p1.receive_input(temp);
			cell_filled = game_scene.fillGrid(p1.row_convert(), p1.col_convert(), p1.filler());
			while (!cell_filled) {
				cout << "This cell is already filled!, please pick another cell!" << endl << endl;
				cin >> temp;
				p1.receive_input(temp);
				cell_filled = game_scene.fillGrid(p1.row_convert(), p1.col_convert(), p1.filler());
			}
			game_scene.gridPrint();
			game_scene.gridCheck(p1.row_convert(), p1.col_convert());
		} else {
			temp = bot.decision_maker(game_scene);
			bot.receive_input(temp);
			bot.record_move(temp);
			game_scene.fillGrid(bot.row_convert(), bot.col_convert(), bot.filler());
			cout << "The AI chose cell " << temp << "." << endl;
			game_scene.gridPrint();
			game_scene.gridCheck(bot.row_convert(), bot.col_convert());
		}
		turn++;
	}
	game_scene.endGame();
	if (turn%2) {
		p1.winner(game_scene.winstatOut());
	} else {
		bot.winner(game_scene.winstatOut());
	}
}

int main() {
	TicTacToe game;
	game.TutorialGame();
	
	int main_gm = game.gamemode_choose();
	
	if (main_gm == 0) {
		gameplay_reg(game);
	} else {
		gameplay_ai(game);
	}
	
	return 0;
}
