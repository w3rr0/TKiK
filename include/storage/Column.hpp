/**
 * @file Column.hpp
 * @brief Definition of the Column class, managing contiguous data storage.
 */

#ifndef COLUMN_HPP
#define COLUMN_HPP

#include <string>
#include <vector>
#include <variant>
#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/variant.hpp>
#include "storage/Cell.hpp"

/**
 * @class Column
 * @brief Manages data for a single column using columnar storage architecture.
 * This class stores data in type-specific vectors to maximize cache locality
 * and performance. It uses a bitmask to handle NULL values without
 * disrupting the contiguous memory layout of the primary data.
 */
class Column {
public:
    Column() = default;

    /**
     * @brief A variant holding the actual vector of data.
     * The order aligns with the Cell::Type enum to allow index-based access.
     */
    using ColumnData = std::variant<
        std::monostate,
        std::vector<int>,
        std::vector<double>,
        std::vector<std::string>,
        std::vector<bool>
    >;

    /**
     * @brief Constructs a new column with a specific name and rigid data type.
     * @param colName The name of the column.
     * @param colType The data type for all elements in this column.
     */
    Column(std::string colName, Cell::Type colType);

    /**
     * @brief Appends a new value to the end of the column.
     * Automatically handles the insertion of dummy values to maintain index synchronization
     * when a NULL is added.
     * @param cell The cell containing the data to insert.
     * @throws std::invalid_argument If the cell's type does not match the column's type.
     */
    void add(const Cell& cell);

    /**
     * @brief Retrieves the value at a specific row index.
     * @param rowIndex The index of the row.
     * @return A Cell object constructed from the underlying raw data.
     * @throws std::out_of_range If the row index is invalid.
     */
    Cell get(size_t rowIndex) const;

    /**
     * @brief Updates the value in the given row.
     * @param rowIndex The index of the row to modify.
     * @param newValue New value.
     * @throws std::out_of_range If the row does not exist.
     * @throws std::invalid_argument If the new type does not fit the column.
     */
    void set(size_t rowIndex, const Cell& newValue);

    /**
     * @brief Gets the total number of rows currently stored in this column.
     * @return The size of the column.
     */
    size_t size() const;

    /**
     * @brief Checks if the value at the specified row is NULL.
     * @param rowIndex The index of the row to check.
     * @return true if the row contains a NULL, false otherwise.
     */
    bool isNull(size_t rowIndex) const;

    // Getters

    /**
     * @brief Retrieves the name of the column.
     * @return A constant reference to the column's name.
     */
    const std::string& getName() const { return name; }

    /**
     * @brief Retrieves the data type of the column.
     * @return The Cell::Type assigned to this column.
     */
    Cell::Type getType() const { return type; }

private:
    std::string name;
    Cell::Type type;

    ColumnData data;
    std::vector<bool> nullMask;

    friend class cereal::access;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(name, type, data, nullMask);
    }
};

#endif
