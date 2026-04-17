/**
* @file Statement.hpp
 * @brief Header definition for the base Statement class.
 */
#pragma once

/**
 * @class Statement
 * @brief base class for all SQL statement types (SELECT, INSERT, ...).
 * * Every SQL command in the interpreter is represented by a class inheriting
 * from this interface.
 */
class Statement {
public:
    /** @brief Virtual destructor to ensure proper cleanup of derived classes. */
    virtual ~Statement () = default;

    /**
     * @brief virtual function that executes the logic of a specific SQL statement.
     * * Must be implemented by all derived classes.
     */
    virtual void execute() = 0;
};