#pragma once

#include <string>

struct LoginRequest {
    std::string user;
};

struct SendMessageRequest {
    std::string from, to, text;
};

struct HistoryRequest {
    std::string user, with;
};

struct SearchRequest {
    std::string user, query;
};

struct IncomingMessage {
    std::string from, text;
};

namespace MessageUtils {

// Сериализация
inline std::string serialize(const LoginRequest& r) {
    return "LOGIN:" + r.user;
}

inline std::string serialize(const SendMessageRequest& r) {
    return "MESSAGE:" + r.from + "|" + r.to + "|" + r.text;
}

inline std::string serialize(const HistoryRequest& r) {
    return "HISTORY:" + r.user + "|" + r.with;
}

inline std::string serialize(const SearchRequest& r) {
    return "SEARCH:" + r.user + "|" + r.query;
}

inline std::string serialize(const IncomingMessage& m) {
    return "MSG:" + m.from + "|" + m.text;
}

// Десериализация
inline bool deserializeLogin(const std::string& s, LoginRequest& r) {
    if (s.size() < 7 || s.substr(0, 6) != "LOGIN:") return false;
    r.user = s.substr(6);
    return true;
}

inline bool deserializeMessage(const std::string& s, SendMessageRequest& r) {
    if (s.size() < 9 || s.substr(0, 8) != "MESSAGE:") return false;
    size_t p1 = s.find('|', 8);
    if (p1 == std::string::npos) return false;
    size_t p2 = s.find('|', p1 + 1);
    if (p2 == std::string::npos) return false;
    r.from = s.substr(8, p1 - 8);
    r.to = s.substr(p1 + 1, p2 - p1 - 1);
    r.text = s.substr(p2 + 1);
    return true;
}

inline bool deserializeHistory(const std::string& s, HistoryRequest& r) {
    if (s.size() < 9 || s.substr(0, 8) != "HISTORY:") return false;
    size_t p = s.find('|', 8);
    if (p == std::string::npos) return false;
    r.user = s.substr(8, p - 8);
    r.with = s.substr(p + 1);
    return true;
}

inline bool deserializeSearch(const std::string& s, SearchRequest& r) {
    if (s.size() < 8 || s.substr(0, 7) != "SEARCH:") return false;
    size_t p = s.find('|', 7);
    if (p == std::string::npos) return false;
    r.user = s.substr(7, p - 7);
    r.query = s.substr(p + 1);
    return true;
}

} // namespace MessageUtils