#include "Chip8.hpp"

Chip8::Chip8()
{
    this->reset();
}

void Chip8::reset()
{
    // Program counter
    this->pc = 0x0000;

    // Memory
    memset(this->memory, 0x00, 4096);

    // Stack
    memset(this->stack, 0x00, sizeof(dbyte) / sizeof(byte) * 16);

    // Stack pointer
    this->sp = 0x00;

    // V Registers
    memset(this->v, 0x00, 16);

    // I Register
    this->i = 0x0000;

    // Delay timer
    this->delayTimer = 0x00;

    // Sound timer
    this->soundTimer = 0x00;
}

void Chip8::run()
{
    dbyte opcode;
    while (true)
    {
        opcode = readOpcode();

        switch (opcode & 0xf000)
        {
        case 0:
        {
            switch (opcode)
            {
            // CLS
            case 0x00E0:
                // todo
                break;
            // RET
            case 0x00EE:
                pc = stack[--sp];
                break;
            // SYS addr
            default:
                // todo
                //This instruction is only used on the old
                //computers on which Chip-8 was originally
                //implemented. It is ignored by modern interpreters.
                break;
            }
            break;
        }

        case 1:
        {
            pc = getAddr(opcode);
            break;
        }

        case 2:
        {
            stack[sp++] = pc;
            pc = getAddr(opcode);
            break;
        }

        case 3:
        {
            if (v[getX(opcode)] == getByte(opcode))
                pc += 2;
            break;
        }

        case 4:
        {
            if (v[getX(opcode)] != getByte(opcode))
                pc += 2;
            break;
        }

        case 5:
        {
            if (v[getX(opcode)] == v[getY(opcode)])
                pc += 2;
            break;
        }

        case 6:
        {
            v[getX(opcode)] == getByte(opcode);
            break;
        }

        case 7:
        {
            v[getX(opcode)] += getByte(opcode);
            break;
        }

        case 8:
        {
            switch (opcode & 0x000f)
            {
            case 1:
            {
                v[getX(opcode)] |= v[getY(opcode)];
                break;
            }
            case 2:
            {
                v[getX(opcode)] &= v[getY(opcode)];
                break;
            }
            case 3:
            {
                v[getX(opcode)] ^= v[getY(opcode)];
                break;
            }
            case 4:
            {
                v[15] = (v[getX(opcode)] + v[getY(opcode)]) >> 8;
                v[getX(opcode)] += v[getY(opcode)];
                break;
            }
            case 5:
            {
                v[15] = v[getX(opcode)] > v[getY(opcode)];
                v[getX(opcode)] -= v[getY(opcode)];
                break;
            }
            case 6:
            {
                v[15] = v[getX(opcode)] & 0x1;
                v[getX(opcode)] >>= 1;
                break;
            }
            case 7:
            {
                v[15] = v[getY(opcode)] > v[getX(opcode)];
                v[getX(opcode)] = v[getY(opcode) - v[getX(opcode)]];
                break;
            }
            case 0xe:
            {
                v[15] = (v[getX(opcode)] >> 7) & 0x1;
                v[getX(opcode)] <<= 1;
                break;
            }
            }
            break;
        }
        case 9:
        {
            if (v[getX(opcode)] != v[getY(opcode)])
                pc += 2;
            break;
        }
        case 0xA:
        {
            this->i = getAddr(opcode);
            break;
        }
        case 0xB:
        {
            pc = getAddr(opcode) + v[0];
            break;
        }
        case 0xC:
        {
            srand((unsigned)time(NULL));
            v[getX(opcode)] = rand() & 0xff & getByte(opcode);
            break;
        }
        case 0xD:
        {
            //todo
            break;
        }
        default:
            break;
        }
    }
}