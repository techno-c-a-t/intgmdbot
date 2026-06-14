#pragma once
#include <string>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

// Initializes the logger (sets log file path)
void init_logger(const std::string& log_file_path);

// Log message with specific level, tag, and string
void log_msg(LogLevel level, const std::string& tag, const std::string& msg);
