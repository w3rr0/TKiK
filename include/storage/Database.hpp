/**
 * @file Database.hpp
 * @brief Definition of the Database class that manages a set of tables.
 */

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <unordered_map>
#include "storage/Table.hpp"

/**
 * @class Database
 * @brief Represents the entire database.
 * Stores tables in a hash table for fast access.
 */
class Database {
public:
    /**
     * @brief Create a new database.
     * @param dbName Name of the database.
     */
    explicit Database(std::string dbName);

    /**
     * @brief Creates a new, empty table in the database.
     * @param tableName Name of the new table.
     * @throws std::invalid_argument If a table with that name already exists.
     */
    void createTable(const std::string& tableName);

    /**
     * @brief Deletes a table from the database.
     * @param tableName Name of the table to be deleted.
     * @throws std::invalid_argument If the table does not exist.
     */
    void dropTable(const std::string& tableName);

    /**
     * @brief Checks whether a table with the given name exists.
     * @param tableName Name of table.
     * @return true, if table exists, otherwise false.
     */
    bool hasTable(const std::string& tableName) const;

    /**
     * @brief Gets a constant reference to a table (read-only).
     */
    const Table& getTable(const std::string& tableName) const;

    /**
     * @brief Gets a modifiable reference to a table.
     */
    Table& getTable(const std::string& tableName);

    /**
     * @brief Returns the database name.
     */
    const std::string& getName() const { return name; }

private:
    std::string name;

    /**
     * @brief Hash map stores tabele.
     * The key is the table name, the value is the Table object itself.
     */
    std::unordered_map<std::string, Table> tables;
};

#endif
