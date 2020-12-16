#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "Card.h"

class Player {
    std::string name;
    std::vector<Card> hand;
    int chips;

    public:
        Player(std::string name, int chips);
        Player(std::string name, std::vector<Card> hand, int chips);
        Player receiveDeal(std::vector<Card> cards);
        Player bet(int amt);
        std::vector<Card>& getHand();
        std::string getName();
        int getChipAmt();
};

#endif