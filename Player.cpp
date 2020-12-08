#include <string>
#include <vector>

#include "Player.h"

Player::Player(std::string name) {
    this->name = name;
    this->hand = std::vector<Card>();
}

void Player::receiveDeal(Card card) {
    hand.push_back(card);
}

