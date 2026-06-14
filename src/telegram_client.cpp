#include "httplib.h"
#include "telegram_client.h"
#include "logger.h"
#include <format>
#include <algorithm>
#include <sstream>

namespace {
// UTF-8 to UTF-16 helper (necessary because Telegram offsets are UTF-16 code units)
std::u16string utf8_to_utf16(const std::string& utf8) {
    std::u16string utf16;
    for (size_t i = 0; i < utf8.size(); ) {
        char32_t cp = 0;
        unsigned char c = utf8[i];
        size_t len = 0;
        if (c < 0x80) { cp = c; len = 1; }
        else if ((c & 0xE0) == 0xC0) { cp = c & 0x1F; len = 2; }
        else if ((c & 0xF0) == 0xE0) { cp = c & 0x0F; len = 3; }
        else if ((c & 0xF8) == 0xF0) { cp = c & 0x07; len = 4; }
        else { i++; continue; }
        
        if (i + len > utf8.size()) break;
        for (size_t j = 1; j < len; ++j) {
            cp = (cp << 6) | (utf8[i + j] & 0x3F);
        }
        i += len;
        
        if (cp < 0x10000) {
            utf16.push_back(static_cast<char16_t>(cp));
        } else {
            cp -= 0x10000;
            utf16.push_back(static_cast<char16_t>((cp >> 10) + 0xD800));
            utf16.push_back(static_cast<char16_t>((cp & 0x3FF) + 0xDC00));
        }
    }
    return utf16;
}

// UTF-16 to UTF-8 helper
std::string utf16_to_utf8(const std::u16string& utf16) {
    std::string utf8;
    for (size_t i = 0; i < utf16.size(); ) {
        char32_t cp = utf16[i];
        if (cp >= 0xD800 && cp <= 0xDBFF && i + 1 < utf16.size()) {
            char32_t trail = utf16[i + 1];
            if (trail >= 0xDC00 && trail <= 0xDFFF) {
                cp = ((cp - 0xD800) << 10) + (trail - 0xDC00) + 0x10000;
                i += 2;
            } else {
                i++;
            }
        } else {
            i++;
        }
        
        if (cp < 0x80) {
            utf8.push_back(static_cast<char>(cp));
        } else if (cp < 0x800) {
            utf8.push_back(static_cast<char>((cp >> 6) | 0xC0));
            utf8.push_back(static_cast<char>((cp & 0x3F) | 0x80));
        } else if (cp < 0x10000) {
            utf8.push_back(static_cast<char>((cp >> 12) | 0xE0));
            utf8.push_back(static_cast<char>(((cp >> 6) & 0x3F) | 0x80));
            utf8.push_back(static_cast<char>((cp & 0x3F) | 0x80));
        } else {
            utf8.push_back(static_cast<char>((cp >> 18) | 0xF0));
            utf8.push_back(static_cast<char>(((cp >> 12) & 0x3F) | 0x80));
            utf8.push_back(static_cast<char>(((cp >> 6) & 0x3F) | 0x80));
            utf8.push_back(static_cast<char>((cp & 0x3F) | 0x80));
        }
    }
    return utf8;
}

// Reconstruct markdown from parsed TelegramEntities
std::string reconstruct_markdown(const std::string& text, const std::vector<TelegramEntity>& entities) {
    if (entities.empty()) return text;
    
    std::u16string u16_text = utf8_to_utf16(text);
    
    struct TagInsertion {
        int pos;
        std::string tag;
        bool is_close;
        int priority;
    };
    
    std::vector<TagInsertion> insertions;
    for (const auto& ent : entities) {
        std::string open_tag, close_tag;
        int priority = 0;
        
        if (ent.type == "bold") {
            open_tag = "**"; close_tag = "**"; priority = 1;
        } else if (ent.type == "italic") {
            open_tag = "_"; close_tag = "_"; priority = 2;
        } else if (ent.type == "underline") {
            open_tag = "__"; close_tag = "__"; priority = 3;
        } else if (ent.type == "strikethrough") {
            open_tag = "~~"; close_tag = "~~"; priority = 4;
        } else if (ent.type == "code") {
            open_tag = "`"; close_tag = "`"; priority = 5;
        } else if (ent.type == "pre") {
            open_tag = "\n```\n"; close_tag = "\n```\n"; priority = 6;
        } else if (ent.type == "text_link") {
            open_tag = "[";
            close_tag = "](" + ent.url + ")";
            priority = 7;
        } else {
            continue;
        }
        
        insertions.push_back({ent.offset, open_tag, false, priority});
        insertions.push_back({ent.offset + ent.length, close_tag, true, priority});
    }
    
    // Sort insertions
    std::sort(insertions.begin(), insertions.end(), [](const TagInsertion& a, const TagInsertion& b) {
        if (a.pos != b.pos) return a.pos < b.pos;
        if (a.is_close != b.is_close) return a.is_close; // Close tag comes first
        if (a.is_close) {
            return a.priority < b.priority;
        } else {
            return a.priority > b.priority;
        }
    });
    
    std::u16string result;
    size_t current_pos = 0;
    for (const auto& ins : insertions) {
        if (ins.pos > (int)current_pos) {
            result += u16_text.substr(current_pos, ins.pos - current_pos);
            current_pos = ins.pos;
        }
        result += utf8_to_utf16(ins.tag);
    }
    if (current_pos < u16_text.size()) {
        result += u16_text.substr(current_pos);
    }
    
    return utf16_to_utf8(result);
}
} // namespace

TelegramClient::TelegramClient(const std::string& token) : token_(token) {
    polling_client_ = std::make_unique<httplib::Client>("https://api.telegram.org");
    api_client_ = std::make_unique<httplib::Client>("https://api.telegram.org");

    // Enable persistent Keep-Alive connections
    polling_client_->set_keep_alive(true);
    api_client_->set_keep_alive(true);

    // Timeout configurations
    polling_client_->set_read_timeout(40, 0);       // Long poll read timeout
    polling_client_->set_connection_timeout(5, 0); // Polling connection timeout

    api_client_->set_read_timeout(10, 0);           // Standard API read timeout
    api_client_->set_connection_timeout(5, 0);     // Standard API connection timeout
}

TelegramClient::~TelegramClient() = default;

nlohmann::json TelegramClient::postRequest(const std::string& method, const nlohmann::json& payload) {
    std::lock_guard<std::mutex> lock(api_mutex_);
    
    std::string path = std::format("/bot{}/{}", token_, method);
    std::string body = payload.dump();

    auto res = api_client_->Post(path.c_str(), body, "application/json");
    if (!res) {
        log_msg(LogLevel::ERROR, "[Client]", std::format("HTTP request failed for method: {}", method));
        return nlohmann::json::object();
    }

    if (res->status != 200) {
        log_msg(LogLevel::ERROR, "[Client]", std::format("HTTP status error {} for method: {}", res->status, method));
        log_msg(LogLevel::ERROR, "[Client]", std::format("Response: {}", res->body));
        try {
            return nlohmann::json::parse(res->body);
        } catch (...) {
            return nlohmann::json::object();
        }
    }

    try {
        auto parsed = nlohmann::json::parse(res->body);
        log_msg(LogLevel::INFO, "[Client]", std::format("Method: {} | Response: {}", method, parsed.dump()));
        return parsed;
    } catch (const std::exception& e) {
        log_msg(LogLevel::ERROR, "[Client]", std::format("JSON parse exception for method {}: {}", method, e.what()));
        return nlohmann::json::object();
    }
}

std::vector<TelegramUpdate> TelegramClient::getUpdates(int64_t offset, int timeout_seconds) {
    nlohmann::json payload = {
        {"offset", offset},
        {"timeout", timeout_seconds},
        {"allowed_updates", {"message"}}
    };

    std::string path = std::format("/bot{}/getUpdates", token_);
    std::string body = payload.dump();

    auto res = polling_client_->Post(path.c_str(), body, "application/json");
    if (!res) {
        log_msg(LogLevel::ERROR, "[Client]", "HTTP request failed for getUpdates");
        return {};
    }

    nlohmann::json response;
    try {
        response = nlohmann::json::parse(res->body);
    } catch (const std::exception& e) {
        log_msg(LogLevel::ERROR, "[Client]", std::string("JSON parse exception for getUpdates response: ") + e.what());
        return {};
    }

    std::vector<TelegramUpdate> updates;
    if (response.contains("ok") && response["ok"].get<bool>() && response.contains("result")) {
        auto result = response["result"];
        for (const auto& item : result) {
            TelegramUpdate update;
            update.update_id = item["update_id"].get<int64_t>();

            if (item.contains("message")) {
                auto msg = item["message"];
                update.has_message = true;
                update.message_id = msg["message_id"].get<int64_t>();
                update.chat_id = msg["chat"]["id"].get<int64_t>();
                if (msg.contains("from")) {
                    update.user_id = msg["from"]["id"].get<int64_t>();
                }
                
                std::string raw_text;
                if (msg.contains("text")) {
                    raw_text = msg["text"].get<std::string>();
                }

                std::vector<TelegramEntity> entities;
                if (msg.contains("entities")) {
                    for (const auto& ent : msg["entities"]) {
                        TelegramEntity entity;
                        entity.type = ent["type"].get<std::string>();
                        entity.offset = ent["offset"].get<int>();
                        entity.length = ent["length"].get<int>();
                        if (ent.contains("url")) {
                            entity.url = ent["url"].get<std::string>();
                        }
                        entities.push_back(entity);
                    }
                }
                update.entities = entities;
                update.text = reconstruct_markdown(raw_text, entities);
            }
            updates.push_back(update);
        }
    }
    return updates;
}

SendResult TelegramClient::sendMessage(int64_t chat_id, const std::string& text, const std::string& parse_mode) {
    nlohmann::json payload = {
        {"chat_id", chat_id},
        {"text", text},
        {"link_preview_options", {
            {"prefer_large_media", true}
        }}
    };
    if (!parse_mode.empty()) {
        payload["parse_mode"] = parse_mode;
    }

    nlohmann::json response = postRequest("sendMessage", payload);
    SendResult result;
    if (response.contains("ok") && response["ok"].get<bool>()) {
        result.ok = true;
    } else if (response.contains("description")) {
        result.error_description = response["description"].get<std::string>();
    }
    return result;
}

SendResult TelegramClient::sendRichMessage(int64_t chat_id, const nlohmann::json& rich_message) {
    nlohmann::json payload = {
        {"chat_id", chat_id},
        {"rich_message", rich_message},
        {"link_preview_options", {
            {"prefer_large_media", true}
        }}
    };

    nlohmann::json response = postRequest("sendRichMessage", payload);
    SendResult result;
    if (response.contains("ok") && response["ok"].get<bool>()) {
        result.ok = true;
    } else if (response.contains("description")) {
        result.error_description = response["description"].get<std::string>();
    }
    return result;
}
