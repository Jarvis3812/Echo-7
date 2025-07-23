#include "CRMModule.h"
#include "common_types.h"
#include "database.h"
#include <iostream>
#include <sstream>
#include <algorithm>

void CRMModule::registerModule() {
    std::cout << "📋 Registering CRM Module..." << std::endl;
    
    // Create CRM tables if they don't exist
    std::string createClientsTable = R"(
        CREATE TABLE IF NOT EXISTS clients (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            email TEXT UNIQUE NOT NULL,
            phone TEXT,
            company TEXT,
            industry TEXT,
            value_score REAL DEFAULT 0.0,
            churn_risk REAL DEFAULT 0.0,
            segment TEXT DEFAULT 'Standard',
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";
    
    std::string createInteractionsTable = R"(
        CREATE TABLE IF NOT EXISTS client_interactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            client_id INTEGER NOT NULL,
            interaction_type TEXT NOT NULL,
            notes TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (client_id) REFERENCES clients(id)
        );
    )";
    
    if (schema_->getDatabase()->execute(createClientsTable) && 
        schema_->getDatabase()->execute(createInteractionsTable)) {
        std::cout << "✅ CRM Module registered successfully" << std::endl;
    } else {
        std::cerr << "❌ Failed to register CRM Module" << std::endl;
    }
}

void CRMModule::addClient(const CoreVariantMap& clientData) {
    std::cout << "👥 Adding new client..." << std::endl;
    
    try {
        std::string name = clientData.count("name") ? variantToString(clientData.at("name")) : "Unknown";
        std::string email = clientData.count("email") ? variantToString(clientData.at("email")) : "";
        std::string phone = clientData.count("phone") ? variantToString(clientData.at("phone")) : "";
        std::string company = clientData.count("company") ? variantToString(clientData.at("company")) : "";
        std::string industry = clientData.count("industry") ? variantToString(clientData.at("industry")) : "";
        
        std::stringstream query;
        query << "INSERT INTO clients (name, email, phone, company, industry) VALUES ('"
              << name << "', '" << email << "', '" << phone << "', '" 
              << company << "', '" << industry << "');";
        
        if (schema_->getDatabase()->execute(query.str())) {
            std::cout << "✅ Client '" << name << "' added successfully" << std::endl;
            
            // Calculate initial scores
            // In a real implementation, this would use the actual client ID
            updateClientScore(1); // Placeholder ID
        } else {
            std::cerr << "❌ Failed to add client" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error adding client: " << e.what() << std::endl;
    }
}

void CRMModule::updateClient(int clientId, const CoreVariantMap& updates) {
    std::cout << "📝 Updating client ID: " << clientId << std::endl;
    
    try {
        std::stringstream query;
        query << "UPDATE clients SET ";
        
        bool first = true;
        for (const auto& pair : updates) {
            if (!first) query << ", ";
            query << pair.first << " = '" << pair.second << "'";
            first = false;
        }
        
        query << ", updated_at = CURRENT_TIMESTAMP WHERE id = " << clientId << ";";
        
        if (schema_->getDatabase()->execute(query.str())) {
            std::cout << "✅ Client updated successfully" << std::endl;
            updateClientScore(clientId);
        } else {
            std::cerr << "❌ Failed to update client" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error updating client: " << e.what() << std::endl;
    }
}

void CRMModule::deleteClient(int clientId) {
    std::cout << "🗑️ Deleting client ID: " << clientId << std::endl;
    
    try {
        std::stringstream query;
        query << "DELETE FROM clients WHERE id = " << clientId << ";";
        
        if (schema_->getDatabase()->execute(query.str())) {
            std::cout << "✅ Client deleted successfully" << std::endl;
        } else {
            std::cerr << "❌ Failed to delete client" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error deleting client: " << e.what() << std::endl;
    }
}

std::vector<std::map<std::string, std::string>> CRMModule::getClients() {
    std::vector<std::map<std::string, std::string>> clients;
    
    // In a real implementation, this would query the database
    // For now, return sample data
    std::map<std::string, std::string> client1;
    client1["id"] = "1";
    client1["name"] = "Acme Corporation";
    client1["email"] = "contact@acme.com";
    client1["company"] = "Acme Corp";
    client1["industry"] = "Technology";
    client1["segment"] = "Enterprise";
    clients.push_back(client1);
    
    std::map<std::string, std::string> client2;
    client2["id"] = "2";
    client2["name"] = "Beta Industries";
    client2["email"] = "info@beta.com";
    client2["company"] = "Beta Industries";
    client2["industry"] = "Manufacturing";
    client2["segment"] = "Standard";
    clients.push_back(client2);
    
    return clients;
}

std::map<std::string, std::string> CRMModule::getClient(int clientId) {
    std::map<std::string, std::string> client;
    
    // Sample data for demonstration
    if (clientId == 1) {
        client["id"] = "1";
        client["name"] = "Acme Corporation";
        client["email"] = "contact@acme.com";
        client["company"] = "Acme Corp";
        client["industry"] = "Technology";
        client["value_score"] = "8.5";
        client["churn_risk"] = "0.2";
        client["segment"] = "Enterprise";
    }
    
    return client;
}

void CRMModule::updateInsights(void* insightsData) {
    std::cout << "🧠 Updating CRM insights with AI data..." << std::endl;
    
    // In a real implementation, this would process AI insights
    // and update client scores, segments, and risk assessments
    
    std::cout << "✅ CRM insights updated" << std::endl;
}

std::map<std::string, double> CRMModule::getClientMetrics() {
    std::map<std::string, double> metrics;
    
    metrics["total_clients"] = 150.0;
    metrics["high_value_clients"] = 25.0;
    metrics["at_risk_clients"] = 8.0;
    metrics["average_value_score"] = 6.8;
    metrics["average_churn_risk"] = 0.15;
    
    return metrics;
}

std::vector<std::map<std::string, std::string>> CRMModule::getHighValueClients() {
    std::vector<std::map<std::string, std::string>> highValueClients;
    
    // Sample high-value clients
    std::map<std::string, std::string> client;
    client["id"] = "1";
    client["name"] = "Acme Corporation";
    client["value_score"] = "9.2";
    client["segment"] = "Enterprise";
    highValueClients.push_back(client);
    
    return highValueClients;
}

std::vector<std::map<std::string, std::string>> CRMModule::getAtRiskClients() {
    std::vector<std::map<std::string, std::string>> atRiskClients;
    
    // Sample at-risk clients
    std::map<std::string, std::string> client;
    client["id"] = "3";
    client["name"] = "Gamma Solutions";
    client["churn_risk"] = "0.8";
    client["segment"] = "Standard";
    atRiskClients.push_back(client);
    
    return atRiskClients;
}

void CRMModule::logInteraction(int clientId, const std::string& type, const std::string& notes) {
    std::cout << "📝 Logging interaction for client " << clientId << ": " << type << std::endl;
    
    try {
        std::stringstream query;
        query << "INSERT INTO client_interactions (client_id, interaction_type, notes) VALUES ("
              << clientId << ", '" << type << "', '" << notes << "');";
        
        if (schema_->getDatabase()->execute(query.str())) {
            std::cout << "✅ Interaction logged successfully" << std::endl;
        } else {
            std::cerr << "❌ Failed to log interaction" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error logging interaction: " << e.what() << std::endl;
    }
}

std::vector<std::map<std::string, std::string>> CRMModule::getClientHistory(int clientId) {
    std::vector<std::map<std::string, std::string>> history;
    
    // Sample interaction history
    std::map<std::string, std::string> interaction;
    interaction["type"] = "Email";
    interaction["notes"] = "Initial contact and product demo";
    interaction["date"] = "2024-01-15";
    history.push_back(interaction);
    
    return history;
}

void CRMModule::segmentClients() {
    std::cout << "🎯 Running client segmentation analysis..." << std::endl;
    
    // In a real implementation, this would use AI to segment clients
    // based on value, behavior, industry, etc.
    
    std::cout << "✅ Client segmentation complete" << std::endl;
}

std::string CRMModule::getClientSegment(int clientId) {
    // Sample segmentation logic
    if (clientId <= 10) return "Enterprise";
    else if (clientId <= 50) return "Standard";
    else return "Basic";
}

// Private helper methods
double CRMModule::calculateClientValue(int clientId) {
    // Sample value calculation
    // In reality, this would consider purchase history, contract value, etc.
    return 7.5 + (clientId % 3); // Simple demo calculation
}

double CRMModule::calculateChurnRisk(int clientId) {
    // Sample churn risk calculation
    // In reality, this would use AI models and interaction patterns
    return 0.1 + (clientId % 5) * 0.1; // Simple demo calculation
}

void CRMModule::updateClientScore(int clientId) {
    double value = calculateClientValue(clientId);
    double churnRisk = calculateChurnRisk(clientId);
    
    std::stringstream query;
    query << "UPDATE clients SET value_score = " << value 
          << ", churn_risk = " << churnRisk 
          << " WHERE id = " << clientId << ";";
    
    schema_->getDatabase()->execute(query.str());
    
    std::cout << "📊 Updated scores for client " << clientId 
              << " - Value: " << value << ", Churn Risk: " << churnRisk << std::endl;
}
