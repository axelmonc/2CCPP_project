#include "Analytics.hpp"
#include "Concert.hpp"
#include "DataManager.hpp"
#include "Play.hpp"
#include "Screening.hpp"
#include "Workshop.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

const std::string ADMIN_CODE = "CULT";

class EndOfInput : public std::runtime_error {
public:
    EndOfInput() : std::runtime_error{"Fin de l'entree"} {}
};

std::string readText(const std::string& message) {
    std::string value;
    std::cout << message;
    if (!std::getline(std::cin, value)) {
        throw EndOfInput{};
    }
    return value;
}

int readInt(const std::string& message) {
    std::istringstream ss{readText(message)};
    int value = 0;
    char extra = '\0';
    if (!(ss >> value) || (ss >> extra)) {
        throw std::invalid_argument{"Veuillez entrer un nombre entier"};
    }
    return value;
}

double readDouble(const std::string& message) {
    std::istringstream ss{readText(message)};
    double value = 0.0;
    char extra = '\0';
    if (!(ss >> value) || (ss >> extra)) {
        throw std::invalid_argument{"Veuillez entrer un nombre"};
    }
    return value;
}

void addEvent(Venue& venue) {
    std::cout << "Type : 1 Concert | 2 Theatre | 3 Projection | 4 Atelier\n";
    const int type = readInt("Votre choix : ");
    const std::string title = readText("Titre : ");
    const std::string date = readText("Date (AAAA-MM-JJ) : ");
    const std::string time = readText("Heure (HH:MM) : ");
    const int duration = readInt("Duree en minutes : ");
    const int roomId = readInt("Identifiant de la salle : ");
    const int capacity = readInt("Nombre de places a vendre : ");
    const double price = readDouble("Prix d'une place : ");

    if (type == 1) {
        const std::string lineUp = readText("Artistes (separes par des virgules) : ");
        const int soundCheck = readInt("Duree du sound-check en minutes : ");
        venue.scheduleEvent(std::make_unique<Concert>(
            title, date, time, duration, roomId, capacity, price, 0,
            lineUp, soundCheck));
    } else if (type == 2) {
        const int interval = readInt("Duree de l'entracte en minutes : ");
        venue.scheduleEvent(std::make_unique<Play>(
            title, date, time, duration, roomId, capacity, price, 0, interval));
    } else if (type == 3) {
        const std::string rating = readText("Classification (ex : Tout public) : ");
        venue.scheduleEvent(std::make_unique<Screening>(
            title, date, time, duration, roomId, capacity, price, 0, rating));
    } else if (type == 4) {
        const std::string instructor = readText("Nom de l'animateur : ");
        venue.scheduleEvent(std::make_unique<Workshop>(
            title, date, time, duration, roomId, capacity, price, 0, instructor));
    } else {
        throw std::invalid_argument{"Type d'evenement invalide"};
    }
    std::cout << "Evenement ajoute.\n";
}

void showStatistics(const Venue& venue) {
    const std::vector<double> prices = venue.prices();
    const std::vector<int> durations = venue.durations();
    const std::vector<int> attendances = venue.attendances();

    std::cout << "Recettes totales : " << venue.totalTakings() << " euros\n";
    if (prices.empty()) {
        std::cout << "Pas encore de donnees statistiques.\n";
        return;
    }
    std::cout << "Prix - min : " << minimum(prices)
              << ", max : " << maximum(prices)
              << ", moyenne : " << average(prices) << '\n';
    std::cout << "Durees - min : " << minimum(durations)
              << ", max : " << maximum(durations)
              << ", moyenne : " << average(durations) << " minutes\n";
    std::cout << "Reservations - min : " << minimum(attendances)
              << ", max : " << maximum(attendances)
              << ", moyenne : " << average(attendances) << '\n';
    venue.displayBusiestRooms();
}

int main() {
    Venue venue;
    bool canSave = true;
    try {
        std::unique_ptr<Venue> loaded = loadVenue("programme.txt");
        venue = std::move(*loaded);
        std::cout << "Programme charge depuis programme.txt\n";
    } catch (const std::exception& e) {
        std::cerr << "Chargement non effectue : " << e.what() << '\n';
        std::cout << "Demarrage avec un programme vide.\n";
        std::ifstream test{"programme.txt"};
        if (test) {
            canSave = false;
            std::cout << "Sauvegarde desactivee pour ne pas ecraser programme.txt.\n";
        }
    }

    bool running = true;
    while (running) {
        std::cout << "\n===== LIEU CULTUREL =====\n"
                << "1. Afficher le programme\n"
                << "2. Rechercher un evenement par titre\n"
                << "3. Afficher le programme d'une date\n"
                << "4. Reserver des places\n"
                << "5. Afficher les salles\n"
                << "6. Statistiques et recettes\n"
                << "7. Administration : ajouter une salle ou un evenement\n"
                << "8. Sauvegarder\n"
                << "0. Quitter\n";
        try {
            const int choice = readInt("Choix : ");
            if (choice == 1) {
                venue.displayProgramme();
            } else if (choice == 2) {
                const std::string title = readText("Titre recherche : ");
                const Event* event = venue.findEvent(title);
                if (event == nullptr) {
                    std::cout << "Evenement introuvable.\n";
                } else {
                    event->display(std::cout);
                    std::cout << '\n';
                }
            } else if (choice == 3) {
                venue.displayEventsByDate(readText("Date (AAAA-MM-JJ) : "));
            } else if (choice == 4) {
                const std::string title = readText("Titre de l'evenement : ");
                const std::string date = readText("Date (AAAA-MM-JJ) : ");
                const int seats = readInt("Nombre de places : ");
                if (venue.bookSeats(title, date, seats)) {
                    std::cout << "Reservation confirmee.\n";
                } else {
                    std::cout << "Reservation refusee (evenement absent ou salle complete).\n";
                }
            } else if (choice == 5) {
                venue.displayRooms();
            } else if (choice == 6) {
                showStatistics(venue);
            } else if (choice == 7) {
                const std::string code = readText("Code administrateur : ");
                if (code != ADMIN_CODE) {
                    std::cout << "Code incorrect.\n";
                } else {
                    std::cout << "1. Ajouter une salle\n2. Ajouter un evenement\n";
                    const int adminChoice = readInt("Choix : ");
                    if (adminChoice == 1) {
                        const int id = readInt("Identifiant : ");
                        const std::string name = readText("Nom : ");
                        const int capacity = readInt("Capacite : ");
                        venue.addRoom(Room{id, name, capacity});
                        std::cout << "Salle ajoutee.\n";
                    } else if (adminChoice == 2) {
                        addEvent(venue);
                    } else {
                        std::cout << "Choix invalide.\n";
                    }
                }
            } else if (choice == 8) {
                if (!canSave) {
                    throw std::runtime_error{"Sauvegarde desactivee (fichier programme.txt invalide)"};
                }
                saveVenue(venue, "programme.txt");
                std::cout << "Programme sauvegarde.\n";
            } else if (choice == 0) {
                if (canSave) {
                    saveVenue(venue, "programme.txt");
                    std::cout << "Programme sauvegarde. ";
                }
                std::cout << "Au revoir.\n";
                running = false;
            } else {
                std::cout << "Choix invalide.\n";
            }
        } catch (const EndOfInput&) {
            std::cout << "\nFin de l'entree. Au revoir.\n";
            if (canSave) {
                try {
                    saveVenue(venue, "programme.txt");
                } catch (const std::exception& e) {
                    std::cerr << "Erreur : " << e.what() << '\n';
                }
            }
            running = false;
        } catch (const std::exception& e) {
            std::cerr << "Erreur : " << e.what() << '\n';
        }
    }
    return 0;
}