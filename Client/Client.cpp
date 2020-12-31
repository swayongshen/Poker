#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <iostream>

void sendMsg(sf::TcpSocket& socket, std::string msg) {
    sf::Packet sendPkt;
    sendPkt << msg;
    socket.send(sendPkt);
}

void sendMsg(sf::TcpSocket& socket, int i) {
    sf::Packet sendPkt;
    sendPkt << i;
    socket.send(sendPkt);
}

bool isContinueGame() {
    std::string continueGame;
    while(true) {
        std::cout << "Continue game? (Y/N).\n";
        std::cin >> continueGame;
        if (!std::cin.fail() && (continueGame == "N" || continueGame == "Y" || continueGame == "n" || continueGame == "y")) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            if (continueGame == "N" || continueGame == "n") {
                std::cout << "Thank you for playing!\n";
                return false;
            } else {
                return true;
            }
        } else {
            std::cout << "Invalid input, try again.\n";
        }
    }
    //Won't reach
    return false;
}

int main() {
    sf::TcpSocket socket;
    std::string ipAddr = "127.0.0.1";
    int portNumber = 53000;
    sf::Socket::Status status = socket.connect(ipAddr, portNumber);
    if (status != sf::Socket::Done) {
        std::cout << "Error connecting to " + ipAddr + ":" + std::to_string(portNumber) << std::endl;
        return 1;
    }

    std::cout << "Please enter a name: ";
    std::string input;
    std::getline(std::cin, input);
    sendMsg(socket, input);

    while (true) {
        sf::Packet promptPacket;
        socket.receive(promptPacket);
        if (promptPacket.getDataSize() == 0) {
            std::cout << "Server has closed, stopping client now.\n";
            break;
        }
        std::string promptString;
        promptPacket >> promptString;
        if (promptString.substr(0, 3) == "ACT") {
            std::cout << promptString.substr(3, promptString.length() - 3) << std::endl;
            std::string input;
            std::getline(std::cin, input);
            sendMsg(socket, input);
        } else if (promptString.substr(0, 3) == "END") {
            if (isContinueGame()) {
                sendMsg(socket, 1);
            } else {
                socket.disconnect();
            }
        } else {
            std::cout << promptString << std::endl;
        }
    }
}