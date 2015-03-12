CC=g++
OBJ=obj/
BIN=bin/
LIB=obj/BIS_Client.o
L=-L usr/lib -lpthread

all: $(OBJ) buildLib SampleChat

buildLib:
	$(CC) -c BISlib/BIS_Client.cpp -o $(OBJ)BIS_Client.o

SampleChat: SampleChat.o
	$(CC) $(OBJ)SampleChat.o $(LIB) -o $(BIN)RunSampleChat $(L)

SampleChat.o: SampleChat.cpp
	$(CC) -c SampleChat.cpp -o $(OBJ)SampleChat.o

$(OBJ):
	mkdir obj bin

clean:
	rm -rf obj bin RunSampleChat
