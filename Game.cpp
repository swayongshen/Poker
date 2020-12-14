#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>

#include "Game.h"
#include "Card.h"
#include "Player.h"

std::vector<Card> Game::loadHandAndTable(Player player) {
    std::vector<Card> loaded;
    for (Card card : table) {
        loaded.push_back(card);
        if(card.getRank() == 1) {
            loaded.push_back(Card(card.getSuit(), 14));
        }
    }
    for (Card card : player.getHand()) {
        loaded.push_back(card);
        if (card.getRank() == 1) {
            loaded.push_back(Card(card.getSuit(), 14));
        }
    }
    return loaded;
}

std::vector<int> Game::hasFlush(Player player) {
    if (table.size() >= 3) {
        std::unordered_map<Suit, int> countSuits = {{Club, 0}, {Diamond, 0}, {Heart,0}, {Spade,0}};
        std::vector<Card> cards;
        for (Card card : table) {
            countSuits[card.getSuit()] += 1;
            cards.push_back(card);
        }
        for (Card card : player.getHand()) {
            countSuits[card.getSuit()] += 1;
            cards.push_back(card);
        }
        for (auto const& pair : countSuits) {
            //For the suit that has more than or equal to 5 cards
            if (pair.second >= 5) {
                std::vector<int> cardsOfSuit;
                int suit = pair.first;
                std::for_each(cards.begin(), cards.end(), [suit, &cardsOfSuit](Card card) -> bool {
                    if (card.getSuit() == suit) {
                        cardsOfSuit.push_back(card.getRank());
                    }
                });
                std::sort(cardOfSuit.begin(), cardsOfSuit.end());
                return cardsOfSuit;
            }
        }
        //None of the suits have 5 cards
        return {-1};
    } else {
        return {-1};
    }
}

int Game::hasNConsecutive(std::vector<int> cardRanks, int n) {
    std::unordered_set<int> num_set;
    for (int num : cardRanks) {
        num_set.insert(num);
    }

    int longestStreak = 0;
    int currMaxNum = -1;

    for (int num : num_set) {
        if (num_set.find(num-1) == num_set.end()) {
            int currentNum = num;
            int currentStreak = 1;

            while (num_set.find(currentNum+1) != num_set.end()) {
                currentNum += 1;
                currentStreak += 1;
            }

            if (currentStreak >= n && currentNum > currMaxNum) {
                currMaxNum = currentNum;
            }
        }
    }
    return currMaxNum;
}

int Game::hasStraight(Player player) {
    //Put all numbers into a vector and sort and check for 5 consecutive.
    std::vector<Card> loaded = loadHandAndTable(player);
    std::vector<int> loadedInt;
    std::for_each(loaded.begin(), loaded.end(), [&loadedInt](Card card) {
        int rank = card.getRank();
        loadedInt.push_back(rank);
    });

    //Algorithm to find if any 5 consecutively ranked cards
    return hasNConsecutive(loadedInt, 5);

};

/*
* For each card that has the same rank as the straight high card,
* check if there is 5 of the suit of that card.
*/ 
int Game::hasStraightFlush(Player player) {
    //Check if each card in the straight is all part of same suit.
    std::vector<int> playerHasFlush = hasFlush(player);
    if (playerHasFlush[0] != -1) {
        return hasNConsecutive(playerHasFlush, 5);
    }
    //None of the suits had a flush
    return -1;
}

std::vector<int> Game::hasPairTripsQuads(Player player) {
    std::vector<Card> cards = loadHandAndTable(player);
    std::unordered_map<int, int> countRank;
    for (Card card : cards) {
        countRank[card.getRank()] += 1;
    }
    std::vector<int> currHighest = {-1, -1, -1};
    for (auto& pair : countRank) {
        for (int i = 2; i <= 4; i++) {
            if (pair.second >= i) {
                currHighest[i - 2] = std::max(currHighest[i - 2], pair.first);
            }
        }
    }
    return currHighest;
}

std::vector<int> Game::hasFullHouse(Player player) {
    std::vector<Card> cards = loadHandAndTable(player);
    std::unordered_map<int, int> countRank;
    for (Card card : cards) {
        countRank[card.getRank()] += 1;
    }
    //Find highest triple and highest pair
    int highestTriple = -1;
    int highestPair = -1;

    for (auto& pair : countRank) {
        if (pair.second >= 3 && pair.first > highestTriple) {
            highestTriple = pair.first;
        }
        if (pair.second >= 2 && pair.second > highestPair && pair.first != highestTriple) {
            highestPair = pair.first;
        }
    }

    if (highestTriple != -1 && highestPair != -1) {
        return {highestTriple, highestPair};
    }

    return {-1, -1};

}

std::vector<int> Game::hasTwoPair(Player player) {
    std::vector<Card> cards = loadHandAndTable(player);
    std::unordered_map<int, int> countRank;
    for (Card card : cards) {
        countRank[card.getRank()] += 1;
    }
    int currHighest = -1;
    int currSecondHighest = -1;

    for (auto& pair : countRank) {
        if (pair.second >= 2 && pair.first >= currSecondHighest) {
            if (pair.first > currHighest) {
                currSecondHighest = currHighest;
                currHighest = pair.first;
            } else {
                currSecondHighest = pair.first;
            }
        }
    }
    return {currHighest, currSecondHighest};
}

int Game::getHighCard(Player player) {
    std::vector<Card> cards = loadHandAndTable(player);
    int highCard = cards[0].getRank();
    for (Card card : cards) {
        highCard = std::max(highCard, card.getRank());
    }
    return highCard;
}


int Game::handRank(Player player) {
    std::pair<Suit, int> playerHasStraightFlush = hasStraightFlush(player);
    //Royal flush/straight flush
    if (playerHasStraightFlush.second != -1) {
        //If rank of highest card is Ace it is royal flush
        return playerHasStraightFlush.second == 14 ? 1000 + playerHasStraightFlush.first : 900 + playerHasStraightFlush.first;
    }
    
    //Four of a kind/Quads
    std::vector<int> playerHasPairTripsQuads = hasPairTripsQuads(player);
    if (playerHasPairTripsQuads[2] != -1) {
        return 800 + playerHasPairTripsQuads[2];
    }

    //Full house
    std::vector<int> playerHasFullHouse = hasFullHouse(player);
    if (playerHasFullHouse.second != -1) {
        return 700 + playerHasFullHouse[0];
    }

    //Flush
    std::vector<int> playerHasFlush = hasFlush(player);
    if (playerHasFlush[0] != -1) {
        sum_of_ranks = 0;
        for (int n : plyaerHasFlush) sum_of_ranks += n;
        return 600 + sum_of_ranks;
    }

    //Straight
    int playerHasStraight = hasStraight(player);
    if (playerHasStraight != -1) {
        return 500 + playerHasStraight;
    }

    //Three of a kind
    if (playerHasPairTripsQuads[1] != -1) {
        return 400 + playerHasPairTripsQuads[1];
    }

    //Two pair
    std::vector<int> playerHasTwoPair = hasTwoPair(player);
    if (playerHasTwoPair[0] != -1 && playerHasTwoPair[1] != -1) {
        return 300 + playerHasTwoPair[0];
    }

    //Pair
    if (playerHasPairTripsQuads[0] != -1) {
        return 200 + playerHasPairTripsQuads[0];
    
    //High card
    } else {
        return 100 + getHighCard(player);
    }
}

static int compareInt(int A, int B) {
    if (A < B) {
        return -1;
    } else if (A > B) {
        return 1;
    } else {
        return 0;
    }
}



int Game::compareHands(Player playerA, Player playerB) {
    int AType = handType(playerA);
    int BType = handType(playerB);

    if (AType > BType) {
        return 1;
    } else if (AType < BType) {
        return -1;
    } else {
        int compareKickers(std::vector<int> cardsToRemove, int limitOfKickers) {
            std::vector<Card> playerACards = loadHandAndTable(playerA);
            std::vector<int> playerACardsLeftover;
            std::for_each(playerACards.begin(), playerACards.end(), [&playerACardsLeftover, &cardsToRemove] (Card card) {if (cardsToRemove.find(card.getRank()) != cardsToRemove.end()) playerACardsLeftover.push_back(card.getRank();)};
            std::vector<Card> playerBCards = loadHandAndTable(playerB);
            std::vector<int> playerBCardsLeftover;
            std::for_each(playerBCards.begin(), playerBCards.end(), [&playerBCardsLeftover, &cardsToRemove] (Card card) {if (cardsToRemove.find(card.getRank()) != cardsToRemove.end()) playerBCardsLeftover.push_back(card.getRank();)};
            for (int i = 0; i < playerACardsLeftOver.size() && i < playerBCardsLeftover.size() && i < limitOfKickers; i++) {
                if (compareInt(playerACardsLeftOver[i], playerBCardsLeftOver[i]) != 0) {
                    return compareInt(playerACardsLeftOver[i], playerBCardsLeftOver[i]);
                }
            }
            return 0;
        }

        //Full house with same triple, compare the pair.
        if (Atype > 700 && Atype < 800) {
            return compareInt(hasFullHouse(playerA)[1], hasFullHouse(playerB)[1]);
        }
        //Three of a kind: Check up to 2 kickers.
        if (AType > 400 && AType < 500) {
            int tripleRank = AType % 100;
            return compareKickers({tripleRank}, 2);
        }

        //Two pair check second pair, then check 1 kicker.
        if (AType > 300 && AType < 400) {
            int playerASecondPair = hasTwoPair(playerA)[1];
            int playerBSecondPair = hasTwoPair(playerB)[1];
            if (compareInt(playerASecondPair, playerBSecondPair) == 0) {
                return compareKickers({AType % 100, playerASecondPair}, 1);
            } else {
                return compareInt(playerASecondPair, playerBSecondPair);
            }
        }

        //Pair check up to 3 kickers.
        if (AType > 200 && AType < 300) {
            return compareKickers({AType % 100}, 3);
        }

        //High card check up to 4 kickers.
        if (AType > 100 && AType < 200) {
            return compareKickers({AType % 100}, 4);
        }

        //Doesn't reach here
        return 0;

    }
}


void Game::test() {

    table.push_back(Card(Spade, 1));
    table.push_back(Card(Spade, 13));
    table.push_back(Card(Club, 10));
    table.push_back(Card(Heart, 10));
    table.push_back(Card(Spade, 10));
    players[0].getHand().pop_back();
    players[0].getHand().pop_back();
    players[0].getHand().push_back(Card(Diamond, 10));
    players[0].getHand().push_back(Card(Club, 13));


}


Game::Game() {
    this->players = std::vector<Player>();
    this->deck = Deck();
    this->table = std::vector<Card>();
}



void Game::addPlayer(std::string name) {
    Player newPlayer(name);
    players.push_back(newPlayer);
}

void Game::firstDeal() {
    for (Player& player : players) {
        Card firstCard = deck.dealCard();
        player.receiveDeal(firstCard);
        Card secondCard = deck.dealCard();
        player.receiveDeal(secondCard);
    }
}


