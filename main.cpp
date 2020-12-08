#include <iostream>
#include "Deck.h"
#include "Card.h"

int main() {
    Deck deck = Deck();
    deck.enumerateCards();
    deck = deck.shuffleDeck();
    std::cout << "------------------------------------";
    deck.enumerateCards();
}