#include "os.h" 
#include <unistd.h>

namespace os {
    int Pipe(int pipefd[2]) {
        return pipe(pipefd);
    }

    pid_t Fork() {
        return fork();
    }

    int Dup2(int fd1, int fd2) {
        return dup2(fd1, fd2);
    }

    int Close(int fd) {
        return close(fd);
    }

    int Execl(const char* processPath, const char* processName) {
        execl(processPath, processName, NULL);
        return -1;
    }

    ssize_t Write(int fd, const char* buf, size_t bytes) {
        return write(fd, buf, bytes);
    }  

    void Exit(int status) {
        return _exit(status);
    }

    void Perror(const char* s) {
        perror(s);
    }

    pid_t Wait(pid_t pid) {
        return waitpid(pid, nullptr, 0);
    }
}