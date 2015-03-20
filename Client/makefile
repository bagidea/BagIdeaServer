CC=g++
OBJ=obj/
BIN=bin/
LIB=obj/BIS_Client.o
L=-L usr/lib -lpthread

all: $(OBJ) buildLib SampleChat SampleChatRoom

buildLib:
	$(CC) -c BISlib/BIS_Client.cpp -o $(OBJ)BIS_Client.o

SampleChat: SampleChat.o
	$(CC) $(OBJ)SampleChat.o $(LIB) -o $(BIN)RunSampleChat $(L)

SampleChatRoom: SampleChatRoom.o
	$(CC) $(OBJ)SampleChatRoom.o $(LIB) -o $(BIN)RunSampleChatRoom $(L)

SampleChat.o: SampleChat.cpp
	$(CC) -c SampleChat.cpp -o $(OBJ)SampleChat.o

SampleChatRoom.o: SampleChatRoom.cpp
	$(CC) -c SampleChatRoom.cpp -o $(OBJ)SampleChatRoom.o

$(OBJ):
	mkdir obj bin

clean:
	rm -rf obj bin
