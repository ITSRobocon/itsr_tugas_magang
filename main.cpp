#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>
#include "keyboard.cpp"

using namespace std;

struct Position {
    int x;
    int y;
};

class Peta {
    private :
        int height;
        int width;
        vector<string> grid;

    public :
        Peta(int height, int width) {
            this->height = height;
            this->width = width;

            this->grid = vector<string>(height, string(width, ' '));

            for (int i = 0; i < width;i++) {
                this->grid[0][i] = '#';
            }

            for (int i = 0; i < height;i++) {
                this->grid[i][0] = '#';
                this->grid[i][width - 1] = '#';
            }

            for (int i = 0; i < width;i++) {
                this->grid[height - 1][i] = '#';
            }

            int jumlah_obstacle = 35; 
            int dibuat = 0;

            while (dibuat < jumlah_obstacle) {
                int rand_y = rand() % (height - 2) + 1;
                int rand_x = rand() % (width - 2) + 1;

                if (this->grid[rand_y][rand_x] == ' ') {
                    this->grid[rand_y][rand_x] = '#';
                    dibuat++;
                }
            }
        }

        bool is_wall(int x, int y) {
            if (x < 0 || x >= width)
            return true;

            if (y < 0 || y >= height)
                return true;

            return grid[y][x] == '#';
        }

        int getWidth() {
        return width;
        }

        int getHeight() {
            return height;
        }

        vector<string> getGrid() {
            return grid;
        }

        void drawBase() {

            for (int y = 0; y < height; y++) {
                cout << grid[y] << endl;
            }
        }
};


class Player {
    private :
        Position position;
    
        public :
            void gacha_position(int peta_height, int peta_width, vector<string> grid) {
                while(true) {
                    int angka_acak_y = rand() % (peta_height - 1);
                    int angka_acak_x = rand() % (peta_width - 1);
    
                    if(grid[angka_acak_y][angka_acak_x] != '#') {
                        this->position.x = angka_acak_x;
                        this->position.y = angka_acak_y;
                        break;
                    }
                }

                cout << "player posisi" << this->position.x;
            }

            void move(char arah, Peta& map) {

                int newX = position.x;
                int newY = position.y;

                if (arah == 'U')
                    newY--;

                else if (arah == 'D')
                    newY++;

                else if (arah == 'L')
                    newX--;

                else if (arah == 'R')
                    newX++;

                if (!map.is_wall(newX, newY)) {
                    position.x = newX;
                    position.y = newY;
                }
            }

            Position get_position() {
                return this->position;
            } 
};

class CariJalanTercepat {
    public :
        Position cari_jalur(Position start, Position target, Peta& peta) {
            queue<Position> antrian;

            vector<vector<bool>> visited (
                peta.getHeight(),
                vector<bool> (peta.getWidth(), false)
            );

            vector<vector<Position>> parent (
                peta.getHeight(),
                vector<Position> (peta.getWidth(),{-1,-1})
            );

            antrian.push(start);

            visited[start.y][start.x] = true;

            int dx[4] = {1, -1, 0, 0};
            int dy[4] = {0, 0, 1, -1};

            while (!antrian.empty()) {

                Position current = antrian.front();
                antrian.pop();

                if (
                    current.x == target.x &&
                    current.y == target.y
                ) {
                    break;
                }

                for (int i = 0; i < 4; i++) {

                    int newX = current.x + dx[i];
                    int newY = current.y + dy[i];

                    if (
                        !peta.is_wall(newX, newY) &&
                        !visited[newY][newX]
                    ) {

                        visited[newY][newX] = true;

                        parent[newY][newX] = current;

                        antrian.push({newX, newY});
                    }
                }
            }

            if (!visited[target.y][target.x]) {
                return {};
            }

            vector<Position> path;

            Position current = target;

            while (
                current.x != start.x ||
                current.y != start.y
            ) {

                path.push_back(current);

                current = parent[current.y][current.x];
            }

            
            reverse(path.begin(), path.end());
            
            if (path.empty()) {
                return start;
            }

            return path[0];
        }
};

class Musuh {
    protected :
        Position position;
    
    public :
        void gacha_position(int peta_height, int peta_width, vector<string> grid) {
                while(true) {
                    int angka_acak_y = rand() % (peta_height - 1);
                    int angka_acak_x = rand() % (peta_width - 1);
    
                    if(grid[angka_acak_y][angka_acak_x] != '#') {
                        this->position.x = angka_acak_x;
                        this->position.y = angka_acak_y;
                        break;
                    }
                }
            }
        
        Position get_position() {
            return this->position;
        }
};

class Pemburu : public Musuh{
    private:
        CariJalanTercepat cari_jalan;

    public :
       void move(Player& player, Peta& peta) {
            Position next_step = cari_jalan.cari_jalur(this->position,player.get_position(),peta);

            this->position = next_step;
       } 
};

class Game {
    private :
        Peta peta;

        Player player;

        Pemburu hunter1;
        Pemburu hunter2;
        Pemburu hunter3; 
        Pemburu hunter4;

        Keyboard keyboard;

        bool gameOver;

        chrono::steady_clock::time_point lastEnemyMove;
        chrono::steady_clock::time_point lastRender;
    
    public :
        Game(int height, int width) : peta(height, width) {
            player.gacha_position(peta.getHeight(), peta.getWidth(), peta.getGrid());
            hunter1.gacha_position(peta.getHeight(), peta.getWidth(), peta.getGrid());
            hunter2.gacha_position(peta.getHeight(), peta.getWidth(), peta.getGrid());
            hunter3.gacha_position(peta.getHeight(), peta.getWidth(), peta.getGrid());
            hunter4.gacha_position(peta.getHeight(), peta.getWidth(), peta.getGrid());

            gameOver = false;

            lastEnemyMove = chrono::steady_clock::now();
            lastRender = chrono::steady_clock::now();
        }

        void clearScreen() {
            cout << "\033[3J\033[2J\033[H";
        }   

        bool checkCollision() {
            Position p = player.get_position();

            Position h1 = hunter1.get_position();
            Position h2 = hunter2.get_position();
            Position h3 = hunter3.get_position();
            Position h4 = hunter4.get_position();

            if (p.x == h1.x && p.y == h1.y) return true;
            if (p.x == h2.x && p.y == h2.y) return true;
            if (p.x == h3.x && p.y == h3.y) return true;
            if (p.x == h4.x && p.y == h4.y) return true;

            return false;
        }

        void draw() {
            clearScreen();

            vector<string> screen(
                peta.getHeight(),
                string(peta.getWidth(), ' ')
            );

            for (int y = 0; y < peta.getHeight(); y++) {
                for (int x = 0; x < peta.getWidth(); x++) {
                    if (peta.is_wall(x, y))
                        screen[y][x] = '#';
                }
            }

            Position p = player.get_position();
            screen[p.y][p.x] = 'P';

            Position h1 = hunter1.get_position();
            screen[h1.y][h1.x] = 'H';

            Position h2 = hunter2.get_position();
            screen[h2.y][h2.x] = 'H';

            Position h3 = hunter3.get_position();
            screen[h3.y][h3.x] = 'H';            

            Position h4 = hunter4.get_position();
            screen[h4.y][h4.x] = 'H';             

            for (int y = 0; y < peta.getHeight(); y++) {
                cout << screen[y] << endl;
            }

            cout << endl;
            cout << "P = Player" << endl;
            cout << "H = Hunter" << endl;
            cout << "# = Wall" << endl;
            cout << "Arrow Key = Move" << endl;
            cout << "Q = Quit" << endl;
        }

        void start() {
            keyboard.start();

            bool running = true;
            auto gameStart = chrono::steady_clock::now();

            while (running && !gameOver) {
                auto now = chrono::steady_clock::now();

                auto elapsed = chrono::duration_cast<chrono::seconds>(now - gameStart).count();
                int remaining = 180 - elapsed;

                if (remaining <= 0) {
                    gameOver = true;
                    break;
                }

                char key = keyboard.getKey();

                if (key == 'U' || key == 'D' || key == 'L' || key == 'R') {
                    player.move(key, peta);
                }

                if (key == 'q' || key == 'Q') {
                    running = false;
                    break;
                }

                auto enemyElapsed = chrono::duration_cast<chrono::milliseconds>(now - lastEnemyMove).count();

                if (enemyElapsed >= 500) {
                    hunter1.move(player, peta);
                    hunter2.move(player, peta);
                    hunter3.move(player, peta); 
                    hunter4.move(player, peta); 

                    lastEnemyMove = now;
                }

                if (checkCollision()) {
                    gameOver = true;
                    break;
                }

                auto renderElapsed = chrono::duration_cast<chrono::milliseconds>(now - lastRender).count();

                if (renderElapsed >= 100) {
                    draw();

                    cout << endl;
                    cout << "Time remaining: " << remaining << " seconds" << endl;

                    lastRender = now;
                }

                this_thread::sleep_for(chrono::milliseconds(20));
            }

            keyboard.stop();
            clearScreen();

            if (gameOver) {
                if (checkCollision()) {
                    cout << "================================\n";
                    cout << "         YOU GOT CAUGHT!\n";
                    cout << "================================\n";
                } else {
                    cout << "================================\n";
                    cout << "       YOU SURVIVED!\n";
                    cout << "================================\n";
                }
            } else {
                cout << "Game keluar.\n";
            }
        }
};

int main() {
    std::srand(std::time(nullptr));
    Game game = Game(20,40);

    game.start();

    return 0;
}