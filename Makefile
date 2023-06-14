# Makefile
ChatCode: main.cpp ChatArray ChatUser
	g++ -o ChatCode main.cpp -L. -lChatArray -lChatUser

ChatArray: ChatArray.cpp ChatArray.h
	g++ -o ChatArray.o ChatArray.cpp -c
	ar rc libChatArray.a ChatArray.o

ChatUser: ChatUser.cpp ChatUser.h
	g++ -o ChatUser.o ChatUser.cpp -c
	ar rc libChatUser.a ChatUser.o

sha1: sha1.cpp sha1.h
	g++ -o sha1.o sha1.cpp -c
	ar rc libsha1.a sha1.o

clean:
	rm ChatCode *.o *.a
