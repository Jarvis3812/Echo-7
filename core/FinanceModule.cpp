#include "FinanceModule.h"
#include "common_types.h"
#include "database.h"
#include <iostream>
#include <sstream>
#include <map>

void FinanceModule::registerModule() {
    std::cout << "💳 Registering Finance Module..." << std::endl;
    
    std::string createExpensesTable = R"(
        CREATE TABLE IF NOT EXISTS expenses (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            description TEXT NOT NULL,
            amount REAL NOT NULL,
            category TEXT,
            date DATE,
            employee TEXT,
            approved BOOLEAN DEFAULT 0,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";
    
    if (schema_->getDatabase()->execute(createExpensesTable)) {
        std::cout << "✅ Finance Module registered successfully" << std::endl;
    } else {
        std::cerr << "❌ Failed to register Finance Module" << std::endl;
    }
}

void FinanceModule::addExpense(const CoreVariantMap& expenseData) {
    std::cout << "💳 Adding new expense..." << std::endl;
    
    try {
        std::string description = expenseData.count("description") ? expenseData.at("description") : "Expense";
        std::string amount = expenseData.count("amount") ? expenseData.at("amount") : "0";
        std::string category = expenseData.count("category") ? expenseData.at("category") : "General";
        std::string employee = expenseData.count("employee") ? expenseData.at("employee") : "Unknown";
        
        std::stringstream query;
        query << "INSERT INTO expenses (description, amount, category, employee, date) VALUES ('"
              << description << "', " << amount << ", '" << category << "', '" << employee << "', DATE('now'));";
        
        if (schema_->getDatabase()->execute(query.str())) {
            std::cout << "✅ Expense '" << description << "' added successfully" << std::endl;
        } else {
            std::cerr << "❌ Failed to add expense" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error adding expense: " << e.what() << std::endl;
    }
}

std::map<std::string, double> FinanceModule::getFinanceMetrics() {
    std::map<std::string, double> metrics;
    
    metrics["total_revenue"] = 850000.0;
    metrics["total_expenses"] = 320000.0;
    metrics["net_profit"] = 530000.0;
    metrics["profit_margin"] = 0.62;
    metrics["monthly_burn_rate"] = 45000.0;
    
    return metrics;
}
