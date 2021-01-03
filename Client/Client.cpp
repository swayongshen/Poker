#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <iostream>
#include <thread>

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

std::string receiveMsg(sf::TcpSocket& socket) {
    sf::Packet receivePkt;
    socket.receive(receivePkt);
    std::string receiveStr;
    receivePkt >> receiveStr;
    return receiveStr;
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
                std::cout << "Thx\n";
                return true;
            }
        } else {
            std::cout << "Invalid input, try again.\n";
        }
    }
    //Won't reach
    std::cout << "Reach\n";
    return false;
}



int main() {
    /**
     * Establish connection to server
     */
    sf::TcpSocket socket;
    std::string ipAddr = "127.0.0.1";
    int portNumber = 53000;
    sf::Socket::Status status = socket.connect(ipAddr, portNumber);
    if (status != sf::Socket::Done) {
        std::cout << "Error connecting to " + ipAddr + ":" + std::to_string(portNumber) << std::endl;
        return 1;
    }
    //Get name and send to server
    std::cout << "Please enter a name: ";
    std::string input;
    std::getline(std::cin, input);
    sendMsg(socket, input);

    bool isWaiting = false;
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
            isWaiting = false;
            std::cout << promptString.substr(3, promptString.length() - 3) << std::endl;
            std::string input;

            //Use separate thread to achieve 20s timeout for input
            std::thread inputTimeout([&input] () {
                std::getline(std::cin, input);
            });
            int timeWaited = 0;
            while (input.length() < 1 && timeWaited < 20) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                timeWaited += 1;
            }
            if (input.length() < 1) {
                input = "F";
                std::cout << "!!- You have folded due to timeout. -!!\n";
                inputTimeout.detach();
            } else {
                inputTimeout.join();
            }

            sendMsg(socket, input);
        } else if (promptString == "CHECK") {
            std::cout << "Get check\n";
            sendMsg(socket, 1);
        } else if (promptString == "END") {
            if (!isContinueGame()) {
                socket.disconnect();
                break;
            }
        } else if (promptString == "WAIT") {
            if (!isWaiting) {
                std::cout << "Waiting for more players\n";
            }
            isWaiting = true;
        } else {
            isWaiting = false;
            std::cout << promptString << std::endl;
        }
    }
}