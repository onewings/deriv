#include "WsClient.h"

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

WsClient::WsClient()
{
    // remove verbose logging
    m_client.set_access_channels(websocketpp::log::alevel::none);
    m_client.clear_access_channels(websocketpp::log::alevel::none);
    m_client.set_error_channels(websocketpp::log::elevel::none);

    // Initialize ASIO
    m_client.init_asio();

    // Registering handlers
    m_client.set_message_handler([&](websocketpp::connection_hdl hdl, Client::message_ptr msg) {
        return OnMessage(hdl, msg);
    });
    m_client.set_tls_init_handler(
            [&](websocketpp::connection_hdl hdl) {
                return OnTlsInit(hdl);
            });
    m_client.set_open_handler([&](websocketpp::connection_hdl hdl) {
        OnOpen(hdl);
    });
}

WsClient& WsClient::Connect()
{
    websocketpp::lib::error_code ec;
    Client::connection_ptr con = m_client.get_connection(m_uri, ec);
    if (ec)
    {
        std::string msg = "could not create connection because: ";
        msg += ec.message();
        std::cout << msg << std::endl;
        throw std::runtime_error(msg);
    }

    m_client.connect(con);
    return (*this);
}

WsClient& WsClient::SetUri(const std::string& uri)
{
    m_uri = uri;
    return (*this);
}

WsClient& WsClient::SetInstrument(const std::string& instrument)
{
    m_instrument = instrument;
    return (*this);
}

WsClient& WsClient::SetMessageHandler(MessageHandler handler)
{
    m_handler = handler;
    return (*this);
}

void WsClient::Run()
{
    m_client.run();
}

void WsClient::OnMessage(websocketpp::connection_hdl, Client::message_ptr msg)
{
    m_handler(msg->get_payload());
}

void WsClient::OnOpen(websocketpp::connection_hdl hdl)
{
    // now it is safe to use the connection
    // e.g.  R_10
    std::string message = R"({"ticks":")" + m_instrument + R"("})";
    m_client.send(hdl, message.c_str(), websocketpp::frame::opcode::text);
}

websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> WsClient::OnTlsInit(websocketpp::connection_hdl hdl)
{
    (void) hdl;
    websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    try
    {
        ctx->set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::no_sslv2 | boost::asio::ssl::context::no_sslv3 | boost::asio::ssl::context::single_dh_use);
        ctx->set_verify_mode(boost::asio::ssl::verify_none);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return ctx;
}