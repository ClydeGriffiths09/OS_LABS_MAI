#include "parent.h"
#include <iostream>
#include <string>

int main() {
    try {
        std::string filename1, filename2;
        
        std::cout << "Enter filename for child1: ";
        std::getline(std::cin, filename1);
        
        std::cout << "Enter filename for child2: ";
        std::getline(std::cin, filename2);
        
        parent::Parent parent;
        parent.CreateChildProcesses(filename1, filename2);
        parent.Work();
        
        std::cout << "Program finished successfully" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}