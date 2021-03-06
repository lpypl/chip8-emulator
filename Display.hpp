#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cstdint>
#include <string>
#include <vector>

const unsigned int channels = 4;

static uint8_t HEXDIGITS[16][5] = {
    {0xF0, 0x90, 0x90, 0x90, 0xF0},
    {0x20, 0x60, 0x20, 0x20, 0x70},
    {0xF0, 0x10, 0xF0, 0x80, 0xF0},
    {0xF0, 0x10, 0xF0, 0x10, 0xF0},
    {0x90, 0x90, 0xF0, 0x10, 0x10},
    {0xF0, 0x80, 0xF0, 0x10, 0xF0},
    {0xF0, 0x80, 0xF0, 0x90, 0xF0},
    {0xF0, 0x10, 0x20, 0x40, 0x40},
    {0xF0, 0x90, 0xF0, 0x90, 0xF0},
    {0xF0, 0x90, 0xF0, 0x10, 0xF0},
    {0xF0, 0x90, 0xF0, 0x90, 0x90},
    {0xE0, 0x90, 0xE0, 0x90, 0xE0},
    {0xF0, 0x80, 0x80, 0x80, 0xF0},
    {0xE0, 0x90, 0x90, 0x90, 0xE0},
    {0xF0, 0x80, 0xF0, 0x80, 0xF0},
    {0xF0, 0x80, 0xF0, 0x80, 0x80},
};

enum CHIP8KEY
{
    KEY0,
    KEY1,
    KEY2,
    KEY3,
    KEY4,
    KEY5,
    KEY6,
    KEY7,
    KEY8,
    KEY9,
    KEYA,
    KEYB,
    KEYC,
    KEYD,
    KEYE,
    KEYF,
};

class Display
{
    unsigned int width;
    unsigned int height;

    sf::RenderWindow window;
    sf::Uint8 *pixels;
    sf::Texture texture;
    sf::Sprite sprite;

    sf::Thread *poll_thread;
    sf::Mutex pixels_mutex;

public:
    time_t keyPressedTime;
    CHIP8KEY pressedKey;

    Display(unsigned int w, unsigned int h, std::string winame = "CHIP-8");
    ~Display();

    void updateBuffer(std::vector<bool> buffer);

    void clearDisplay();

    void drawSprite(uint8_t *sprite, uint8_t height, uint8_t x, uint8_t y);

    bool drawSpriteXor(uint8_t *spriteBuffer, uint8_t spriteHeight,
                       uint8_t baseX, uint8_t baseY);

    void poll();

    void startDisplayLoop();

    static void *poll_wrapper(void *object);

    bool isOpen();
};

#endif
