#include <gtest/gtest.h>
#include "ast/AlterTableStmt.hpp"
#include "gui/Utils.hpp"
#include "storage/Cell.hpp"
#include "storage/Database.hpp"
#include "storage/Cell.hpp"

extern Database db;

// helper setup class that creates a table to test
class AlterTableExecuteTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (db.hasTable("products")) {
            db.dropTable("products");
        }
        db.createTable("products");
    }
};

// DROP COLUMN CONSTRUCTOR tests
TEST(AlterTableStmtTest, DropColumnConstructorDouble) {
    // checking if command was successfully done
    AlterTableStmt stmt("products", "price", AlterTableStmt::DROP);
    SUCCEED();
}
TEST(AlterTableStmtTest, DropColumnConstructorBool){
    // checking if command was successfully done
    AlterTableStmt stmt("people", "isStudent",AlterTableStmt::DROP);
    SUCCEED();
}
TEST(AlterTableStmtTest, DropColumnConstructorInt){
    // checking if command was successfully done
    AlterTableStmt stmt("math", "num", AlterTableStmt::DROP);
    SUCCEED();
}
TEST(AlterTableStmtTest, DropColumnConstructorText){
    // checking if command was successfully done
    AlterTableStmt stmt("clients", "city", AlterTableStmt::DROP);
    SUCCEED();
}

// DROP COLUMN EXECUTE tests
TEST_F(AlterTableExecuteTest, ExecuteDropColumn) {
    // creating a column to be drooped
    db.getTable("products").addColumn("old_col", Cell::INT);
    AlterTableStmt stmt("products", "old_col", AlterTableStmt::DROP);
    stmt.execute();

    Table& t = db.getTable("products");
    // expected 0 cols afetr drop command
    EXPECT_EQ(t.getColumnCount(), 0);
    EXPECT_THROW(t.getColumn("old_col"), std::invalid_argument);
}

// ADD COLUMN CONSTRUCTOR tests
TEST(AlterTableStmtTest, AddColumnConstructorDouble) {
    // // checking if command was successfully done
    AlterTableStmt stmt("products", "price", Cell::DOUBLE, AlterTableStmt::ADD);
    SUCCEED();
}
TEST(AlterTableStmtTest, AddColumnConstructorBool){
    // checking if command was successfully done
    AlterTableStmt stmt("people", "isStudent", Cell::BOOL, AlterTableStmt::ADD);
    SUCCEED();
}
TEST(AlterTableStmtTest, AddColumnConstructorInt){
    // checking if command was successfully done
    AlterTableStmt stmt("math", "num", Cell::INT, AlterTableStmt::ADD);
    SUCCEED();
}
TEST(AlterTableStmtTest, AddColumnConstructorText){
    // checking if command was successfully done
    AlterTableStmt stmt("clients", "city", Cell::TEXT, AlterTableStmt::ADD);
    SUCCEED();
}

// ADD COLUMN EXECUTE tests
TEST_F(AlterTableExecuteTest, ExecuteAddColumn) {
    // add column execution
    AlterTableStmt stmt("products", "price", Cell::DOUBLE, AlterTableStmt::ADD);
    stmt.execute();

    Table& t = db.getTable("products");
    // checking if column count correct
    EXPECT_EQ(t.getColumnCount(), 1);
    EXPECT_EQ(t.getColumn("price").getType(), Cell::DOUBLE);
}

