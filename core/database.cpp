// SQLite database wrapper for Riley Corpbrain - Enterprise-grade data management
#include "database.h"
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

// Constructor - Initialize SQLite database
Database::Database() : db(nullptr) {
    std::cout << "🗄️ INITIALIZING DATABASE..." << std::endl;

    int rc = sqlite3_open("riley_corpbrain.db", reinterpret_cast<sqlite3**>(&db));
    if (rc != SQLITE_OK) {
        std::string error = "Cannot open database: ";
        error += sqlite3_errmsg(reinterpret_cast<sqlite3*>(db));
        sqlite3_close(reinterpret_cast<sqlite3*>(db));
        throw std::runtime_error(error);
    }

    // Enable foreign keys and WAL mode for performance
    execute("PRAGMA foreign_keys = ON;");
    execute("PRAGMA journal_mode = WAL;");
    execute("PRAGMA synchronous = NORMAL;");
    execute("PRAGMA cache_size = 10000;");
    execute("PRAGMA temp_store = MEMORY;");

    // Initialize schema
    initializeSchema();

    std::cout << "✅ Database initialized successfully" << std::endl;
}

// Destructor - Clean shutdown
Database::~Database() {
    if (db) {
        sqlite3_close(reinterpret_cast<sqlite3*>(db));
        std::cout << "✅ Database connection closed" << std::endl;
    }
}

// Execute SQL statement
bool Database::execute(const std::string& sql) {
    if (!db) {
        std::cerr << "❌ Database not initialized" << std::endl;
        return false;
    }

    char* errMsg = nullptr;
    int rc = sqlite3_exec(reinterpret_cast<sqlite3*>(db), sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "❌ SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

// Query with results
std::vector<std::map<std::string, std::string>> Database::query(const std::string& sql) {
    std::vector<std::map<std::string, std::string>> results;

    if (!db) {
        std::cerr << "❌ Database not initialized" << std::endl;
        return results;
    }

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(reinterpret_cast<sqlite3*>(db), sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "❌ SQL prepare error: " << sqlite3_errmsg(reinterpret_cast<sqlite3*>(db)) << std::endl;
        return results;
    }

    int columnCount = sqlite3_column_count(stmt);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::map<std::string, std::string> row;

        for (int i = 0; i < columnCount; i++) {
            const char* columnName = sqlite3_column_name(stmt, i);
            const char* columnValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));

            row[columnName] = columnValue ? columnValue : "";
        }

        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}

// Initialize database schema from schema.sql
void Database::initializeSchema() {
    std::cout << "📋 Loading database schema..." << std::endl;

    std::ifstream schemaFile("data/schema.sql");
    if (!schemaFile.is_open()) {
        throw std::runtime_error("Cannot open schema.sql file");
    }

    std::stringstream buffer;
    buffer << schemaFile.rdbuf();
    std::string schema = buffer.str();

    if (!execute(schema)) {
        throw std::runtime_error("Failed to initialize database schema");
    }

    // Add additional enterprise tables
    createEnterpriseSchema();

    std::cout << "✅ Schema loaded successfully" << std::endl;
}

// Create additional enterprise tables
void Database::createEnterpriseSchema() {
    std::cout << "🏢 Creating enterprise schema extensions..." << std::endl;

    // Financial tables
    execute(R"(
        CREATE TABLE IF NOT EXISTS financial_transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT NOT NULL,
            amount REAL NOT NULL,
            description TEXT,
            category TEXT,
            date TEXT DEFAULT CURRENT_TIMESTAMP,
            project_id INTEGER,
            employee_id INTEGER,
            FOREIGN KEY(project_id) REFERENCES projects(id),
            FOREIGN KEY(employee_id) REFERENCES employees(id)
        );
    )");

    // Inventory tables
    execute(R"(
        CREATE TABLE IF NOT EXISTS inventory_items (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            sku TEXT UNIQUE,
            quantity INTEGER DEFAULT 0,
            unit_price REAL,
            supplier TEXT,
            category TEXT,
            reorder_point INTEGER DEFAULT 10,
            location TEXT,
            created_at TEXT DEFAULT CURRENT_TIMESTAMP
        );
    )");

    // Compliance tables
    execute(R"(
        CREATE TABLE IF NOT EXISTS compliance_audits (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            audit_type TEXT NOT NULL,
            status TEXT DEFAULT 'pending',
            findings TEXT,
            risk_level TEXT,
            due_date TEXT,
            assigned_to INTEGER,
            created_at TEXT DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(assigned_to) REFERENCES employees(id)
        );
    )");

    // Performance metrics
    execute(R"(
        CREATE TABLE IF NOT EXISTS performance_metrics (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            metric_name TEXT NOT NULL,
            metric_value REAL,
            metric_type TEXT,
            department TEXT,
            period TEXT,
            created_at TEXT DEFAULT CURRENT_TIMESTAMP
        );
    )");

    std::cout << "✅ Enterprise schema extensions created" << std::endl;
}

#include <sqlite3.h>
#include <string>

class Database {
public:
    Database() {
        if (sqlite3_open("../data/corpbrain.sqlite", &db)) {
            // Handle error
        }
        // Optionally, run schema.sql to initialize tables
    }
    ~Database() {
        sqlite3_close(db);
    }
    // Example query method
    bool execute(const std::string& sql) {
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            sqlite3_free(errMsg);
            return false;
        }
        return true;
    }
    // Add more query and fetch methods as needed
private:
    sqlite3* db;
};
