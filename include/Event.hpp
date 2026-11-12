#pragma once
#include <iostream>
#include <string>

class Event {
private:
    std::string m_title;
    std::string m_date;
    std::string m_startTime;
    int m_duration;
    int m_roomId;
    int m_capacity;
    int m_bookedSeats;
    double m_price;

public:
    Event(const std::string& title, const std::string& date,
          const std::string& startTime, int duration, int roomId,
          int capacity, double price, int bookedSeats = 0);
    virtual ~Event() = default;

    const std::string& title() const { return m_title; }
    const std::string& date() const { return m_date; }
    const std::string& startTime() const { return m_startTime; }
    int duration() const { return m_duration; }
    int roomId() const { return m_roomId; }
    int capacity() const { return m_capacity; }
    int bookedSeats() const { return m_bookedSeats; }
    double price() const { return m_price; }
    int endMinutes() const;
    virtual std::string type() const = 0;
    virtual void display(std::ostream& os) const;
    virtual void save(std::ostream& os) const = 0;
};