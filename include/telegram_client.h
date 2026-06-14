#pragma once
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>

namespace httplib {
    class Client;
}

struct TelegramEntity {
    std::string type;
    int offset = 0;
    int length = 0;
    std::string url;
};

struct TelegramUpdate {
    int64_t update_id = 0;
    
    // Message info (optional)
    bool has_message = false;
    int64_t message_id = 0;
    int64_t chat_id = 0;
    int64_t user_id = 0;
    std::string text;
    std::vector<TelegramEntity> entities;
};

struct SendResult {
    bool ok = false;
    std::string error_description;
};

class TelegramClient {
public:
    TelegramClient(const std::string& token);
    ~TelegramClient();

    // Long polling updates. Returns list of updates.
    std::vector<TelegramUpdate> getUpdates(int64_t offset, int timeout_seconds = 30);

    // Send text message (MarkdownV2 or HTML)
    SendResult sendMessage(int64_t chat_id, const std::string& text, const std::string& parse_mode = "");

    // Send Rich Message
    SendResult sendRichMessage(int64_t chat_id, const nlohmann::json& rich_message);

private:
    nlohmann::json postRequest(const std::string& method, const nlohmann::json& payload);

    std::string token_;
    std::unique_ptr<httplib::Client> polling_client_;
    std::unique_ptr<httplib::Client> api_client_;
    std::mutex api_mutex_;
};
