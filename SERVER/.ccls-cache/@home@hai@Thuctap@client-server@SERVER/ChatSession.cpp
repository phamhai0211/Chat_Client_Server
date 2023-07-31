
#include "ChatSession.hpp"

#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <queue>

ChatSession::ChatSession(ChatRoom& chat_room, tcp::socket socket) :
    chat_room(chat_room),
    socket(std::move(socket))
{
}

void ChatSession::on_message(std::array<char, Protocol::MAX_MESSAGE_SIZE> const& message)
{
    auto is_writing = !write_messages.empty();

    write_messages.push(message);

    if (!is_writing)
    {
        do_write();
    }
}

void ChatSession::start()
{
    do_name();
}

void ChatSession::do_name()
{
    auto self(shared_from_this());

    boost::asio::async_read(socket, boost::asio::buffer(name, name.size()),
        [this, self](auto const& error_code, auto)
        {
            chat_room.add_participant(shared_from_this());

            do_read();
        });
}

void ChatSession::do_read()
{
    auto self(shared_from_this());

    boost::asio::async_read(socket, boost::asio::buffer(read_message, read_message.size()),
        [this, self](auto const& error_code, auto)
        {
            if (!error_code)
            {
                chat_room.broadcast(shared_from_this(), read_message);

                do_read();
            }
            else
            {
                chat_room.remove_participant(shared_from_this());
            }
        });
}

void ChatSession::do_write()
{
    auto self(shared_from_this());

    auto const& message = write_messages.front();

    boost::asio::async_write(socket, boost::asio::buffer(message, message.size()),
        [this, self](auto const& error_code, auto)
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
                chat_room.remove_participant(shared_from_this());
            }
        });
}
