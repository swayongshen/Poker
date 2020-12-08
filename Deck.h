#ifndef DECK_H
#define DECH_H
#include <vector>
#include "Card.h"

class Deck {
    std::vector<Card> cards;

    public:
        Deck();
        Deck(std::vector<Card> cards);
        void enumerateCards();
        Deck shuffleDeck();
        Card dealCard();
};

#endif