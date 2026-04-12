/**
 * @file Table.cpp
 * @brief Implementation of Table class.
 */

#include "storage/Table.hpp"
#include <algorithm>

Table::Table(std::string tableName)
    : name(std::move(tableName)) {}

void Table::addColumn(const std::string& colName, Cell::Type colType) {
    for (const auto& col : columns) {
        if (col.getName() == colName) {
            throw std::invalid_argument("Error: Column '" + colName + "' already exists in table '" + name + "'.");
        }
    }

    columns.emplace_back(colName, colType);
}

void Table::insertRow(const std::vector<Cell>& row) {
    if (row.size() != columns.size()) {
        throw std::invalid_argument(
            "Error INSERT: Table '" + name + "' has " + std::to_string(columns.size()) +
            " columns, but tried to insert " + std::to_string(row.size()) + " values."
        );
    }

    // Data types validation
    for (size_t i = 0; i < columns.size(); ++i) {
        const Cell& cell = row[i];
        if (cell.getType() != Cell::Type::NULL_TYPE && cell.getType() != columns[i].getType()) {
            throw std::invalid_argument(
                "Type error in column '" + columns[i].getName() +
                "': Expecting type " + std::to_string(static_cast<int>(columns[i].getType())) +
                ", but got " + std::to_string(static_cast<int>(cell.getType()))
            );
        }
    }

    // Inserting data
    for (size_t i = 0; i < columns.size(); ++i) {
        columns[i].add(row[i]);
    }
}

std::vector<Cell> Table::getRow(size_t rowIndex) const {
    if (rowIndex >= getRowCount()) {
        throw std::out_of_range("Błąd: Wiersz o indeksie " + std::to_string(rowIndex) + " nie istnieje.");
    }

    std::vector<Cell> resultRow;
    resultRow.reserve(columns.size());

    for (const auto& col : columns) {
        resultRow.push_back(col.get(rowIndex));
    }

    return resultRow;
}

Column& Table::getColumn(const std::string& colName) {
    // Wrapper for const getColumn version
    return const_cast<Column&>(static_cast<const Table*>(this)->getColumn(colName));
}

const Column& Table::getColumn(const std::string& colName) const {
    auto it = std::find_if(columns.begin(), columns.end(),
        [&colName](const Column& col) { return col.getName() == colName; }
    );

    if (it != columns.end()) {
        return *it;
    }

    throw std::invalid_argument("Error: Column '" + colName + "' don't exist in table '" + name + "'.");
}

size_t Table::getRowCount() const {
    if (columns.empty()) {
        return 0;
    }
    return columns[0].size();
}
