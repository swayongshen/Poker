#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <thread>
#include <atomic>
#include <SFML/Network.hpp>
#include <csignal>
#include <random>

#include "Game.h"
#include "Utils.h"
#include "Printer.h"

std::atomic<bool> isStop(false);
sf::TcpListener listener;

void stopThread(int s) {
    listener.close();
    exit(1);
}

class Main {
private:
    Game game;
    int maxNumPlayers;
    int smallBlindAmt;
    std::atomic<bool> isStop;
    std::thread acceptConnectionsThread;
    std::shared_ptr<Printer> printer;
    Utils utils;

public:
    Main(std::shared_ptr<Printer> printer) : game(printer), utils(printer) {
        this->maxNumPlayers = this->utils.getMaxNumPlayers();
        this->smallBlindAmt = this->utils.getSmallBlindAmt();
        this->utils.clearStdin();
        //If CTRL + C pressed, stop thread
        signal(SIGINT, stopThread);
        signal(SIGSTOP, stopThread);
        this->printer = printer;
    }

    void bindListenerToPort(int port) {
        // bind the listener to a port
        while (listener.listen(port) != sf::Socket::Done) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(500ms);
            this->printer->print("Error binding TCP listener to port " + std::to_string(port));
        }
    }

    void startThreadAcceptConnections() {
        this->printer->print("Starting separate thread to accept client connections \n");
        //Start thread which continuously accepts new connections until numPlayers == maxPlayer
        this->acceptConnectionsThread = std::thread(&Game::acceptConnections, std::ref(this->game),
                                                    std::unique_ptr<sf::TcpListener>(&listener),
                                                    maxNumPlayers, std::ref(isStop));
    }

    void waitForEnoughPlayers() {
        this->printer->print("Waiting for at least 2 players to join...\n");
        game.waitForEnoughPlayers();
    }

    void chooseFirstDealer() {
        //Decide who is the first dealer.
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, game.getNumPlayers() - 1);
        int dealerPosition = distribution(generator);
        this->printer->print("Player at index " + std::to_string(dealerPosition) +
                             " has been chosen randomly to be the first dealer.\n\n");

        //Rotate the players vector to make dealer the first player in vector.
        game.rotatePlayersLeft(dealerPosition);

        //Reshuffle after using cards to decide who to be dealer
        game.restartDeck();
    }

    void prepareBeforeNextRound() {
        //After every round, the player after the dealer becomes the next dealer
        game.rotatePlayersLeft(1);
        game.resetCards();
        game.confirmIfPlayersWantToContinue();
        game.checkConnectedAll();
        while (game.numActivePlayers < 2) {
            game.broadcastMsg("WAIT");
            game.waitForEnoughPlayers();
            game.checkConnectedAll();
        }
    }

    void prepareNewRound() {
        game.printStatus("NEW GAME");

        //Small blind, big blind added to pot.
        game.blindsBid(smallBlindAmt);
    }

    /**
     * Runs the pre-flop round of the poker game
     * @return
     */
    bool runPreFlop() {
        //Deal pocket cards
        game.firstDeal();
        //Each player after the big blind starting from the under the gun starts their pre-flop action
        game.printStatus("PRE-FLOP");
        game.displayTable();
        game.preFlopRound();
        if (game.hasWinner() != -1) {
            game.awardWinners();
            return true;
        }
        return false;
    }

    bool runFlop() {
        // Burns 1 card and draws 3 card
        game.dealFlop();
        // Round 2 betting after flop, starting from small blind after dealer.
        game.printStatus("FLOP");
        game.displayTableAndHand();
        game.postFlopRound();
        if (game.hasWinner() != -1) {
            game.awardWinners();
            return true;
        }
        return false;
    }

    bool runTurn() {
        //Burns another card and deal 1 card for the turn
        game.dealTurnOrRiver();
        game.printStatus("TURN");
        game.displayTableAndHand();
        game.postFlopRound();
        if (game.hasWinner() != -1) {
            game.awardWinners();
            return true;
        }
        return false;
    }

    void runRiver() {
        //Burns a card and deal 1 card for the river.
        game.dealTurnOrRiver();
        game.printStatus("RIVER");
        game.displayTableAndHand();
        game.postFlopRound();
        //At this point, winner has to be determined.
        game.awardWinners();
    }

    void stopAcceptingConnections() {
        this->acceptConnectionsThread.join();
        listener.close();
    }
};

/**
 * Game follows instructions from 
 * https://www.instructables.com/Learn-To-Play-Poker---Texas-Hold-Em-aka-Texas-Ho/
 */
int main() {
    Main main(std::shared_ptr<Printer>(new Printer));
    /**
     * Networking
     */
    main.bindListenerToPort(53000);
    main.startThreadAcceptConnections();

    try {
        main.waitForEnoughPlayers();
        main.chooseFirstDealer();

        int round = 0;
        while (true) {
            if (round != 0) {
                main.prepareBeforeNextRound();
            }
            round++;
            main.prepareNewRound();

            bool gameEndedPreFlop = main.runPreFlop();
            if (gameEndedPreFlop) {
                continue;
            }

            bool gameEndedFlop = main.runFlop();
            if (gameEndedFlop) {
                continue;
            }

            bool gameEndedTurn = main.runTurn();
            if (gameEndedTurn) {
                continue;
            }

            main.runRiver();
            fscanf(stdin, "c");
        }
    } catch (...) {
        main.stopAcceptingConnections();
        fscanf(stdin, "c");
    }
}