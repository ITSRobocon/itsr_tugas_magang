#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

class Keyboard {
private:
    struct termios original;

public:

    void start() {
        tcgetattr(STDIN_FILENO, &original);

        struct termios raw = original;

        raw.c_lflag &= ~(ICANON | ECHO);

        tcsetattr(STDIN_FILENO, TCSANOW, &raw);

        // Membuat stdin menjadi non-blocking
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }

    void stop() {
        tcsetattr(STDIN_FILENO, TCSANOW, &original);

        // Kembalikan stdin ke blocking mode
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
    }

    char getKey() {

        char c;

        // Tidak menunggu input
        if (read(STDIN_FILENO, &c, 1) <= 0) {
            return '\0';
        }

        // Arrow key diawali ESC
        if (c == '\033') {

            char sequence[2];

            if (read(STDIN_FILENO, &sequence[0], 1) <= 0)
                return '\0';

            if (read(STDIN_FILENO, &sequence[1], 1) <= 0)
                return '\0';

            if (sequence[0] == '[') {

                if (sequence[1] == 'A')
                    return 'U'; // Up

                if (sequence[1] == 'B')
                    return 'D'; // Down

                if (sequence[1] == 'C')
                    return 'R'; // Right

                if (sequence[1] == 'D')
                    return 'L'; // Left
            }
        }

        return c;
    }
};