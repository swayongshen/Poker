#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "Game.h"
#include "Card.h"
#include "Player.h"

int Game::hasFlush(Player player) {
    if (river.size() >= 3) {
        std::unordered_map<int, int> countSuits = {{0, 0}, {1, 0}, {2,0}, {3,0}};
        std::vector<Card> cards;
        for (Card card : river) {
            countSuits[card.getSuit()] += 1;
            cards.push_back(card);
        }
        for (Card card : player.getHand()) {
            countSuits[card.getSuit()] += 1;
            cards.push_back(card);
        }
        for (auto const& pair : countSuits) {
            if (pair.second >= 5) {
                std::vector<Card> cardsOfSuit;
                int suit = pair.first;
                std::copy_if(cards.begin(), cards.end(), cardsOfSuit.begin(), [&suit](Card card) {return card.getSuit() == suit;});
                //If Ace exists, return ace, else, return max
                if (std::count_if(cards.begin(), cards.end(), [](Card card) {return card.getRank() == 1;}) == 1) {
                    return 1;
                } else {
                    Card maxCard = *std::max_element(cards.begin(), cards.end(), [](Card cardA, Card cardB) {return cardA.getRank() < cardB.getRank();});
                    return maxCard.getRank();
                }

            }
        }
        return false;
    } else {
        return false;
    }
}

void Game::test() {
    for (int i = 0; i < 5; i++) {
        river.push_back(deck.dealCard());
        std::cout << river[i] << std::endl;
    }

    for (Card card : players[0].getHand()) {
        std::cout << card << std::endl;
    }

    std::cout << hasFlush(players[0]) << std::endl;


}


Game::Game() {
    this->players = std::vector<Player>();
    this->deck = Deck();
    this->river = std::vector<Card>();
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


