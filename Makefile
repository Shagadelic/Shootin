<<<<<<< HEAD
CXXLFLAGS = `guile-config compile` std=c++11 -Wall -Wextra -g
LIBS = `guile-config link` -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -ldl -lpthread

.PHONY: clean build run

build: first

clean:
	rm -f first first.o

run: first
	./first

first: first.o
	g++ $< -o $@ $(LIBS)

first.o: first.cpp
	g++ -c $< -o $@ $(CXXFLAGS)
=======
export CFLAGS = "-Wno-error"
NAME=shootin

shootin: $(NAME).h
	gcc -g -o $(NAME).exe $(NAME).c -lncurses -lpthread


IGNORE: clean
clean:
	-rm *.exe
>>>>>>> 4b53eb315dc3061bacce573a23f4a45d189fab2a
