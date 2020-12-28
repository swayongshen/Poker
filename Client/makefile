PokerClient: Client.o
	g++ Client.o -o PokerClient -L /usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
	export LD_LIBRARY_PATH=/usr/local/lib

Client.o:
	g++ -c Client.cpp

Clean:
	rm *.o PokerClient