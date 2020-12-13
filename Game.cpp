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

std::pair<Suit, int> Game::hasFlush(Player player) {
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
                std::vector<Card> cardsOfSuit;
                int suit = pair.first;
                std::copy_if(cards.begin(), cards.end(), std::back_inserter(cardsOfSuit), [suit](Card card) -> bool {return card.getSuit() == suit;});
                //If Ace exists, return ace, else, return max
                if (std::count_if(cardsOfSuit.begin(), cardsOfSuit.end(), [](Card card) {return card.getRank() == 1;}) == 1) {
                    return std::make_pair(pair.first, 14);
                } else {
                    Card maxCard = *std::max_element(cards.begin(), cards.end(), [](Card cardA, Card cardB) {return cardA.getRank() < cardB.getRank();});
                    return std::make_pair(pair.first, maxCard.getRank());
                }
            }
        }
        //None of the suits have 5 cards
        return std::make_pair(unknown, -1);
    } else {
        return std::make_pair(unknown, -1);
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

std::pair<Suit, int> Game::hasStraight(Player player) {
    //Put all numbers into a vector and sort and check for 5 consecutive.
    std::vector<Card> loaded = loadHandAndTable(player);
    std::vector<int> loadedInt;
    std::for_each(loaded.begin(), loaded.end(), [&loadedInt](Card card) {
        int rank = card.getRank();
        loadedInt.push_back(rank);
    });

    //Algorithm to find if any 5 consecutively ranked cards
    return std::make_pair(unknown, hasNConsecutive(loadedInt, 5));

};

/*
* For each card that has the same rank as the straight high card,
* check if there is 5 of the suit of that card.
*/ 
std::pair<Suit, int> Game::hasStraightFlush(Player player) {
    std::vector<Card> cards = loadHandAndTable(player);
    //Check if each card in the straight is all part of same suit.
    Suit suit = hasFlush(player).first;
    if (suit != unknown) {
        std::vector<int> suited;
        std::for_each(cards.begin(), cards.end(), [suit, &suited] (Card card) {
            if (card.getSuit() == suit) {
                suited.push_back(card.getRank());
            };
        });
        int hasFiveInSuit = hasNConsecutive(suited, 5);
        if (hasFiveInSuit != -1) {
            return std::make_pair(static_cast<Suit>(suit), hasFiveInSuit);
        }
    }
    //None of the suits had a flush
    return std::make_pair(unknown, -1);
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

std::pair<Suit, int> Game::hasFullHouse(Player player) {
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
        return std::make_pair(unknown, highestTriple);
    }

    return std::make_pair(unknown, -1);

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


int Game::handType(Player player) {
    std::pair<Suit, int> playerHasStraightFlush = hasStraightFlush(player);
    //Royal flush/straight flush
    if (playerHasStraightFlush.second != -1) {
        //If rank of highest card is Ace it is royal flush
        return playerHasStraightFlush.second == 14 ? 10 : 9;
    }
    std::vector<int> playerHasPairTripsQuads = hasPairTripsQuads(player);
    //Quads
    if (playerHasPairTripsQuads[2] != -1) {
        return 8;
    }
    //Full house
    std::pair<Suit, int> playerHasFullHouse = hasFullHouse(player);
    if (playerHasFullHouse.second != -1) {
        return 7;
    }
    //Flush
    std::pair<Suit, int> playerHasFlush = hasFlush(player);
    if (playerHasFlush.second != -1) {
        return 6;
    }
    //Straight
    std::pair<Suit, int> playerHasStraight = hasStraight(player);
    if (playerHasStraight.second != -1) {
        return 5;
    }
    //Three of a kind
    if (playerHasPairTripsQuads[1] != -1) {
        return 4;
    }
    //Two pair
    std::vector<int> playerHasTwoPair = hasTwoPair(player);
    if (playerHasTwoPair[0] != -1 && playerHasTwoPair[1] != -1) {
        return 3;
    }
    //Pair
    if (playerHasPairTripsQuads[0] != -1) {
        return 2;
    } else {
        return 1;
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


    std::vector<Card> cards = loadHandAndTable(players[0]);
    std::for_each(cards.begin(), cards.end(), [](Card card) {std::cout << card << std::endl;});

    std::pair<Suit, int> hasStraightL = hasStraight(players[0]);
    std::pair<Suit, int> hasStraightFlushL = hasStraightFlush(players[0]);
    int hasPairL = hasPairTripsQuads(players[0])[0];
    std::pair<Suit, int> hasFullHouseL = hasFullHouse(players[0]);
    std::cout << hasStraightL.first << " "  << hasStraightL.second << std::endl;
    std::cout << hasStraightFlushL.first << " " << hasStraightFlushL.second<< std::endl;
    std::cout << hasPairL << std::endl;
    std::cout << hasPairTripsQuads(players[0])[1] << std::endl;
    std::cout << hasPairTripsQuads(players[0])[2] << std::endl;
    std::cout << hasTwoPair(players[0])[0] << " " << hasTwoPair(players[0])[1] << std::endl;
    std::cout << hasFullHouseL.second << std::endl;
    std::cout << handType(players[0]) << std::endl;
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


