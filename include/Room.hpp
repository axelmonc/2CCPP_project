#pragma once
#include <string>

class Room {
private:
    int m_id;
    std::string m_name;
    int m_capacity;

public:
    Room(int id, const std::string& name, int capacity);
    int id() const { return m_id; }
    const std::string& name() const { return m_name; }
    int capacity() const { return m_capacity; }
};