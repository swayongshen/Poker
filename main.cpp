#include <iostream>
#include "Game.h"

int main() {
    //Start new game
    Game game = Game();

    game.addPlayer("player1");
    game.addPlayer("player2");

    game.firstDeal();



    game.test();

    /**
     * TODO:
     * Player's state because deal cards doesn't seem to work in game.test();
     */
    

}