# Makefile
ChatCode: main.cpp ChatArray ChatUser system_connection ChatClient ChatServer
	g++ -o ChatCode main.cpp -L. -lChatArray -lChatUser -lsystem_connection -lChatClient -lChatServer

ChatArray: ChatArray.cpp ChatArray.h
	g++ -o ChatArray.o ChatArray.cpp -c
	ar rc libChatArray.a ChatArray.o

ChatUser: ChatUser.cpp ChatUser.h
	g++ -o ChatUser.o ChatUser.cpp -c
	ar rc libChatUser.a ChatUser.o

system_connection: system_connection.cpp system_connection.h
	g++ -o system_connection.o system_connection.cpp -c
	ar rc libsystem_connection.a system_connection.o

ChatClient: ChatClient.cpp
	g++ -o ChatClient.o ChatClient.cpp -c
	ar rc libChatClient.a ChatClient.o

ChatServer: ChatServer.cpp
	g++ -o ChatServer.o ChatServer.cpp -c
	ar rc libChatServer.a ChatServer.o

clean:
	rm ChatCode *.o *.a
