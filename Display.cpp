#include "Display.hpp"
#include "TimeUtils.hpp"

Display::Display(unsigned int w, unsigned int h, std::string winame)
    : width(w), height(h), window(sf::VideoMode(width, height), winame), keyPressedTime(0), pressedKey(KEY0)
{
    pixels = new sf::Uint8[width * height * channels]{0};
    texture.create(width, height);
    sprite.setTexture(texture);
}

void Display::clearDisplay()
{
    pixels_mutex.lock();
    delete[] pixels;
    pixels = new sf::Uint8[width * height * channels]{0};
    pixels_mutex.unlock();
}

void Display::drawSprite(uint8_t *spriteBuffer, uint8_t spriteHeight, uint8_t baseX, uint8_t baseY)
{
    const uint8_t spriteWidth = 8;
    uint8_t tx, ty;
    unsigned int pixelAddress;
    uint8_t pixelValue;

    pixels_mutex.lock();
    for (uint8_t r = 0; r < spriteHeight; r++)
    {
        for (uint8_t c = 0; c < spriteWidth; c++)
        {
            tx = (baseX + c) % width;
            ty = (baseY + r) % height;

            pixelAddress = (ty * width + tx) * channels;
            pixelValue = (spriteBuffer[r] << c) & 0x80 ? 255 : 0;
            pixels[pixelAddress] = pixelValue;     //R
            pixels[pixelAddress + 1] = pixelValue; //G
            pixels[pixelAddress + 2] = pixelValue; //B
            pixels[pixelAddress + 3] = pixelValue; //A
        }
    }
    pixels_mutex.unlock();
}

bool Display::drawSpriteXor(uint8_t *spriteBuffer, uint8_t spriteHeight, uint8_t baseX, uint8_t baseY)
{
    const uint8_t spriteWidth = 8;
    uint8_t tx, ty;
    unsigned int pixelAddress;
    uint8_t pixelValue;
    bool changed = false;
    uint8_t originalValue;

    pixels_mutex.lock();
    for (uint8_t r = 0; r < spriteHeight; r++)
    {
        for (uint8_t c = 0; c < spriteWidth; c++)
        {
            tx = (baseX + c) % width;
            ty = (baseY + r) % height;

            pixelAddress = (ty * width + tx) * channels;
            originalValue = pixels[pixelAddress];
            pixelValue = (spriteBuffer[r] << c) & 0x80 ? 255 : 0;
            changed = pixelValue == originalValue;
            pixelValue ^= originalValue;
            pixels[pixelAddress] = pixelValue;     //R
            pixels[pixelAddress + 1] = pixelValue; //G
            pixels[pixelAddress + 2] = pixelValue; //B
            pixels[pixelAddress + 3] = pixelValue; //A
        }
    }
    pixels_mutex.unlock();
    return changed;
}

void Display::poll()
{
    window.setActive(true);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Num1:
                    pressedKey = KEY1;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::Num2:
                    pressedKey = KEY2;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::Num3:
                    pressedKey = KEY3;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::Num4:
                    pressedKey = KEYC;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::Q:
                    pressedKey = KEY4;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::W:
                    pressedKey = KEY5;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::E:
                    pressedKey = KEY6;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::R:
                    pressedKey = KEYD;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::A:
                    pressedKey = KEY7;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::S:
                    pressedKey = KEY8;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::D:
                    pressedKey = KEY9;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::F:
                    pressedKey = KEYE;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::Z:
                    pressedKey = KEYA;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::X:
                    pressedKey = KEY0;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::C:
                    pressedKey = KEYB;
                    keyPressedTime = getMillTimeStamp();
                    break;
                case sf::Keyboard::V:
                    pressedKey = KEYF;
                    keyPressedTime = getMillTimeStamp();
                    break;
                default:
                    break;
                }
                printf("[KEY] keycode=%d, time=%ld\n", pressedKey, keyPressedTime);
            }
        }

        pixels_mutex.lock();
        texture.update(pixels);
        pixels_mutex.unlock();

        window.clear();
        window.draw(sprite);
        window.display();
    }
}

void Display::startDisplayLoop()
{
    window.setActive(false);
    poll_thread = new sf::Thread(Display::poll_wrapper, this);
    poll_thread->launch();
}

void *Display::poll_wrapper(void *object)
{
    reinterpret_cast<Display *>(object)->poll();
    return 0;
}

bool Display::isOpen()
{
    return window.isOpen();
}

Display::~Display()
{
    delete[] pixels;
    if (poll_thread)
        delete poll_thread;
}
