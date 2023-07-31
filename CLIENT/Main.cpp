#include "Protocol.hpp"
#include "ChatClient.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>

#include <array>
#include <thread>
#include <iostream>
#include <wx-3.2/wx/wx.h>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{

    try
    {
        if (argc != 4)
        {
            std::cerr << "Usage: PalindromeNetworkClient <name> <host> <port>\n";

            return 1;
        }

        std::array<char, Protocol::MAX_NAME_SIZE> name;
        strcpy(name.data(), argv[1]);

        boost::asio::io_service service;

        tcp::resolver resolver(service);
        auto endpoint_iterator = resolver.resolve({ argv[2], argv[3] });

        ChatClient chat_client(name, service, endpoint_iterator);

        std::thread thread([&service]() { service.run(); });

        std::array<char, Protocol::MAX_MESSAGE_SIZE> message;

        while (true)
        {
            memset(message.data(), '\0', message.size());

            if (std::cin.getline(message.data(), Protocol::MAX_MESSAGE_SIZE - Protocol::PADDING_SIZE - Protocol::MAX_NAME_SIZE))
            {
                std::cin.clear();
            }

            chat_client.write(message);
        }

        chat_client.close();
        thread.join();
    }
    catch (std::exception const& exception)
    {
        std::cerr << "Exception: " << exception.what() << '\n';
    }
}
