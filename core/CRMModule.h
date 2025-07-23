#pragma once
#include "schema_model.h"
#include <string>
#include <vector>
#include <map>

// Forward declaration for Qt type
class QVariantMap;

class CRMModule {
public:
    CRMModule(SchemaModel* schema) : schema_(schema) {}

    // Module lifecycle
    void registerModule();

    // Client management
    void addClient(const QVariantMap& clientData);
    void updateClient(int clientId, const QVariantMap& updates);
    void deleteClient(int clientId);
    std::vector<std::map<std::string, std::string>> getClients();
    std::map<std::string, std::string> getClient(int clientId);

    // Client insights and analytics
    void updateInsights(void* insightsData);
    std::map<std::string, double> getClientMetrics();
    std::vector<std::map<std::string, std::string>> getHighValueClients();
    std::vector<std::map<std::string, std::string>> getAtRiskClients();

    // Client interaction tracking
    void logInteraction(int clientId, const std::string& type, const std::string& notes);
    std::vector<std::map<std::string, std::string>> getClientHistory(int clientId);

    // Client segmentation
    void segmentClients();
    std::string getClientSegment(int clientId);

private:
    SchemaModel* schema_;

    // Helper methods
    double calculateClientValue(int clientId);
    double calculateChurnRisk(int clientId);
    void updateClientScore(int clientId);
};
