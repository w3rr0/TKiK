#ifndef COLUMN_HPP
#define COLUMN_HPP

#include <string>
#include <vector>
#include <variant>
#include "storage/Cell.hpp"

class Column {
public:
    using ColumnData = std::variant<
        std::monostate,
        std::vector<int>,
        std::vector<double>,
        std::vector<std::string>,
        std::vector<bool>
    >;

    Column(std::string colName, Cell::Type colType);

    void add(const Cell& cell);

    Cell get(size_t rowIndex) const;

    size_t size() const;

    bool isNull(size_t rowIndex) const;

    // Getters
    const std::string& getName() const { return name; }
    Cell::Type getType() const { return type; }

private:
    std::string name;
    Cell::Type type;

    ColumnData data;
    std::vector<bool> nullMask;
};

#endif
