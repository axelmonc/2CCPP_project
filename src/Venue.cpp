#include "Venue.hpp"
#include <iostream>
#include <stdexcept>

void Venue::addRoom(const Room& room) {
    if (findRoom(room.id()) != nullptr) {
        throw std::invalid_argument{"Cet identifiant de salle existe déjà"};
    }
    m_rooms.push_back(room);
}

void Venue::displayRooms() const {
    for (const auto& room : m_rooms) {
        std::cout << "Salle " << room.id() << " - " << room.name() << " -capacite : " << room.capacity() << '\n';
    }
}

const std::vector<Room>& Venue::rooms() const { return m_rooms; }