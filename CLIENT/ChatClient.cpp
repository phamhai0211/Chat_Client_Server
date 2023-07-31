
#include "ChatClient.hpp"

#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>

#include <iostream>


ChatClient::ChatClient(std::array<char, Protocol::MAX_NAME_SIZE> const& name, boost::asio::io_service& service,
    tcp::resolver::iterator endpoint_iterator) :
    name(name),
    service(service),
    socket(service)
{
    memset(read_message.data(), '\0', Protocol::MAX_MESSAGE_SIZE);

    boost::asio::async_connect(socket, endpoint_iterator,
        [this](auto const& error_code, auto)
        {
            if (!error_code)
            {
                do_name();
            }
        });
}

void ChatClient::write(std::array<char, Protocol::MAX_MESSAGE_SIZE> const& message)
{
    service.post(
        [this, message]()
        {
            auto is_writing = !write_messages.empty();

            write_messages.push(message);

            if (!is_writing)
            {
                do_write();
            }
        });
}

void ChatClient::close()
{
    service.post([this]() { socket.close(); });
}

void ChatClient::do_name()
{
    boost::asio::async_write(socket, boost::asio::buffer(name, name.size()),
        [this](auto const& error_code, auto)
        {
            if (!error_code)
            {
                do_read();
            }
            else
            {
                socket.close();
            }
        });
}

void ChatClient::do_read()
{
    boost::asio::async_read(socket, boost::asio::buffer(read_message, read_message.size()),
        [this](auto const& error_code, auto)
        {
            if (!error_code)
            {
                std::cout << read_message.data() << '\n';

                do_read();
            }
            else
            {
                socket.close();
            }
        });
}

void ChatClient::do_write()
{
    auto const& message = write_messages.front();

    boost::asio::async_write(socket, boost::asio::buffer(message, message.size()),
        [this](auto const& error_code, auto)
        {
            if (!error_code)
            {
                write_messages.pop();

                if (!write_messages.empty())
                {
                    do_write();
                }
            }
            else
            {
                socket.close();
            }
        });
}
