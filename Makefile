CC = g++-mp-4.8
CFLAGS = -g -Wall -std=c++11 -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_mixer.framework/Headers -I/Library/Frameworks/SDL2_image.framework/Headers -I/Library/Frameworks/SDL2_ttf.framework/Headers
LDFLAGS = -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf -framework Cocoa

all: Soundrietta

Soundrietta : obj/Soundrietta.o obj/Util.o obj/Tile.o obj/Dot.o obj/LTexture.o obj/Soundrietta.o
	${CC} obj/Util.o obj/Tile.o obj/Dot.o obj/LTexture.o obj/Soundrietta.o -o bin/Soundrietta ${CFLAGS} ${LDFLAGS}

obj/Soundrietta.o : src/Soundrietta.cpp src/Soundrietta.h src/Util.h src/Tile.h src/Dot.h src/LTexture.h
	${CC} ${CFLAGS} -c src/Soundrietta.cpp -o obj/Soundrietta.o

obj/Util.o :  src/Util.cpp src/Util.h
	${CC} ${CFLAGS} -c src/Util.cpp -o obj/Util.o

obj/Tile.o :  src/Tile.cpp src/Tile.h
	${CC} ${CFLAGS} -c src/Tile.cpp -o obj/Tile.o

obj/Dot.o :  src/Dot.cpp src/Dot.h
	${CC} ${CFLAGS} -c src/Dot.cpp -o obj/Dot.o

obj/LTexture.o :  src/LTexture.cpp src/LTexture.h
	${CC} ${CFLAGS} -c src/LTexture.cpp -o obj/LTexture.o

clean:
	rm -rf obj/*o bin/Soundrietta