#include "Screen.hpp"

Screen::Screen(unsigned int w, unsigned int h, std::string winame)
    : width(w), height(h), window(sf::VideoMode(width, height), winame)
{
    pixels = new sf::Uint8[width * height * channels];
    texture.create(width, height);
    sprite.setTexture(texture);
}

void Screen::updateBuffer(std::vector<bool> buffer)
{
    pixels_mutex.lock();
    for (register int i = 0;
         i < width * height; i += 1)
    {
        int start = i * channels;
        int value = buffer[i] ? 255 : 0;
        pixels[start] = value;     //R
        pixels[start + 1] = value; //G
        pixels[start + 2] = value; //B
        pixels[start + 3] = value; //A
    }
    pixels_mutex.unlock();
}

void Screen::poll()
{
    window.setActive(true);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        pixels_mutex.lock();
        texture.update(pixels);
        pixels_mutex.unlock();

        window.clear();
        window.draw(sprite);
        window.display();
    }
}

void Screen::startDisplayLoop()
{
    window.setActive(false);
    poll_thread = new sf::Thread(Screen::poll_wrapper, this);
    poll_thread->launch();
}

void *Screen::poll_wrapper(void *object)
{
    reinterpret_cast<Screen *>(object)->poll();
    return 0;
}