#include "child.h"
#include <iostream>
#include <string>

int main() {
    std::string filename;
    std::getline(std::cin, filename);
    
    child::Child::Run(1, filename);
    
    return 0;
}