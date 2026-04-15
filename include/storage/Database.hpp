/**
 * @file Database.hpp
 * @brief Definition of the Database class that manages a set of tables.
 */

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <unordered_map>
#include "storage/Table.hpp"
#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>

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
     *
     * @return list of all tables in database.
     */
    std::vector<std::string> getTableNames() const;

    /**
     * @brief Gets a modifiable reference to a table.
     */
    Table& getTable(const std::string& tableName);

    /**
     * @brief Performs VACUUM on all tables in the database.
     */
    void vacuum();

    /**
     * @brief Performs VACUUM on a specific table.
     * @param tableName The name of the table to clean.
     * @throws std::invalid_argument If the table does not exist.
     */
    void vacuum(const std::string& tableName);

    /**
     * @brief Returns the database name.
     */
    const std::string& getName() const { return name; }

    /**
     * @brief Saves the database state to a binary file.
     * @param filepath The path where the database file will be saved.
     */
    void saveToFile(const std::string& filepath) const;

    /**
     * @brief Loads the database state from a binary file.
     * @param filepath The path to the database file.
     */
    void loadFromFile(const std::string& filepath);

private:
    std::string name;

    /**
     * @brief Hash map stores table.
     * The key is the table name, the value is the Table object itself.
     */
    std::unordered_map<std::string, Table> tables;

    friend class cereal::access;

    Database() = default;

    /**
     * @brief Serializes the Database object using Cereal.
     */
    template<class Archive>
    void serialize(Archive& archive) {
        archive(name, tables);
    }
};

#endif
