#include <gtest/gtest.h>
#include "ast/UpdateStmt.hpp"
#include "storage/Database.hpp"
#include "storage/Table.hpp"

// global base defined in TestSetup.cpp
extern Database db;

// helper setup class that creates a table to test
class UpdateStmtExecuteTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (db.hasTable("employees")) {
            db.dropTable("employees");
        }
        db.createTable("employees");
        db.getTable("employees").addColumn("age", Cell::INT);
        db.getTable("employees").addColumn("salary", Cell::INT);
        db.getTable("employees").insertRow({Cell(30), Cell(3000)});
    }
};

// COSNTRUCTOR tests
TEST(UpdateStmtTest, SingleColConstructor) {
    // updating one row
    UpdateStmt testStmt{"employees", "age", "67"};

    EXPECT_EQ(testStmt.getTableName(), "employees");
    EXPECT_EQ(testStmt.getUpdates().size(), 1);
    EXPECT_EQ(testStmt.getUpdates().at("age"), "67");
    EXPECT_EQ(testStmt.getWhere(), nullptr);
}

TEST(UpdateStmtTest, MultiColConstructor) {
    // updating multi rows at once
    std::map<std::string, std::string> updates = {
        {"salary", "5000"},
        {"status", "active"}
    };

    UpdateStmt testStmt("employees", updates);

    EXPECT_EQ(testStmt.getTableName(), "employees");
    EXPECT_EQ(testStmt.getUpdates().size(), 2);
    EXPECT_EQ(testStmt.getUpdates().at("status"), "active");
    EXPECT_EQ(testStmt.getUpdates().at("salary"), "5000");
    EXPECT_EQ(testStmt.getWhere(), nullptr);
}

// EXECUTE tests
TEST_F(UpdateStmtExecuteTest, ExecuteSuccessfulUpdate) { // TEST_F because of helper class with setup
    UpdateStmt stmt("employees", "age", "67");
    stmt.execute();

    Table& t = db.getTable("employees");
    EXPECT_EQ(t.getRow(0)[0].as<int>(), 67);
}

TEST_F(UpdateStmtExecuteTest, ExecuteMultipleUpdate) {
    std::map<std::string, std::string> updates = {{"age", "40"}, {"salary", "5000"}};
    UpdateStmt stmt("employees", updates);
    stmt.execute();

    Table& t = db.getTable("employees");
    EXPECT_EQ(t.getRow(0)[0].as<int>(), 40);
    EXPECT_EQ(t.getRow(0)[1].as<int>(), 5000);
}
