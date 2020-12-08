#include <vector>
#include <string>

#include "Deck.h"
#include "Card.h"
#include "Player.h"

class Game {
    std::vector<Player> players;
    Deck deck;

    public:
        Game();
        void addPlayer(std::string name);
        void firstDeal();
};