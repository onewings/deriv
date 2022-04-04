#pragma once
#include "WsClient.h"

class DelayedMessageHandler : public IHandler
{
public:
    DelayedMessageHandler() = default;
    void OnMessage(const std::string& message) override;

    using MessageHandler = std::function<void(const std::string& message)>;
    DelayedMessageHandler& SetMessageHandler(MessageHandler handler);

private:
    MessageHandler m_handler;
};
