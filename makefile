Poker: Deck.o Card.o main.o
	g++ Deck.o Card.o main.o -o Poker

Deck.o: Deck.cpp
	g++ -c Deck.cpp

Card.o: Card.cpp
	g++ -c Card.cpp

main.o: main.cpp
	g++ -c main.cpp

Clean:
	rm *.o Poker