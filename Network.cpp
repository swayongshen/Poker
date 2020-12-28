#include <SFML/Network.hpp>
#include <vector>
#include <iostream>

#include <Network.h>

void acceptConnections(sf::TcpListener& listener, std::vector<sf::TcpSocket>& playerClients, int& numPlayers) {
    while (true) {
        // accept a new connection
        sf::TcpSocket client;
        if (listener.accept(client) != sf::Socket::Done) {
            std::cout << "Error accepting new connection\n";
        } else {
            numPlayers += 1;
            playerClients.push_back(client);
            sf::Packet packet;
            client.receive(packet);
            std::string playerName;
            packet >> playerName;
        }
    }
}