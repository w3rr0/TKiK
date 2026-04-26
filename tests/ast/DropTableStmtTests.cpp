#include <gtest/gtest.h>
#include "ast/DropTableStmt.hpp"
#include "storage/Database.hpp"

extern Database db;
extern std::vector<std::string> gui_log;

// helper setup class that creates a table to test
class DropTableStmtTest : public ::testing::Test {
protected:
    void SetUp() override {
        for (const auto& name : db.getTableNames()) {
            db.dropTable(name);
        }
        gui_log.clear();
    }
};

// CONSTRUCTOR tests
TEST_F(DropTableStmtTest, ConstructorStoresTableName) {
    DropTableStmt stmt("toDropTable");
    EXPECT_EQ(stmt.getTableName(), "toDropTable");
}

// EXECUTE tests
TEST_F(DropTableStmtTest, ExecuteRemovesTableFromDatabase) {
    db.createTable("toDropTable");
    ASSERT_TRUE(db.hasTable("toDropTable"));

    // DROP TABLE execution
    DropTableStmt stmt("toDropTable");
    stmt.execute();

    // checking if table is removed
    EXPECT_FALSE(db.hasTable("toDropTable"));

    // checking gui logs
    ASSERT_FALSE(gui_log.empty());
    EXPECT_EQ(gui_log.back(), "DROP TABLE toDropTable");
}

TEST_F(DropTableStmtTest, ExecuteLogsErrorForNonExistentTable) {
    DropTableStmt stmt("ghost_table");
    stmt.execute();

    // trying to drop table that does not exist error test
    ASSERT_FALSE(gui_log.empty());
    EXPECT_NE(gui_log.back().find("Error"), std::string::npos);
}