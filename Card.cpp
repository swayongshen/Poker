#include <iostream>

#include "Card.h"

std::string suitToName(Suit suit) {
    switch (suit) {
        case(Club):
            return "Club";
        case(Diamond):
            return "Diamond";
        case(Heart):
            return "Heart";
        case (Spade):
            return "Spade";
    }
    return "Bug";
}

int Card::getRank() {
    return rank;
}

Suit Card::getSuit() {
    return suit;
}

Card::Card(Suit suit, int rank) {
    this->suit = suit;
    this->rank = rank;
}

std::ostream& operator << (std::ostream& output, Card& card) {
    return output << card.rank << " " << suitToName(card.suit) << std::endl;
}