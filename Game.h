#include <vector>
#include <string>

#include "Deck.h"
#include "Card.h"
#include "Player.h"

class Game {
    std::vector<Player> players;
    Deck deck;
    std::vector<Card> table;
    std::vector<Card> loadHandAndTable(Player player);
    /**
     * Returns highest rank if player has flush and -1 if there is no flush.
     */
    int hasFlush(Player player);
    int hasStraight(Player player);
    int hasStraightFlush(Player player);
    int hasRoyalFlush(Player player);
    int hasPair(Player player);
    int hasThreeKind(Player player);
    int hasTwoPair(Player player);

    
    /**
     * Returns 0 if equal hands, 1 if playerA's hand is better than playerB's hand
     * and -1 if playerA's hand is worse than playerB's hand.
     */
    static int compareHands(Player playerA, Player playerB);
    public:
        Game();
        void addPlayer(std::string name);
        void firstDeal();
        void test();
};