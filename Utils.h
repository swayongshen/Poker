//
// Created by Swa, Yong Shen on 21/2/24.
//

#ifndef RUNPOKER_UTILS_H
#define RUNPOKER_UTILS_H

#include <memory>
#include "Printer.h"

class Utils {
public:
    Utils(std::shared_ptr<Printer> printer);

    /**
     * Prompts the game admin for a number 2 <= num <= 11 for the max number of players allowed in a game
     * @return the max number of players allowed
     */
    int getMaxNumPlayers();

    int getSmallBlindAmt();

    void clearStdin();

private:
    std::shared_ptr<Printer> printer;
};


#endif //RUNPOKER_UTILS_H
