#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "Card.h"

class Player {
    std::string name;
    std::vector<Card> hand;

    public:
        Player(std::string name);
        void receiveDeal(Card card);
};

#endif