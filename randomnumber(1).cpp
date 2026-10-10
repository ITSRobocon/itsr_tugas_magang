#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;


class Player
{
private:
    string name;

public:
    Player(string n)
    {
        name = n;
    }

    string getName() { return name; }

    int makeGuess(int low, int high)
    {
        while (true)
        {
            int guess;
            cout << name << ", tebak angka (" << low << "-" << high << "): ";
            cin >> guess;

            if (cin.fail())   // kalau yang diketik bukan angka
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Inputan nya berupa angka ya!" << endl;
                continue;
            }

            if (guess < low || guess > high)
            {
                cout << "Rentang angka hanya berkisar " << low << " dan " << high << "!" << endl;
            }
            else
            {
                return guess;
            }
        }
    }
};


class Game
{
private:
    int secretNumber;
    int low;
    int high;
    int totalGuesses;
    Player player1;
    Player player2;

    bool playTurn(Player p)
    {
        int guess = p.makeGuess(low, high);
        totalGuesses++;

        if (guess == secretNumber)
        {
            cout << "BENAR! Angkanya " << secretNumber << "." << endl;
            cout << p.getName() << " MENANG setelah " << totalGuesses
                 << " kali menebak!" << endl;
            return true;
        }

        if (guess < secretNumber)
        {
            cout << "Terlalu kecil!" << endl;
            low = guess + 1;
        }
        else
        {
            cout << "Terlalu besar!" << endl;
            high = guess - 1;
        }
        cout << endl;
        return false;
    }

public:
    Game() : player1("Player 1"), player2("Player 2")
    {
        secretNumber = rand() % 100 + 1;
        low = 1;
        high = 100;
        totalGuesses = 0;
    }

    void play()
    {
        cout << "Komputer punya 1 angka dengan rentang 1 - 100." << endl;
        cout << "Player 1 dan Player 2 bergantian menebak. Siapa yang benar duluan, menang!" << endl;
        cout << "Komputer akan menilai tebakan: Terlalu kecil, Terlalu besar, atau Benar" << endl << endl;

        while (true)
        {
            if (playTurn(player1)) break;
            if (playTurn(player2)) break;
        }
    }
};


int main()
{
    srand(time(0));

    cout << "Tebak Angka: Player 1 vs Player 2" << endl << endl;

    Game game;
    game.play();

    return 0;
}