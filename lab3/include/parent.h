#pragma once

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <random>
#include <string>

#include "os.h"

namespace parent {
class Parent {
private:
    ProcessHandle child1 = nullptr;
    ProcessHandle child2 = nullptr;
    SharedMemHandle shm_handle1 = nullptr;
    SharedMemHandle shm_handle2 = nullptr;
    char* shm_ptr1 = nullptr;
    char* shm_ptr2 = nullptr;
    std::string shm_name1 = "/shm_child1";
    std::string shm_name2 = "/shm_child2";
    const size_t shm_size = SHMSIZE;
    
    int lineCount = 0;

    void CreateSharedMemory();
    void SendToChild(const std::string& data, int childNum);
    void CheckChildrenAlive();

public:
    Parent();
    void CreateChildProcesses(std::string filename1, std::string filename2);
    void Work();
    void EndChildren();
    ~Parent();
};
}