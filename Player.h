//
// Created by Swa, Yong Shen on 25/2/24.
//

#ifndef RUNPOKER_PLAYER_H
#define RUNPOKER_PLAYER_H

#include <string>
#include <vector>
#include "Card.h"
#include <SFML/Network.hpp>

class Player {
    std::string name;
    std::vector<Card> hand;
    int chips;
    std::unique_ptr<sf::TcpSocket> socket;

public:
    Player(std::string name, int chips);

    Player(std::string name, std::vector<Card> hand, int chips);

    Player(std::string name, std::vector<Card> hand, int chips, std::unique_ptr<sf::TcpSocket> &socket);

    void receiveDeal(std::vector<Card> cards);

    void bet(int amt);

    std::vector<Card> &getHand();

    std::string getName();

    int getChipAmt();

    Player awardChips(int amt);

    Player resetHand();

    sf::Packet receiveMsg();

    void disconnectSocket();

    void sendMsg(std::string msg);

    sf::Packet waitReceive();
};


#endif //RUNPOKER_PLAYER_H
