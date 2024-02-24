//
// Created by Swa, Yong Shen on 21/2/24.
//
#include "Utils.h"
#include <iostream>

int Utils::getMaxNumPlayers() {
    this->printer->print("Enter the max number of players: ");
    int maxPlayers;
    while (true) {
        std::cin >> maxPlayers;
        if (!std::cin.fail() && maxPlayers > 1 && maxPlayers <= 11) {
            this->printer->print("\n");
            break;
        } else {
            this->printer->print("Please enter a valid number of players: ");
        }
    }
    return maxPlayers;
}

int Utils::getSmallBlindAmt() {
    this->printer->print("Please enter the small blind amount for the game: ");
    int smallBlindAmt;
    while (true) {
        std::cin >> smallBlindAmt;
        if (!std::cin.fail() && smallBlindAmt > 0) {
            this->printer->print("\n");
            break;
        } else {
            this->printer->print("Please enter a number greater than 0: ");
        }
    }
    return smallBlindAmt;
}

void Utils::clearStdin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

Utils::Utils(std::shared_ptr<Printer> printer) {
    this->printer = printer;
}
