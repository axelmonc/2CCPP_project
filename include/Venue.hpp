#pragma once
#include "Room.hpp"
#include <vector>

class Venue {
    private:
        std::vector<Room> m_rooms;
        
    public:
        void addRoom(const Room& room);
        const Room* findRoom(int id) const;
        void displayRooms() const;
        const std::vector<Room>& rooms() const;
};