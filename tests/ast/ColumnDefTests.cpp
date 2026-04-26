#include <gtest/gtest.h>
#include "ast/ColumnDef.hpp"


// INT type
TEST(ColumnDefTest, IntInitialization) {
    ColumnDef col{"id", "INT"};

    EXPECT_EQ(col.name, "id");
    EXPECT_EQ(col.type, "INT");
}

// VARCHAR type
TEST(ColumnDefTest, VarcharInitialization) {
    ColumnDef col{"surname", "VARCHAR"};

    EXPECT_EQ(col.name, "surname");
    EXPECT_EQ(col.type, "VARCHAR");
}

// TEXT type
TEST(ColumnDefTest, TextInitialization) {
    ColumnDef col{"name", "TEXT"};

    EXPECT_EQ(col.name, "name");
    EXPECT_EQ(col.type, "TEXT");
}

// STRING type
TEST(ColumnDefTest, StringInitialization) {
    ColumnDef col{"name", "STRING"};

    EXPECT_EQ(col.name, "name");
    EXPECT_EQ(col.type, "STRING");
}

// BOOLEAN type
TEST(ColumnDefTest, BoolInitialization) {
    ColumnDef col{"isBool", "BOOLEAN"};

    EXPECT_EQ(col.name, "isBool");
    EXPECT_EQ(col.type, "BOOLEAN");
}

// NULL type
TEST(ColumnDefTest, NullInitialization) {
    ColumnDef col{"isNull", "NULL"};

    EXPECT_EQ(col.name, "isNull");
    EXPECT_EQ(col.type, "NULL");
}

// FLOAT type
TEST(ColumnDefTest, FloatInitialization) {
    ColumnDef col{"num", "FLOAT"};

    EXPECT_EQ(col.name, "num");
    EXPECT_EQ(col.type, "FLOAT");
}

// DOUBLE type
TEST(ColumnDefTest, DoubleInitialization) {
    ColumnDef col{"num", "DOUBLE"};

    EXPECT_EQ(col.name, "num");
    EXPECT_EQ(col.type, "DOUBLE");
}