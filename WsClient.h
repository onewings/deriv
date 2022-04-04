#pragma once

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include "IHandler.h"

class WsClient
{

public:
    WsClient();
    WsClient& SetUri(const std::string& uri);
    WsClient& SetInstrument(const std::string& instrument);

    using MessageHandler = std::function<void(const std::string& message)>;
    WsClient& SetMessageHandler(MessageHandler handler);
    WsClient& Connect();
    void Run();

private:
    using Client = websocketpp::client<websocketpp::config::asio_tls_client>;

    void OnMessage(websocketpp::connection_hdl, Client::message_ptr msg);
    void OnOpen(websocketpp::connection_hdl hdl);
    websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> OnTlsInit(websocketpp::connection_hdl);

    Client m_client;
    std::string m_uri;
    std::string m_instrument;
    MessageHandler m_handler;
};
