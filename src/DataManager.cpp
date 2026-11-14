#include "DataManager.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

int parseInt(const std::string& value) {
    std::istringstream stream{value};
    int result = 0;
    char extra = '\0';
    if (!(stream >> result) || (stream >> extra)) {
        throw std::invalid_argument{"Nombre entier invalide : " + value};
    }
    return result;
}

double parseDouble(const std::string& value) {
    std::istringstream stream{value};
    double result = 0.0;
    char extra = '\0';
    if (!(stream >> result) || (stream >> extra)) {
        throw std::invalid_argument{"Nombre decimal invalide : " + value};
    }
    return result;
}

std::vector<std::string> splitLine(const std::string& line) {
    std::vector<std::string> fields;
    std::istringstream stream{line};
    std::string field;
    while (std::getline(stream, field, ';')) {
        fields.push_back(field);
    }
    return fields;
}

void saveVenue(const Venue& venue, const std::string& path) {
    std::ofstream out{path}; // ouvre le fichier pour le modif
    if (!out) {
        throw std::runtime_error{"Impossible d'ouvrir le fichier : " + path};
    }
    for (const auto& room : venue.rooms()) {
        out << "ROOM;" << room.id() << ';' << room.name()
            << ';' << room.capacity() << '\n';
    }
    for (const auto& event : venue.events()) {
        event->save(out);
    }
}