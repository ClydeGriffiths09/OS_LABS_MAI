#include <iostream>
#include <string>
#include <stdexcept>

#include "child.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <shm_name> <output_filename>" << std::endl;
        return 1;
    }

    std::string shm_name = argv[1];
    std::string filename = argv[2];

    try {
        child::Child child(shm_name, filename);
        child.Work();
    } catch (const std::exception& e) {
        std::cerr << "Child error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}