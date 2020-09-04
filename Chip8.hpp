#ifndef _CHIP_H
#define _CHIP_H

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <ctime>

typedef uint8_t byte;
typedef uint16_t dbyte;

class Chip8
{
private:
    // general register
    byte v[16];
    // register i
    dbyte i;
    byte delayTimer, soundTimer;
    dbyte pc;
    byte sp;
    dbyte stack[16];
    byte memory[4096];

    inline dbyte readOpcode()
    {
        dbyte opcode = memory[pc] << 8 | memory[pc + 1];
        pc += 2;
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

public:
    Chip8();
    
    void run();

    void reset();
};

#endif