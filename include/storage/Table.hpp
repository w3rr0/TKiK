/**
 * @file Table.hpp
 * @brief Definition of the Table class, a logical structure that manages columns.
 */

#ifndef TABLE_HPP
#define TABLE_HPP

#include <string>
#include <vector>
#include "storage/Column.hpp"

/**
 * @class Table
 * @brief Represents a table in a database using columnar storage.
 * It ensures atomicity of row insertion and easy access to column data.
 */
class Table {
public:
    /**
     * @brief Creates a new table.
     * @param tableName Table name.
     */
    explicit Table(std::string tableName);

    /**
     * @brief Adds a new column to the table schema.
     * @param colName Name of the new column.
     * @param colType Data type of the new column.
     */
    void addColumn(const std::string& colName, Cell::Type colType);

    /**
     * @brief Inserts a new row of data.
     * The types must match the state in the table to be able to add a row.
     * @param row A vector of cells corresponding to the order of the columns in the table.
     * @throws std::invalid_argument If the number of columns or data types are wrong.
     */
    void insertRow(const std::vector<Cell>& row);

    /**
     * @brief Retrieves a complete row from the database as a set of Cell objects.
     * @param rowIndex Index of the row to retrieve.
     * @return Vector of cells reconstructed from column data.
     */
    std::vector<Cell> getRow(size_t rowIndex) const;

    /**
     * @brief Searches for a column by its name.
     * @param colName The name of the column to find.
     * @return Reference to the desired column.
     * @throws std::invalid_argument If the column does not exist.
     */
    Column& getColumn(const std::string& colName);
    const Column& getColumn(const std::string& colName) const;

    /**
     * @brief Returns the number of rows in the table.
     */
    size_t getRowCount() const;

    /**
     * @brief Returns the number of columns in the table.
     */
    size_t getColumnCount() const { return columns.size(); }

    /**
     * @brief Returns table name.
     */
    const std::string& getName() const { return name; }

private:
    std::string name;

    /**
     * @brief A vector storing the column objects.
     * Memory and data management are delegated to the Column class.
     */
    std::vector<Column> columns;
};

#endif
