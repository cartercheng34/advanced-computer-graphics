CC = g++
CXXFLAGS=-g -Wall -MMD -std=c++11

hw4:test.o algebra3.o Base.o Camera.o Raytracer.o kdtree.o Primitive.o
	${CC} $(CXXFLAGS) -o hw4 test.o algebra3.o Base.o Camera.o Raytracer.o kdtree.o Primitive.o

test.o:test.cpp algebra3.o
	${CC} $(CXXFLAGS) -c test.cpp

Raytracer.o:Raytracer.cpp algebra3.o Camera.o Base.o
	${CC} $(CXXFLAGS) -c Raytracer.cpp

kdtree.o:kdtree.cpp algebra3.o Base.o Primitive.o
	${CC} $(CXXFLAGS) -c kdtree.cpp

Primitive.o: Primitive.cpp Base.o
	${CC} $(CXXFLAGS) -c Primitive.cpp

Base.o:Base.cpp algebra3.o kdtree.o
	${CC} $(CXXFLAGS) -c Base.cpp

Camera.o:Camera.cpp algebra3.o
	${CC} $(CXXFLAGS) -c Camera.cpp

algebra3.o:algebra3.cpp
	${CC} $(CXXFLAGS) -c algebra3.cpp

clean:
	rm -rf *.o *.d
