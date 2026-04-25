#include <gtest/gtest.h>
#include "storage/Table.hpp"
#include "storage/Cell.hpp"
#include <vector>
#include <string>

TEST(TableTest, ConstructorAndName) {
    Table table("users");
    EXPECT_EQ(table.getName(), "users");
    EXPECT_EQ(table.getColumnCount(), 0);
    EXPECT_EQ(table.getRowCount(), 0);
}

TEST(TableTest, AddColumn) {
    Table table("users");
    table.addColumn("id", Cell::INT);
    table.addColumn("name", Cell::TEXT);
    
    EXPECT_EQ(table.getColumnCount(), 2);
    
    const Column& c1 = table.getColumn("id");
    EXPECT_EQ(c1.getType(), Cell::INT);
    
    const Column& c2 = table.getColumn("name");
    EXPECT_EQ(c2.getType(), Cell::TEXT);
}

TEST(TableTest, InsertAndGetRow) {
    Table table("users");
    table.addColumn("id", Cell::INT);
    table.addColumn("name", Cell::TEXT);

    std::vector<Cell> row1 = {Cell(1), Cell(std::string("Alice"))};
    table.insertRow(row1);
    
    EXPECT_EQ(table.getRowCount(), 1);
    
    std::vector<Cell> fetchedRow = table.getRow(0);
    ASSERT_EQ(fetchedRow.size(), 2);
    EXPECT_EQ(fetchedRow[0].as<int>(), 1);
    EXPECT_EQ(fetchedRow[1].as<std::string>(), "Alice");
}

TEST(TableTest, InsertRowInvalidTypeThrows) {
    Table table("users");
    table.addColumn("id", Cell::INT);

    std::vector<Cell> invalidRow = {Cell(std::string("not an int"))};
    EXPECT_THROW(table.insertRow(invalidRow), std::invalid_argument);
}

TEST(TableTest, UpdateCell) {
    Table table("users");
    table.addColumn("id", Cell::INT);
    table.insertRow({Cell(1)});
    
    table.updateCell(0, "id", Cell(2));
    
    std::vector<Cell> fetched = table.getRow(0);
    EXPECT_EQ(fetched[0].as<int>(), 2);
}

TEST(TableTest, DeleteRowAndVacuum) {
    Table table("users");
    table.addColumn("id", Cell::INT);
    table.insertRow({Cell(1)});
    table.insertRow({Cell(2)});
    table.insertRow({Cell(3)});

    EXPECT_EQ(table.getRowCount(), 3);
    
    table.deleteRow(1); // Delete the row with ID 2
    EXPECT_TRUE(table.isDeleted(1));
    EXPECT_FALSE(table.isDeleted(0));
    EXPECT_FALSE(table.isDeleted(2));

    table.vacuum();
    
    // After vacuum, the logically deleted row should be gone
    EXPECT_EQ(table.getRowCount(), 2);
    EXPECT_EQ(table.getRow(0)[0].as<int>(), 1);
    EXPECT_EQ(table.getRow(1)[0].as<int>(), 3);
}

TEST(TableTest, DropColumn) {
    Table table("users");
    table.addColumn("id", Cell::INT);
    table.addColumn("name", Cell::TEXT);

    table.dropColumn("id");
    EXPECT_EQ(table.getColumnCount(), 1);
    EXPECT_THROW(table.getColumn("id"), std::invalid_argument);
    EXPECT_NO_THROW(table.getColumn("name"));
}
