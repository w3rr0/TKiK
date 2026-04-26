#include <gtest/gtest.h>
#include "ast/DeleteStmt.hpp"
#include "ast/WhereClause.hpp"
#include "storage/Database.hpp"

extern Database db;
extern std::vector<std::string> gui_log;

// helper setup class that creates a table to test
class DeleteStmtTest : public ::testing::Test {
protected:
    void SetUp() override {
        for (const auto& name : db.getTableNames()) db.dropTable(name);
        gui_log.clear();

        db.createTable("products");
        db.getTable("products").addColumn("id", Cell::TEXT);
        db.getTable("products").addColumn("price", Cell::INT);
        db.getTable("products").insertRow({Cell(std::string("C1")), Cell(20)});
        db.getTable("products").insertRow({Cell(std::string("C2")), Cell(30)});
        db.getTable("products").insertRow({Cell(std::string("C3")), Cell(40)});
    }
};

// CONSTRUCTOR tests
TEST_F(DeleteStmtTest, ConstructorAndWhereAssignment) {
    DeleteStmt stmt("products");
    EXPECT_EQ(stmt.getTableName(), "products");
    EXPECT_EQ(stmt.getWhere(), nullptr);

    // id = 'C1' condition WHERE
    auto cond1 = std::make_unique<ComparisonCondition>("id", "=", "'C1'");
    WhereClause* rawPtr1 = cond1.get();
    stmt.setWhere(std::move(cond1));
    EXPECT_EQ(stmt.getWhere(), rawPtr1);

    // price = 20 condition WHERE
    auto cond2 = std::make_unique<ComparisonCondition>("price", "=", "20");
    WhereClause* rawPtr2 = cond2.get();
    stmt.setWhere(std::move(cond2));
    EXPECT_EQ(stmt.getWhere(), rawPtr2);
}

// EXECUTE tests
TEST_F(DeleteStmtTest, ExecuteDeletesAllWithoutWhere) {
    DeleteStmt stmt("products");
    stmt.execute();

    Table& t = db.getTable("products");
    // deleting all rows with no WHERE condition is products TABLE
    EXPECT_TRUE(t.isDeleted(0));
    EXPECT_TRUE(t.isDeleted(1));
    EXPECT_TRUE(t.isDeleted(2));
}

TEST_F(DeleteStmtTest, ExecuteDeletesOnlyMatchingRows) {
    DeleteStmt stmt("products");
    // WHERE id = 'C2'
    stmt.setWhere(std::make_unique<ComparisonCondition>("id", "=", "'C2'"));

    stmt.execute();

    Table& t = db.getTable("products");
    // only row idx 1 is deleted
    EXPECT_FALSE(t.isDeleted(0));
    EXPECT_TRUE(t.isDeleted(1)); // C2 deleted
    EXPECT_FALSE(t.isDeleted(2));
}

TEST_F(DeleteStmtTest, ExecuteLogsErrorForMissingTable) {
    // trying to delete rows from table that does not exist
    DeleteStmt stmt("error_table");
    stmt.execute();

    ASSERT_FALSE(gui_log.empty());
    EXPECT_NE(gui_log.back().find("Error"), std::string::npos);
}