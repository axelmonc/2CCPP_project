#include "Workshop.hpp"
#include <ostream>
#include <stdexcept>

Workshop::Workshop(const std::string& title, const std::string& date,
                   const std::string& startTime, int duration, int roomId,
                   int capacity, double price, int bookedSeats,
                   const std::string& instructor)
    : Event{title, date, startTime, duration, roomId, capacity, price, bookedSeats},
      m_instructor{instructor} {
    if (instructor.empty() || instructor.find(';') != std::string::npos) {
        throw std::invalid_argument{"Nom d'animateur invalide (vide ou contenant ';')"};
    }
}

std::string Workshop::type() const { return "Atelier"; }

void Workshop::display(std::ostream& os) const {
    Event::display(os);
    os << " | Animateur : " << m_instructor;
}

void Workshop::save(std::ostream& os) const {
    os << "WORKSHOP;" << title() << ';' << date() << ';' << startTime()
       << ';' << duration() << ';' << roomId() << ';' << capacity()
       << ';' << price() << ';' << bookedSeats() << ';' << m_instructor << '\n';
}