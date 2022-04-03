#include <iostream>
#include "WsClient.h"


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
        WsClient c;
        c.SetUri(uri).SetInstrument(instrument).Connect().Run();
    }
    catch (websocketpp::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
}