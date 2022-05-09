# to compile your project, type make and press enter

all: main

main: main.cpp
	g++ main.cpp libggfonts.a -Wall -omain -lX11 -lGL -lGLU -lXext

clean:
	rm -f main

