#include <gtest/gtest.h>
#include "storage/Database.hpp"
#include "storage/Table.hpp"
#include <string>
#include <stdexcept>
#include <filesystem>
#include <vector>

TEST(DatabaseTest, ConstructorAndName) {
    Database db("test_db");
    EXPECT_EQ(db.getName(), "test_db");
}

TEST(DatabaseTest, CreateAndHasTable) {
    Database db("test_db");
    EXPECT_FALSE(db.hasTable("users"));

    db.createTable("users");
    EXPECT_TRUE(db.hasTable("users"));

    // Attempting to create duplicate table throws
    EXPECT_THROW(db.createTable("users"), std::invalid_argument);
}

TEST(DatabaseTest, GetTableNames) {
    Database db("test_db");
    db.createTable("users");
    db.createTable("products");

    std::vector<std::string> names = db.getTableNames();
    EXPECT_EQ(names.size(), 2);
    
    bool hasUsers = false;
    bool hasProducts = false;
    for (const auto& name : names) {
        if (name == "users") hasUsers = true;
        if (name == "products") hasProducts = true;
    }
    EXPECT_TRUE(hasUsers);
    EXPECT_TRUE(hasProducts);
}

TEST(DatabaseTest, DropTable) {
    Database db("test_db");
    db.createTable("users");
    EXPECT_TRUE(db.hasTable("users"));

    db.dropTable("users");
    EXPECT_FALSE(db.hasTable("users"));

    // Dropping non-existent table throws
    EXPECT_THROW(db.dropTable("users"), std::invalid_argument);
}

TEST(DatabaseTest, GetTable) {
    Database db("test_db");
    db.createTable("users");
    
    Table& table = db.getTable("users");
    EXPECT_EQ(table.getName(), "users");

    // Getting non-existent table throws
    EXPECT_THROW(db.getTable("nonexistent"), std::invalid_argument);
}

TEST(DatabaseTest, SaveAndLoadFromFile) {
    const std::string filename = "test_db.bin";
    
    // Clean up if exists
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }

    {
        Database db("save_test_db");
        db.createTable("users");
        Table& table = db.getTable("users");
        table.addColumn("id", Cell::INT);
        table.insertRow({Cell(42)});
        
        db.saveToFile(filename);
    }

    {
        Database loadedDb("dummy");
        loadedDb.loadFromFile(filename);
        
        EXPECT_EQ(loadedDb.getName(), "save_test_db");
        EXPECT_TRUE(loadedDb.hasTable("users"));
        
        Table& table = loadedDb.getTable("users");
        EXPECT_EQ(table.getColumnCount(), 1);
        EXPECT_EQ(table.getRowCount(), 1);
        EXPECT_EQ(table.getRow(0)[0].as<int>(), 42);
    }

    // Clean up
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
}
