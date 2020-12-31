#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <limits>
#include <thread>
#include <SFML/Network.hpp>
#include <signal.h>

#include "Game.h"

bool isStop = false;

void stopThread(int s) {
    isStop = true;
}
/**
 * Game follows instructions from 
 * https://www.instructables.com/Learn-To-Play-Poker---Texas-Hold-Em-aka-Texas-Ho/
 */
int main() {

    //If CTRL + C pressed, stop thread
    signal(SIGINT, stopThread);
    /**
     * Configuration of game: max number of players, small blind amt
     */

    //Get max number of players
    std::cout << "Enter the max number of players: ";
    int maxPlayers;
    while(true) {
        std::cin >> maxPlayers;
        if (!std::cin.fail() && maxPlayers > 1 && maxPlayers <= 11) {
            std::cout << std::endl;
            break;
        } else {
            std::cout << "Please enter a valid number of players: ";
        }
    }

    //Get small blind amount.
    std::cout << "Please enter the small blind amount for the game: ";
    int smallBlindAmt;
    while(true) {
        std::cin >> smallBlindAmt;
        if (!std::cin.fail() && smallBlindAmt > 0) {
            std::cout << std::endl;
            break;
        } else {
            std::cout << "Please enter a number greater than 0: ";
        }
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    

    //Start new game
    Game game = Game();

    /**
     * Networking
     */
    sf::TcpListener listener;
    int portNumber = 53000;
    //flag to make thread stop
    
    try {
        // bind the listener to a port
        if (listener.listen(portNumber) != sf::Socket::Done)
        {
            std::cout << "Error binding TCP listener to port " + std::to_string(portNumber) << std::endl;
        }
        //Start thread which continuously accepts new connections until numPlayers == maxPlayer
        std::thread tcpAccept (&Game::acceptConnections, std::ref(game), std::unique_ptr<sf::TcpListener>(&listener), maxPlayers, std::ref(isStop));

        //Wait for at least 2 players
        std::cout << "Waiting for at least 2 players to join...\n"; 
        while (game.numPlayers < 2) {
        }
        std::cout << "Players found, starting game.\n";

        game.numActivePlayers = game.numPlayers;
        game.acceptWaitingPlayers();

        //Decide who is the first dealer.
        srand(time(NULL));
        int dealerPosition = rand() % game.numActivePlayers;
        std::cout << "Player at index " + std::to_string(dealerPosition) + " has been chosen randomly to be the first dealer.\n\n";

        //Rotate the players vector to make dealer the first player in vector.
        game.rotatePlayersLeft(dealerPosition);
        
        //Reshuffle after using cards to decide who to be dealer
        game.restartDeck();

        /**
         * Game processes
         */
        int round = 0;
        while (true) {
            if (round != 0) {
                game.checkConnectedAll();
                while (game.numActivePlayers < 2) {
                    game.checkConnectedAll();
                }
            }
            round++;
            game.printStatus("NEW GAME");

            //Small blind, big blind added to pot. 
            game.blindsBid(smallBlindAmt);

            /**
             * Pre-flop
             */
            //Deal pocket cards
            game.firstDeal();
            //Each player after the big blind starting from the under the gun starts their preflop action
            game.printStatus("PRE-FLOP");
            game.preFlopRound();
            if (game.hasWinner() != -1) {
                game.awardWinnersAndRotatePlayers();
            }

            /**
             * Flop
             */
            //Burns 1 card and draws 3 card before flop
            game.dealFlop();
            //Round 2 betting after flop, starting from small blind after dealer.
            game.printStatus("FLOP");
            game.displayTableAndHand();
            game.round(0, true);
            if (game.hasWinner() != -1) {
                game.awardWinnersAndRotatePlayers();
            }

            /**
             * Turn
             */
            //Burns another card and deal 1 card for the turn
            game.dealTurnOrRiver();
            game.printStatus("TURN");
            game.displayTableAndHand();
            game.round(0, true);
            if (game.hasWinner() != -1) {
                game.awardWinnersAndRotatePlayers();
            }

            /**
             * River
             */
            //Burns a card and deal 1 card for the river.
            game.dealTurnOrRiver();
            game.printStatus("RIVER");
            game.displayTableAndHand();
            game.round(0, true);
            //At this point, winner has to be determined.
            game.awardWinnersAndRotatePlayers();
        }
    } catch (...) {
        isStop = true;
        listener.close();
    }
}