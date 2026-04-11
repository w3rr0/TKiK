#include "ast/WhereClause.hpp"
#include <iostream>
#include <algorithm>

// helper method that removes '' or "" from text
std::string clearQuotes(std::string s) {
    if (s.size() >= 2 && ((s.front() == '\'' && s.back() == '\'') || (s.front() == '\"' && s.back() == '\"'))) {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

// helper method that converts safely to the num
double sToD(const std::string& s) {
    try {
        return std::stod(clearQuotes(s)); // clearing before convertion
    } catch(...) { return 0; }
}
// ComparisonCondition -> OPERATORS and LIKE
bool ComparisonCondition::evaluate(const std::map<std::string, std::string>& row) const {
    if (row.find(column) == row.end()) return false;
    std::string v = row.at(column);
    std::string compareValue = value;

    // removing ' ' and " "
    if (compareValue.size() >= 2 &&
       ((compareValue.front() == '\'' && compareValue.back() == '\'') ||
        (compareValue.front() == '\"' && compareValue.back() == '\"'))) {
        compareValue = compareValue.substr(1, compareValue.size() - 2);}

    if (op == "=") return v == compareValue;
    if (op == ">") return sToD(v) > sToD(compareValue);
    if (op == "<") return sToD(v) < sToD(compareValue);
    if (op == ">=") return sToD(v) >= sToD(compareValue);
    if (op == "<=") return sToD(v) <= sToD(compareValue);
    if (op == "LIKE") {
        // where is '%'
        bool startWildcard = (compareValue.front() == '%');
        bool endWildcard = (compareValue.back() == '%');

        // removing '%' after
        std::string cleanPattern = compareValue;
        if (startWildcard) cleanPattern.erase(0, 1);
        if (endWildcard) cleanPattern.pop_back();

        // to extend later ...
        if (startWildcard && endWildcard) {
            return v.find(cleanPattern) != std::string::npos;
        } else if (startWildcard) {
            // %text
            if (v.length() < cleanPattern.length()) return false;
            return v.compare(v.length() - cleanPattern.length(), cleanPattern.length(), cleanPattern) == 0;
        } else if (endWildcard) {
            // text%
            return v.compare(0, cleanPattern.length(), cleanPattern) == 0;
        } else {
            // no '%'
            return v == cleanPattern;
        }
    }
    return false;
}

void ComparisonCondition::print() const { std::cout << column << " " << op << " " << value; }

// LogicalCondition -> AND / OR
bool LogicalCondition::evaluate(const std::map<std::string, std::string>& row) const {
    if (op == "AND") return left->evaluate(row) && right->evaluate(row);
    if (op == "OR") return left->evaluate(row) || right->evaluate(row);
    return false;
}

void LogicalCondition::print() const {
    std::cout << "("; left->print(); std::cout << " " << op << " "; right->print(); std::cout << ")";
}

// BetweenCondition -> BETWEEN
bool BetweenCondition::evaluate(const std::map<std::string, std::string>& row) const {
    if (row.find(column) == row.end()) return false;
    double v = sToD(row.at(column));
    return v >= sToD(valMin) && v <= sToD(valMax);
}

void BetweenCondition::print() const { std::cout << column << " BETWEEN " << valMin << " AND " << valMax; }

// InCondition -> IN
bool InCondition::evaluate(const std::map<std::string, std::string>& row) const {
    if (row.find(column) == row.end()) return false;
    const std::string& v = row.at(column);

    for (const auto& val : values) {
        if (v == clearQuotes(val)) return true;
    }
    return false;
}

void InCondition::print() const {
    std::cout << column << " IN (";
    for(size_t i=0; i<values.size(); ++i) std::cout << values[i] << (i==values.size()-1 ? "" : ", ");
    std::cout << ")";
}