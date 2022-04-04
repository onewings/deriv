#pragma once

class IHandler
{
public:
    virtual void OnMessage(const std::string& message) = 0;
};
