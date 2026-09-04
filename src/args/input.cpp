#include <iostream>
#include <termios.h>
#include <unistd.h>

using namespace std;

struct RawTerminal {
    termios orig_termios;

    RawTerminal() {
        tcgetattr(STDIN_FILENO, &orig_termios);
        termios raw = orig_termios;
        raw.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }

    ~RawTerminal() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }
};

string read_line(const string& prompt) {
    RawTerminal raw;
    std::string buffer = "";
    size_t cursor_pos = 0;

    std::cout << prompt << std::flush;

    while (true) {
        char c;
        if (read(STDIN_FILENO, &c, 1) <= 0)
            break;

        if (c == '\n' || c == '\r') {
            std::cout << "\n";
            break;
        } else if (c == 27) {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 0)
                continue;
            if (read(STDIN_FILENO, &seq[1], 1) == 0)
                continue;

            if (seq[0] == '[') {
                if (seq[1] == 'D' && cursor_pos > 0) {
                    cursor_pos--;
                    std::cout << "\033[D" << std::flush;
                } else if (seq[1] == 'C' && cursor_pos < buffer.size()) {
                    cursor_pos++;
                    std::cout << "\033[C" << std::flush;
                }
            }
        } else if (c == 127 || c == 8) {
            if (cursor_pos > 0) {
                buffer.erase(cursor_pos - 1, 1);
                cursor_pos--;

                std::cout << "\r\033[K" << prompt << buffer;
                std::cout << "\r" << prompt << buffer.substr(0, cursor_pos)
                          << std::flush;
            }
        } else {
            buffer.insert(cursor_pos, 1, c);
            cursor_pos++;

            std::cout << "\r\033[K" << prompt << buffer;
            std::cout << "\r" << prompt << buffer.substr(0, cursor_pos)
                      << std::flush;
        }
    }

    return buffer;
}
