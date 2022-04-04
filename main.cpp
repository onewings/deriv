#include <iostream>
#include "WsClient.h"
#include "DelayedMessageHandler.h"

int main(int argc, char* argv[])
{
    std::string instrument;
    std::string uri = "wss://ws.binaryws.com/websockets/v3?app_id=1089";

    if (argc == 2)
    {
        instrument = argv[1];
    }
    else
    {
        std::cerr << "Invalid number of arguments" << std::endl;
        std::cout << "Usage:" << std::endl;
        std::cout << "deriv_test <instrument>" << std::endl;
        return 1;
    }

    try
    {
        DelayedMessageHandler delayedMsgHandler;
        //here you can set your own handler
        delayedMsgHandler.SetMessageHandler([](const std::string& message) {
            std::cout << message << std::endl;
        });

        WsClient client;
        client.SetUri(uri).SetInstrument(instrument).SetMessageHandler([&](const std::string& message) {
                                                        delayedMsgHandler.OnMessage(message);
                                                    })
                .Connect()
                .Run();
    }
    catch (websocketpp::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
}