#include "DataManager.hpp"
#include "Concert.hpp"
#include "Play.hpp"
#include "Screening.hpp"
#include "Workshop.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {

int parseInt(const std::string& value, const std::string& field) {
    std::istringstream stream{value};
    int result = 0;
    char extra = '\0';
    if (!(stream >> result) || (stream >> extra)) {
        throw std::invalid_argument{"Champ '" + field + "' : nombre entier invalide : \"" + value + "\""};
    }
    return result;
}

double parseDouble(const std::string& value, const std::string& field) {
    std::istringstream stream{value};
    double result = 0.0;
    char extra = '\0';
    if (!(stream >> result) || (stream >> extra)) {
        throw std::invalid_argument{"Champ '" + field + "' : nombre decimal invalide : \"" + value + "\""};
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

// Champs communs a tous les evenements : tag;titre;date;heure;duree;salle;capacite;prix;reservations
struct CommonFields {
    std::string title;
    std::string date;
    std::string time;
    int duration;
    int roomId;
    int capacity;
    double price;
    int booked;
};

CommonFields parseCommon(const std::vector<std::string>& f) {
    return CommonFields{f[1], f[2], f[3],
                        parseInt(f[4], "duree"), parseInt(f[5], "salle"),
                        parseInt(f[6], "capacite"), parseDouble(f[7], "prix"),
                        parseInt(f[8], "reservations")};
}

void expectFieldCount(const std::string& tag, const std::vector<std::string>& f, std::size_t expected) {
    if (f.size() != expected) {
        throw std::invalid_argument{"Nombre de champs invalide pour " + tag + " : " +
                                    std::to_string(f.size()) + " trouve(s), " +
                                    std::to_string(expected) + " attendu(s)"};
    }
}

// Choisit la classe concrete a partir du tag. C'est le seul endroit a completer
// pour qu'un nouveau type d'evenement puisse etre relu depuis le fichier.
std::unique_ptr<Event> makeEvent(const std::string& tag, const std::vector<std::string>& f) {
    if (tag == "CONCERT") {
        expectFieldCount(tag, f, 11);
        const CommonFields c = parseCommon(f);
        return std::make_unique<Concert>(c.title, c.date, c.time, c.duration, c.roomId, c.capacity,
                                         c.price, c.booked, f[9], parseInt(f[10], "soundcheck"));
    }
    if (tag == "PLAY") {
        expectFieldCount(tag, f, 10);
        const CommonFields c = parseCommon(f);
        return std::make_unique<Play>(c.title, c.date, c.time, c.duration, c.roomId, c.capacity,
                                      c.price, c.booked, parseInt(f[9], "entracte"));
    }
    if (tag == "SCREENING") {
        expectFieldCount(tag, f, 10);
        const CommonFields c = parseCommon(f);
        return std::make_unique<Screening>(c.title, c.date, c.time, c.duration, c.roomId, c.capacity,
                                           c.price, c.booked, f[9]);
    }
    if (tag == "WORKSHOP") {
        expectFieldCount(tag, f, 10);
        const CommonFields c = parseCommon(f);
        return std::make_unique<Workshop>(c.title, c.date, c.time, c.duration, c.roomId, c.capacity,
                                          c.price, c.booked, f[9]);
    }
    throw std::invalid_argument{"Type d'evenement inconnu : \"" + tag + "\""};
}

} // namespace

void saveVenue(const Venue& venue, const std::string& path) {
    std::ofstream out{path};
    if (!out) {
        throw std::runtime_error{"Impossible d'ouvrir le fichier : " + path};
    }
    // 15 chiffres significatifs : les prix sont relus exactement (le defaut, 6, les arrondirait).
    out << std::setprecision(15);
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

std::unique_ptr<Venue> loadVenue(const std::string& path) {
    std::ifstream in{path};
    if (!in) {
        throw std::runtime_error{"Impossible d'ouvrir le fichier : " + path};
    }
    // Le programme est construit dans un objet temporaire : en cas d'erreur il est detruit
    // et l'exception remonte, donc aucun programme partiel n'est jamais renvoye.
    std::unique_ptr<Venue> venue = std::make_unique<Venue>();
    std::string line;
    int lineNumber = 0;
    while (std::getline(in, line)) {
        ++lineNumber;
        if (!line.empty() && line.back() == '\r') { // fichier edite sous Windows (CRLF)
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }
        try {
            const std::vector<std::string> f = splitLine(line);
            if (f.empty()) {
                throw std::invalid_argument{"Ligne vide"};
            }
            if (f[0] == "ROOM") {
                expectFieldCount("ROOM", f, 4);
                venue->addRoom(Room{parseInt(f[1], "id"), f[2], parseInt(f[3], "capacite")});
            } else {
                venue->scheduleEvent(makeEvent(f[0], f));
            }
        } catch (const std::exception& e) {
            std::ostringstream message;
            message << "Fichier " << path << ", ligne " << lineNumber << " : " << e.what();
            throw std::runtime_error{message.str()};
        }
    }
    return venue;
}
