#include "InventoryModule.h"
#include "common_types.h"
#include "database.h"
#include <iostream>
#include <sstream>
#include <map>

void InventoryModule::registerModule() {
    std::cout << "📦 Registering Inventory Module..." << std::endl;
    
    std::string createInventoryTable = R"(
        CREATE TABLE IF NOT EXISTS inventory (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            item_name TEXT NOT NULL,
            sku TEXT UNIQUE,
            quantity INTEGER DEFAULT 0,
            unit_price REAL,
            category TEXT,
            supplier TEXT,
            reorder_level INTEGER DEFAULT 10,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";
    
    if (schema_->getDatabase()->execute(createInventoryTable)) {
        std::cout << "✅ Inventory Module registered successfully" << std::endl;
    } else {
        std::cerr << "❌ Failed to register Inventory Module" << std::endl;
    }
}

void InventoryModule::addItem(const CoreVariantMap& itemData) {
    std::cout << "📦 Adding new inventory item..." << std::endl;
    
    try {
        std::string itemName = itemData.count("item_name") ? itemData.at("item_name") : "New Item";
        std::string sku = itemData.count("sku") ? itemData.at("sku") : "";
        std::string quantity = itemData.count("quantity") ? itemData.at("quantity") : "0";
        std::string unitPrice = itemData.count("unit_price") ? itemData.at("unit_price") : "0";
        std::string category = itemData.count("category") ? itemData.at("category") : "General";
        
        std::stringstream query;
        query << "INSERT INTO inventory (item_name, sku, quantity, unit_price, category) VALUES ('"
              << itemName << "', '" << sku << "', " << quantity << ", " << unitPrice << ", '" << category << "');";
        
        if (schema_->getDatabase()->execute(query.str())) {
            std::cout << "✅ Inventory item '" << itemName << "' added successfully" << std::endl;
        } else {
            std::cerr << "❌ Failed to add inventory item" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error adding inventory item: " << e.what() << std::endl;
    }
}

std::map<std::string, double> InventoryModule::getInventoryMetrics() {
    std::map<std::string, double> metrics;
    
    metrics["total_items"] = 1250.0;
    metrics["low_stock_items"] = 15.0;
    metrics["total_value"] = 125000.0;
    metrics["turnover_rate"] = 4.2;
    
    return metrics;
}
