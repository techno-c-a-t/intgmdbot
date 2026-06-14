#include "config.h"
#include "message_accumulator.h"
#include "telegram_client.h"
#include "logger.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <format>

// Replaces non-breaking spaces with normal spaces to make GFM list indentation work correctly
std::string replace_nbsps(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    for (size_t i = 0; i < str.size(); ) {
        if (i + 1 < str.size() && (unsigned char)str[i] == 0xC2 && (unsigned char)str[i+1] == 0xA0) {
            result += ' ';
            i += 2;
        } else {
            result += str[i];
            i++;
        }
    }
    return result;
}

// Balance unclosed formatting tags to prevent RICH_MESSAGE_MARKDOWN_INVALID
std::string balance_markdown(const std::string& text) {
    std::string balanced = text;
    std::vector<std::string> open_tags;
    
    size_t i = 0;
    while (i < text.size()) {
        if (text.compare(i, 3, "```") == 0) {
            if (!open_tags.empty() && open_tags.back() == "```") {
                open_tags.pop_back();
            } else {
                open_tags.push_back("```");
            }
            i += 3;
        } else if (text.compare(i, 2, "**") == 0) {
            if (!open_tags.empty() && open_tags.back() == "**") {
                open_tags.pop_back();
            } else {
                open_tags.push_back("**");
            }
            i += 2;
        } else if (text.compare(i, 2, "__") == 0) {
            if (!open_tags.empty() && open_tags.back() == "__") {
                open_tags.pop_back();
            } else {
                open_tags.push_back("__");
            }
            i += 2;
        } else if (text.compare(i, 2, "~~") == 0) {
            if (!open_tags.empty() && open_tags.back() == "~~") {
                open_tags.pop_back();
            } else {
                open_tags.push_back("~~");
            }
            i += 2;
        } else if (text[i] == '`') {
            if (!open_tags.empty() && open_tags.back() == "`") {
                open_tags.pop_back();
            } else {
                open_tags.push_back("`");
            }
            i++;
        } else if (text[i] == '_') {
            if (!open_tags.empty() && open_tags.back() == "_") {
                open_tags.pop_back();
            } else {
                open_tags.push_back("_");
            }
            i++;
        } else {
            i++;
        }
    }
    
    // Append closing tags in reverse order
    for (auto it = open_tags.rbegin(); it != open_tags.rend(); ++it) {
        balanced += *it;
    }
    return balanced;
}

int main() {
    // Initialize rotating file logger (saves WARNING and above to bot.log)
    init_logger("bot.log");
    log_msg(LogLevel::INFO, "[System]", "Starting Telegram Bot (DM Accumulator only)");

    AppConfig config;
    if (!config.load()) {
        log_msg(LogLevel::CRITICAL, "[System]", "Failed to load configuration!");
        return 1;
    }

    log_msg(LogLevel::INFO, "[System]", std::format("Default Parse Mode: {}", config.default_parse_mode));
    log_msg(LogLevel::INFO, "[System]", std::format("Accumulation Timeout: {} ms", config.accumulation_timeout_ms));

    // Initialize the Telegram client
    TelegramClient client(config.telegram_token);

    // Setup the message accumulator callback
    MessageAccumulator accumulator(
        config.accumulation_timeout_ms,
        [&client](int64_t chat_id, int64_t user_id, const std::string& merged_text) {
            log_msg(LogLevel::INFO, "[Accumulator]", std::format("Processing accumulated message for user {}", user_id));
            log_msg(LogLevel::INFO, "[Accumulator]", std::format("Sending Rich Message. Length: {} characters.", merged_text.size()));
            
            std::string balanced = balance_markdown(merged_text);
            std::string sanitized = replace_nbsps(balanced);

            // Build the new Bot API 10.1 Rich Message payload
            nlohmann::json rich_message = {
                {"markdown", sanitized}
            };
            
            bool success = client.sendRichMessage(chat_id, rich_message);
            if (success) {
                log_msg(LogLevel::INFO, "[Accumulator]", "Rich Message sent successfully.");
            } else {
                log_msg(LogLevel::ERROR, "[Accumulator]", "Failed to send Rich Message.");
            }
        }
    );

    log_msg(LogLevel::INFO, "[System]", "Starting long polling loop. Press Ctrl+C to exit.");

    int64_t last_update_id = 0;
    while (true) {
        try {
            auto updates = client.getUpdates(last_update_id + 1, 10); // 10s poll timeout
            for (const auto& update : updates) {
                last_update_id = std::max(last_update_id, update.update_id);
                
                if (update.has_message) {
                    log_msg(LogLevel::INFO, "[DM]", std::format("Queuing message from user {} (size: {})", update.user_id, update.text.size()));
                    accumulator.addMessage(update.chat_id, update.user_id, update.text);
                }
            }
        } catch (const std::exception& e) {
            log_msg(LogLevel::ERROR, "[System]", std::format("Exception in main loop: {}", e.what()));
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    // Stop accumulator background thread cleanly
    accumulator.stop();
    return 0;
}
