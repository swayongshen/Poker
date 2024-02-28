//
// Created by Swa, Yong Shen on 25/2/24.
//

#include "Player.h"

Player::Player(std::string name, int chips) {
    this->name = name;
    this->hand = std::vector<Card>();
    this->chips = chips;
}

Player::Player(std::string name, std::vector<Card> hand, int chips) {
    this->name = name;
    this->hand = hand;
    this->chips = chips;
}

Player::Player(std::string name, std::vector<Card> hand, int chips, std::unique_ptr<sf::TcpSocket> &socket) {
    this->name = name;
    this->chips = chips;
    this->socket = std::move(socket);
}


void Player::receiveDeal(std::vector<Card> cards) {
    std::vector<Card> newHand = hand;
    for (Card &card: cards) {
        newHand.push_back(card);
    }
    this->hand = cards;
}

void Player::bet(int amt) {
    int newChipsAmt = chips - amt;
    this->chips = newChipsAmt;
}

std::vector<Card> &Player::getHand() {
    return hand;
}

std::string Player::getName() {
    return name;
}

int Player::getChipAmt() {
    return chips;
}

Player Player::awardChips(int amt) {
    return Player(name, hand, chips + amt);
}

Player Player::resetHand() {
    return Player(name, std::vector<Card>(), chips);
}

sf::Packet Player::receiveMsg() {
    sf::Packet receivePkt;
    socket->receive(receivePkt);
    return receivePkt;
}

sf::Packet Player::waitReceive() {
    sf::Packet receivePkt;
    sf::SocketSelector selector;
    selector.add(*socket);
    if (selector.wait(sf::seconds(2.0))) {
        receivePkt = receiveMsg();
    };
    return receivePkt;
}

void Player::sendMsg(std::string msg) {
    sf::Packet sendPkt;
    sendPkt << msg;
    socket->send(sendPkt);
}

void Player::disconnectSocket() {
    socket->disconnect();
}