CC = g++
CXXFLAGS=-g -Wall -MMD -std=c++11

hw1:test.o algebra3.o Base.o Camera.o Raytracer.o
	${CC} $(CXXFLAGS) -o hw2 test.o algebra3.o Base.o Camera.o Raytracer.o

test.o:test.cpp algebra3.o
	${CC} $(CXXFLAGS) -c test.cpp

Raytracer.o:Raytracer.cpp algebra3.o Camera.o Base.o
	${CC} $(CXXFLAGS) -c Raytracer.cpp

Base.o:Base.cpp algebra3.o
	${CC} $(CXXFLAGS) -c Base.cpp

Camera.o:Camera.cpp algebra3.o
	${CC} $(CXXFLAGS) -c Camera.cpp

algebra3.o:algebra3.cpp
	${CC} $(CXXFLAGS) -c algebra3.cpp

clean:
	rm -rf *.o *.d
