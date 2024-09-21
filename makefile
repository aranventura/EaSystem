ALL: IluvatarSonCompile ArdaCompile

all.o:
	gcc all.c -c -Wall -Wextra

sockets.o:
	gcc sockets.c -c -Wall -Wextra

manageFile.o:
	gcc Iluvatar/manageFile.c -c -Wall -Wextra

queueMsg.o:
	gcc Iluvatar/queueMsg.c -c -Wall -Wextra

commandParser.o:
	gcc Iluvatar/commandParser.c -c -Wall -Wextra

socketsIluvatar.o:
	gcc Iluvatar/socketsIluvatar.c -c -Wall -Wextra

IluvatarSon.o:
	gcc Iluvatar/IluvatarSon.c -c -Wall -Wextra

IluvatarSonCompile: IluvatarSon.o all.o commandParser.o manageFile.o sockets.o queueMsg.o socketsIluvatar.o
	gcc IluvatarSon.o all.o commandParser.o manageFile.o sockets.o queueMsg.o socketsIluvatar.o -lpthread -o IluvatarSon -Wall -Wextra

filesArda.o:
	gcc Arda/filesArda.c -c -Wall -Wextra

socketsArda.o:
	gcc Arda/socketsArda.c -c -Wall -Wextra

linkedListClient.o:
	gcc Arda/linkedListClient.c -c -Wall -Wextra

Arda.o:
	gcc Arda/Arda.c -c -Wall -Wextra

ArdaCompile: Arda.o all.o sockets.o socketsArda.o filesArda.o linkedListClient.o
	gcc Arda.o all.o sockets.o socketsArda.o linkedListClient.o -lpthread filesArda.o  -o arda -Wall -Wextra

clean:
	rm *.o