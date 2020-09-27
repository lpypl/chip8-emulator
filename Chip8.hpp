#ifndef _CHIP_H
#define _CHIP_H

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "Display.hpp"

typedef uint8_t byte;
typedef uint16_t dbyte;

class Chip8
{
private:
    // general register
    byte v[16];
    // register i
    dbyte i;
    // Timer & Mutex
    byte delayTimer, soundTimer;
    const int delayTimerFreq = 60;
    const int soundTimerFreq = 60;
    const int chipFreq = 500;
    sf::Mutex delayTimerMutex, soundTimerMutex;
    sf::Thread *delayTimerThread, *soundTimerThread;
    void delayTimerPoll(), soundTimerPoll();
    static void *delayTimerPollWrapper(void *object), *soundTimerPollWrapper(void *object);

    dbyte pc;
    byte sp;
    dbyte stack[16];
    byte memory[4096];

    // Display Module
    const uint8_t SCREEN_WIDTH = 64, SCREEN_HEIGHT = 32;
    Display screen;

    inline dbyte readOpcode()
    {
        dbyte opcode = (memory[pc] << 8) | memory[pc + 1];
        pc += 2;
	    return opcode;
    }

    inline dbyte getAddr(dbyte opcode)
    {
        return 0x0fff & opcode;
    }

    inline byte getByte(dbyte opcode)
    {
        return 0x00ff & opcode;
    }

    //?x??
    inline byte getX(dbyte opcode)
    {
        return 0x000f & (opcode >> 8);
    }

    //??y?
    inline byte getY(dbyte opcode)
    {
        return 0x000f & (opcode >> 4);
    }

    //???n
    inline byte getN(dbyte opcode)
    {
        return 0x000f & (opcode >> 0);
    }

public:
    Chip8();
    
    void run(const char *fname);

    void reset();

    void setDelayTimer(const byte value), setSoundTimer(const byte value);
    byte getDelayTimer(), getSoundTimer();
};

#endif
