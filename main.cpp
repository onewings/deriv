#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

void on_message(websocketpp::connection_hdl, client::message_ptr msg) {
    std::cout << msg->get_payload() << std::endl;
}

void on_open(client* c, const std::string& instrument, websocketpp::connection_hdl hdl) 
{
    // now it is safe to use the connection
    //e.g.  R_10
    std::string message = R"({"ticks":")"+instrument+R"("})";
    c->send(hdl, message.c_str(), websocketpp::frame::opcode::text);
}

context_ptr on_tls_init(websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);

    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);
        ctx->set_verify_mode(boost::asio::ssl::verify_none);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return ctx;
}

int main(int argc, char* argv[]) {

    std::string instrument;
    std::string uri = "wss://ws.binaryws.com/websockets/v3?app_id=1089";

    if (argc == 2) {
        instrument = argv[1];
    }
    else
    {
        std::cerr << "Invalid number of arguments" << std::endl;
        std::cout << "Usage:" << std::endl;
        std::cout << "deriv_test <instrument>"<< std::endl;
        return 1;
    }

    client c;
    try {
        // remove verbose logging
        c.set_access_channels(websocketpp::log::alevel::none);
        c.clear_access_channels(websocketpp::log::alevel::none);
        c.set_error_channels(websocketpp::log::elevel::none);

        // Initialize ASIO
        c.init_asio();

        // Registering handlers
        c.set_message_handler(&on_message);
        c.set_open_handler(bind(&on_open,&c,instrument,::_1));
        c.set_tls_init_handler(&on_tls_init);

        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return 0;
        }

        c.connect(con);
        c.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}