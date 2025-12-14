#pragma once

#include <string>

// --- Запросы от клиента ---
struct LoginRequest {
    std::string user;
};

struct SendMessageRequest {
    std::string from;
    std::string to;
    std::string text;
};

struct HistoryRequest {
    std::string user;
    std::string with;
};

struct SearchRequest {
    std::string user;
    std::string query;
};

struct IncomingMessage {
    std::string from;
    std::string text;
};


namespace MessageUtils {

std::string serialize(const LoginRequest& r);
std::string serialize(const SendMessageRequest& r);
std::string serialize(const HistoryRequest& r);
std::string serialize(const SearchRequest& r);
std::string serialize(const IncomingMessage& m);

bool deserializeLogin(const std::string& s, LoginRequest& r);
bool deserializeMessage(const std::string& s, SendMessageRequest& r);
bool deserializeHistory(const std::string& s, HistoryRequest& r);
bool deserializeSearch(const std::string& s, SearchRequest& r);

}