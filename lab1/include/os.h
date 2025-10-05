#pragma once

#include <cstddef>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>

namespace os {
    int Pipe(int pipefd[2]);

    pid_t Fork();

    int Dup2(int fd1, int fd2);

    int Close(int fd);

    int Execl(const char* processPath, const char* processName);

    ssize_t Write(int fd, const char* buf, size_t bytes);

    void Exit(int status);
    
    void Perror(const char* s);
    
    pid_t Wait(pid_t pid);
}