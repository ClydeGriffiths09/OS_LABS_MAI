#pragma once

#include <cstddef>
#include <initializer_list>

using ProcessHandle = void*;
using SharedMemHandle = void*;

using SignalHandle = void(*)(int);

constexpr size_t SHMSIZE = 2048;

struct ProcessParams {
    const char* app;
    const char* arg1 = nullptr;
    const char* arg2 = nullptr;
    const char* arg3 = nullptr;
};

namespace os {

ProcessHandle CreateProcess(const ProcessParams& params);
void TerminateProcess(ProcessHandle handle);
bool IsAliveProcess(ProcessHandle handle);

int GetChildPid(ProcessHandle handle);

SharedMemHandle CreateSharedMemory(const char* name, size_t size);
char* MapSharedMemory(SharedMemHandle handle, size_t size);
void UnmapSharedMemory(char* ptr, size_t size);
void CloseSharedMemory(SharedMemHandle handle);

const int SIGNAL_DATA_READY = 10;
void Signal(int signal, SignalHandle handle);
void SendSignal(ProcessHandle handle, int signal);

int GetPid();
int Pause();
void Sleep(int seconds);
void Exit(int status);
}