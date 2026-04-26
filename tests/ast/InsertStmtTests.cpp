#include <gtest/gtest.h>
#include "ast/InsertStmt.hpp"
#include "storage/Database.hpp"
#include <vector>
#include <string>

extern Database db;
extern std::vector<std::string> gui_log;

// helper setup class that creates a table to test
class InsertStmtTest : public ::testing::Test {
protected:
    void SetUp() override {
        for (const auto& name : db.getTableNames()) {
            db.dropTable(name);
        }
        gui_log.clear();

        db.createTable("users");
        db.getTable("users").addColumn("id", Cell::INT);
        db.getTable("users").addColumn("name", Cell::TEXT);
    }
};

// CONSTRUCTOR test
TEST_F(InsertStmtTest, ConstructorStoresDataCorrectly) {
    // checking if column is creted correctly
    std::vector<std::string> row = {"1", "'Jan'"};
    InsertStmt stmt("users", row);

    EXPECT_EQ(stmt.getTableName(), "users");
    ASSERT_EQ(stmt.getValues().size(), 1);
    EXPECT_EQ(stmt.getValues()[0][0], "1");
}

// EXECUTE TESTS
TEST_F(InsertStmtTest, ExecuteInsertsRowIntoTable) {
    // inserting one row to table
    InsertStmt stmt("users", std::vector<std::string>{"10", "Krzysztof"});
    stmt.execute();

    Table& t = db.getTable("users");
    // checking all cells
    ASSERT_EQ(t.getRowCount(), 1);
    EXPECT_EQ(t.getRow(0)[0].as<int>(), 10);
    EXPECT_EQ(t.getRow(0)[1].as<std::string>(), "Krzysztof");
}

TEST_F(InsertStmtTest, ExecuteHandlesMultiRowInsert) {
    // inserting many rows at once
    std::vector<std::vector<std::string>> multi = {{"1", "'A'"}, {"2", "'B'"}};
    InsertStmt stmt("users", multi);

    stmt.execute();

    EXPECT_EQ(db.getTable("users").getRowCount(), 2);
}

TEST_F(InsertStmtTest, ExecuteLogsErrorOnInvalidType) {
    // checking error gui message test
    InsertStmt stmt("users", std::vector<std::string>{"'not_an_int'", "'Jan'"});
    stmt.execute();

    ASSERT_FALSE(gui_log.empty());
    EXPECT_NE(gui_log.back().find("Error"), std::string::npos);
}