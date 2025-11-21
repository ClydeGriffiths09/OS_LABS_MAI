#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <cstdint> 

#include "os.h"

namespace os {

ProcessHandle CreateProcess(const ProcessParams& params) {
    pid_t pid = fork();
    
    if (pid == -1) {
        return nullptr;
    }
    
    if (pid == 0) {
        
        if (params.arg3) {
            execl(params.app, params.app, params.arg1, params.arg2, params.arg3, nullptr);
        } else if (params.arg2) {
            execl(params.app, params.app, params.arg1, params.arg2, nullptr);
        } else if (params.arg1) {
            execl(params.app, params.app, params.arg1, nullptr);
        } else {
            execl(params.app, params.app, nullptr);
        }
        
        Exit(1);
    }
    
    return reinterpret_cast<ProcessHandle>(static_cast<uintptr_t>(pid));
}

void TerminateProcess(ProcessHandle handle) {
    if (handle == nullptr) return;
    pid_t pid = static_cast<pid_t>(reinterpret_cast<uintptr_t>(handle));
    kill(pid, SIGTERM);
    waitpid(pid, nullptr, 0);
}

bool IsAliveProcess(ProcessHandle handle) {
    if (handle == nullptr) return false;
    pid_t pid = static_cast<pid_t>(reinterpret_cast<uintptr_t>(handle));
    return kill(pid, 0) == 0;
}

int GetChildPid(ProcessHandle handle) {
    if (handle == nullptr) return -1;
    return static_cast<int>(reinterpret_cast<uintptr_t>(handle));
}

SharedMemHandle CreateSharedMemory(const char* name, size_t size) {
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        return nullptr;
    }
    
    if (ftruncate(shm_fd, size) == -1) {
        close(shm_fd);
        return nullptr;
    }
    
    return reinterpret_cast<SharedMemHandle>(static_cast<uintptr_t>(shm_fd));
}

char* MapSharedMemory(SharedMemHandle handle, size_t size) {
    if (handle == nullptr) return nullptr;
    int shm_fd = static_cast<int>(reinterpret_cast<uintptr_t>(handle));
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    if (ptr == MAP_FAILED) {
        return nullptr;
    }
    
    return static_cast<char*>(ptr);
}

void UnmapSharedMemory(char* ptr, size_t size) {
    if (ptr != nullptr) {
        munmap(ptr, size);
    }
}

void CloseSharedMemory(SharedMemHandle handle) {
    if (handle == nullptr) return;
    int shm_fd = static_cast<int>(reinterpret_cast<uintptr_t>(handle));
    close(shm_fd);
}

void Signal(int signal, SignalHandle handler) {
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(signal, &sa, nullptr);
}

void SendSignal(ProcessHandle handle, int signal) {
    if (handle == nullptr) return;
    pid_t pid = static_cast<pid_t>(reinterpret_cast<uintptr_t>(handle));
    kill(pid, signal);
}

int GetPid() {
    return getpid();
}

int Pause() {
    return pause();
}

void Sleep(int seconds) {
    sleep(seconds);
}

void Exit(int status) {
    _exit(status);
}
}