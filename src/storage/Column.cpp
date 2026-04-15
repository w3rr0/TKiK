#include "storage/Column.hpp"
#include <stdexcept>
#include <cereal/types/vector.hpp>

Column::Column(std::string colName, Cell::Type colType)
    : name(std::move(colName)), type(colType)
{
    // We don't allow creating a column of NULL_TYPE
    if (type == Cell::Type::NULL_TYPE) {
        throw std::invalid_argument("Cannot create a column of type NULL_TYPE.");
    }

    switch (type) {
        case Cell::Type::INT:    data = std::vector<int>(); break;
        case Cell::Type::DOUBLE: data = std::vector<double>(); break;
        case Cell::Type::TEXT:   data = std::vector<std::string>(); break;
        case Cell::Type::BOOL:   data = std::vector<bool>(); break;
        default: break;
    }
}

void Column::add(const Cell& cell) {
    // Update nullMask and add default value in data vector
    if (cell.getType() == Cell::Type::NULL_TYPE) {
        nullMask.push_back(true);

        switch (type) {
            case Cell::Type::INT:    std::get<std::vector<int>>(data).push_back(0); break;
            case Cell::Type::DOUBLE: std::get<std::vector<double>>(data).push_back(0.0); break;
            case Cell::Type::TEXT:   std::get<std::vector<std::string>>(data).push_back(""); break;
            case Cell::Type::BOOL:   std::get<std::vector<bool>>(data).push_back(false); break;
            default: break;
        }
        return;
    }

    // Value type validation
    if (cell.getType() != type) {
        throw std::invalid_argument(
            "Cannot add value of type " +
            std::to_string(static_cast<int>(cell.getType())) +
            " to column '" + name + "' of type " + std::to_string(static_cast<int>(type))
        );
    }

    nullMask.push_back(false);

    switch (type) {
        case Cell::Type::INT:    std::get<std::vector<int>>(data).push_back(cell.as<int>()); break;
        case Cell::Type::DOUBLE: std::get<std::vector<double>>(data).push_back(cell.as<double>()); break;
        case Cell::Type::TEXT:   std::get<std::vector<std::string>>(data).push_back(cell.as<std::string>()); break;
        case Cell::Type::BOOL:   std::get<std::vector<bool>>(data).push_back(cell.as<bool>()); break;
        default: break;
    }
}

Cell Column::get(size_t rowIndex) const {
    if (rowIndex >= nullMask.size()) {
        throw std::out_of_range("Index out of column '" + name + "' range.");
    }

    // If the value is NULL, we return a Cell with NULL_TYPE
    if (nullMask[rowIndex]) {
        return Cell();
    }

    switch (type) {
        case Cell::Type::INT:    return Cell(std::get<std::vector<int>>(data)[rowIndex]);
        case Cell::Type::DOUBLE: return Cell(std::get<std::vector<double>>(data)[rowIndex]);
        case Cell::Type::TEXT:   return Cell(std::get<std::vector<std::string>>(data)[rowIndex]);
        case Cell::Type::BOOL:   return Cell(std::get<std::vector<bool>>(data)[rowIndex]);
        default: throw std::runtime_error("Unknown column state.");
    }
}

void Column::set(size_t rowIndex, const Cell& newValue) {
    if (rowIndex >= nullMask.size()) {
        throw std::out_of_range("Error UPDATE: Row index out of range in column '" + name + "'.");
    }

    if (newValue.getType() == Cell::Type::NULL_TYPE) {
        nullMask[rowIndex] = true;
        return;
    }

    if (newValue.getType() != type) {
        throw std::invalid_argument(
            "Type error in UPDATE for column '" + name + "'. "
            "Expecting " + std::to_string(static_cast<int>(type)) +
            ", received " + std::to_string(static_cast<int>(newValue.getType()))
        );
    }

    nullMask[rowIndex] = false;

    std::visit([rowIndex, &newValue](auto& vec) {
        using VecType = std::decay_t<decltype(vec)>;

        if constexpr (!std::is_same_v<VecType, std::monostate>) {
            using ElementType = typename VecType::value_type;

            vec[rowIndex] = newValue.as<ElementType>();
        }
    }, data);
}

size_t Column::size() const {
    // nullMask always has the same size as the data vector
    return nullMask.size();
}

bool Column::isNull(size_t rowIndex) const {
    if (rowIndex >= nullMask.size()) {
        throw std::out_of_range("Index out of column '" + name + "' range.");
    }
    return nullMask[rowIndex];
}

std::string Column::getTypeString() const {
    switch (type) {
        case Cell::INT:    return "INT";
        case Cell::DOUBLE: return "DOUBLE";
        case Cell::TEXT:   return "VARCHAR";
        case Cell::BOOL:   return "BOOL";
        default:           return "UNKNOWN";
    }
}
