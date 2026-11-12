#include "Event.hpp"
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

bool isDigit(char value) {
    return value >= '0' && value <= '9';
}

bool hasSeparator(const std::string& text) {
    return text.find(';') != std::string::npos;
}

bool validDate(const std::string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }
    for (std::size_t i = 0; i < 10; ++i) {
        if (i != 4 && i != 7 && !isDigit(date[i])) {
            return false;
        }
    }
    const int year = (date[0] - '0') * 1000 + (date[1] - '0') * 100 +
                     (date[2] - '0') * 10 + (date[3] - '0');
    const int month = (date[5] - '0') * 10 + (date[6] - '0');
    const int day = (date[8] - '0') * 10 + (date[9] - '0');
    if (month < 1 || month > 12 || day < 1) {
        return false;
    }
    const std::vector<int> daysInMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDay = daysInMonth[month - 1];
    const bool leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    if (month == 2 && leap) {
        maxDay = 29;
    }
    return day <= maxDay;
}

bool validTime(const std::string& time) {
    if (time.size() != 5 || time[2] != ':' ||
        !isDigit(time[0]) || !isDigit(time[1]) ||
        !isDigit(time[3]) || !isDigit(time[4])) {
        return false;
    }
    const int hours = (time[0] - '0') * 10 + (time[1] - '0');
    const int minutes = (time[3] - '0') * 10 + (time[4] - '0');
    return hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59;
}

Event::Event(const std::string& title, const std::string& date,
             const std::string& startTime, int duration, int roomId,
             int capacity, double price, int bookedSeats)
    : m_title{title}, m_date{date}, m_startTime{startTime},
      m_duration{duration}, m_roomId{roomId}, m_capacity{capacity},
      m_bookedSeats{bookedSeats}, m_price{price} {
    if (title.empty() || hasSeparator(title) || !validDate(date) || !validTime(startTime) ||
        duration <= 0 || roomId <= 0 || capacity <= 0 ||
        price < 0.0 || bookedSeats < 0 || bookedSeats > capacity) {
        throw std::invalid_argument{"Informations d'evenement invalides (titre, date, heure, duree, salle, capacite, prix ou reservations)"};
    }
    if (endMinutes() > 24 * 60) {
        throw std::invalid_argument{"Un evenement ne peut pas finir apres minuit"};
    }
}

int Event::endMinutes() const {
    const int hours = (m_startTime[0] - '0') * 10 + (m_startTime[1] - '0');
    const int minutes = (m_startTime[3] - '0') * 10 + (m_startTime[4] - '0');
    return hours * 60 + minutes + m_duration;
}

void Event::display(std::ostream& os) const {
    os << type() << " | " << m_title << " | " << m_date << " "
       << m_startTime << " | Salle " << m_roomId << " | "
       << m_bookedSeats << "/" << m_capacity << " places | "
       << m_price << " euros";
}