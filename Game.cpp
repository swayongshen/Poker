#include <vector>

#include "Game.h"


Game::Game() {
    this->players = std::vector<Player>();
    this->deck = Deck();
}

void Game::addPlayer(std::string name) {
    Player newPlayer(name);
    players.push_back(newPlayer);
}

void Game::firstDeal() {
    for (Player player : players) {
        Card firstCard = deck.dealCard();
        player.receiveDeal(firstCard);
        Card secondCard = deck.dealCard();
        player.receiveDeal(secondCard);
    }
}

