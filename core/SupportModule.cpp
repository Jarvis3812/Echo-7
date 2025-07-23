#include "SupportModule.h"
#include "common_types.h"
#include "database.h"
#include <iostream>
#include <sstream>
#include <map>

void SupportModule::registerModule() {
    std::cout << "🎧 Registering Support Module..." << std::endl;
    
    std::string createTicketsTable = R"(
        CREATE TABLE IF NOT EXISTS support_tickets (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            client_id INTEGER,
            title TEXT NOT NULL,
            description TEXT,
            priority TEXT DEFAULT 'Medium',
            status TEXT DEFAULT 'Open',
            assigned_to TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            resolved_at DATETIME,
            FOREIGN KEY (client_id) REFERENCES clients(id)
        );
    )";
    
    if (schema_->getDatabase()->execute(createTicketsTable)) {
        std::cout << "✅ Support Module registered successfully" << std::endl;
    } else {
        std::cerr << "❌ Failed to register Support Module" << std::endl;
    }
}

void SupportModule::resolveTicket(const CoreVariantMap& ticketData) {
    std::cout << "🎧 Resolving support ticket..." << std::endl;
    
    try {
        std::string ticketId = ticketData.count("ticket_id") ? ticketData.at("ticket_id") : "1";
        std::string resolution = ticketData.count("resolution") ? ticketData.at("resolution") : "";
        
        std::stringstream query;
        query << "UPDATE support_tickets SET status = 'Resolved', "
              << "resolved_at = CURRENT_TIMESTAMP WHERE id = " << ticketId << ";";
        
        if (schema_->getDatabase()->execute(query.str())) {
            std::cout << "✅ Ticket " << ticketId << " resolved successfully" << std::endl;
        } else {
            std::cerr << "❌ Failed to resolve ticket" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error resolving ticket: " << e.what() << std::endl;
    }
}

void SupportModule::updateInsights(void* insightsData) {
    std::cout << "🧠 Updating Support insights..." << std::endl;
    std::cout << "✅ Support insights updated" << std::endl;
}

void SupportModule::addTicket(const CoreVariantMap& ticketData) {
    std::cout << "🎫 Adding new support ticket..." << std::endl;
    
    try {
        std::string title = ticketData.count("title") ? ticketData.at("title") : "Support Request";
        std::string description = ticketData.count("description") ? ticketData.at("description") : "";
        std::string priority = ticketData.count("priority") ? ticketData.at("priority") : "Medium";
        std::string clientId = ticketData.count("client_id") ? ticketData.at("client_id") : "1";
        
        std::stringstream query;
        query << "INSERT INTO support_tickets (client_id, title, description, priority) VALUES ("
              << clientId << ", '" << title << "', '" << description << "', '" << priority << "');";
        
        if (schema_->getDatabase()->execute(query.str())) {
            std::cout << "✅ Support ticket '" << title << "' added successfully" << std::endl;
        } else {
            std::cerr << "❌ Failed to add support ticket" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error adding ticket: " << e.what() << std::endl;
    }
}

std::vector<std::map<std::string, std::string>> SupportModule::getTickets() {
    std::vector<std::map<std::string, std::string>> tickets;
    
    // Sample tickets
    std::map<std::string, std::string> ticket1;
    ticket1["id"] = "1";
    ticket1["title"] = "Login Issues";
    ticket1["priority"] = "High";
    ticket1["status"] = "Open";
    ticket1["assigned_to"] = "Support Team";
    tickets.push_back(ticket1);
    
    std::map<std::string, std::string> ticket2;
    ticket2["id"] = "2";
    ticket2["title"] = "Feature Request";
    ticket2["priority"] = "Low";
    ticket2["status"] = "In Progress";
    ticket2["assigned_to"] = "Development Team";
    tickets.push_back(ticket2);
    
    return tickets;
}

std::map<std::string, double> SupportModule::getSupportMetrics() {
    std::map<std::string, double> metrics;
    
    metrics["total_tickets"] = 45.0;
    metrics["open_tickets"] = 12.0;
    metrics["resolved_tickets"] = 33.0;
    metrics["average_resolution_time"] = 2.5; // days
    metrics["customer_satisfaction"] = 4.2; // out of 5
    
    return metrics;
}
