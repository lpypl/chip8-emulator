#include "Display.hpp"
#include <iostream>

using namespace std;

namespace lpy
{
    struct Size
    {
        uint8_t width;
        uint8_t height;
    };

    struct Position
    {
        uint8_t x;
        uint8_t y;
    };
} // namespace lpy

int main()
{
    const uint8_t SCREEN_WIDTH = 128, SCREEN_HEIGHT = 64;
    Display screen(SCREEN_WIDTH, SCREEN_HEIGHT);
    screen.startDisplayLoop();

    vector<bool> buffer(SCREEN_WIDTH * SCREEN_HEIGHT, 0);

    int ct = 0;
    while (screen.isOpen())
    {
        if (++ct == 16)
        {
            ct = 0;
        }
        // sf::sleep(sf::microseconds(33333)); // 30 Hz
        screen.drawSprite(HEXDIGITS[ct], 5, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        sf::sleep(sf::milliseconds(100)); // 30 Hz
    }

    return 0;
}