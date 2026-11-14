#include "Concert.hpp"
#include "Workshop.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

// Un evenement valide reutilisable : Workshop(titre, date, heure, duree, salle, capacite, prix, reservations, animateur)
static Workshop makeWorkshop(int booked = 0) {
    return Workshop("Atelier", "2026-12-08", "14:00", 90, 1, 20, 5.0, booked, "Camille");
}

TEST(Event, EndMinutesIsStartPlusDuration) {
    EXPECT_EQ(makeWorkshop().endMinutes(), 14 * 60 + 90);
}

TEST(Event, AvailableSeats) {
    EXPECT_EQ(makeWorkshop(5).availableSeats(), 15);
}

TEST(Event, BookReducesAvailableSeats) {
    Workshop w = makeWorkshop();
    EXPECT_TRUE(w.book(4));
    EXPECT_EQ(w.bookedSeats(), 4);
    EXPECT_EQ(w.availableSeats(), 16);
}

TEST(Event, BookRefusesTooManySeats) {
    Workshop w = makeWorkshop();
    EXPECT_FALSE(w.book(21));
    EXPECT_EQ(w.bookedSeats(), 0);
}

TEST(Event, BookRefusesZeroOrNegative) {
    Workshop w = makeWorkshop();
    EXPECT_FALSE(w.book(0));
    EXPECT_FALSE(w.book(-3));
}

TEST(Event, TypeIsCorrectForEachKind) {
    Workshop w = makeWorkshop();
    Concert c("Concert", "2026-12-05", "20:00", 60, 1, 50, 10.0, 0, "Blue Quartet", 30);
    EXPECT_EQ(w.type(), "Atelier");
    EXPECT_EQ(c.type(), "Concert");
}

// --- Constructeur : les donnees invalides doivent lever une exception ---

TEST(EventValidation, RejectsEmptyTitle) {
    EXPECT_THROW(Workshop("", "2026-12-08", "14:00", 90, 1, 20, 5.0, 0, "C"), std::invalid_argument);
}

TEST(EventValidation, RejectsTitleWithSemicolon) {
    EXPECT_THROW(Workshop("A;B", "2026-12-08", "14:00", 90, 1, 20, 5.0, 0, "C"), std::invalid_argument);
}

TEST(EventValidation, RejectsImpossibleDate) {
    EXPECT_THROW(Workshop("A", "2026-13-40", "14:00", 90, 1, 20, 5.0, 0, "C"), std::invalid_argument);
    EXPECT_THROW(Workshop("A", "2026-02-29", "14:00", 90, 1, 20, 5.0, 0, "C"), std::invalid_argument);
}

TEST(EventValidation, AcceptsLeapDay) {
    EXPECT_NO_THROW(Workshop("A", "2028-02-29", "14:00", 90, 1, 20, 5.0, 0, "C"));
}

TEST(EventValidation, RejectsBadTime) {
    EXPECT_THROW(Workshop("A", "2026-12-08", "25:00", 90, 1, 20, 5.0, 0, "C"), std::invalid_argument);
    EXPECT_THROW(Workshop("A", "2026-12-08", "9:00", 90, 1, 20, 5.0, 0, "C"), std::invalid_argument);
}

TEST(EventValidation, RejectsEventEndingAfterMidnight) {
    EXPECT_THROW(Workshop("A", "2026-12-08", "23:00", 90, 1, 20, 5.0, 0, "C"), std::invalid_argument);
}

TEST(EventValidation, RejectsBadNumbers) {
    EXPECT_THROW(Workshop("A", "2026-12-08", "14:00", 0, 1, 20, 5.0, 0, "C"), std::invalid_argument);   // duree
    EXPECT_THROW(Workshop("A", "2026-12-08", "14:00", 90, 0, 20, 5.0, 0, "C"), std::invalid_argument);  // salle
    EXPECT_THROW(Workshop("A", "2026-12-08", "14:00", 90, 1, 0, 5.0, 0, "C"), std::invalid_argument);   // capacite
    EXPECT_THROW(Workshop("A", "2026-12-08", "14:00", 90, 1, 20, -1.0, 0, "C"), std::invalid_argument); // prix
    EXPECT_THROW(Workshop("A", "2026-12-08", "14:00", 90, 1, 20, 5.0, 21, "C"), std::invalid_argument); // reservations > capacite
}
