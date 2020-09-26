#include "Chip8.hpp"

unsigned char chip8_fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, //0
        0x20, 0x60, 0x20, 0x20, 0x70, //1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
        0x90, 0x90, 0xF0, 0x10, 0x10, //4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
        0xF0, 0x10, 0x20, 0x40, 0x40, //7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
        0xF0, 0x90, 0xF0, 0x90, 0x90, //A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
        0xF0, 0x80, 0x80, 0x80, 0xF0, //C
        0xE0, 0x90, 0x90, 0x90, 0xE0, //D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
        0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

Chip8::Chip8()
    : screen(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    delayTimerThread = soundTimerThread = nullptr;
    this->reset();
    screen.startDisplayLoop();
}

void Chip8::reset()
{
    // Program counter
    this->pc = 0x0000;

    // Memory
    memset(this->memory, 0x00, 4096);

    // Store digits
    // memcpy(memory, chip8_fontset, 80);
    for (int ind = 0; ind < 80; ind++)
    {
        memory[ind] = chip8_fontset[ind];
    }

    // Stack
    memset(this->stack, 0x00, sizeof(dbyte) / sizeof(byte) * 16);

    // Stack pointer
    this->sp = 0x00;

    // V Registers
    memset(this->v, 0x00, 16);

    // I Register
    this->i = 0x0000;

    // Delay timer
    // this->delayTimer = 0x00;
    setDelayTimer(0x00);

    // Sound timer
    // this->soundTimer = 0x00;
    setSoundTimer(0x00);

    if (delayTimerThread != nullptr)
    {
        delayTimerThread->terminate();
        delete delayTimerThread;
    }
    delayTimerThread = new sf::Thread(delayTimerPollWrapper, this);

    if (soundTimerThread != nullptr)
    {
        soundTimerThread->terminate();
        delete soundTimerThread;
    }
    soundTimerThread = new sf::Thread(soundTimerPollWrapper, this);

    delayTimerThread->launch();
    soundTimerThread->launch();

    srand((unsigned)time(NULL));
}

void Chip8::run(const char *fname)
{
    byte buffer[4096];
    FILE *fp = fopen(fname, "rb");
    if (fp == nullptr)
    {
        printf("Error in opening file: %s\n", fname);
        exit(-1);
    }

    int ct = fread(buffer, sizeof(byte), 4096, fp);
    if (ct < 0)
    {
        printf("Error in reading file: %s\n", fname);
        exit(-1);
    }

    for (int ind = 0; ind < ct; ind++)
    {
        if (memcpy(memory + 0x200, buffer, ct) == nullptr)
        {
            printf("Error in copying rom to memory\n");
            exit(-1);
        }
        pc = 0x200;
    }

    printf("Executing...\n");

    dbyte opcode;
    while (true)
    {
        opcode = readOpcode();

        printf("Current instruction is: %.04x - %.4x\n", pc - 2, opcode);

        sf::sleep(sf::milliseconds(1000 / chipFreq));

        switch ((opcode >> 12) & 0x000f)
        {
        case 0:
        {
            printf("[CASE 0]\n");
            switch (opcode)
            {
            // CLS
            case 0x00E0:
                screen.clearDisplay();
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
            printf("[CASE 1]\n");
            pc = getAddr(opcode);
            printf("Jump to %.4x, content is %.4x\n", pc, (memory[pc] << 8) | memory[pc + 1]);
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
            v[getX(opcode)] = getByte(opcode);
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
            case 0:
            {
                v[getX(opcode)] = v[getY(opcode)];
                break;
            }
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
                v[15] = v[getX(opcode)] >= v[getY(opcode)];
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
                v[15] = v[getY(opcode)] >= v[getX(opcode)];
                v[getX(opcode)] = v[getY(opcode)] - v[getX(opcode)];
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
            v[getX(opcode)] = rand() & 0xff & getByte(opcode);
            break;
        }
        case 0xD:
        {
            printf("[CASE D]: painting..., sprite addr is %.4x * %d, loc is (%.2d, %.2d), content is %.4x\n",
                   this->i, getN(opcode), this->v[getX(opcode)], this->v[getY(opcode)], memory[this->i]);
            v[0xf] = screen.drawSpriteXor(memory + this->i, getN(opcode), this->v[getX(opcode)], this->v[getY(opcode)]);
            break;
        }
        case 0xE:
        {
            switch (opcode & 0xFF)
            {
            case 0x9E:
            {
                //todo
                // if (getX(opcode) == xxx)
                //
                break;
            }
            case 0xA1:
            {
                //todo
                // if (getX(opcode) != xxx)
                this->pc += 2;
                break;
            }
            }
            break;
        }
        case 0xF:
        {
            switch (opcode & 0xff)
            {
            case 0x07:
            {
                v[getX(opcode)] = getDelayTimer();
                break;
            }
            case 0x0A:
            {
                //todo
                // waiting a key
                // All execution stops until a key is pressed, then the value of that key is stored in Vx.
                //
                for (;;)
                    ;
                break;
            }
            case 0x15:
            {
                setDelayTimer(v[getX(opcode)]);
                break;
            }
            case 0x18:
            {
                setSoundTimer(v[getX(opcode)]);
                break;
            }
            case 0x1E:
            {
                this->i += v[getX(opcode)];
                break;
            }
            case 0x29:
            {
                // todo
                // this->i = location of sprite for the value for vx
                this->i = 5 * v[getX(opcode)];
                break;
            }
            case 0x33:
            {
                memory[i + 0] = v[getX(opcode)] / 100;
                memory[i + 1] = v[getX(opcode)] % 100 / 10;
                memory[i + 2] = v[getX(opcode)] % 100 % 10;
                break;
            }
            case 0x55:
            {
                for (int ind = 0; ind <= getX(opcode); ind++)
                {
                    memory[i + ind] = v[ind];
                }
                break;
            }
            case 0x65:
            {
                for (int ind = 0; ind <= getX(opcode); ind++)
                {
                    v[ind] = memory[i + ind];
                }
                break;
            }
            }
            break;
        }
        default:
            break;
        }
    }
}

void Chip8::setDelayTimer(const byte value)
{
    delayTimerMutex.lock();
    delayTimer = value;
    delayTimerMutex.unlock();
}

byte Chip8::getDelayTimer()
{
    delayTimerMutex.lock();
    byte value = delayTimer;
    delayTimerMutex.unlock();
    return value;
}

void Chip8::setSoundTimer(const byte value)
{
    soundTimerMutex.lock();
    soundTimer = value;
    soundTimerMutex.unlock();
}

byte Chip8::getSoundTimer()
{
    soundTimerMutex.lock();
    byte value = soundTimer;
    soundTimerMutex.unlock();
    return value;
}

void Chip8::delayTimerPoll()
{
    for (;;)
    {
        sf::sleep(sf::milliseconds(1000 / delayTimerFreq));
        if (delayTimer != 0)
        {
            delayTimerMutex.lock();
            delayTimer--;
            delayTimerMutex.unlock();
        }
    }
}

void *Chip8::delayTimerPollWrapper(void *object)
{
    reinterpret_cast<Chip8 *>(object)->delayTimerPoll();
    return 0;
}

void Chip8::soundTimerPoll()
{
    for (;;)
    {
        sf::sleep(sf::milliseconds(1000 / soundTimerFreq));
        if (soundTimer != 0)
        {
            soundTimerMutex.lock();
            soundTimer--;
            soundTimerMutex.unlock();
        }
    }
}

void *Chip8::soundTimerPollWrapper(void *object)
{
    reinterpret_cast<Chip8 *>(object)->soundTimerPoll();
    return 0;
}