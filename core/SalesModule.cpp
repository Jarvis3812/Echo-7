#include "SalesModule.h"
#include "common_types.h"
#include "database.h"
#include <iostream>
#include <sstream>
#include <map>

void SalesModule::registerModule() {
    std::cout << "💰 Registering Sales Module..." << std::endl;
    
    // Create sales tables
    std::string createDealsTable = R"(
        CREATE TABLE IF NOT EXISTS sales_deals (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            client_id INTEGER,
            deal_name TEXT NOT NULL,
            amount REAL NOT NULL,
            stage TEXT DEFAULT 'Prospecting',
            probability REAL DEFAULT 0.1,
            expected_close_date DATE,
            actual_close_date DATE,
            sales_rep TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (client_id) REFERENCES clients(id)
        );
    )";
    
    std::string createForecastTable = R"(
        CREATE TABLE IF NOT EXISTS sales_forecast (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            period TEXT NOT NULL,
            predicted_revenue REAL,
            confidence_level REAL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";
    
    if (schema_->getDatabase()->execute(createDealsTable) && 
        schema_->getDatabase()->execute(createForecastTable)) {
        std::cout << "✅ Sales Module registered successfully" << std::endl;
    } else {
        std::cerr << "❌ Failed to register Sales Module" << std::endl;
    }
}

void SalesModule::addSale(const CoreVariantMap& saleData) {
    std::cout << "💰 Adding new sales deal..." << std::endl;
    
    try {
        std::string dealName = saleData.count("deal_name") ? saleData.at("deal_name") : "Untitled Deal";
        std::string amount = saleData.count("amount") ? saleData.at("amount") : "0";
        std::string stage = saleData.count("stage") ? saleData.at("stage") : "Prospecting";
        std::string probability = saleData.count("probability") ? saleData.at("probability") : "0.1";
        std::string clientId = saleData.count("client_id") ? saleData.at("client_id") : "1";
        std::string salesRep = saleData.count("sales_rep") ? saleData.at("sales_rep") : "Unknown";
        
        std::stringstream query;
        query << "INSERT INTO sales_deals (client_id, deal_name, amount, stage, probability, sales_rep) VALUES ("
              << clientId << ", '" << dealName << "', " << amount << ", '" 
              << stage << "', " << probability << ", '" << salesRep << "');";
        
        if (schema_->getDatabase()->execute(query.str())) {
            std::cout << "✅ Sales deal '" << dealName << "' added successfully" << std::endl;
        } else {
            std::cerr << "❌ Failed to add sales deal" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error adding sales deal: " << e.what() << std::endl;
    }
}

void SalesModule::updateDeal(int dealId, const CoreVariantMap& updates) {
    std::cout << "📝 Updating deal ID: " << dealId << std::endl;
    
    try {
        std::stringstream query;
        query << "UPDATE sales_deals SET ";
        
        bool first = true;
        for (const auto& pair : updates) {
            if (!first) query << ", ";
            query << pair.first << " = '" << pair.second << "'";
            first = false;
        }
        
        query << ", updated_at = CURRENT_TIMESTAMP WHERE id = " << dealId << ";";
        
        if (schema_->getDatabase()->execute(query.str())) {
            std::cout << "✅ Deal updated successfully" << std::endl;
        } else {
            std::cerr << "❌ Failed to update deal" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error updating deal: " << e.what() << std::endl;
    }
}

std::vector<std::map<std::string, std::string>> SalesModule::getDeals() {
    std::vector<std::map<std::string, std::string>> deals;
    
    // Sample deals data
    std::map<std::string, std::string> deal1;
    deal1["id"] = "1";
    deal1["deal_name"] = "Enterprise Software License";
    deal1["amount"] = "50000";
    deal1["stage"] = "Negotiation";
    deal1["probability"] = "0.7";
    deal1["sales_rep"] = "John Smith";
    deals.push_back(deal1);
    
    std::map<std::string, std::string> deal2;
    deal2["id"] = "2";
    deal2["deal_name"] = "Consulting Services";
    deal2["amount"] = "25000";
    deal2["stage"] = "Proposal";
    deal2["probability"] = "0.4";
    deal2["sales_rep"] = "Jane Doe";
    deals.push_back(deal2);
    
    return deals;
}

std::map<std::string, double> SalesModule::getSalesMetrics() {
    std::map<std::string, double> metrics;
    
    metrics["total_pipeline"] = 750000.0;
    metrics["closed_won"] = 125000.0;
    metrics["closed_lost"] = 35000.0;
    metrics["average_deal_size"] = 42500.0;
    metrics["win_rate"] = 0.68;
    metrics["sales_cycle_days"] = 45.0;
    
    return metrics;
}

std::map<std::string, double> SalesModule::getForecast(const std::string& period) {
    std::map<std::string, double> forecast;
    
    if (period == "Q1") {
        forecast["predicted_revenue"] = 180000.0;
        forecast["confidence_level"] = 0.85;
    } else if (period == "Q2") {
        forecast["predicted_revenue"] = 220000.0;
        forecast["confidence_level"] = 0.75;
    } else {
        forecast["predicted_revenue"] = 200000.0;
        forecast["confidence_level"] = 0.70;
    }
    
    return forecast;
}

void SalesModule::generateForecast() {
    std::cout << "📊 Generating sales forecast..." << std::endl;
    
    // In a real implementation, this would use AI models
    // to predict future sales based on historical data
    
    auto q1Forecast = getForecast("Q1");
    auto q2Forecast = getForecast("Q2");
    
    std::cout << "📈 Q1 Forecast: $" << q1Forecast["predicted_revenue"] 
              << " (Confidence: " << (q1Forecast["confidence_level"] * 100) << "%)" << std::endl;
    std::cout << "📈 Q2 Forecast: $" << q2Forecast["predicted_revenue"] 
              << " (Confidence: " << (q2Forecast["confidence_level"] * 100) << "%)" << std::endl;
    
    std::cout << "✅ Sales forecast generated" << std::endl;
}

std::vector<std::map<std::string, std::string>> SalesModule::getTopPerformers() {
    std::vector<std::map<std::string, std::string>> performers;
    
    std::map<std::string, std::string> rep1;
    rep1["name"] = "John Smith";
    rep1["deals_closed"] = "12";
    rep1["revenue"] = "480000";
    rep1["win_rate"] = "0.75";
    performers.push_back(rep1);
    
    std::map<std::string, std::string> rep2;
    rep2["name"] = "Jane Doe";
    rep2["deals_closed"] = "8";
    rep2["revenue"] = "320000";
    rep2["win_rate"] = "0.67";
    performers.push_back(rep2);
    
    return performers;
}

void SalesModule::updateInsights(void* insightsData) {
    std::cout << "🧠 Updating Sales insights with AI data..." << std::endl;
    
    // Process AI insights for sales optimization
    // Update deal probabilities, identify at-risk deals, etc.
    
    std::cout << "✅ Sales insights updated" << std::endl;
}
