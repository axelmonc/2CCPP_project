#include "Concert.hpp"
#include "DataManager.hpp"
#include "Play.hpp"
#include "Screening.hpp"
#include "Workshop.hpp"
#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

namespace {

std::string writeTempFile(const std::string& name, const std::string& content) {
    std::ofstream out(name);
    out << content;
    return name;
}

// Charge un fichier temporaire contenant "content" et renvoie le message de l'exception
// (chaine vide si aucune exception). Le fichier est supprime ensuite.
std::string loadAndGetError(const std::string& name, const std::string& content) {
    const std::string path = writeTempFile(name, content);
    std::string message;
    try {
        loadVenue(path);
    } catch (const std::runtime_error& e) {
        message = e.what();
    }
    std::remove(path.c_str());
    return message;
}

}  // namespace

// --- Sauvegarde puis rechargement ---

TEST(Persistence, RoundTripKeepsRoomsAndEventKinds) {
    Venue venue;
    venue.addRoom(Room{1, "Grande salle", 200});
    venue.addRoom(Room{2, "Petite salle", 60});
    venue.scheduleEvent(std::make_unique<Concert>("Jazz", "2026-12-05", "20:00", 120, 1, 180, 18.5, 35, "Blue Quartet", 45));
    venue.scheduleEvent(std::make_unique<Play>("Malade", "2026-12-06", "18:00", 100, 2, 55, 12.0, 20, 15));
    venue.scheduleEvent(std::make_unique<Screening>("Voyage", "2026-12-07", "20:30", 95, 1, 180, 9.5, 72, "Tout public"));
    venue.scheduleEvent(std::make_unique<Workshop>("Atelier", "2026-12-08", "14:00", 90, 2, 40, 5.0, 12, "Camille"));

    saveVenue(venue, "roundtrip.tmp");
    const auto loaded = loadVenue("roundtrip.tmp");
    std::remove("roundtrip.tmp");

    ASSERT_EQ(loaded->rooms().size(), 2u);
    ASSERT_EQ(loaded->events().size(), 4u);
    EXPECT_EQ(loaded->events()[0]->type(), "Concert");
    EXPECT_EQ(loaded->events()[1]->type(), "Theatre");
    EXPECT_EQ(loaded->events()[2]->type(), "Projection");
    EXPECT_EQ(loaded->events()[3]->type(), "Atelier");
    EXPECT_EQ(loaded->events()[0]->bookedSeats(), 35);
    EXPECT_DOUBLE_EQ(loaded->events()[0]->price(), 18.5);
    EXPECT_DOUBLE_EQ(loaded->totalTakings(), venue.totalTakings());
}

TEST(Persistence, EmptyFileGivesEmptyVenue) {
    const std::string path = writeTempFile("empty.tmp", "");
    const auto loaded = loadVenue(path);
    std::remove(path.c_str());
    EXPECT_TRUE(loaded->rooms().empty());
    EXPECT_TRUE(loaded->events().empty());
}

TEST(Persistence, SkipsBlankLines) {
    const std::string path = writeTempFile("blank.tmp", "ROOM;1;Salle;40\n\n\nROOM;2;Autre;20\n");
    const auto loaded = loadVenue(path);
    std::remove(path.c_str());
    EXPECT_EQ(loaded->rooms().size(), 2u);
}

// --- Fichiers invalides ---

TEST(Persistence, MissingFileThrows) {
    EXPECT_THROW(loadVenue("n_existe_pas.txt"), std::runtime_error);
}

TEST(Persistence, ReportsFileAndLineOnBadLine) {
    const std::string message = loadAndGetError(
        "bad.tmp", "ROOM;1;Salle;40\nPLAY;X;2026-13-40;18:00;90;1;30;5;0;15\n");
    EXPECT_NE(message.find("bad.tmp"), std::string::npos);
    EXPECT_NE(message.find("ligne 2"), std::string::npos);
}

TEST(Persistence, RejectsMissingField) {
    // ROOM avec 3 champs au lieu de 4
    const std::string message = loadAndGetError("missing.tmp", "ROOM;1;Salle\n");
    EXPECT_NE(message.find("ligne 1"), std::string::npos);
}

TEST(Persistence, RejectsEventWithTooFewFields) {
    const std::string message = loadAndGetError(
        "few.tmp", "ROOM;1;Salle;40\nPLAY;X;2026-12-06;18:00;90;1\n");
    EXPECT_NE(message.find("ligne 2"), std::string::npos);
}

TEST(Persistence, RejectsUnknownEventType) {
    const std::string message = loadAndGetError(
        "type.tmp", "ROOM;1;Salle;40\nBALLET;X;2026-12-06;18:00;90;1;30;5;0;15\n");
    EXPECT_NE(message.find("ligne 2"), std::string::npos);
}

TEST(Persistence, RejectsNonNumericValue) {
    const std::string message = loadAndGetError("nan.tmp", "ROOM;abc;Salle;40\n");
    EXPECT_NE(message.find("ligne 1"), std::string::npos);
}

TEST(Persistence, RejectsUnknownRoom) {
    const std::string message = loadAndGetError(
        "noroom.tmp", "ROOM;1;Salle;40\nPLAY;X;2026-12-06;18:00;90;9;30;5;0;15\n");
    EXPECT_NE(message.find("ligne 2"), std::string::npos);
}

TEST(Persistence, RejectsDuplicateRoom) {
    const std::string message = loadAndGetError(
        "dup.tmp", "ROOM;1;Salle;40\nROOM;1;Autre;20\n");
    EXPECT_NE(message.find("ligne 2"), std::string::npos);
}

TEST(Persistence, RejectsOverlappingEventsInFile) {
    const std::string message = loadAndGetError(
        "overlap.tmp",
        "ROOM;1;Salle;100\n"
        "PLAY;A;2026-12-06;18:00;90;1;30;5;0;15\n"
        "PLAY;B;2026-12-06;18:30;90;1;30;5;0;15\n");
    EXPECT_NE(message.find("ligne 3"), std::string::npos);
}