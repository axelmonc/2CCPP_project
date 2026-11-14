#include "DataManager.hpp"
#include <sstream>
#include <stdexcept>

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