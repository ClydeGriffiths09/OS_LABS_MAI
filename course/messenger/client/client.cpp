#include <iostream>

#include "client.hpp"

ChatClient::ChatClient(const std::string& server_addr)
    : ctx(1), dealer(ctx, zmq::socket_type::dealer) {
    dealer.connect(server_addr);
}

bool ChatClient::login(const std::string& login) {
    my_login = login;
    std::string msg = MessageUtils::serialize(LoginRequest{login});
    zmq::message_t empty, data(msg);
    dealer.send(empty, zmq::send_flags::sndmore);
    dealer.send(data, zmq::send_flags::none);
    return true;
}

void ChatClient::sendMessage(const std::string& to, const std::string& text) {
    std::string msg = MessageUtils::serialize(SendMessageRequest{my_login, to, text});
    zmq::message_t empty, data(msg);
    dealer.send(empty, zmq::send_flags::sndmore);
    dealer.send(data, zmq::send_flags::none);
}

void ChatClient::requestHistory(const std::string& with) {
    std::string msg = MessageUtils::serialize(HistoryRequest{my_login, with});
    zmq::message_t empty, data(msg);
    dealer.send(empty, zmq::send_flags::sndmore);
    dealer.send(data, zmq::send_flags::none);
}

void ChatClient::requestSearch(const std::string& query) {
    std::string msg = MessageUtils::serialize(SearchRequest{my_login, query});
    zmq::message_t empty, data(msg);
    dealer.send(empty, zmq::send_flags::sndmore);
    dealer.send(data, zmq::send_flags::none);
}

void ChatClient::receiveLoop() {
    while (running) {
        zmq::message_t empty, data;
        if (dealer.recv(empty) && dealer.recv(data)) {
            std::string str(static_cast<char*>(data.data()), data.size());
            if (str.substr(0, 4) == "MSG:") {
                size_t p = str.find('|', 4);
                if (p != std::string::npos) {
                    std::string from = str.substr(4, p - 4);
                    std::string text = str.substr(p + 1);
                    std::cout << "\n[MSG from " << from << "] " << text << "\nYou: " << std::flush;
                }
            }
            else if (str.substr(0, 12) == "HISTORY_MSG:") {
                size_t p = str.find('|', 12);
                if (p != std::string::npos) {
                    std::string from = str.substr(12, p - 12);
                    std::string text = str.substr(p + 1);
                    std::cout << "\n[HISTORY] " << from << ": " << text << "\nYou: " << std::flush;
                }
            }
            else if (str.substr(0, 14) == "SEARCH_RESULT:") {
                size_t p = str.find('|', 13);
                if (p != std::string::npos) {
                    std::string from = str.substr(13, p - 13);
                    std::string text = str.substr(p + 1);
                    std::cout << "\n[SEARCH] " << from << ": " << text << "\nYou: " << std::flush;
                }
            }
        }
    }
}

void ChatClient::startReceiving() {
    running = true;
    receiver_thread = new Thread([this]() {
        this->receiveLoop();
    });
    receiver_thread->start();
}

void ChatClient::stop() {
    running = false;
    dealer.close();
    if (receiver_thread) {
        receiver_thread->join();
        delete receiver_thread;
        receiver_thread = nullptr;
    }
}