#include <string>
#include <vector>

#include "Player.h"

Player::Player(std::string name, int chips) {
    this->name = name;
    this->hand = std::vector<Card>();
    this->chips = chips;
}

Player::Player(std::string name, std::vector<Card> hand, int chips) {
    this->name = name;
    this-> hand = hand;
    this->chips = chips;
}

Player Player::receiveDeal(std::vector<Card> cards) {
    std::vector<Card> newHand = hand;
    for (Card& card : cards) {
        newHand.push_back(card);
    }
    return Player(name, newHand, chips);
}

Player Player::bet(int amt) {
    int newChipsAmt = chips - amt;
    return Player(name, hand, newChipsAmt);
}

std::vector<Card>& Player::getHand() {
    return hand;
}

std::string Player::getName() {
    return name;
}

int Player::getChipAmt() {
    return chips;
}


