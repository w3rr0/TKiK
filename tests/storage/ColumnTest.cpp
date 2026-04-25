#include <gtest/gtest.h>
#include "storage/Column.hpp"
#include "storage/Cell.hpp"
#include <string>
#include <stdexcept>

TEST(ColumnTest, ConstructorSetsNameAndType) {
    Column col("age", Cell::INT);
    EXPECT_EQ(col.getName(), "age");
    EXPECT_EQ(col.getType(), Cell::INT);
    EXPECT_EQ(col.size(), 0);
}

TEST(ColumnTest, AddAndGetElements) {
    Column col("price", Cell::DOUBLE);
    col.add(Cell(10.5));
    col.add(Cell(20.0));

    EXPECT_EQ(col.size(), 2);
    
    Cell c1 = col.get(0);
    EXPECT_EQ(c1.getType(), Cell::DOUBLE);
    EXPECT_DOUBLE_EQ(c1.as<double>(), 10.5);
    EXPECT_FALSE(col.isNull(0));

    Cell c2 = col.get(1);
    EXPECT_DOUBLE_EQ(c2.as<double>(), 20.0);
}

TEST(ColumnTest, AddNullHandling) {
    Column col("name", Cell::TEXT);
    col.add(Cell(std::string("Alice")));
    col.add(Cell()); // Null cell

    EXPECT_EQ(col.size(), 2);
    EXPECT_FALSE(col.isNull(0));
    EXPECT_TRUE(col.isNull(1));

    Cell c2 = col.get(1);
    EXPECT_EQ(c2.getType(), Cell::NULL_TYPE);
}

TEST(ColumnTest, SetElement) {
    Column col("flag", Cell::BOOL);
    col.add(Cell(true));
    col.add(Cell(false));

    col.set(1, Cell(true));
    EXPECT_TRUE(col.get(1).as<bool>());
    EXPECT_FALSE(col.isNull(1));
    
    // Set to Null
    col.set(0, Cell());
    EXPECT_TRUE(col.isNull(0));
}

TEST(ColumnTest, AddInvalidTypeThrows) {
    Column col("age", Cell::INT);
    EXPECT_THROW(col.add(Cell(std::string("twenty"))), std::invalid_argument);
}

TEST(ColumnTest, GetOutOfRangeThrows) {
    Column col("age", Cell::INT);
    EXPECT_THROW(col.get(0), std::out_of_range);
}

TEST(ColumnTest, SetOutOfRangeThrows) {
    Column col("age", Cell::INT);
    EXPECT_THROW(col.set(0, Cell(10)), std::out_of_range);
}

TEST(ColumnTest, VacuumRemovesDeletedRows) {
    Column col("id", Cell::INT);
    col.add(Cell(1));
    col.add(Cell(2));
    col.add(Cell(3));
    col.add(Cell(4));

    std::vector<bool> deleteMask = {false, true, false, true};
    col.vacuum(deleteMask);

    EXPECT_EQ(col.size(), 2);
    EXPECT_EQ(col.get(0).as<int>(), 1);
    EXPECT_EQ(col.get(1).as<int>(), 3);
}
