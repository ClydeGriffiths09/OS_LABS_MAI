#include "thread.hpp"
#include <pthread.h>
#include <cstring>

Thread::Thread(ThreadFunc f) : func(std::move(f)) {}

Thread::~Thread() {
    if (is_running) {
        join();
    }
}

void* Thread::threadEntry(void* arg) {
    Thread* self = static_cast<Thread*>(arg);
    if (self->func) {
        self->func();
    }
    return nullptr;
}

void Thread::start() {
    if (is_running) {
        throw std::runtime_error("Thread already running");
    }
    int result = pthread_create(thread_id_ptr, nullptr, threadEntry, this);
    if (result != 0) {
        throw std::runtime_error("Failed to create thread: " + std::string(std::strerror(result)));
    }
    is_running = true;
}

void Thread::join() {
    if (is_running) {
        pthread_join(*thread_id_ptr, nullptr);
        is_running = false;
    }
}