#include <parent.h>
#include <os.h>

namespace parent {
    void Parent::Run() {
        std::string filename1;
        std::string filename2;

        std::cout << "Enter the name for child1: ";
        std::getline(std::cin, filename1);

        std::cout << "Enter the name for child2: ";
        std::getline(std::cin, filename2);

        if (!CreatePipes()) {
            return;
        }

        if (!CreateChildProcess(filename1, filename2)) {
            return;
        }

        ProcessUserInput();
        Clean();
    }

    bool Parent::CreatePipes() {
        if (os::Pipe(pipe1) == -1 || os::Pipe(pipe2) == -1) {
            os::Perror("pipe");
            return false;
        }
        return true;
    }

    bool Parent::CreateChildProcess(const std::string& filename1, const std::string& filename2) {
        pid1 = os::Fork();
        if (pid1 == -1) {
            os::Perror("fork pid1 failed");
            return false;
        }

        if (pid1 == 0) {
            os::Close(pipe1[PIPE_WRITE]);
            os::Close(pipe2[PIPE_READ]);
            os::Close(pipe2[PIPE_WRITE]);

            os::Dup2(pipe1[PIPE_READ], STDIN_FILENO);
            os::Close(pipe1[PIPE_READ]);

            child::Child::Run(1, filename1);
            os::Exit(1);
        }

        pid2 = os::Fork();
        if (pid2 == -1) {
            os::Perror("fork pid2 failed");
            return false;
        }

        if (pid2 == 0) {
            os::Close(pipe2[PIPE_WRITE]);
            os::Close(pipe1[PIPE_READ]);
            os::Close(pipe1[PIPE_WRITE]);

            os::Dup2(pipe2[PIPE_READ], STDIN_FILENO);
            os::Close(pipe2[PIPE_READ]);

            child::Child::Run(2, filename2);
            os::Exit(1);
        }

        os::Close(pipe1[PIPE_READ]);
        os::Close(pipe2[PIPE_READ]);

        return true;
    }

    void Parent::ProcessUserInput() {
        std::cout << "Enter strings (empty line to stop): " << std::endl;

        std::string line;
        lineCount = 0;

        while(std::getline(std::cin, line)) {
            if (line.empty()) {
                break;
            }

            lineCount++;

            if (lineCount % 2 == 1) {
                SendToPipe(line, 1);
                std::cout << "Sent to pipe1: " << line << std::endl;
            } else {
                SendToPipe(line, 2);
                std::cout << "Sent to pipe2: " << line << std::endl;
            }
        }
    }

    void Parent::SendToPipe(const std::string& line, int pipeNum) {
        if (pipeNum == 1) {
            os::Write(pipe1[PIPE_WRITE], line.c_str(), line.size());
        } else {
            os::Write(pipe2[PIPE_WRITE], line.c_str(), line.size());
        }
    }
    
    void Parent::Clean() {
        os::Close(pipe1[PIPE_WRITE]);
        os::Close(pipe2[PIPE_WRITE]);

        os::Wait(pid1);
        os::Wait(pid2);

        std::cout << "Finished" << std::endl;
    }
}