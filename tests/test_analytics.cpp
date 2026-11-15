#include "Analytics.hpp"
#include <gtest/gtest.h>
#include <vector>

// Les fonctions sont generiques (templates) : on les teste avec des double et des int.

TEST(Analytics, MinimumAndMaximumOnDoubles) {
    const std::vector<double> prices{5.0, 12.0, 9.5};
    EXPECT_DOUBLE_EQ(minimum(prices), 5.0);
    EXPECT_DOUBLE_EQ(maximum(prices), 12.0);
}

TEST(Analytics, MinimumAndMaximumOnInts) {
    const std::vector<int> durations{90, 120, 60};
    EXPECT_EQ(minimum(durations), 60);
    EXPECT_EQ(maximum(durations), 120);
}

TEST(Analytics, AverageOnInts) {
    const std::vector<int> durations{90, 120};
    EXPECT_DOUBLE_EQ(average(durations), 105.0);
}

TEST(Analytics, AverageOnDoubles) {
    const std::vector<double> prices{5.0, 12.0, 9.5};
    EXPECT_DOUBLE_EQ(average(prices), 26.5 / 3.0);
}

TEST(Analytics, SingleElement) {
    const std::vector<int> one{42};
    EXPECT_EQ(minimum(one), 42);
    EXPECT_EQ(maximum(one), 42);
    EXPECT_DOUBLE_EQ(average(one), 42.0);
}

TEST(Analytics, NegativeValuesAndDuplicates) {
    const std::vector<int> values{-5, 3, -5, 10, 10};
    EXPECT_EQ(minimum(values), -5);
    EXPECT_EQ(maximum(values), 10);
}

// Vecteur vide volontairement NON teste : minimum() et maximum() lisent values[0]
// sans verification (comportement indefini). Pour le couvrir, ajouter d'abord un
// "throw std::invalid_argument" dans Analytics.hpp, puis un EXPECT_THROW ici.