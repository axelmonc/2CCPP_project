#pragma once
#include "Event.hpp"
#include <string>

class Workshop : public Event {
private:
    std::string m_instructor;

public:
    Workshop(const std::string& title, const std::string& date,
             const std::string& startTime, int duration, int roomId,
             int capacity, double price, int bookedSeats,
             const std::string& instructor);
    std::string type() const override;
    void display(std::ostream& os) const override;
    void save(std::ostream& os) const override;
};