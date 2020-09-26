#include "Chip8.hpp"

int main(const int argc, const char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s rompath\n", argv[0]);
        exit(-1);
    }
    Chip8 chip;
    chip.reset();
    chip.run(argv[1]);
}
