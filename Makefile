# to compile your project, type make and press enter

all: main

main: main.cpp
	g++ main.cpp libggfonts.a timers.cpp -Wall -omain -lX11 -lGL -lGLU -lXext -lm -lpthread

clean:
	rm -f main

