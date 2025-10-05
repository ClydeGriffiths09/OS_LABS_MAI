#pragma once

#include <string>
#include <iostream>

namespace child {
    class Child {
        public:
            static void Run(int childId, const std::string& outputFilename);
            
        private:
            static void ProcessInput(const std::string& outputFilename);
            
            static constexpr int BUFFER_SIZE = 1024;
    };
}