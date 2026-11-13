#include "Venue.hpp"
#include <iostream>
#include <stdexcept>

void Venue::addRoom(const Room& room) {
    if (findRoom(room.id()) != nullptr) {
        throw std::invalid_argument{"Cet identifiant de salle existe déjà"};
    }
    m_rooms.push_back(room);
}

const Room* Venue::findRoom(int id) const {
    for (const auto& room : m_rooms) {
        if (room.id() == id) {
            return &room;
        }
    }
    return nullptr;
}

void Venue::scheduleEvent(std::unique_ptr<Event> event) {
    if (event == nullptr) {
        throw std::invalid_argument{"Evenement nul"};
    }
    const Room* room = findRoom(event->roomId());
    if (room == nullptr) {
        throw std::invalid_argument{"La salle choisie n'existe pas"};
    }
    if (event->capacity() > room->capacity()) {
        throw std::invalid_argument{"La capacite de l'evenement depasse celle de la salle"};
    }
    for (const auto& existing : m_events) {
        if (existing->title() == event->title() &&
            existing->date() == event->date()) {
            throw std::invalid_argument{"Un evenement avec ce titre existe deja a cette date"};
        }
    }
    for (const auto& existing : m_events) {
        if (existing->roomId() == event->roomId() &&
            existing->date() == event->date()) {
            const int newStart = (event->startTime()[0] - '0') * 600 +
                                 (event->startTime()[1] - '0') * 60 +
                                 (event->startTime()[3] - '0') * 10 +
                                 (event->startTime()[4] - '0');
            const int oldStart = (existing->startTime()[0] - '0') * 600 +
                                 (existing->startTime()[1] - '0') * 60 +
                                 (existing->startTime()[3] - '0') * 10 +
                                 (existing->startTime()[4] - '0');
            if (newStart < existing->endMinutes() &&
                oldStart < event->endMinutes()) {
                throw std::invalid_argument{"Conflit d'horaire dans la salle"};
            }
        }
    }
    m_events.push_back(std::move(event));
}

void Venue::displayRooms() const {
    for (const auto& room : m_rooms) {
        std::cout << "Salle " << room.id() << " - " << room.name() << " -capacite : " << room.capacity() << '\n';
    }
}

const std::vector<Room>& Venue::rooms() const { return m_rooms; }
const std::vector<std::unique_ptr<Event>>& Venue::events() const { return m_events; }