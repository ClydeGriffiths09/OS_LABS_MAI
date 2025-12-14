#pragma once

#include <functional>
#include <stdexcept>
#include <pthread.h>

class Thread {
public:
    using ThreadFunc = std::function<void()>;

private:
    pthread_t* thread_id_ptr = nullptr; 
    ThreadFunc func;
    bool is_running = false;

    static void* threadEntry(void* arg);

public:
    explicit Thread(ThreadFunc f);
    ~Thread();

    void start();
    void join();

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
};