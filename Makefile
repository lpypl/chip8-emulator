all :

sfml : Screen.hpp Sfml.cpp
	g++ Sfml.cpp -lsfml-graphics -lsfml-window -lsfml-system -lpthread