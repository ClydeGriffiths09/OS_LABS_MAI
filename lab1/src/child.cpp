#include "child.h"
#include "stringprocessor.h"
#include <iostream>
#include <fstream>
#include <unistd.h>

namespace child {

    void Child::Run(int childId, const std::string& outputFilename) {
        std::cout << "Child" << childId << " started, output file: " << outputFilename << std::endl;
        
        ProcessInput(outputFilename);
        
        std::cout << "Child" << childId << " finished" << std::endl;
    }

    void Child::ProcessInput(const std::string& outputFilename) {
        std::ofstream outfile(outputFilename);
        if (!outfile.is_open()) {
            std::cerr << "Error opening file: " << outputFilename << std::endl;
            return;
        }

        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;

        while ((bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            std::string input_str(buffer);
            std::string result = utils::StringProcessor::RemoveVowels(input_str);

            outfile << result << std::endl;
            std::cout << "Child processed: \"" << input_str << "\" -> \"" << result << "\"" << std::endl;
            std::cout.flush();
        }

        outfile.close();
    }
}