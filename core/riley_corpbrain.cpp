// Riley Corpbrain - Master Enterprise Intelligence Engine
// The most advanced corporate tracking system ever created
#include "riley_corpbrain.h"
#include "schema_model.h"
#include "database.h"
#include "python_embed.h"
#include "CRMModule.h"
#include "SalesModule.h"
#include "SupportModule.h"
#include "ProjectsModule.h"
#include "FinanceModule.h"
#include "HRModule.h"
#include "InventoryModule.h"
#include "ComplianceModule.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <chrono>
#include <thread>

// Constructor - Initialize the corporate brain
RileyCorpBrain::RileyCorpBrain() : initialized(false) {
    std::cout << "🧠 RILEY CORPBRAIN INITIALIZING..." << std::endl;

    try {
        // Initialize core systems
        db = new Database();
        schema = new SchemaModel(db);
        ai = new PythonEmbed();

        // Initialize all business modules
        crm = new CRMModule(schema);
        sales = new SalesModule(schema);
        support = new SupportModule(schema);
        projects = new ProjectsModule(schema);
        finance = new FinanceModule(schema);
        hr = new HRModule(schema);
        inventory = new InventoryModule(schema);
        compliance = new ComplianceModule(schema);

        std::cout << "✅ Core systems initialized" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "❌ Initialization failed: " << e.what() << std::endl;
        throw;
    }
}

// Destructor - Clean shutdown
RileyCorpBrain::~RileyCorpBrain() {
    std::cout << "🔄 RILEY CORPBRAIN SHUTTING DOWN..." << std::endl;

    // Clean up modules
    delete compliance;
    delete inventory;
    delete hr;
    delete finance;
    delete projects;
    delete support;
    delete sales;
    delete crm;

    // Clean up core systems
    delete ai;
    delete schema;
    delete db;

    std::cout << "✅ Shutdown complete" << std::endl;
}

// Initialize all modules and establish cross-module intelligence
void RileyCorpBrain::initializeModules() {
    if (initialized) {
        std::cout << "⚠️ Modules already initialized" << std::endl;
        return;
    }

    std::cout << "🚀 INITIALIZING ENTERPRISE MODULES..." << std::endl;

    try {
        // Register all modules
        crm->registerModule();
        sales->registerModule();
        support->registerModule();
        projects->registerModule();
        finance->registerModule();
        hr->registerModule();
        inventory->registerModule();
        compliance->registerModule();

        // Initialize database schema
        if (!db->execute("PRAGMA foreign_keys = ON;")) {
            throw std::runtime_error("Failed to enable foreign keys");
        }

        initialized = true;
        std::cout << "✅ All modules initialized successfully" << std::endl;

        // Run initial AI analytics
        runAIAnalytics();

    } catch (const std::exception& e) {
        std::cerr << "❌ Module initialization failed: " << e.what() << std::endl;
        throw;
    }
}

// AI Analytics Engine - The brain of the operation
void RileyCorpBrain::runAIAnalytics() {
    if (!initialized) {
        std::cerr << "⚠️ Cannot run analytics - modules not initialized" << std::endl;
        return;
    }

    std::cout << "🧠 RUNNING AI ANALYTICS..." << std::endl;

    try {
        // Gather data from all modules
        auto salesData = schema->getSalesData();
        auto clientData = schema->getClientData();

        // Run AI analysis on sales trends
        auto salesInsights = ai->callFunction("analyze_sales_trends", salesData);
        std::cout << "📈 Sales trend analysis complete" << std::endl;

        // Run churn prediction
        auto churnInsights = ai->callFunction("predict_churn", clientData);
        std::cout << "🎯 Churn prediction complete" << std::endl;

        // Update module insights
        crm->updateInsights(churnInsights);
        support->updateInsights(salesInsights);

        std::cout << "✅ AI analytics complete" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ AI analytics failed: " << e.what() << std::endl;
    }
}

// Handle UI actions and route to appropriate modules
void RileyCorpBrain::handleUIAction(const std::string& action, const QVariantMap& params) {
    if (!initialized) {
        std::cerr << "⚠️ Cannot handle action - modules not initialized" << std::endl;
        return;
    }

    std::cout << "🎯 Handling action: " << action << std::endl;

    try {
        // Route actions to appropriate modules
        if (action == "add_client") {
            crm->addClient(params);
            std::cout << "✅ Client added successfully" << std::endl;
        }
        else if (action == "add_sale") {
            sales->addSale(params);
            std::cout << "✅ Sale added successfully" << std::endl;
        }
        else if (action == "resolve_ticket") {
            support->resolveTicket(params);
            std::cout << "✅ Ticket resolved successfully" << std::endl;
        }
        else if (action == "add_project") {
            projects->addProject(params);
            std::cout << "✅ Project added successfully" << std::endl;
        }
        else if (action == "add_employee") {
            hr->addEmployee(params);
            std::cout << "✅ Employee added successfully" << std::endl;
        }
        else if (action == "add_expense") {
            finance->addExpense(params);
            std::cout << "✅ Expense added successfully" << std::endl;
        }
        else if (action == "add_item") {
            inventory->addItem(params);
            std::cout << "✅ Inventory item added successfully" << std::endl;
        }
        else if (action == "audit") {
            compliance->audit(params);
            std::cout << "✅ Audit completed successfully" << std::endl;
        }
        else if (action == "run_analytics") {
            runAIAnalytics();
        }
        else {
            std::cerr << "⚠️ Unknown action: " << action << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "❌ Action failed: " << e.what() << std::endl;
    }
}
