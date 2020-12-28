#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <iostream>

int main() {
    sf::TcpSocket socket;
    std::string ipAddr = "127.0.0.1";
    int portNumber = 53000;
    sf::Socket::Status status = socket.connect(ipAddr, portNumber);
    if (status != sf::Socket::Done) {
        std::cout << "Error connecting to " + ipAddr + ":" + std::to_string(portNumber) << std::endl;
        return 1;
    }
    sf::Packet namePacket;
    std::cout << "Please enter a name: ";
    std::string input;
    std::getline(std::cin, input);
    namePacket << input;
    socket.send(namePacket);
}