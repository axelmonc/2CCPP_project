#include "Play.hpp"
#include <ostream>
#include <stdexcept>

Play::Play(const std::string& title, const std::string& date,
           const std::string& startTime, int duration, int roomId,
           int capacity, double price, int bookedSeats, int intervalMinutes)
    : Event{title, date, startTime, duration, roomId, capacity, price, bookedSeats},
      m_intervalMinutes{intervalMinutes} {
    if (intervalMinutes < 0 || intervalMinutes >= duration) {
        throw std::invalid_argument{"Duree d'entracte invalide"};
    }
}

std::string Play::type() const { return "Theatre"; }

void Play::display(std::ostream& os) const {
    Event::display(os);
    os << " | Entracte : " << m_intervalMinutes << " min";
}

void Play::save(std::ostream& os) const {
    os << "PLAY;" << title() << ';' << date() << ';' << startTime()
       << ';' << duration() << ';' << roomId() << ';' << capacity()
       << ';' << price() << ';' << bookedSeats() << ';'
       << m_intervalMinutes << '\n';
}