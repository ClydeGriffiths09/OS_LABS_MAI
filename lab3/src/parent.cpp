#include <stdexcept>
#include <csignal>

#include "parent.h"
#include "os.h"

namespace parent {

Parent::Parent() {
    CreateSharedMemory();
}

void Parent::CreateSharedMemory() {
    shm_handle1 = os::CreateSharedMemory(shm_name1.c_str(), shm_size);
    if (shm_handle1 == nullptr) {
        throw std::runtime_error("Failed to create shared memory for child1");
    }
    
    shm_ptr1 = os::MapSharedMemory(shm_handle1, shm_size);
    if (!shm_ptr1) {
        throw std::runtime_error("Failed to map shared memory for child1");
    }
    memset(shm_ptr1, 0, shm_size);
    
    shm_handle2 = os::CreateSharedMemory(shm_name2.c_str(), shm_size);
    if (shm_handle2 == nullptr) {
        throw std::runtime_error("Failed to create shared memory for child2");
    }
    
    shm_ptr2 = os::MapSharedMemory(shm_handle2, shm_size);
    if (!shm_ptr2) {
        throw std::runtime_error("Failed to map shared memory for child2");
    }
    memset(shm_ptr2, 0, shm_size);
    
    std::cout << "Shared memory created successfully" << std::endl;
}

void Parent::CreateChildProcesses(std::string filename1, std::string filename2) {
    ProcessParams params1;
    params1.app = "./child";
    params1.arg1 = shm_name1.c_str();
    params1.arg2 = filename1.c_str();
    
    child1 = os::CreateProcess(params1);
    if (child1 == nullptr) {  
        throw std::runtime_error("Failed to create child process 1");
    }

    ProcessParams params2;
    params2.app = "./child";
    params2.arg1 = shm_name2.c_str();
    params2.arg2 = filename2.c_str();
    
    child2 = os::CreateProcess(params2);
    if (child2 == nullptr) {  
        os::TerminateProcess(child1);
        throw std::runtime_error("Failed to create child process 2");
    }

    std::cout << "Child processes created successfully: PID1=" << child1 
                << ", PID2=" << child2 << std::endl;
    std::cout << "Waiting for children to initialize..." << std::endl;
    
    os::Sleep(1);
}

void Parent::Work() {
    std::cout << "Enter strings (type 'exit' or 'quit' to stop):" << std::endl;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "exit" || line == "quit") {
            break;
        }

        if (line.empty()) {
            continue;
        }

        CheckChildrenAlive();

        lineCount++;
        
        if (lineCount % 2 == 1) {
            SendToChild(line, 1);
            std::cout << "Sent to child1 (odd): " << line << std::endl;
        } else {
            SendToChild(line, 2);
            std::cout << "Sent to child2 (even): " << line << std::endl;
        }
    }
    
    std::cout << "Finished processing input" << std::endl;
}

void Parent::SendToChild(const std::string& data, int childNum) {
    char* shm_ptr = (childNum == 1) ? shm_ptr1 : shm_ptr2;
    ProcessHandle child = (childNum == 1) ? child1 : child2;

    strncpy(shm_ptr, data.c_str(), shm_size - 1);
    shm_ptr[shm_size - 1] = '\0';

    os::SendSignal(child, os::SIGNAL_DATA_READY);
}

void Parent::CheckChildrenAlive() {
    if (!os::IsAliveProcess(child1)) {
        throw std::runtime_error("Child process 1 is not alive");
    }
    if (!os::IsAliveProcess(child2)) {
        throw std::runtime_error("Child process 2 is not alive");
    }
}

void Parent::EndChildren() {
    
    if (shm_ptr1) {
        strncpy(shm_ptr1, "TERMINATE", shm_size - 1);
        os::SendSignal(child1, os::SIGNAL_DATA_READY);
    }
    
    if (shm_ptr2) {
        strncpy(shm_ptr2, "TERMINATE", shm_size - 1);
        os::SendSignal(child2, os::SIGNAL_DATA_READY);
    }

    os::Sleep(1);
    
    if (os::IsAliveProcess(child1)) {
        os::TerminateProcess(child1);
    }
    if (os::IsAliveProcess(child2)) {
        os::TerminateProcess(child2);
    }
}

Parent::~Parent() {
    EndChildren();
    
    if (shm_ptr1) {
        os::UnmapSharedMemory(shm_ptr1, shm_size);
    }
    if (shm_ptr2) {
        os::UnmapSharedMemory(shm_ptr2, shm_size);
    }
    
    if (shm_handle1 != nullptr) {
        os::CloseSharedMemory(shm_handle1);
    }
    if (shm_handle2 != nullptr) {
        os::CloseSharedMemory(shm_handle2);
    }
}
}