#ifndef CARD_H
#define CARD_H

#include <string>

enum Suit {unknown = -1, Club = 0, Diamond = 1, Heart = 2, Spade = 3};

std::string suitToName(Suit suit);

class Card {
    Suit suit;
    int rank;

    public:
        int getRank();
        Suit getSuit();

        Card(Suit suit, int rank);

        friend std::ostream& operator << (std::ostream& output, Card& card);
        friend std::string& operator << (std::string& output, Card& card);
};

#endif