#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>

struct UserSession {
    int64_t chat_id = 0;
    std::vector<std::string> messages;
    std::chrono::steady_clock::time_point last_received;
    bool timer_active = false;
};

class MessageAccumulator {
public:
    // Callback is executed when the idle timer expires.
    using CallbackType = std::function<void(int64_t chat_id, int64_t user_id, const std::string& merged_text)>;

    MessageAccumulator(int timeout_ms, CallbackType callback);
    ~MessageAccumulator();

    // Add a message to the accumulator for a user
    void addMessage(int64_t chat_id, int64_t user_id, const std::string& text);

    // Stops the worker thread
    void stop();

  private:
    void workerLoop();

    int timeout_ms_;
    CallbackType callback_;

    std::unordered_map<int64_t, UserSession> sessions_;
    std::mutex mutex_;

    std::thread worker_thread_;
    std::atomic<bool> running_;
};
