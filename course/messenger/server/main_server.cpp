#include "server.hpp"
#include <iostream>

int main() {
    try {
        ChatServer server("tcp://*:5556");
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}