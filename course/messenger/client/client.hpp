#pragma once

#include <zmq.hpp>
#include <string>
#include <thread>
#include <atomic>

#include "../common/message.hpp"
#include "thread.hpp"

class ChatClient {
private:
    zmq::context_t ctx;
    zmq::socket_t dealer;
    std::string my_login;
    std::atomic<bool> running{true};
    Thread* receiver_thread = nullptr;

    void receiveLoop();

public:
    explicit ChatClient(const std::string& server_addr);
    bool login(const std::string& login);
    void sendMessage(const std::string& to, const std::string& text);
    void requestHistory(const std::string& with);
    void requestSearch(const std::string& query);
    void startReceiving();
    void stop();
};