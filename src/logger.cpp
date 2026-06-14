#include "logger.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <mutex>
#include <filesystem>
#include <sstream>

namespace {
std::string g_log_file_path;
std::mutex g_log_mutex;

std::string level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
    }
    return "UNKNOWN";
}
} // namespace

void init_logger(const std::string& log_file_path) {
    std::lock_guard<std::mutex> lock(g_log_mutex);
    g_log_file_path = log_file_path;
    
    // Ensure the parent directory exists
    std::filesystem::path p(log_file_path);
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }
}

void log_msg(LogLevel level, const std::string& tag, const std::string& msg) {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
    std::tm tm_now;
    localtime_r(&time_t_now, &tm_now);
    
    std::ostringstream ss;
    ss << "[" << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S") << "." 
       << std::setfill('0') << std::setw(3) << ms.count() << "] "
       << "[" << level_to_string(level) << "] "
       << "[Thread " << std::this_thread::get_id() << "] "
       << tag << " " << msg << "\n";
    
    std::string formatted = ss.str();
    
    std::lock_guard<std::mutex> lock(g_log_mutex);
    
    // Always print to stdout/stderr
    if (level == LogLevel::ERROR || level == LogLevel::CRITICAL) {
        std::cerr << formatted << std::flush;
    } else {
        std::cout << formatted << std::flush;
    }
    
    // Only write to file if level is above WARNING (i.e. WARNING, ERROR, CRITICAL)
    if (level != LogLevel::INFO && !g_log_file_path.empty()) {
        std::filesystem::path p(g_log_file_path);
        
        // Log rotation: if file size >= 10MB (10 * 1024 * 1024 bytes)
        if (std::filesystem::exists(p)) {
            std::error_code ec;
            auto size = std::filesystem::file_size(p, ec);
            if (!ec && size >= 10 * 1024 * 1024) {
                std::filesystem::path old_p = p;
                old_p = p.parent_path() / (p.stem().string() + ".log.old");
                std::filesystem::remove(old_p, ec);
                std::filesystem::rename(p, old_p, ec);
            }
        }
        
        std::ofstream log_file(g_log_file_path, std::ios::app);
        if (log_file.is_open()) {
            log_file << formatted;
        }
    }
}
