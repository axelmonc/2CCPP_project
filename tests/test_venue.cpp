#include "Concert.hpp"
#include "Venue.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

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

// --- Statistiques (recettes, prix, durees, frequentation) ---

TEST_F(VenueTest, TotalTakingsIsZeroWhenNoEvent) {
    EXPECT_DOUBLE_EQ(venue.totalTakings(), 0.0);
}

TEST_F(VenueTest, TotalTakingsSumsPriceTimesSeats) {
    venue.scheduleEvent(makeConcert("A", "20:00"));   // prix 10.0
    venue.scheduleEvent(makeConcert("B", "22:00"));
    venue.bookSeats("A", "2026-12-05", 3);
    venue.bookSeats("B", "2026-12-05", 2);
    EXPECT_DOUBLE_EQ(venue.totalTakings(), 50.0);
}

TEST_F(VenueTest, PricesDurationsAttendances) {
    venue.scheduleEvent(makeConcert("A", "20:00", 60));
    venue.scheduleEvent(makeConcert("B", "22:00", 90));
    venue.bookSeats("B", "2026-12-05", 4);

    EXPECT_EQ(venue.prices(), (std::vector<double>{10.0, 10.0}));
    EXPECT_EQ(venue.durations(), (std::vector<int>{60, 90}));
    EXPECT_EQ(venue.attendances(), (std::vector<int>{0, 4}));
}

TEST_F(VenueTest, StatisticsAreEmptyWithoutEvents) {
    EXPECT_TRUE(venue.prices().empty());
    EXPECT_TRUE(venue.durations().empty());
    EXPECT_TRUE(venue.attendances().empty());
}

// --- Affichages (on capture ce qui est ecrit sur la sortie standard) ---

TEST_F(VenueTest, DisplayProgrammeWhenEmpty) {
    testing::internal::CaptureStdout();
    venue.displayProgramme();
    EXPECT_NE(testing::internal::GetCapturedStdout().find("Aucun evenement"), std::string::npos);
}

TEST_F(VenueTest, DisplayProgrammeShowsEvents) {
    venue.scheduleEvent(makeConcert("Jazz", "20:00"));
    testing::internal::CaptureStdout();
    venue.displayProgramme();
    EXPECT_NE(testing::internal::GetCapturedStdout().find("Jazz"), std::string::npos);
}

TEST_F(VenueTest, DisplayEventsByDate) {
    venue.scheduleEvent(makeConcert("Jazz", "20:00"));

    testing::internal::CaptureStdout();
    venue.displayEventsByDate("2026-12-05");
    EXPECT_NE(testing::internal::GetCapturedStdout().find("Jazz"), std::string::npos);

    testing::internal::CaptureStdout();
    venue.displayEventsByDate("2030-01-01");
    EXPECT_NE(testing::internal::GetCapturedStdout().find("Aucun evenement a cette date"), std::string::npos);
}

TEST_F(VenueTest, DisplayRoomsShowsNameAndCapacity) {
    testing::internal::CaptureStdout();
    venue.displayRooms();
    const std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("Grande salle"), std::string::npos);
    EXPECT_NE(out.find("100"), std::string::npos);
}

TEST_F(VenueTest, BusiestRoomIsTheOneWithMostBookings) {
    venue.addRoom(Room{2, "Petite salle", 100});
    venue.scheduleEvent(makeConcert("A", "20:00"));  // salle 1
    venue.scheduleEvent(std::make_unique<Concert>("B", "2026-12-05", "20:00", 60, 2, 50, 10.0, 0, "X", 30));  // salle 2
    venue.bookSeats("A", "2026-12-05", 5);
    venue.bookSeats("B", "2026-12-05", 20);

    testing::internal::CaptureStdout();
    venue.displayBusiestRooms();
    const std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("Petite salle"), std::string::npos);
    EXPECT_EQ(out.find("Grande salle"), std::string::npos);
}

TEST(VenueNoRoom, BusiestRoomsWithoutRoom) {
    Venue empty;
    testing::internal::CaptureStdout();
    empty.displayBusiestRooms();
    EXPECT_NE(testing::internal::GetCapturedStdout().find("Aucune salle"), std::string::npos);
}