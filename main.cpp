#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <limits>

#include "Game.h"


int main() {
    /**
     * Game follows instructions from 
     * https://www.instructables.com/Learn-To-Play-Poker---Texas-Hold-Em-aka-Texas-Ho/
     */
    //Start new game
    Game game = Game();

    //Get number of players
    std::cout << "Enter the number of players: ";
    int numPlayers;
    while(true) {
        std::cin >> numPlayers;
        if (!std::cin.fail() && numPlayers > 1) {
            std::cout << std::endl;
            break;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << "Please enter a valid number of players: ";
        }
    }

    //If not enough players, return.
    if (numPlayers <= 1) {
        std::cout << "Not enough players \n";
        return 1;
    }

    //Get small blind amount.
    std::cout << "Please enter the small blind amount for the table: ";
    int smallBlindAmt;
    while(true) {
        std::cin >> smallBlindAmt;
        if (!std::cin.fail() && smallBlindAmt > 0) {
            std::cout << std::endl;
            break;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << "Please enter a number greater than 0: ";
        }
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //Add players, each player has $500 chips
    for (int i = 0; i < numPlayers; i++) {
        game.addPlayer("Player " + std::to_string(i), 500);
    }

    //Decide who is the first dealer.
    srand(time(NULL));
    int dealerPosition = rand() % numPlayers;

    std::cout << "Player " + std::to_string(dealerPosition) + " has been chosen randomly to be the dealer. \n\n"; 

    //Rotate the players vector to make dealer the first player in vector.
    game.rotatePlayersLeft(dealerPosition);

    //Reshuffle after using cards to decide who to be dealer
    game.restartDeck();

    //Small blind, big blind added to pot. 
    game.blindsBid(smallBlindAmt);

    //Deal pocket cards
    game.firstDeal();

    //Each player after the big blind starting from the under the gun starts their preflop action
    game.printStatus("PRE-FLOP");
    game.preFlopRound();

    //Burns 1 card and draws 3 card before flop
    game.dealFlop();

    //Round 2 betting after flop, starting from small blind after dealer.
    game.printStatus("FLOP");
    game.round(0);

    //Burns another card and deal 1 card for the turn
    game.dealTurn();

    game.printStatus("TURN");
    game.round(0);
    

}