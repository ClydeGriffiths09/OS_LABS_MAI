#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>

#include "os.h"
#include "stringprocessor.h"

inline std::string BASEDIRECTORYFORFILES = "";

namespace child {
class Child {
private:
    int pid;
    std::string filename;
    std::ofstream file;
    char* shm_ptr;
    size_t shm_size = SHMSIZE;
    std::string shm_name;
    
    static volatile sig_atomic_t data_ready;
    static void SignalHandler(int sig);

public:
    Child(const std::string& shm_name, const std::string& filename);
    void Work();
    ~Child();
};
}