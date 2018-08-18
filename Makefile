CC=g++
WINCC=x86_64-w64-mingw32-g++
SFML=-L/usr/share/doc/  -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s -lfreetype -lX11 -lXrandr -lxcb -lxcb-randr -lxcb-image -lGL -lFLAC -logg -lvorbis -lvorbisenc -lvorbisfile -lopenal -lpthread -ludev
LIBSFML=./SFML/build/lib/libsfml-audio-s.a ./SFML/build/lib/libsfml-graphics-s.a ./SFML/build/lib/libsfml-system-s.a ./SFML/build/lib/libsfml-window-s.a ./SFML/build/lib/libsfml-network-s.a 
SRC=$(shell ls ./src/*.cpp)
OBJ=$(SRC:.cpp=.o)
.PHONY: clean
.PHONY: clean-all
all: test.x UnixEditor.exe engine.exe
clean:
	rm -f ./src/*.o
clean-all:
	rm -f ./src/*.o test.x UnixEditor.exe engine.exe
%.o: %cpp
	$(CC) -c $(SRC) -DSFML_STATIC -static
engine.x: $(OBJ) 
	$(CC) $(OBJ) $(SFML) -o $@
	rm -f ./src/*.o

UnixEditor.exe: $(OBJ) UnixEditor.o
	$(CC) $(OBJ) UnixEditor.o -o $@ $(SFML)
UnixEditor.o: $(SRC)
	$(CC) -c $(SRC) -I ./SFML/include
	
test.x: test.o
	$(CC) test.o -o test.x
test.o: test.cpp
	$(CC) -c test.cpp
	
