all :

sfml : Display.cpp Display.hpp Sfml.cpp
	g++ Sfml.cpp Display.cpp -lsfml-graphics -lsfml-window -lsfml-system