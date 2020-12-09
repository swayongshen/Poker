Poker: Deck.o Card.o Game.o Player.o main.o
	g++ Deck.o Card.o main.o Game.o Player.o -o Poker

Deck.o: Deck.cpp
	g++ -c Deck.cpp

Card.o: Card.cpp
	g++ -c Card.cpp

main.o: main.cpp
	g++ -c main.cpp

Game.o: Game.cpp
	g++ -c Game.cpp

Player.o: Player.cpp
	g++ -c Player.cpp

Clean:
	rm *.o Poker