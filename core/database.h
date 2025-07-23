// Database header - Enterprise-grade data management
#pragma once
#include <string>
#include <vector>
#include <map>

class Database {
public:
    Database();
    ~Database();

    // Core database operations
    bool execute(const std::string& sql);
    std::vector<std::map<std::string, std::string>> query(const std::string& sql);

    // Schema management
    void initializeSchema();
    void createEnterpriseSchema();

    // Transaction support
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    // Prepared statements for performance
    bool executeWithParams(const std::string& sql, const std::vector<std::string>& params);

private:
    void* db;  // SQLite database handle
};
