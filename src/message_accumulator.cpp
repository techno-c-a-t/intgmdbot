#include "message_accumulator.h"
#include "logger.h"
#include <tuple>

MessageAccumulator::MessageAccumulator(int timeout_ms, CallbackType callback)
    : timeout_ms_(timeout_ms), callback_(callback), running_(true) {
    worker_thread_ = std::thread(&MessageAccumulator::workerLoop, this);
}

MessageAccumulator::~MessageAccumulator() {
    stop();
}

void MessageAccumulator::addMessage(int64_t chat_id, int64_t user_id, const std::string& text) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto& session = sessions_[user_id];
    session.chat_id = chat_id;
    session.messages.push_back(text);
    session.last_received = std::chrono::steady_clock::now();
    session.timer_active = true;
}

void MessageAccumulator::stop() {
    if (running_.exchange(false)) {
        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }
    }
}

void MessageAccumulator::workerLoop() {
    while (running_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::vector<std::tuple<int64_t, int64_t, std::string>> triggered;

        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto now = std::chrono::steady_clock::now();
            for (auto& [user_id, session] : sessions_) {
                if (session.timer_active) {
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - session.last_received).count();
                    if (elapsed >= timeout_ms_) {
                        std::string merged;
                        for (const auto& msg : session.messages) {
                            if (!merged.empty()) {
                                merged += "\n\n";
                            }
                            merged += msg;
                        }
                        triggered.push_back({session.chat_id, user_id, merged});

                        session.messages.clear();
                        session.timer_active = false;
                    }
                }
            }
        }

        // Execute callbacks outside the lock to prevent deadlocks
        for (const auto& [chat_id, user_id, merged_text] : triggered) {
            if (callback_) {
                try {
                    callback_(chat_id, user_id, merged_text);
                } catch (const std::exception& e) {
                    log_msg(LogLevel::ERROR, "[Accumulator]", std::string("Exception in message accumulator callback: ") + e.what());
                }
            }
        }
    }
}
