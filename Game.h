#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <utility>
#include <SFML/Network.hpp>
#include <mutex>

#include "Deck.h"
#include "Card.h"
#include "Printer.h"

class Game {

public:
    Game(std::shared_ptr<Printer> printer);

    int numPlayers = 0;
    std::mutex numPlayersMutex;
    int numActivePlayers = 0;

    /**
     * Constructors
     */
    Game(Printer &printer);

    Game(const Game &); // non construction-copyable
    Game &operator=(const Game &); // non copyable

    /**
     * This method is run in a separate thread to accept incoming TCP connections in the background and put them in a waiting list.
     */
    void acceptConnections(std::unique_ptr<sf::TcpListener> listener, int maxPlayers, std::atomic<bool> &isStop);

    void waitForEnoughPlayers();

    /**
     * Moves waiting players from waiting list into the active list and empties waitign list
     */
    void acceptWaitingPlayers();

    /**
     * Create a new player with a name and a designated amount of chips and add it to the vector of players.
     */
    void addPlayer(std::string name, int chips);

    /**
     * Deals 2 pocket cards to all players
     */
    void firstDeal();

    /**
     * Restarts the state of the deck to 52 cards and shuffles it.
     */
    void restartDeck();

    /**
     * Takes the small blind amount (smallBlindAmt) of chips from the small blind player and
     * the big blind amount which is 2 * smallBlindAmt from the big blind player and place
     * the chips in the pot.
     */
    void blindsBid(int smallBlindAmt);

    /**
     * Rotates the players vector and playerClients vector leftwards by d places.
     */
    void rotatePlayersLeft(int d);

    /**
     * Allows the player at playerIndex in the players vector to bet amt amount of chips.
     */
    void bet(int playerIndex, int amt);

    /**
     * Sends a message to all players.
     */
    void broadcastMsg(std::string msg);

    /**
     * Sends the state of the poker table (the community cards and the amount of chips in the pot) to all players:
     */
    void displayTable();

    /**
     * Sends the state of the poker table (the community cards and the amount of chips in the pot) and
     * also their own pocket cards to all players.
     */
    void displayTableAndHand();

    /**
     * Sends the state of the poker table (the community cards and the amount of chips in the pot) and
     * all pairs of pocket cards belonging to unfold hands to all players.
     */
    void displayTableAndAllUnfoldHands();

    /**
     * Sends a framed status message to all players. E.g.
     *  **********
     *    STATUS
     *  **********
     */
    void printStatus(std::string status);

    /**
     * Simulates a pre-flop round whereby the player after the big blind is the first to act.
     */
    int preFlopRound();

    /**
     * Simulate any round (flop, turn, river) after the flop whereby the small blind is the first to act.
     */
    int postFlopRound();

    /**
     * Burns 1 card from the top of the deck and deals 3 cards to the community (flop)
     */
    void dealFlop();

    /**
     * Burns 1 card from the top of the deck and deals 1 card to the community (turn/river).
     */
    void dealTurnOrRiver();

    /**
     * Return the playerIndex of the winner if there is one, and -1 if none yet.
     */
    int hasWinner();

    /**
     * Awards the player(s) who won and rotate the table clockwise by 1 position.
     */
    void awardWinners();

    /**
     * Takes back the cards from the community and from players and reshuffles the deck.
     */
    void resetCards();

    /**
     * Prompts players to indicate if they want to continue with the next game. Disconnect players
     * that do not want to continue.
     */
    void confirmIfPlayersWantToContinue();


    /**
     * Sends a ping message to all players to check if they are still connected.
     * Removes disconnected players.
     */
    void checkConnectedAll();

    int getNumPlayers();

private:
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

        std::vector<Card> &getHand();

        std::string getName();

        int getChipAmt();

        Player awardChips(int amt);

        Player resetHand();
    };

    // Stdio printing util
    std::shared_ptr<Printer> printer;

    //Stores Player objects of each active players.
    std::vector<Player> players;

    //Stores the pointers to the TCP client sockets of each active player.
    std::vector<std::unique_ptr<sf::TcpSocket>> playerClients;

    //Mutex to allow thread safety for playerClients.
    std::mutex playerClientsMutex;

    //Stores pointers to the TCP client sockets of waiting players.
    std::vector<std::pair<std::string, std::unique_ptr<sf::TcpSocket>>> waitingPlayers;

    //Mutex to allow thread safety for waitingPlayers.
    std::mutex waitingPlayersMutex;

    std::condition_variable waitPlayerCv;
    std::mutex waitPlayerCvMutex;

    //Deck of cards
    Deck deck;

    //Table cards a.k.a community cards
    std::vector<Card> table;

    int pot;

    //Stores the bet made by each active player.
    std::vector<int> bets;

    /**
     * Draws a card from the deck and returns it.
     */
    Card draw();

    /**
     * Returns a vector which contains all cards from table as well as the pocket cards from the player's hand.
     */
    std::vector<Card> loadHandAndTable(Player player);

    /**
     * Returns the ranks of the top 5 cards in the flush if any and {-1} if there is no flush.
     */
    std::vector<int> hasFlush(Player player);


    /**
     * Returns the top card rank of the sequence if there are n consecutively ranked cards in cardRanks and
     * -1 if there are no n consecutively ranked cards.
     */
    int hasNConsecutive(std::vector<int> cardRanks, int n);

    /**
     * Returns the rank of the highest card in the straight if there is a straight.
     * Return -1 if there is no straight.
     */
    int hasStraight(Player player);

    /**
     * Returns the rank of the highest card in the straight flush if there is a straight flush.
     * Return -1 if there is no straight flush.
     */
    int hasStraightFlush(Player player);

    //hasRoyalFlush is not needed because it is just hasStraightFlush which returns 14.

    /**
     * Returns a vector {a, b, c} where a represents the highest pair,
     * b represents the highest three of a kind and c represents the highest quad.
     * If they have value of -1 it means none e.g. -1 for a means no pair found.
     */
    std::vector<int> hasPairTripsQuads(Player player);

    /**
     * Returns {highest triple, highest pair} where the triple and the pair forms a full house and
     * returns {-1, -1} if there is no full house.
     */
    std::vector<int> hasFullHouse(Player player);


    /**
     * Returns {Highest pair, second highest pair} if any. Returns {-1, -1} for no pair
     * and {X, -1} if 1 pair of X is found and {X, Y} if 2 pairs X and Y are found and X > Y.
     */
    std::vector<int> hasTwoPair(Player player);

    /**
     * Returns the rank of the highest ranked card available to the player.
     */
    int getHighCard(Player player);

    /**
     * There are 10 types of poker hands. Returns XYY where X is the prefix which represents the type of hand
     * e.g. 10 for royal flush, 1 for high card. YY is the value associated. E.g. 14 for Ace.
     * A royal flush would return 1014 and a straight flush with 10 as the highest card will return 910.
     *
     * This is a preliminary rank, if 2 players have the same hand rank, we might have to compare kicker cards.
     */
    int handRank(Player player);

    /**
     * Compare up to limitOfKickers number of cards between playerA and playerB excluding the cards to remove from comparison.
     * E.g. if both player A and B have a pair of 5, cardsToRmoeve = {5}, we remove the pair of 5 from consideration
     * then compare the highest 3 (limitOfKickers = 3) from rest of their cards to see who has a higher kicker.
     *
     * Returns 1 if A > B, -1 if A < B and 0 if A == B.
     */
    int compareKickers(std::vector<int> cardsToRemove, int limitOfKickers, Player playerA, Player playerB);

    /**
     * Prompts the player at playerIndex to input an action for his turn and processes it.
     *
     * Returns 1 if player raises, 0 if he calls or checks and -1 if he folds.
     */
    int act(int playerIndex);

    /**
     * The method serves to simulate a round of poker starting from the player after the player at playerIndex.
     * Everyone acts until all players have either folded or called the last raise.
     */
    int round(int playerIndex);

    /**
     * Checks if all other players except the player at playerIndex has folded.
     */
    bool allOthersFolded(int playerIndex);

    /**
     * Returns 0 if equal hands, 1 if playerA's hand is better than playerB's hand
     * and -1 if playerA's hand is worse than playerB's hand.
     */
    int compareHands(Player playerA, Player playerB);

    //Helper methods to send/receive message to client
    void sendMsg(int clientIndex, std::string msg);

    void sendMsg(sf::TcpSocket &clientSocket, std::string msg);

    sf::Packet receiveMsg(sf::TcpSocket &clientSocket);

    sf::Packet receiveMsg(int clientIndex);
};

#endif
