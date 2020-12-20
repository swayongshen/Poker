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
        if (std::cin >> numPlayers && numPlayers > 1) {
            std::cout << std::endl;
            break;
        } else {
            std::cout << "Please enter a valid number of players" << std::endl;
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
        if (std::cin >> smallBlindAmt && smallBlindAmt > 0) {
            std::cout << std::endl;
            break;
        } else {
            std::cout << "Please enter an amount more than 0." << std::endl;
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
    if (numPlayers >= 3) {
        game.bet(1, smallBlindAmt);
        game.bet(2, smallBlindAmt * 2);
    } else {
        game.bet(1, smallBlindAmt);
        game.bet(0, smallBlindAmt * 2);
    }

    //Deal pocket cards
    game.firstDeal();

    //Each player, starting from under the gun starts their preflop action
    int UTGIndex = 3 % numPlayers;
    for (int i = 0; i < numPlayers; i++) {
        int playerIndex = (i + UTGIndex) % numPlayers;
        game.preflop(playerIndex);
    }

    







    

    

}