
#include "DelayedMessageHandler.h"

void DelayedMessageHandler::OnMessage(const std::string& message)
{
    m_handler(message);
    
}

DelayedMessageHandler& DelayedMessageHandler::SetMessageHandler(MessageHandler handler)
{
    m_handler = handler;
    return (*this);
}