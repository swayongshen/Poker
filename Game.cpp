#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <limits>

#include "Game.h"
#include "Card.h"

Game::Player::Player(std::string name, int chips) {
    this->name = name;
    this->hand = std::vector<Card>();
    this->chips = chips;
}

Game::Player::Player(std::string name, std::vector<Card> hand, int chips) {
    this->name = name;
    this-> hand = hand;
    this->chips = chips;
}

Game::Player Game::Player::receiveDeal(std::vector<Card> cards) {
    std::vector<Card> newHand = hand;
    for (Card& card : cards) {
        newHand.push_back(card);
    }
    return Player(name, newHand, chips);
}

Game::Player Game::Player::bet(int amt) {
    int newChipsAmt = chips - amt;
    return Player(name, hand, newChipsAmt);
}

std::vector<Card>& Game::Player::getHand() {
    return hand;
}

std::string Game::Player::getName() {
    return name;
}

int Game::Player::getChipAmt() {
    return chips;
}

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
                std::for_each(cards.begin(), cards.end(), [suit, &cardsOfSuit](Card card) {
                    if (card.getSuit() == suit) {
                        cardsOfSuit.push_back(card.getRank());
                    }
                });
                std::sort(cardsOfSuit.begin(), cardsOfSuit.end());
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
    int playerHasStraightFlush = hasStraightFlush(player);
    //Royal flush/straight flush
    if (playerHasStraightFlush != -1) {
        //If rank of highest card is Ace it is royal flush
        return playerHasStraightFlush == 14 ? 1000 + playerHasStraightFlush : 900 + playerHasStraightFlush;
    }
    
    //Four of a kind/Quads
    std::vector<int> playerHasPairTripsQuads = hasPairTripsQuads(player);
    if (playerHasPairTripsQuads[2] != -1) {
        return 800 + playerHasPairTripsQuads[2];
    }

    //Full house
    std::vector<int> playerHasFullHouse = hasFullHouse(player);
    if (playerHasFullHouse[0] != -1) {
        return 700 + playerHasFullHouse[0];
    }

    //Flush
    std::vector<int> playerHasFlush = hasFlush(player);
    if (playerHasFlush[0] != -1) {
        int sum_of_ranks = 0;
        for (int n : playerHasFlush) sum_of_ranks += n;
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

int Game::compareInt(int A, int B) {
    if (A < B) {
        return -1;
    } else if (A > B) {
        return 1;
    } else {
        return 0;
    }
}

int Game::compareKickers(std::vector<int> cardsToRemove, int limitOfKickers, Player playerA, Player playerB) {
    std::vector<Card> playerACards = loadHandAndTable(playerA);
    std::vector<int> playerACardsLeftover;
    std::for_each(playerACards.begin(), playerACards.end(), [&playerACardsLeftover, &cardsToRemove] (Card card) {
        if (std::find(cardsToRemove.begin(), cardsToRemove.end(), card.getRank()) != cardsToRemove.end()) playerACardsLeftover.push_back(card.getRank());
    });
    std::vector<Card> playerBCards = loadHandAndTable(playerB);
    std::vector<int> playerBCardsLeftover;
        std::for_each(playerBCards.begin(), playerBCards.end(), [&playerBCardsLeftover, &cardsToRemove] (Card card) {
        if (std::find(cardsToRemove.begin(), cardsToRemove.end(), card.getRank()) != cardsToRemove.end()) playerBCardsLeftover.push_back(card.getRank());
    });
    for (int i = 0; i < playerACardsLeftover.size() && i < playerBCardsLeftover.size() && i < limitOfKickers; i++) {
        if (compareInt(playerACardsLeftover[i], playerBCardsLeftover[i]) != 0) {
            return compareInt(playerACardsLeftover[i], playerBCardsLeftover[i]);
        }
    }
    return 0;
}

int Game::compareHands(Player playerA, Player playerB) {
    int AType = handRank(playerA);
    int BType = handRank(playerB);

    if (AType > BType) {
        return 1;
    } else if (AType < BType) {
        return -1;
    } else {
        //Full house with same triple, compare the pair.
        if (AType > 700 && AType < 800) {
            return compareInt(hasFullHouse(playerA)[1], hasFullHouse(playerB)[1]);
        }
        //Three of a kind: Check up to 2 kickers.
        if (AType > 400 && AType < 500) {
            int tripleRank = AType % 100;
            return compareKickers({tripleRank}, 2, playerA, playerB);
        }

        //Two pair check second pair, then check 1 kicker.
        if (AType > 300 && AType < 400) {
            int playerASecondPair = hasTwoPair(playerA)[1];
            int playerBSecondPair = hasTwoPair(playerB)[1];
            if (compareInt(playerASecondPair, playerBSecondPair) == 0) {
                return compareKickers({AType % 100, playerASecondPair}, 1, playerA, playerB);
            } else {
                return compareInt(playerASecondPair, playerBSecondPair);
            }
        }

        //Pair check up to 3 kickers.
        if (AType > 200 && AType < 300) {
            return compareKickers({AType % 100}, 3, playerA, playerB);
        }

        //High card check up to 4 kickers.
        if (AType > 100 && AType < 200) {
            return compareKickers({AType % 100}, 4, playerA, playerB);
        }

        //Doesn't reach here
        return 0;

    }
}


void Game::test() {

    // /**
    //  * Setup table:
    //  * S11, S13, C10, H10, S10
    //  */
    // table.push_back(Card(Spade, 9));
    // table.push_back(Card(Spade, 13));
    // table.push_back(Card(Club, 10));
    // table.push_back(Card(Heart, 10));
    // table.push_back(Card(Spade, 10));

    // /**
    //  * Setup player A:
    //  * D10, C13
    //  */
    // players[0].getHand().pop_back();
    // players[0].getHand().pop_back();
    // players[0].getHand().push_back(Card(Diamond, 9));
    // players[0].getHand().push_back(Card(Club, 12));

    // /**
    //  * Setup player B:
    //  * D13, C1
    //  */
    // players[1].getHand().pop_back();
    // players[1].getHand().pop_back();
    // players[1].getHand().push_back(Card(Diamond, 13));
    // players[1].getHand().push_back(Card(Club, 1));

    // std::cout << "player A rank: " << handRank(players[0]) << " | player B rank: " << handRank(players[1]) << std::endl;
    // std::cout << compareHands(players[0], players[1]) << std::endl;

    


}


Game::Game() {
    this->players = std::vector<Player>();
    this->deck = Deck();
    this->table = std::vector<Card>();
    this->pot = 0;
    this->bets = std::vector<int>();
}



void Game::addPlayer(std::string name, int chips) {
    Player newPlayer(name, chips);
    players.push_back(newPlayer);
    bets.push_back(0);
    std::cout << players.size() << std::endl;
}

void Game::firstDeal() {
    for (int i = 0; i < players.size(); i++) {
        players[i] = players[i].receiveDeal({deck.dealCard(), deck.dealCard()});
    }
}

Card Game::draw() {
    return deck.dealCard();
}

void Game::restartDeck() {
    this->deck = Deck();
}

void Game::rotatePlayersLeft(int d) {
    int n = players.size();
    /* To handle if d >= n */
    d = d % n; 
    int g_c_d = std::__gcd(d, n); 
    for (int i = 0; i < g_c_d; i++) { 
        /* move i-th values of blocks */
        Player temp = players[i]; 
        int j = i; 
  
        while (1) { 
            int k = j + d; 
            if (k >= n) 
                k = k - n; 
  
            if (k == i) 
                break; 
  
            players[j] = players[k]; 
            j = k; 
        } 
        players[j] = temp; 
    } 
}

void Game::bet(int playerIndex, int amt) {
    try {
        if (amt <= 0) {
            throw ("Invalid bet, please bet more than $0.");
        } else if (amt > players[playerIndex].getChipAmt()) {
            throw ("Invalid bet, player has insufficient chips.");
        } else {
            players[playerIndex] = players[playerIndex].bet(amt);
            bets[playerIndex] = bets[playerIndex] + amt;
            pot += amt;
        }
    } catch (std::string exception) {
        std::cout << exception << std::endl;
    }
}


void Game::displayTable() {
    std::cout << "------------------------------\n"; 
    std::cout << "Table:" << std::endl;
    
    //Print all cards on table if any.
    for (Card card : table) {
        std::cout << card << " ";
    }
    
    std::cout << "\n\n";
    std::cout << "Pot: $" + std::to_string(pot) << std::endl;
    std::cout << "------------------------------\n";
}

void Game::displayTableAndHand(int playerIndex) {
    displayTable();
    std::cout << players[playerIndex].getName() << " your turn to act!" << std::endl;
    std::vector<Card> hand = players[playerIndex].getHand();
    for (Card card : hand) {
        std::cout << card;
    }
    std::cout << std::endl;
    std::cout << "Your chips: $" + std::to_string(players[playerIndex].getChipAmt()) << std::endl;
    std::cout << std::endl;
}

std::vector<std::string> split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
  }
  return elems;
}

void Game::preflop(int playerIndex) {
    int currBet = *std::max_element(bets.begin(), bets.end());
    if (currBet == bets[playerIndex]) return;
    displayTableAndHand(playerIndex);
    while (true) {
        std::string input;
        std::cout << "Enter C to call the current bet of: $" + std::to_string(currBet) + ","<< std::endl;
        std::cout << "Enter F to fold your hand," << std::endl;
        std::cout << "Enter R X to raise the current bet by $X e.g. R 40" << std::endl;
        
        getline(std::cin, input);
        //Calling
        if (input == "C") {
            if (players[playerIndex].getChipAmt() >= currBet) {
                std::cout << players[playerIndex].name + " calls.\n";
                bet(playerIndex, (currBet - bets[playerIndex]));
                break;
            } else {
                std::cout << "You do not have enough chips.\n";
                continue;
            }
        //Folding
        } else if (input == "F") {
            std::cout << players[playerIndex].name + " folds.\n";
            bets[playerIndex] = -1;
            break;
        //Raising
        } else if (input.length() > 1 && split(input, ' ')[0] == "R") {
            int raiseAmt = std::stoi(split(input, ' ')[1]);
            if (players[playerIndex].getChipAmt() >= currBet + raiseAmt) {
                std::cout << players[playerIndex].name + " raises the bid by $" + std::to_string(raiseAmt) 
                    + " to $" + std::to_string(currBet + raiseAmt) << std::endl;
                bet(playerIndex, (currBet + raiseAmt) - bets[playerIndex]);
                break;
            } else {
                std::cout << "You do not have enough chips.\n";
            }
        } else {
            std::cout << "Invalid input, please try again.\n";
        }
    }
}
