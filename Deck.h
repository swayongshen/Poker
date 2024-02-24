#ifndef DECK_H
#define DECK_H

#include <vector>
#include "Card.h"
#include "Printer.h"

class Deck {
    std::vector<Card> cards;
    std::shared_ptr<Printer> printer;

public:
    Deck(std::shared_ptr<Printer> printer);

    Deck(std::vector<Card> cards, std::shared_ptr<Printer> printer);

    void enumerateCards();

    Deck shuffleDeck();

    Card dealCard();
};

#endif