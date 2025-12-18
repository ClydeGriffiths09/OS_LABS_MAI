#include <iostream>
#include <string>

#include "client.hpp"

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
              << "  exit\n"
              << "\nYou: " << std::flush;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "exit") {
            std::cout << "Disconnecting...\n";
            break;
        }
        else if (line.substr(0, 3) == "to:") {
            size_t space = line.find(' ', 3);
            if (space != std::string::npos && space > 3) {
                std::string to = line.substr(3, space - 3);
                std::string text = line.substr(space + 1);
                if (!to.empty() && !text.empty()) {
                    client.sendMessage(to, text);
                } else {
                    std::cout << "\n[ERROR] Invalid 'to:' command. Format: to:<user> <message>\nYou: " << std::flush;
                }
            } else {
                std::cout << "\n[ERROR] Invalid 'to:' command. Format: to:<user> <message>\nYou: " << std::flush;
            }
        }
        else if (line.substr(0, 8) == "history ") {
            std::string with = line.substr(8);
            if (!with.empty()) {
                client.requestHistory(with);
            } else {
                std::cout << "\n[ERROR] Missing user. Format: history <user>\nYou: " << std::flush;
            }
        }
        else if (line.substr(0, 7) == "search ") {
            std::string query = line.substr(7);
            if (!query.empty()) {
                client.requestSearch(query);
            } else {
                std::cout << "\n[ERROR] Missing keyword. Format: search <keyword>\nYou: " << std::flush;
            }
        }
        else {
            std::cout << "\n[ERROR] Unknown command. Use:\n"
                    << "  to:<user> <message>\n"
                    << "  history <user>\n"
                    << "  search <keyword>\n"
                    << "  exit\n"
                    << "You: " << std::flush;
        }
    }

    client.logout();
    client.stop();
    std::cout << "Goodbye!\n";
    return 0;
}