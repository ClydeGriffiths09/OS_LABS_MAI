#pragma once

#include <cstddef>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <child.h>

namespace parent {
    class Parent {
        private:
            static constexpr int BUFFER_SIZE = 1024;
            static constexpr int PIPE_READ = 0;
            static constexpr int PIPE_WRITE = 1;

            int pipe1[2];
            int pipe2[2];
            pid_t pid1;
            pid_t pid2;
            int lineCount;

        public:
            Parent() = default;
            void Run();
            void Clean();
            bool CreateChildProcess(const std::string& filename1, const std::string& filename2);
            ~Parent() = default;

        private:
            bool CreatePipes();
            void ProcessUserInput();
            void SendToPipe(const std::string& line, int pipeNum);
    };
}