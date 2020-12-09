#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>

#include "Game.h"
#include "Card.h"
#include "Player.h"

std::vector<Card> Game::loadHandAndTable(Player player) {
    std::vector<Card> loaded;
    for (Card card : table) {
        loaded.push_back(card);
    }
    for (Card card : player.getHand()) {
        loaded.push_back(card);
    }
    return loaded;
}

int Game::hasFlush(Player player) {
    if (table.size() >= 3) {
        std::unordered_map<int, int> countSuits = {{0, 0}, {1, 0}, {2,0}, {3,0}};
        std::vector<Card> cards;
        for (Card card : table) {
            countSuits[card.getSuit()] += 1;
            cards.push_back(card);
        }
        for (Card card : player.getHand()) {
            countSuits[card.getSuit()] += 1;
            cards.push_back(card);
        }
        for (auto const& pair : countSuits) {
            //For the suit that has more than or equal to 5 cards
            if (pair.second >= 5) {
                std::vector<Card> cardsOfSuit;
                int suit = pair.first;
                std::copy_if(cards.begin(), cards.end(), std::back_inserter(cardsOfSuit), [suit](Card card) -> bool {return card.getSuit() == suit;});
                //If Ace exists, return ace, else, return max
                if (std::count_if(cardsOfSuit.begin(), cardsOfSuit.end(), [](Card card) {return card.getRank() == 1; std::cout << "reach2";}) == 1) {
                    return 1;
                } else {
                    Card maxCard = *std::max_element(cards.begin(), cards.end(), [](Card cardA, Card cardB) {return cardA.getRank() < cardB.getRank();});
                    return maxCard.getRank();
                }

            }
        }
        //None of the suits have 5 cards
        return -1;
    } else {
        return -1;
    }
}

int Game::hasStraight(Player player) {
    //Put all numbers into a vector and sort and check for 5 consecutive.
    std::vector<Card> loaded = loadHandAndTable(player);
    std::vector<int> loadedInt;
    std::for_each(loaded.begin(), loaded.end(), [&loadedInt](Card card) {
        int rank = card.getRank();
        loadedInt.push_back(rank);
        if (rank == 1) {
            //Ace is represented as both 14 and 1 for straight
            loadedInt.push_back(14);
        }
    });

    //Algorithm to find if any 5 consecutively ranked cards
    std::unordered_set<int> num_set;
    for (int num : loadedInt) {
        num_set.insert(num);
    }

    int longestStreak = 0;

    for (int num : num_set) {
        if (num_set.find(num-1) == num_set.end()) {
            int currentNum = num;
            int currentStreak = 1;

            while (num_set.find(currentNum+1) != num_set.end()) {
                currentNum += 1;
                currentStreak += 1;
            }

            if (currentStreak == 5) {
                return currentNum;
            }
        }
    }
    return -1;

};

void Game::test() {

    for (int i = 0; i < 5; i++) {
        table.push_back(deck.dealCard());
        std::cout << table[i] << std::endl;
    }

    for (Card& card : players[0].getHand()) {
        std::cout << card << std::endl;
    }

    std::cout << hasFlush(players[0]) << std::endl;
    std::cout << hasStraight(players[0]) << std::endl;


}


Game::Game() {
    this->players = std::vector<Player>();
    this->deck = Deck();
    this->table = std::vector<Card>();
}



void Game::addPlayer(std::string name) {
    Player newPlayer(name);
    players.push_back(newPlayer);
}

void Game::firstDeal() {
    for (Player& player : players) {
        Card firstCard = deck.dealCard();
        player.receiveDeal(firstCard);
        Card secondCard = deck.dealCard();
        player.receiveDeal(secondCard);
    }
}


