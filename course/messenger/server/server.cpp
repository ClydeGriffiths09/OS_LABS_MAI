#include <iostream>
#include <chrono>

#include "server.hpp"

void ChatServer::initDatabase() {
    if (sqlite3_open("chat_history.db", &db) != SQLITE_OK) {
        throw std::runtime_error("Cannot open database");
    }
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS messages (
            id INTEGER PRIMARY KEY,
            sender TEXT NOT NULL,
            receiver TEXT NOT NULL,
            text TEXT NOT NULL,
            timestamp INTEGER NOT NULL
        );
    )";
    char* err = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &err) != SQLITE_OK) {
        sqlite3_free(err);
    }
}

ChatServer::ChatServer(const std::string& endpoint)
    : ctx(1), router(ctx, zmq::socket_type::router) {
    router.bind(endpoint);
    initDatabase();
    std::cout << "Server started on " << endpoint << "\n";
}

ChatServer::~ChatServer() {
    if (db) {
        sqlite3_close(db);
    }
}

void ChatServer::saveMessageToDB(const std::string& from, const std::string& to, const std::string& text) {
    auto now = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    const char* sql = "INSERT INTO messages (sender, receiver, text, timestamp) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return;
    }

    sqlite3_bind_text(stmt, 1, from.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, to.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, text.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 4, now);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

std::vector<IncomingMessage> ChatServer::getHistoryFromDB(const std::string& user1, const std::string& user2) {
    std::vector<IncomingMessage> res;
    const char* sql = R"(
        SELECT sender, text FROM messages
        WHERE (sender = ? AND receiver = ?) OR (sender = ? AND receiver = ?)
        ORDER BY timestamp ASC;
    )";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return res;
    }

    sqlite3_bind_text(stmt, 1, user1.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user2.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user2.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, user1.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        res.push_back({
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))),
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)))
        });
    }
    sqlite3_finalize(stmt);
    return res;
}

std::vector<IncomingMessage> ChatServer::searchInDB(const std::string& user, const std::string& keyword) {
    std::vector<IncomingMessage> res;
    const char* sql = R"(
        SELECT sender, text FROM messages
        WHERE (receiver = ? OR sender = ?) AND text LIKE ?
        ORDER BY timestamp DESC;
    )";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return res;
    }

    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user.c_str(), -1, SQLITE_STATIC);
    std::string pattern = "%" + keyword + "%";
    sqlite3_bind_text(stmt, 3, pattern.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        res.push_back({
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))),
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)))
        });
    }
    sqlite3_finalize(stmt);
    return res;
}

void ChatServer::deliverMessage(const std::string& to_login, const std::string& msg_str) {
    if (auto it = online_users.find(to_login); it != online_users.end()) {
        std::string identity = it->second;
        zmq::message_t id_msg(identity.data(), identity.size());
        zmq::message_t empty;
        zmq::message_t data(msg_str);
        router.send(id_msg, zmq::send_flags::sndmore);
        router.send(empty, zmq::send_flags::sndmore);
        router.send(data, zmq::send_flags::none);
    }
}

void ChatServer::run() {
    while (true) {
        zmq::message_t identity;
        zmq::message_t empty;
        zmq::message_t data;

        router.recv(identity);
        router.recv(empty);
        router.recv(data);

        std::string identity_str(static_cast<char*>(identity.data()), identity.size());
        std::string data_str(static_cast<char*>(data.data()), data.size());

        if (data_str.substr(0, 6) == "LOGIN:") {
            LoginRequest req;
            if (MessageUtils::deserializeLogin(data_str, req)) {
                online_users[req.user] = identity_str;
                std::cout << "User logged in: " << req.user << "\n";
            }
        }
        else if (data_str.substr(0, 8) == "MESSAGE:") {
            SendMessageRequest req;
            if (MessageUtils::deserializeMessage(data_str, req)) {
                saveMessageToDB(req.from, req.to, req.text);
                auto msg = MessageUtils::serialize(IncomingMessage{req.from, req.text});
                deliverMessage(req.to, msg);
            }
        }
        else if (data_str.substr(0, 8) == "HISTORY:") {
            HistoryRequest req;
            if (MessageUtils::deserializeHistory(data_str, req)) {
                auto history = getHistoryFromDB(req.user, req.with);
                for (const auto& m : history) {
                    std::string msg = "HISTORY_MSG:" + m.from + "|" + m.text;
                    deliverMessage(req.user, msg);
                }
            }
        }
        else if (data_str.substr(0, 7) == "SEARCH:") {
            SearchRequest req;
            if (MessageUtils::deserializeSearch(data_str, req)) {
                auto results = searchInDB(req.user, req.query);
                for (const auto& m : results) {
                    std::string msg = "SEARCH_RESULT:" + m.from + "|" + m.text;
                    deliverMessage(req.user, msg);
                }
            }
        }
    }
}