#pragma once
#include <string>

struct AppConfig {
    std::string telegram_token;
    int accumulation_timeout_ms = 1500;
    std::string default_parse_mode = "MarkdownV2";

    // Loads configuration. Returns true if successful, false otherwise.
    bool load();
};
