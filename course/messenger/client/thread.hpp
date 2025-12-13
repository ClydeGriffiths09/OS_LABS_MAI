#pragma once

#include <pthread.h>
#include <functional>
#include <stdexcept>

class Thread {
public:
    using ThreadFunc = std::function<void()>;

private:
    pthread_t thread_id;
    ThreadFunc func;
    bool is_running = false;

    static void* threadEntry(void* arg) {
        Thread* self = static_cast<Thread*>(arg);
        if (self->func) {
            self->func();
        }
        return nullptr;
    }

public:
    explicit Thread(ThreadFunc f) : func(std::move(f)) {}

    ~Thread() {
        if (is_running) {
            join();
        }
    }

    void start() {
        if (is_running) {
            throw std::runtime_error("Thread already running");
        }
        int result = pthread_create(&thread_id, nullptr, threadEntry, this);
        if (result != 0) {
            throw std::runtime_error("Failed to create thread: " + std::string(std::strerror(result)));
        }
        is_running = true;
    }

    void join() {
        if (is_running) {
            pthread_join(thread_id, nullptr);
            is_running = false;
        }
    }

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
};