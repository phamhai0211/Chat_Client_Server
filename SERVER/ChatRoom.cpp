#include "ChatRoom.hpp"
#include "Timestamp.hpp"

#include <iostream>
#include <string.h>
#include <cstring>
#include <string>
#include <iterator>
ChatRoom::ChatRoom() :
    max_recent_messages(100)
{
}

void ChatRoom::broadcast(std::shared_ptr<ChatParticipant> participant,
    std::array<char, Protocol::MAX_MESSAGE_SIZE> const& message)
{
    std::array<char, Protocol::MAX_MESSAGE_SIZE> formatted_message;
  // char *data = formatted_message.data();
  //  std::string formatted_message;
    strcpy(formatted_message.data(), Utility::getTimestamp().c_str());
    strcat(formatted_message.data(),format_name(participant->getName()).data());
    strcat(formatted_message.data(),message.data());

    recent_messages.push(formatted_message);

    while (recent_messages.size() > max_recent_messages)
    {
        recent_messages.pop();
    }

    for (auto participant : participants)
    {
        participant->on_message(formatted_message);
    }
}

void ChatRoom::add_participant(std::shared_ptr<ChatParticipant> participant)
{
    participants.insert(participant);

    std::cout << participant->getName().data() << " has joined the chat room.\n";
}

void ChatRoom::remove_participant(std::shared_ptr<ChatParticipant> participant)
{
participants.erase(participant);

    std::cout << participant->getName().data() << " has left the chat room.\n";
}

std::array<char, Protocol::MAX_NAME_SIZE> ChatRoom::format_name(std::array<char,
    Protocol::MAX_NAME_SIZE> name)
{
    if (strlen(name.data()) <= Protocol::MAX_MESSAGE_SIZE - 2)
    {
        strcat(name.data(), ": ");
    }
    else
    {
        name[Protocol::MAX_NAME_SIZE - 2] = ':';
        name[Protocol::MAX_NAME_SIZE - 1] = ' ';
    }

    return name;
}
