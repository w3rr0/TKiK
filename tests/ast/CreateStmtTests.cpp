#include <gtest/gtest.h>
#include "ast/CreateStmt.hpp"
#include "storage/Database.hpp"
#include "ast/ColumnDef.hpp"
#include <vector>
#include <string>

extern Database db;
extern std::vector<std::string> gui_log;

// helper setup class that creates a table to test
class CreateStmtTest : public ::testing::Test {
protected:
    void SetUp() override {
        // dropping table every test
        for (const auto& name : db.getTableNames()) {
            db.dropTable(name);
        }
        gui_log.clear();
    }
};

// CONSTRUCTOR tests
TEST_F(CreateStmtTest, ConstructorStoresDataCorrectly) {
    std::vector<ColumnDef> cols = {{"id", "INT"},
                               {"name", "TEXT"},
                               {"active", "BOOLEAN"},
                               {"price", "DOUBLE"} };
    CreateStmt stmt("users", cols);

    EXPECT_EQ(stmt.getTableName(), "users");
    ASSERT_EQ(stmt.getColumns().size(), 4);

    EXPECT_EQ(stmt.getColumns()[0].name, "id");
    EXPECT_EQ(stmt.getColumns()[0].type, "INT");

    EXPECT_EQ(stmt.getColumns()[1].name, "name");
    EXPECT_EQ(stmt.getColumns()[1].type, "TEXT");

    EXPECT_EQ(stmt.getColumns()[2].name, "active");
    EXPECT_EQ(stmt.getColumns()[2].type, "BOOLEAN");

    EXPECT_EQ(stmt.getColumns()[3].name, "price");
    EXPECT_EQ(stmt.getColumns()[3].type, "DOUBLE");

}

// EXECUTE tests
TEST_F(CreateStmtTest, ExecuteCreatesTableInDatabase) {
    std::vector<ColumnDef> cols = {
        {"id", "INT"},
        {"price", "DOUBLE"},
        {"name", "STRING"},
        {"active", "BOOL"}
    };
    CreateStmt stmt("products", cols);

    stmt.execute();

    ASSERT_TRUE(db.hasTable("products"));

    // table tests
    Table& t = db.getTable("products");
    EXPECT_EQ(t.getName(), "products");

    // cols names an types tests and size
    ASSERT_EQ(t.getColumnCount(), 4);
    EXPECT_EQ(t.getColumn("id").getType(), Cell::INT);
    EXPECT_EQ(t.getColumn("price").getType(), Cell::DOUBLE);
    EXPECT_EQ(t.getColumn("name").getType(), Cell::TEXT);
    EXPECT_EQ(t.getColumn("active").getType(), Cell::BOOL);
}

TEST_F(CreateStmtTest, ExecuteLogsErrorIfTableExists) {
    db.createTable("existing_table");

    CreateStmt stmt("existing_table", {{"id", "INT"}});
    stmt.execute();

    // error in gui shoudl appear
    ASSERT_FALSE(gui_log.empty());
    EXPECT_NE(gui_log.back().find("Error"), std::string::npos);
}