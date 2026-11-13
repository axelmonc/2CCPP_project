#pragma once
#include "Event.hpp"
#include "Room.hpp"
#include <memory>
#include <string>
#include <vector>

class Venue {
    private:
        std::vector<Room> m_rooms;
        std::vector<std::unique_ptr<Event>> m_events;

    public:
        void addRoom(const Room& room);
        void scheduleEvent(std::unique_ptr<Event> event);
        bool bookSeats(const std::string& title, const std::string& date, int seats);
        const Room* findRoom(int id) const;
        const Event* findEvent(const std::string& title) const;
        void displayProgramme() const;
        void displayEventsByDate(const std::string& date) const;
        void displayRooms() const;
        const std::vector<Room>& rooms() const;
        const std::vector<std::unique_ptr<Event>>& events() const;
};