#include "Room.hpp"
#include <stdexcept>

Room::Room(int id, const std::string& name, int capacity)
    : m_id{id}, m_name{name}, m_capacity{capacity} {
    if (id <= 0 || name.empty() || name.find(';') != std::string::npos || capacity <= 0) {
        throw std::invalid_argument{"Informations de salle invalides"};
    }
}
