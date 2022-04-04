#pragma once
#include "WsClient.h"

class DelayedMessageHandler : public IHandler
{
public:
    DelayedMessageHandler();
    ~DelayedMessageHandler();
    void OnMessage(const std::string& message) override;

    using MessageHandler = std::function<void(const std::string& message)>;
    DelayedMessageHandler& SetMessageHandler(MessageHandler handler);

    void Stop();

private:
    MessageHandler m_handler;
    using DequeElement = std::pair<std::chrono::steady_clock::time_point, std::string>;
    std::deque<DequeElement> m_messages;

    // Thread management
    std::thread m_thread;
    std::mutex m_mutex;

    enum state_t
    {
        NONE,
        RUNNING,
        STOP_REQ,
        JOINING,
        JOINED
    };
    state_t m_state {state_t::NONE};
    std::condition_variable m_cond;

    void ThreadBody();
    bool TryStop();
};
