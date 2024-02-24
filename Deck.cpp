#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "Deck.h"
#include "Card.h"
#include "Printer.h"


void randomize(std::vector<Card> &v) {
    // Use a different seed value so that  
    // we don't get same result each time 
    // we run this program  
    srand(time(NULL));

    // Start from the last element and swap  
    // one by one. We don't need to run for  
    // the first element that's why i > 0  
    for (int i = v.size() - 1; i > 0; i--) {
        // Pick a random index from 0 to i  
        int j = rand() % (i + 1);

        // Swap arr[i] with the element  
        // at random index  
        std::iter_swap(v.begin() + i, v.begin() + j);
    }
}

Deck::Deck(std::shared_ptr<Printer> printer) {
    //First generate an unshuffled array of cards
    std::vector<Card> unshuffled;
    for (int i = Club; i <= Spade; i++) {
        for (int j = 1; j <= 13; j++) {
            Suit suit = static_cast<Suit>(i);
            unshuffled.push_back(Card(suit, j));
        }
    }
    randomize(unshuffled);
    this->cards = unshuffled;
    this->printer = printer;
}

Deck::Deck(std::vector<Card> cards, std::shared_ptr<Printer> printer) {
    this->cards = cards;
    this->printer = printer;
}

void Deck::enumerateCards() {
    for (Card card: cards) {
        this->printer->print(card);
    }
}

Deck Deck::shuffleDeck() {
    std::vector<Card> cardsCopy(cards.begin(), cards.begin() + cards.size());
    randomize(cardsCopy);
    return Deck(cardsCopy, this->printer);
}

Card Deck::dealCard() {
    Card dealtCard = cards.back();
    cards.pop_back();
    return dealtCard;
}