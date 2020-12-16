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
    int pot;

    //Helper function to return a vector which contains all cards from table as well as the 2 cards from the player's hand
    std::vector<Card> loadHandAndTable(Player player);

    /**
     * The functions below will return the suit and rank of the highest card that fufils the criteria
     * i.e if a flush is 3, 4, 7, 9, 10 spades, hasFlush will return 10.
     * If criteria is not fufilled, return 0.
     */
    std::vector <int> hasFlush(Player player);

    
    //Helper function to check if there are n consecutively ranked cards in the input.
    int hasNConsecutive(std::vector<int> cardRanks, int n);

    /**
     * Returns the rank of the highest card in the straight if there is a straight.
     * Return -1 if there is no straight flush.
     */
    int hasStraight(Player player);

    /**
     * Returns the rank of the highest card in the straight flush if there is a straight flush.
     * Return -1 if there is no straight flush.
     */
    int hasStraightFlush(Player player);

    //hasRoyalFlush is not needed because it is just hasStraightFlush which returns 14.
    //std::pair<Suit, int> hasRoyalFlush(Player player);

    /**
     * Returns a vector {a, b, c} where a represents the highest pair,
     * b represents the highest three of a kind and c represents the highest quad.
     * If they have value of -1 it means none e.g. -1 for a means no pair found.
     */
    std::vector<int> hasPairTripsQuads(Player player);

    //Returns {highest triple, highest pair} where the triple and the pair forms a full house.
    std::vector<int> hasFullHouse(Player player);

    
    //Returns {Highest pair, second highest pair} if any.
    std::vector<int> hasTwoPair(Player player);

    //Returns the rank of the highest ranked card available to player.
    int getHighCard(Player player);

    /**
     * There are 10 types of poker hands. Returns XYY where X is the prefix which represents the type of hand
     * e.g. 10 for royal flush, 1 for high card. YY is the value associated. E.g. 14 for Ace. 
     * A royal flush would return 1014 and a straight flush with 10 as the highest card will return 910.
     * 
     * This is a preliminary rank, if 2 players have the same hand rank, we might have to compare kicker cards.
     */
    int handRank(Player player);

    //Returns 1 if A > B, -1 if A < B and 0 if A == B.
    static int compareInt(int A, int B);

    /**
     * Compare up to limitOfKicker number of cards between playerA and playerB excluding the cards to remove from comparison.
     * E.g. if both player A and B have a pair of 5 cardsToRmoeve = {5}, we remove the pair of 5 from consideration 
     * then compare the highest 3 (limitOfKickers = 3) from rest of their cards to see who has a higher kicker.
     */
    int compareKickers(std::vector<int> cardsToRemove, int limitOfKickers, Player playerA, Player playerB);

    

    public:
        /**
         * Returns 0 if equal hands, 1 if playerA's hand is better than playerB's hand
         * and -1 if playerA's hand is worse than playerB's hand.
         */
        int compareHands(Player playerA, Player playerB);

        Game();
        void addPlayer(std::string name, int chips);
        void firstDeal();
        void test();
        Card draw();
        void restartDeck();
        //Rotates the players vector left by d places. 
        void rotatePlayersLeft(int d);
        void bet(int playerIndex, int amt);
        void displayTable();
        void displayTableAndHand(int playerIndex);

        
};

#endif
