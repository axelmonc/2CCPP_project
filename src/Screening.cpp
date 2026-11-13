#include "Screening.hpp"
#include <ostream>
#include <stdexcept>

Screening::Screening(const std::string& title, const std::string& date,
                     const std::string& startTime, int duration, int roomId,
                     int capacity, double price, int bookedSeats,
                     const std::string& rating)
    : Event{title, date, startTime, duration, roomId, capacity, price, bookedSeats},
      m_rating{rating} {
    if (rating.empty() || rating.find(';') != std::string::npos) {
        throw std::invalid_argument{"Classification invalide (vide ou contenant ';')"};
    }
}

std::string Screening::type() const { return "Projection"; }

void Screening::display(std::ostream& os) const {
    Event::display(os);
    os << " | Classification : " << m_rating;
}

void Screening::save(std::ostream& os) const {
    os << "SCREENING;" << title() << ';' << date() << ';' << startTime()
       << ';' << duration() << ';' << roomId() << ';' << capacity()
       << ';' << price() << ';' << bookedSeats() << ';' << m_rating << '\n';
}