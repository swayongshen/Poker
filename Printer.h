//
// Created by Swa, Yong Shen on 21/2/24.
//

#ifndef RUNPOKER_PRINTER_H
#define RUNPOKER_PRINTER_H


#include <sstream>
#include <iostream>

class Printer final {
private:
    std::stringstream s;

    template<typename T>
    void accumulate(T &&t) {
        s << std::forward<T>(t);
    }

    template<typename T, typename ... Ts>
    void accumulate(T &&t, Ts &&... ts) {
        s << std::forward<T>(t);
        accumulate(std::forward<Ts>(ts)...);
    }

public:
    template<typename ... Ts>
    void print(Ts &&... ts) {
        //lock

        accumulate(std::forward<Ts>(ts)...);
        std::cout << s.str();

        s.str(std::string());
        s.clear();

        //unlock
    }
};


#endif //RUNPOKER_PRINTER_H
