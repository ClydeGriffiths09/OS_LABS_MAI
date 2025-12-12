#include "client.hpp"
#include <iostream>
#include <string>

int main() {
    std::string login;
    std::cout << "Enter your login: ";
    std::getline(std::cin, login);

    ChatClient client("tcp://localhost:5556");
    client.login(login);
    client.startReceiving();

    std::cout << "\nConnected! Commands:\n"
              << "  to:<user> <message>\n"
              << "  history <user>\n"
              << "  search <keyword>\n"
              << "\nYou: " << std::flush;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.substr(0, 3) == "to:") {
            size_t space = line.find(' ', 3);
            if (space != std::string::npos) {
                std::string to = line.substr(3, space - 3);
                std::string text = line.substr(space + 1);
                client.sendMessage(to, text);
            }
        }
        else if (line.substr(0, 8) == "history ") {
            client.requestHistory(line.substr(8));
        }
        else if (line.substr(0, 7) == "search ") {
            client.requestSearch(line.substr(7));
        }
        std::cout << "You: " << std::flush;
    }

    client.stop();
    return 0;
}