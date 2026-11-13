#include "Concert.hpp"
#include <ostream>
#include <stdexcept>

Concert::Concert(const std::string& title, const std::string& date,
                 const std::string& startTime, int duration, int roomId,
                 int capacity, double price, int bookedSeats,
                 const std::string& lineUp, int soundCheckMinutes)
    : Event{title, date, startTime, duration, roomId, capacity, price, bookedSeats},
      m_lineUp{lineUp}, m_soundCheckMinutes{soundCheckMinutes} {
    if (lineUp.empty() || lineUp.find(';') != std::string::npos || soundCheckMinutes < 0) {
        throw std::invalid_argument{"Informations de concert invalides"};
    }
}

std::string Concert::type() const { return "Concert"; }

void Concert::display(std::ostream& os) const {
    Event::display(os);
    os << " | Artistes : " << m_lineUp
       << " | Balance son : " << m_soundCheckMinutes << " min";
}

void Concert::save(std::ostream& os) const {
    os << "CONCERT;" << title() << ';' << date() << ';' << startTime()
       << ';' << duration() << ';' << roomId() << ';' << capacity()
       << ';' << price() << ';' << bookedSeats() << ';' << m_lineUp
       << ';' << m_soundCheckMinutes << '\n';
}