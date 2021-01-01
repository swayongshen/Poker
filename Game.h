#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <utility>
#include <SFML/Network.hpp>
#include <mutex>

#include "Deck.h"
#include "Card.h"

class Game {
    class Player {
        friend class Game;
        std::string name;
        std::vector<Card> hand;
        int chips;
        bool hasFolded = false;

        Player(std::string name, int chips);
        Player(std::string name, std::vector<Card> hand, int chips);
        Player receiveDeal(std::vector<Card> cards);
        Player bet(int amt);
        std::vector<Card>& getHand();
        std::string getName();
        int getChipAmt();
        Player awardChips(int amt);
        Player resetHand();
    };

    std::vector<Player> players;
    Deck deck;
    std::vector<Card> table;
    int pot;
    std::vector<int> bets;
    std::vector<std::unique_ptr<sf::TcpSocket>> playerClients;
    std::mutex playerClientsMutex;
    std::vector<std::pair<std::string, std::unique_ptr<sf::TcpSocket>>> waitingPlayers;
    std::mutex waitingPlayersMutex;
    

    

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

    int act(int playerIndex);

    bool allOthersFolded(int playerIndex);

    /**
     * Returns 0 if equal hands, 1 if playerA's hand is better than playerB's hand
     * and -1 if playerA's hand is worse than playerB's hand.
     */
    int compareHands(Player playerA, Player playerB);

    //Helper methods to send/receive message to client
    void sendMsg(int clientIndex, std::string msg);
    void sendMsg(sf::TcpSocket& clientSocket, std::string msg);
    sf::Packet receiveMsg(sf::TcpSocket& clientSocket);
    sf::Packet receiveMsg(int clientIndex);

    public:
        int numPlayers = 0;
        std::mutex numPlayersMutex;
        int numActivePlayers = 0;
        void acceptConnections(std::unique_ptr<sf::TcpListener> listener, int maxPlayers, std::atomic<bool>& isStop);
        void acceptWaitingPlayers();
        Game();
        Game( const Game& ); // non construction-copyable
        Game& operator=( const Game& ); // non copyable
        void addPlayer(std::string name, int chips);
        void firstDeal();
        void test();
        Card draw();
        void restartDeck();
        //Small blind and big blind put into pot.
        void blindsBid(int smallBlindAmt);
        //Rotates the players vector left by d places. 
        void rotatePlayersLeft(int d);
        void bet(int playerIndex, int amt);
        void broadcastMsg(std::string msg);
        void displayTable();
        void displayTableAndHand();
        void displayTableAndAllUnfoldHands();
        void printStatus(std::string status);
        

        /**
         * playerIndex takes in the vector index of the player who raised the bet.
         * the betting round begins from the next player.
         * 
         * The method serves to simulate a round of poker whereby everyone acts until
         * all players have either folded or called the last raise. 
         */
        int round(int playerIndex, bool isPreFlop = false);

        /**
         * Calls round(2 % numPlayers, true) which starts the round from after the big blind
         * and also has an additional turn to allow big blind to check/raise.
         */
        int preFlopRound();
        
        //Burn 1 card and draw 3 cards for the flop.
        void dealFlop();

        //Burn 1 card and deal the turn/river card
        void dealTurnOrRiver();

        //Return the playerIndex of the winner if there is one, and -1 if none yet.
        int hasWinner();

        //Decides who won and awards the player(s) and rotate the table clockwise by 1 position.
        void awardWinnersAndRotatePlayers();

        //Mutex locks to ensure thread safety
        void lockNumPlayers();
        void unlockNumPlayers();
        void checkConnectedAll();

        
};

#endif
