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
