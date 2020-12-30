#include <iostream>
#include <string>

#include "Card.h"

std::string suitToName(Suit suit) {
    switch (suit) {
        case(Club):
            return "C";
        case(Diamond):
            return "D";
        case(Heart):
            return "H";
        case (Spade):
            return "S";
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
    return output << card.rank << suitToName(card.suit) << " ";
}

std::string& operator << (std::string& output, Card& card) {
    return output += std::to_string(card.rank) + suitToName(card.suit) + " ";
}