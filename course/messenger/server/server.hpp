#pragma once

#include <zmq.hpp>
#include <sqlite3.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "../common/message.hpp"

class ChatServer {
private:
    zmq::context_t ctx;
    zmq::socket_t router;
    std::unordered_map<std::string, std::string> online_users;
    sqlite3* db = nullptr;

    void initDatabase();
    void saveMessageToDB(const std::string& from, const std::string& to, const std::string& text);
    std::vector<IncomingMessage> getHistoryFromDB(const std::string& user1, const std::string& user2);
    std::vector<IncomingMessage> searchInDB(const std::string& user, const std::string& keyword);
    void deliverMessage(const std::string& to_login, const std::string& msg_str);

public:
    explicit ChatServer(const std::string& endpoint = "tcp://*:5556");
    ~ChatServer();
    void run();
};