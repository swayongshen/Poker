#include <SFML/Network.hpp>
#include <vector>

void acceptConnections(sf::TcpListener listener, std::vector<sf::TcpSocket>& playerClients);