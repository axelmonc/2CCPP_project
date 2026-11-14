#include "DataManager.hpp"
#include "Concert.hpp"
#include "Play.hpp"
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
        out << "ROOM;" << room.id() << ';' << room.name() << ';' << room.capacity() << '\n';
    }
    for (const auto& event : venue.events()) {
        event->save(out);
    }
    if (!out) {
        throw std::runtime_error{"Erreur pendant l'ecriture du fichier : " + path};
    }
}

std::unique_ptr<Event> makeEvent(const std::string& tag,
                                 const std::vector<std::string>& f) {
    const std::string& title = f[1];
    const std::string& date = f[2];
    const std::string& time = f[3];
    const int duration = parseInt(f[4]);
    const int roomId = parseInt(f[5]);
    const int capacity = parseInt(f[6]);
    const double price = parseDouble(f[7]);
    const int booked = parseInt(f[8]);

    if (tag == "CONCERT" && f.size() == 11) {
        return std::make_unique<Concert>(title, date, time, duration, roomId, capacity, price, booked, f[9], parseInt(f[10]));
    }
    if (tag == "PLAY" && f.size() == 10) {
        return std::make_unique<Play>(title, date, time, duration, roomId, capacity, price, booked, parseInt(f[9]));
    }

    throw std::invalid_argument{"Type d'evenement inconnu ou champs invalides"};
}