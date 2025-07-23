#include "ProjectsModule.h"
#include "common_types.h"
#include "database.h"
#include <iostream>
#include <sstream>
#include <map>

void ProjectsModule::registerModule() {
    std::cout << "📋 Registering Projects Module..." << std::endl;
    
    std::string createProjectsTable = R"(
        CREATE TABLE IF NOT EXISTS projects (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            description TEXT,
            client_id INTEGER,
            status TEXT DEFAULT 'Planning',
            start_date DATE,
            end_date DATE,
            budget REAL,
            progress REAL DEFAULT 0.0,
            project_manager TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (client_id) REFERENCES clients(id)
        );
    )";
    
    if (schema_->getDatabase()->execute(createProjectsTable)) {
        std::cout << "✅ Projects Module registered successfully" << std::endl;
    } else {
        std::cerr << "❌ Failed to register Projects Module" << std::endl;
    }
}

void ProjectsModule::addProject(const CoreVariantMap& projectData) {
    std::cout << "📋 Adding new project..." << std::endl;
    
    try {
        std::string name = projectData.count("name") ? projectData.at("name") : "New Project";
        std::string description = projectData.count("description") ? projectData.at("description") : "";
        std::string clientId = projectData.count("client_id") ? projectData.at("client_id") : "1";
        std::string budget = projectData.count("budget") ? projectData.at("budget") : "0";
        std::string manager = projectData.count("project_manager") ? projectData.at("project_manager") : "TBD";
        
        std::stringstream query;
        query << "INSERT INTO projects (name, description, client_id, budget, project_manager) VALUES ('"
              << name << "', '" << description << "', " << clientId << ", " << budget << ", '" << manager << "');";
        
        if (schema_->getDatabase()->execute(query.str())) {
            std::cout << "✅ Project '" << name << "' added successfully" << std::endl;
        } else {
            std::cerr << "❌ Failed to add project" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error adding project: " << e.what() << std::endl;
    }
}

std::vector<std::map<std::string, std::string>> ProjectsModule::getProjects() {
    std::vector<std::map<std::string, std::string>> projects;
    
    std::map<std::string, std::string> project1;
    project1["id"] = "1";
    project1["name"] = "Website Redesign";
    project1["status"] = "In Progress";
    project1["progress"] = "65";
    project1["project_manager"] = "Alice Johnson";
    projects.push_back(project1);
    
    std::map<std::string, std::string> project2;
    project2["id"] = "2";
    project2["name"] = "Mobile App Development";
    project2["status"] = "Planning";
    project2["progress"] = "15";
    project2["project_manager"] = "Bob Wilson";
    projects.push_back(project2);
    
    return projects;
}

std::map<std::string, double> ProjectsModule::getProjectMetrics() {
    std::map<std::string, double> metrics;
    
    metrics["total_projects"] = 25.0;
    metrics["active_projects"] = 8.0;
    metrics["completed_projects"] = 17.0;
    metrics["average_completion_rate"] = 0.72;
    metrics["on_time_delivery"] = 0.85;
    
    return metrics;
}
