#include "Concert.hpp"
#include "Venue.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>

namespace {

std::unique_ptr<Event> makeConcert(const std::string& title, const std::string& start,
                                   int duration = 60, const std::string& date = "2026-12-05") {
    return std::make_unique<Concert>(title, date, start, duration, 1, 50, 10.0, 0,
                                     "Blue Quartet", 30);
}

// Fixture : chaque test demarre avec un Venue contenant deja la salle 1 (capacite 100)
class VenueTest : public ::testing::Test {
protected:
    void SetUp() override { venue.addRoom(Room{1, "Grande salle", 100}); }
    Venue venue;
};

}  // namespace

// --- Salles ---

TEST_F(VenueTest, FindRoom) {
    ASSERT_NE(venue.findRoom(1), nullptr);
    EXPECT_EQ(venue.findRoom(1)->name(), "Grande salle");
    EXPECT_EQ(venue.findRoom(99), nullptr);
}

TEST_F(VenueTest, RefusesDuplicateRoomId) {
    EXPECT_THROW(venue.addRoom(Room{1, "Autre", 10}), std::invalid_argument);
}

// --- Programmation ---

TEST_F(VenueTest, ScheduleThenFind) {
    venue.scheduleEvent(makeConcert("A", "20:00"));
    ASSERT_NE(venue.findEvent("A"), nullptr);
    EXPECT_EQ(venue.events().size(), 1u);
    EXPECT_EQ(venue.findEvent("inconnu"), nullptr);
}

TEST_F(VenueTest, RefusesNullEvent) {
    EXPECT_THROW(venue.scheduleEvent(nullptr), std::invalid_argument);
}

TEST_F(VenueTest, RefusesUnknownRoom) {
    auto event = std::make_unique<Concert>("A", "2026-12-05", "20:00", 60, 9, 50, 10.0, 0, "X", 30);
    EXPECT_THROW(venue.scheduleEvent(std::move(event)), std::invalid_argument);
}

TEST_F(VenueTest, RefusesCapacityAboveRoomCapacity) {
    auto event = std::make_unique<Concert>("A", "2026-12-05", "20:00", 60, 1, 101, 10.0, 0, "X", 30);
    EXPECT_THROW(venue.scheduleEvent(std::move(event)), std::invalid_argument);
}

TEST_F(VenueTest, RefusesSameTitleSameDate) {
    venue.scheduleEvent(makeConcert("A", "10:00"));
    EXPECT_THROW(venue.scheduleEvent(makeConcert("A", "20:00")), std::invalid_argument);
}

TEST_F(VenueTest, RefusesOverlappingEventInSameRoom) {
    venue.scheduleEvent(makeConcert("A", "20:00"));
    EXPECT_THROW(venue.scheduleEvent(makeConcert("B", "20:30")), std::invalid_argument);
}

TEST_F(VenueTest, AcceptsBackToBackEvents) {
    venue.scheduleEvent(makeConcert("A", "20:00"));
    EXPECT_NO_THROW(venue.scheduleEvent(makeConcert("B", "21:00")));
}

TEST_F(VenueTest, AcceptsSameHourOnAnotherDate) {
    venue.scheduleEvent(makeConcert("A", "20:00"));
    EXPECT_NO_THROW(venue.scheduleEvent(makeConcert("B", "20:00", 60, "2026-12-06")));
}

// --- Reservations ---

TEST_F(VenueTest, BookSeatsUpdatesEvent) {
    venue.scheduleEvent(makeConcert("A", "20:00"));
    EXPECT_TRUE(venue.bookSeats("A", "2026-12-05", 10));
    EXPECT_EQ(venue.findEvent("A")->bookedSeats(), 10);
}

TEST_F(VenueTest, RefusesBookingWhenFull) {
    venue.scheduleEvent(makeConcert("A", "20:00"));
    EXPECT_TRUE(venue.bookSeats("A", "2026-12-05", 50));
    EXPECT_FALSE(venue.bookSeats("A", "2026-12-05", 1));
}

TEST_F(VenueTest, BookingUnknownEventReturnsFalse) {
    EXPECT_FALSE(venue.bookSeats("inconnu", "2026-12-05", 1));
}

TEST_F(VenueTest, BookingWrongDateReturnsFalse) {
    venue.scheduleEvent(makeConcert("A", "20:00"));
    EXPECT_FALSE(venue.bookSeats("A", "2030-01-01", 1));
}
