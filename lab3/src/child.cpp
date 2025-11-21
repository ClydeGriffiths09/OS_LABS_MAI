#include <stdexcept>
#include <csignal>
#include <cstring>
#include <algorithm>

#include "child.h"
#include "os.h"
#include "stringprocessor.h"  

namespace child {

volatile sig_atomic_t Child::data_ready = 0;

void Child::SignalHandler(int sig) {
    if (sig == os::SIGNAL_DATA_READY) {
        data_ready = 1;
    }
}

Child::Child(const std::string& shm_name, const std::string& filename) 
    : shm_name(shm_name), filename(BASEDIRECTORYFORFILES + filename), shm_ptr(nullptr) {
    
    pid = os::GetPid();
    
    
    SharedMemHandle shm_handle = os::CreateSharedMemory(shm_name.c_str(), shm_size);
    if (!shm_handle) {
        throw std::runtime_error("Failed to open shared memory: " + shm_name);
    }
    
    shm_ptr = os::MapSharedMemory(shm_handle, shm_size);
    if (!shm_ptr) {
        throw std::runtime_error("Failed to map shared memory: " + shm_name);
    }

    
    file.open(this->filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + this->filename);
    }

    
    os::Signal(os::SIGNAL_DATA_READY, SignalHandler);

    std::cout << "Child " << pid << " initialized. SHM: " << shm_name 
                << ", File: " << this->filename << std::endl;
}

void Child::Work() {
    std::cout << "Child [" << pid << "] started working..." << std::endl;

    while (true) {
        
        while (!data_ready) {
            os::Pause(); 
        }
        
        data_ready = 0;
        
        std::string input_str(shm_ptr);
        
        if (input_str == "TERMINATE") {
            std::cout << "Child [" << pid << "] received termination signal" << std::endl;
            break;
        }

        std::string result = utils::StringProcessor::RemoveVowels(input_str);
        
        file << result << std::endl;
        file.flush();
        
        std::cout << "Child " << pid << " processed: \"" << input_str 
                    << "\" -> \"" << result << "\"" << std::endl;
        
        memset(shm_ptr, 0, shm_size);
    }

    std::cout << "Child " << pid << " finished work" << std::endl;
}

Child::~Child() {
    if (file.is_open()) {
        file.close();
    }
    
    if (shm_ptr) {
        os::UnmapSharedMemory(shm_ptr, shm_size);
    }
}
}