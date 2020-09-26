chip : Chip8.cpp Chip8.hpp Display.cpp Display.hpp Sfml.cpp main.cpp
	g++ -o $@ Chip8.cpp Display.cpp Display.hpp main.cpp -lsfml-graphics -lsfml-window -lsfml-system
sfml : Display.cpp Display.hpp Sfml.cpp
	g++ -o $@ Sfml.cpp Display.cpp -lsfml-graphics -lsfml-window -lsfml-system
