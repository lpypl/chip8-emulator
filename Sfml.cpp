#include "Screen.hpp"
#include <iostream>

using namespace std;

namespace lpy {
    struct Size {
        uint8_t width;
        uint8_t height; 
    };

    struct Position {
        uint8_t x;
        uint8_t y; 
    };
}

void showDigit(vector<bool> &buffer, sf::Uint8 digit,
               lpy::Size shape,
               lpy::Position position)
{
    sf::Uint8 *digitAddr = HEXDIGITS[digit];

    for (size_t r = 0; r < 5; r++)
    {
        for (size_t c = 0; c < 4; c++)
        {
            buffer[(position.x + r) * shape.width +
                   position.y + c] = (digitAddr[r] << c) & 0x80;
        }
    }
}

int main()
{
    Screen screen(64, 64);
    screen.startDisplayLoop();

    vector<bool> buffer(64 * 64, 0);

    int ct = 0;
    while (true)
    {
        if (++ct == 16)
        {
            ct = 0;
        }

        showDigit(buffer, ct, {64, 64}, {32-3, 32-2});
        screen.updateBuffer(buffer);
        // sf::sleep(sf::microseconds(33333)); // 30 Hz
        sf::sleep(sf::milliseconds(100)); // 30 Hz
    }

    return 0;
}