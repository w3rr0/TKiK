#ifndef CELL_HPP
#define CELL_HPP

#include <variant>
#include <string>
#include <iostream>

class Cell {
public:
    using ValueType = std::variant<std::monostate, int, double, std::string, bool>;
    enum Type {
        NULL_TYPE, INT, DOUBLE, TEXT, BOOL
    };

    // Constructors
    Cell() : value(std::monostate{}) {}
    explicit Cell(int v) : value(v) {}
    explicit Cell(double v) : value(v) {}
    explicit Cell(std::string v) : value(std::move(v)) {}
    explicit Cell(bool v) : value(v) {}

    // Getters
    Type getType() const;

    template<typename T>
    const T& as() const { return std::get<T>(value); }

    // Comparisons
    bool operator==(const Cell& other) const;
    bool operator<(const Cell& other) const;
    bool operator!=(const Cell& other) const { return !(*this == other); }
    bool operator>(const Cell& other) const  { return other < *this; }
    bool operator<=(const Cell& other) const { return !(*this > other); }
    bool operator>=(const Cell& other) const { return !(*this < other); }

    void checkNullComparison(const Cell& other) const;

    // Display
    friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

private:
    ValueType value;
};

#endif
