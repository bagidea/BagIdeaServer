CC=g++
OBJ=obj/
BIN=bin/
LIB=obj/BIS_Server.o obj/BIS_ServerObject.o
L=-L usr/lib -lpthread

all: $(OBJ) buildLib build

buildLib: ServerBIS ServerObjectBIS

ServerBIS:
	$(CC) -c BISlib/BIS_Server.cpp -o $(OBJ)BIS_Server.o

ServerObjectBIS:
	$(CC) -c BISlib/BIS_ServerObject.cpp -o $(OBJ)BIS_ServerObject.o

build: main.o
	$(CC) $(OBJ)main.o $(LIB) -o $(BIN)StartServer $(L)

$(OBJ):
	mkdir obj bin

main.o: main.cpp
	$(CC) -c main.cpp -o $(OBJ)main.o

clean:
	rm -rf obj bin StartServer
