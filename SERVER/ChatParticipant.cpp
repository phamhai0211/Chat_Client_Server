#include "ChatParticipant.hpp"


std::array<char, Protocol::MAX_NAME_SIZE> const& ChatParticipant::getName()
{
    return name;
}
