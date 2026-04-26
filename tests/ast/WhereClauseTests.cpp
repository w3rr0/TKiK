#include <gtest/gtest.h>
#include "ast/WhereClause.hpp"

// helper method
Table createTestTable() {
    Table table("test_table");
    table.addColumn("id", Cell::INT);
    table.addColumn("name", Cell::TEXT);
    table.addColumn("price", Cell::DOUBLE);
    table.addColumn("active", Cell::BOOL);
    return table;
}

// COMP OP tests
TEST(WhereClauseTest, ComparisonConditionStorage) {
    ComparisonCondition cond("price", ">=", "67");

    EXPECT_EQ(cond.getColumn(), "price");
    EXPECT_EQ(cond.getOp(), ">=");
    EXPECT_EQ(cond.getValue(), "67");
}

// COMP EVALUATE tests
TEST(WhereClauseTest, ComparisonNumericEvaluate) {
    Table table = createTestTable();
    // id | name | price | active
    std::vector<Cell> row = {Cell(1), Cell(std::string("Laptop")), Cell(100.0), Cell(true)};

    // price >= 100.0 should be TRUE
    ComparisonCondition c1("price", ">=", "100.0");
    EXPECT_TRUE(c1.evaluate(table, row));

    // id != 5 should be TRUE
    ComparisonCondition c2("id", "!=", "5");
    EXPECT_TRUE(c2.evaluate(table, row));

    // price < 50.0 should be FALSE
    ComparisonCondition c3("price", "<=", "100.0");
    EXPECT_TRUE(c3.evaluate(table, row));

    ComparisonCondition c4("price", "=", "100.0");
    EXPECT_TRUE(c4.evaluate(table, row));

    ComparisonCondition c5("price", ">", "101.0");
    EXPECT_FALSE(c5.evaluate(table, row));

    ComparisonCondition c6("price", "<", "50.0");
    EXPECT_FALSE(c6.evaluate(table, row));
}

TEST(WhereClauseTest, ComparisonNullEvaluate) {
    Table table = createTestTable();
    // row with NULL
    std::vector<Cell> row = {Cell(2), Cell(), Cell(0.0), Cell(false)};

    // name IS NULL  should be TRUE
    ComparisonCondition c1("name", "IS", "NULL");
    EXPECT_TRUE(c1.evaluate(table, row));

    // name IS NOT NULL should be FALSE
    ComparisonCondition c2("name", "IS NOT", "NULL");
    EXPECT_FALSE(c2.evaluate(table, row));
}

// LIKE tests
TEST(WhereClauseTest, ComparisonLikeEvaluate) {
    Table table = createTestTable();
    std::vector<Cell> row = {Cell(1), Cell(std::string("Krzysztof")), Cell(0.0), Cell(true)};

    // LIKE 'Krzysz%' shoudl be TRUE
    ComparisonCondition c1("name", "LIKE", "'Krzysz%'");
    EXPECT_TRUE(c1.evaluate(table, row));

    // LIKE '%to_' should also be TRUE
    ComparisonCondition c2("name", "LIKE", "'%to_'");
    EXPECT_TRUE(c2.evaluate(table, row));

    // NOT LIKE 'A%' should be TRUE
    ComparisonCondition c3("name", "NOT LIKE", "'A%'");
    EXPECT_TRUE(c3.evaluate(table, row));

    // LIKE 'Jan' shoudl be FALSE
    ComparisonCondition c4("name", "LIKE", "'Jan'");
    EXPECT_FALSE(c4.evaluate(table, row));

    // LIKE '%a' should be FALSE
    ComparisonCondition c5("name", "LIKE", "'%a'");
    EXPECT_FALSE(c5.evaluate(table, row));
}

// IN tests
TEST(WhereClauseTest, InConditionStorage) {
    std::vector<std::string> values = {"'Apple'", "'Orange'", "'Banana'"};
    InCondition cond("fruit", values);

    EXPECT_EQ(cond.getColumn(), "fruit");
    EXPECT_EQ(cond.getValues(), values);
}

// IN EVALUATE tests
TEST(WhereClauseTest, InEvaluate) {
    Table table = createTestTable();
    std::vector<Cell> row = {Cell(1), Cell(std::string("Apple")), Cell(20.0), Cell(true)};

    // name IN ('Apple', 'Orange', 'Banana') should be TRUE
    std::vector<std::string> fruitList = {"'Apple'", "'Orange'", "'Pear'"};
    InCondition in1("name", fruitList);
    EXPECT_TRUE(in1.evaluate(table, row));

    // name IN ('Banana') (FALSE)
    InCondition i2("name", {"'Banana'"});
    EXPECT_FALSE(i2.evaluate(table, row));

    std::vector<std::string> numList = {"1", "2", "6", "9", "20", "67"};
    InCondition in2("price", numList);
    EXPECT_TRUE(in2.evaluate(table, row));

    std::vector<std::string> numListv2 = {"1", "2", "6", "9", "67"};
    InCondition in3("price", numListv2);
    EXPECT_FALSE(in3.evaluate(table, row));
}

// BETWEEN tests
TEST(WhereClauseTest, BetweenConditionStorage) {
    BetweenCondition cond("age", "1", "67");

    EXPECT_EQ(cond.getColumn(), "age");
    EXPECT_EQ(cond.getMin(), "1");
    EXPECT_EQ(cond.getMax(), "67");
}

// BETWEEN EVALUATE tests
TEST(WhereClauseTest, BetweenEvaluate) {
    Table table = createTestTable();
    std::vector<Cell> row = {Cell(25), Cell(std::string("Jan")), Cell(50.0), Cell(true)};

    // id BETWEEN 20 AND 30 should be TRUE
    BetweenCondition b1("id", "20", "30");
    EXPECT_TRUE(b1.evaluate(table, row));

    // id BETWEEN 40 AND 50 should be FALSE
    BetweenCondition b2("id", "40", "50");
    EXPECT_FALSE(b2.evaluate(table, row));

    // price BETWEEN 40 AND 49 should be FALSE
    BetweenCondition b3("price", "40", "49");
    EXPECT_FALSE(b3.evaluate(table, row));
}

// AND / OR tests
TEST(WhereClauseTest, LogicalConditionNesting) {
    auto left = std::make_unique<ComparisonCondition>("a", "=", "1");
    auto right = std::make_unique<ComparisonCondition>("b", "=", "2");
    LogicalCondition logic(std::move(left), "AND", std::move(right));

    EXPECT_NE(logic.getLeft(), nullptr);
    EXPECT_NE(logic.getRight(), nullptr);
    SUCCEED();
}

// AND / OR EVALUATE tests
TEST(WhereClauseTest, LogicalAndOrEvaluate) {
    Table table = createTestTable();
    std::vector<Cell> row = {Cell(1), Cell(std::string("Test")), Cell(50.0), Cell(true)};

    // (id = 5 AND price > 20.0) should be TRUE
    auto left = std::make_unique<ComparisonCondition>("id", "=", "5");
    auto right = std::make_unique<ComparisonCondition>("price", ">", "20.0");
    LogicalCondition andCond(std::move(left), "AND", std::move(right));
    EXPECT_FALSE(andCond.evaluate(table, row));

    // (id = 99 OR active = true) should be TRUE
    auto leftOr = std::make_unique<ComparisonCondition>("id", "=", "99");
    auto rightOr = std::make_unique<ComparisonCondition>("active", "=", "true");
    LogicalCondition orCond(std::move(leftOr), "OR", std::move(rightOr));
    EXPECT_TRUE(orCond.evaluate(table, row));
}