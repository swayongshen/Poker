#include <iostream>
#include "Game.h"

int main() {
    //Start new game
    Game game = Game();

    game.addPlayer("player1");
    game.addPlayer("player2");

    game.firstDeal();

    game.test();
    

}