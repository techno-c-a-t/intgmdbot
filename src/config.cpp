#include "config.h"
#include "logger.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool AppConfig::load() {
    // 1. Load public configuration
    try {
        std::ifstream configFile("config.json");
        if (configFile.is_open()) {
            json j;
            configFile >> j;
            if (j.contains("accumulation_timeout_ms")) {
                accumulation_timeout_ms = j["accumulation_timeout_ms"].get<int>();
            }
            if (j.contains("default_parse_mode")) {
                default_parse_mode = j["default_parse_mode"].get<std::string>();
            }
            log_msg(LogLevel::INFO, "[Config]", "Loaded config.json successfully.");
        } else {
            log_msg(LogLevel::WARNING, "[Config]", "Could not open config.json, using defaults.");
        }
    } catch (const std::exception& e) {
        log_msg(LogLevel::ERROR, "[Config]", std::string("Error parsing config.json: ") + e.what());
    }

    // 2. Load sensitive secrets
    try {
        std::ifstream secretsFile("secrets.json");
        if (!secretsFile.is_open()) {
            log_msg(LogLevel::CRITICAL, "[Config]", "Could not open secrets.json (needed for Telegram Token).");
            return false;
        }
        json j;
        secretsFile >> j;
        if (j.contains("telegram_token")) {
            telegram_token = j["telegram_token"].get<std::string>();
        } else {
            log_msg(LogLevel::CRITICAL, "[Config]", "secrets.json does not contain 'telegram_token'.");
            return false;
        }
        log_msg(LogLevel::INFO, "[Config]", "Loaded secrets.json successfully.");
    } catch (const std::exception& e) {
        log_msg(LogLevel::CRITICAL, "[Config]", std::string("Error parsing secrets.json: ") + e.what());
        return false;
    }

    return !telegram_token.empty();
}
