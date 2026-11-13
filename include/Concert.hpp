#pragma once
#include "Event.hpp"
#include <string>

class Concert : public Event {
private:
    std::string m_lineUp;
    int m_soundCheckMinutes;

public:
    Concert(const std::string& title, const std::string& date,
            const std::string& startTime, int duration, int roomId,
            int capacity, double price, int bookedSeats,
            const std::string& lineUp, int soundCheckMinutes);
    std::string type() const override;
    void display(std::ostream& os) const override;
    void save(std::ostream& os) const override;
};