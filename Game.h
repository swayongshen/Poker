#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <utility>

#include "Deck.h"
#include "Card.h"
#include "Player.h"

class Game {
    std::vector<Player> players;
    Deck deck;
    std::vector<Card> table;
    std::vector<Card> loadHandAndTable(Player player);
    /**
     * The functions below will return the suit and rank of the highest card that fufils the criteria
     * i.e if a flush is 3, 4, 7, 9, 10 spades, hasFlush will return 10.
     * If criteria is not fufilled, return 0.
     */
    std::pair<Suit, int> hasFlush(Player player);
    int hasNConsecutive(std::vector<int> cardRanks, int n);
    std::pair<Suit, int> hasStraight(Player player);
    std::pair<Suit, int> hasStraightFlush(Player player);
    //std::pair<Suit, int> hasRoyalFlush(Player player);

    /**
     * Returns a vector {a, b, c} where a represents the highest pair,
     * b represents the highest three of a kind and c represents the highest quad.
     * If they have value of -1 it means none e.g. -1 for a means no pair found.
     */
    std::vector<int> hasPairTripsQuads(Player player);
    std::vector<int> hasTwoPair(Player player);

    
    /**
     * Returns 0 if equal hands, 1 if playerA's hand is better than playerB's hand
     * and -1 if playerA's hand is worse than playerB's hand.
     */
    static int compareHands(Player playerA, Player playerB);
    public:
        Game();
        void addPlayer(std::string name);
        void firstDeal();
        void test();
};

#endif