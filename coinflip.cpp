// THIS IS ALL AI, JS FOR DECORATION.

#include "coinflip.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <thread>
using namespace std;

// Everything in here is private to this file; only playCoinFlipAnimation is public.
namespace
{
    const double PI = 3.14159265358979323846;
    const int R = 5;              // jari-jari vertikal koin (dalam baris)
    const int HEIGHT = 2 * R + 1; // tinggi canvas
    const int WIDTH = 4 * R + 5;  // lebar canvas (karakter ~2x lebih tinggi dari lebar)

    // Gambar satu frame koin. angle = sudut rotasi koin terhadap sumbu vertikal.
    // cos(angle) > 0 -> sisi HEADS terlihat, cos(angle) < 0 -> sisi TAILS terlihat.
    string renderFrame(double angle)
    {
        double c = cos(angle);
        bool heads = c >= 0;
        double rx = 2.0 * R * fabs(c); // jari-jari horizontal mengecil saat koin miring

        string out;
        for (int y = -R; y <= R; y++)
        {
            double ratio = (double)y / R;
            int half = (int)round(rx * sqrt(max(0.0, 1.0 - ratio * ratio)));

            string line(WIDTH, ' ');
            int center = WIDTH / 2;

            for (int x = -half; x <= half; x++)
            {
                bool edge = (abs(x) >= half - 1) || (abs(y) == R);
                line[center + x] = edge ? '#' : ' ';
            }

            // huruf di tengah koin, hanya kalau cukup lebar
            if (y == 0 && half >= 2)
            {
                line[center] = heads ? 'H' : 'T';
            }

            out += line + "\n";
        }
        return out;
    }
}

void playCoinFlipAnimation(bool resultHeads)
{
    random_device rd;
    mt19937 gen(rd());

    // jumlah setengah putaran: genap -> berakhir HEADS, ganjil -> TAILS
    int fullSpins = uniform_int_distribution<int>(3, 5)(gen);
    int halfTurns = 2 * fullSpins + (resultHeads ? 0 : 1);

    const int frames = 70;
    const auto delay = chrono::milliseconds(35);

    cout << "\033[?25l"; // sembunyikan kursor
    cout << "Flipping...\n\n";

    for (int f = 0; f <= frames; f++)
    {
        double t = (double)f / frames;
        double eased = 1.0 - pow(1.0 - t, 3); // ease-out: makin lambat di akhir
        double angle = eased * halfTurns * PI;

        cout << renderFrame(angle) << flush;

        if (f < frames)
        {
            cout << "\033[" << HEIGHT << "A"; // naik HEIGHT baris, timpa frame sebelumnya
            this_thread::sleep_for(delay);
        }
    }

    cout << "\033[?25h" << endl; // tampilkan kursor lagi
}