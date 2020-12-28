#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <limits>
#include <thread>
#include <SFML/Network.hpp>

#include "Game.h"

/**
 * Game follows instructions from 
 * https://www.instructables.com/Learn-To-Play-Poker---Texas-Hold-Em-aka-Texas-Ho/
 */
int main() {
    /**
     * Configuration of game
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
    int numPlayers;
    sf::TcpListener listener;
    int portNumber = 53000;
    // bind the listener to a port
    if (listener.listen(portNumber) != sf::Socket::Done)
    {
        std::cout << "Error binding TCP listener to port " + std::to_string(portNumber) << std::endl;
    }
    //Start thread which continuously accepts new connections until numPlayers == maxPlayers
    std::thread tcpAccept (&Game::acceptConnections, std::ref(game), std::unique_ptr<sf::TcpListener>(&listener), std::ref(numPlayers), maxPlayers);

    //Wait for at least 2 players
    std::cout << "Waiting for at least 2 players to join...\n"; 
    while (numPlayers < 2) {
    }
    std::cout << "Players found, starting game.\n";

    //Decide who is the first dealer.
    srand(time(NULL));
    int dealerPosition = rand() % numPlayers;

    std::cout << "Player " + std::to_string(dealerPosition) + " has been chosen randomly to be the first dealer. \n\n"; 

    //Rotate the players vector to make dealer the first player in vector.
    game.rotatePlayersLeft(dealerPosition);

    //Reshuffle after using cards to decide who to be dealer
    game.restartDeck();

    while (true) {
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
            if (game.isContinueGame()) {
                continue;
            } else {
                break;
            }
        }

        /**
         * Flop
         */
        //Burns 1 card and draws 3 card before flop
        game.dealFlop();
        //Round 2 betting after flop, starting from small blind after dealer.
        game.printStatus("FLOP");
        game.displayTable();
        game.round(0, true);
        if (game.hasWinner() != -1) {
            game.awardWinnersAndRotatePlayers();
            if (game.isContinueGame()) {
                continue;
            } else {
                break;
            }
        }

        /**
         * Turn
         */
        //Burns another card and deal 1 card for the turn
        game.dealTurnOrRiver();
        game.printStatus("TURN");
        game.displayTable();
        game.round(0, true);
        if (game.hasWinner() != -1) {
            game.awardWinnersAndRotatePlayers();
            if (game.isContinueGame()) {
                continue;
            } else {
                break;
            }
        }

        /**
         * River
         */
        //Burns a card and deal 1 card for the river.
        game.dealTurnOrRiver();
        game.printStatus("RIVER");
        game.displayTable();
        game.round(0, true);
        //At this point, winner has to be determined.
        game.awardWinnersAndRotatePlayers();
        if (game.isContinueGame()) {
            continue;
        } else {
            break;
        }

    }
}