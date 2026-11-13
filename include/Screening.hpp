#pragma once
#include "Event.hpp"
#include <string>

class Screening : public Event {
private:
    std::string m_rating;

public:
    Screening(const std::string& title, const std::string& date,
              const std::string& startTime, int duration, int roomId,
              int capacity, double price, int bookedSeats,
              const std::string& rating);
    std::string type() const override;
    void display(std::ostream& os) const override;
    void save(std::ostream& os) const override;
};