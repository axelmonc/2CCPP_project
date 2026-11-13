#pragma once
#include "Event.hpp"

class Play : public Event {
private:
    int m_intervalMinutes;

public:
    Play(const std::string& title, const std::string& date,
         const std::string& startTime, int duration, int roomId,
         int capacity, double price, int bookedSeats, int intervalMinutes);
    std::string type() const override;
    void display(std::ostream& os) const override;
    void save(std::ostream& os) const override;
};