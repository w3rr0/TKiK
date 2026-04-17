/**
* @file ColumnDef.hpp
 * @brief Definition of the ColumnDef struct
 */
#include <string>
#pragma once

/**
 * @struct ColumnDef
 * @brief Represents a single column definition in a CREATE TABLE statement.
 */
struct ColumnDef {
    std::string name;
    std::string type;
};