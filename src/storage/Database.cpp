/**
 * @file Database.cpp
 * @brief Implementation of Database class.
 */

#include "storage/Database.hpp"

Database::Database(std::string dbName)
    : name(std::move(dbName)) {}

void Database::createTable(const std::string& tableName) {
    if (hasTable(tableName)) {
        throw std::invalid_argument("Error CREATE: Table '" + tableName + "' already exist in database '" + name + "'.");
    }

    tables.emplace(tableName, Table(tableName));
}

void Database::dropTable(const std::string& tableName) {
    if (tables.erase(tableName) == 0) { // Table not found
        throw std::invalid_argument("Error DROP: Table '" + tableName + "' does not exist.");
    }
}

bool Database::hasTable(const std::string& tableName) const {
    return tables.find(tableName) != tables.end();
}

const Table& Database::getTable(const std::string& tableName) const {
    auto it = tables.find(tableName);

    if (it != tables.end()) {
        return it->second; // Value (Table object)
    }

    throw std::invalid_argument("Error: Table '" + tableName + "' was not found in the database.");
}

Table& Database::getTable(const std::string& tableName) {
    return const_cast<Table&>(static_cast<const Database*>(this)->getTable(tableName));
}
