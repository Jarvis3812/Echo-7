// RileyCorpBrain header
#pragma once
#include <string>
#include <map>
// Forward declaration for Qt type
class QVariantMap;
class CRMModule;
class SalesModule;
class SupportModule;
class ProjectsModule;
class FinanceModule;
class HRModule;
class InventoryModule;
class ComplianceModule;
class Database;
class SchemaModel;
class PythonEmbed;
class RileyCorpBrain {
public:
    RileyCorpBrain();
    ~RileyCorpBrain();
    void initializeModules();
    void runAIAnalytics();
    void handleUIAction(const std::string& action, const QVariantMap& params);
private:
    Database* db;
    SchemaModel* schema;
    PythonEmbed* ai;
    CRMModule* crm;
    SalesModule* sales;
    SupportModule* support;
    ProjectsModule* projects;
    FinanceModule* finance;
    HRModule* hr;
    InventoryModule* inventory;
    ComplianceModule* compliance;
    bool initialized;
};
