#pragma once
#include "common_types.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

// Forward declarations
class Database;
class SchemaModel;

/**
 * AI Reasoning Engine - Advanced decision making and pattern recognition
 * Provides intelligent analysis, recommendations, and automated decision support
 */
class AIReasoningEngine {
public:
    // Reasoning result structure
    struct ReasoningResult {
        std::string conclusion;
        double confidence;
        std::vector<std::string> evidence;
        std::map<std::string, double> metrics;
        std::string recommendation;
    };

    // Decision context for complex reasoning
    struct DecisionContext {
        std::string domain;           // CRM, Sales, Finance, etc.
        std::string objective;        // What we're trying to achieve
        CoreVariantMap data;         // Input data for reasoning
        std::vector<std::string> constraints;  // Business rules/constraints
        double urgency;              // 0.0 to 1.0 urgency level
    };

    // Pattern recognition result
    struct Pattern {
        std::string type;            // trend, anomaly, correlation, etc.
        std::string description;
        double strength;             // Pattern strength 0.0 to 1.0
        std::map<std::string, double> parameters;
        std::vector<std::string> affected_entities;
    };

public:
    AIReasoningEngine(Database* db, SchemaModel* schema);
    ~AIReasoningEngine();

    // Core reasoning capabilities
    ReasoningResult analyzeBusinessScenario(const DecisionContext& context);
    std::vector<Pattern> detectPatterns(const std::string& domain, const CoreVariantMap& data);
    std::string generateRecommendation(const std::string& situation, const CoreVariantMap& context);

    // Specialized reasoning functions
    ReasoningResult analyzeCustomerChurn(int clientId);
    ReasoningResult optimizeSalesStrategy(const CoreVariantMap& salesData);
    ReasoningResult assessProjectRisk(int projectId);
    ReasoningResult evaluateFinancialHealth(const CoreVariantMap& financialData);

    // Pattern detection
    std::vector<Pattern> detectSalesTrends(const CoreVariantMap& salesData);
    std::vector<Pattern> identifyCustomerBehaviorPatterns(const CoreVariantMap& customerData);
    std::vector<Pattern> findOperationalAnomalies(const CoreVariantMap& operationalData);

    // Learning and adaptation
    void learnFromOutcome(const std::string& decisionId, const std::string& outcome, double success_score);
    void updateReasoningModel(const std::string& domain, const CoreVariantMap& newData);

    // Knowledge base management
    void addBusinessRule(const std::string& domain, const std::string& rule, double weight = 1.0);
    void removeBusinessRule(const std::string& domain, const std::string& ruleId);
    std::vector<std::string> getApplicableRules(const std::string& domain, const CoreVariantMap& context);

    // Explanation and transparency
    std::string explainReasoning(const std::string& decisionId);
    std::vector<std::string> getReasoningSteps(const ReasoningResult& result);

private:
    Database* db_;
    SchemaModel* schema_;
    
    // Internal reasoning state
    std::map<std::string, std::vector<std::string>> business_rules_;
    std::map<std::string, double> rule_weights_;
    std::map<std::string, ReasoningResult> reasoning_cache_;
    
    // Helper methods
    double calculateConfidence(const std::vector<std::string>& evidence, const CoreVariantMap& data);
    std::vector<std::string> gatherEvidence(const std::string& domain, const CoreVariantMap& data);
    std::string generateExplanation(const ReasoningResult& result);
    
    // Pattern analysis helpers
    Pattern analyzeTimeSeries(const std::vector<double>& values, const std::string& metric_name);
    Pattern detectCorrelations(const CoreVariantMap& data);
    Pattern identifyAnomalies(const std::vector<double>& values, const std::string& metric_name);
    
    // Business logic helpers
    double assessChurnRisk(const CoreVariantMap& clientData);
    std::string recommendSalesAction(const CoreVariantMap& dealData);
    double calculateProjectRiskScore(const CoreVariantMap& projectData);
    
    // Caching and performance
    void cacheResult(const std::string& key, const ReasoningResult& result);
    ReasoningResult getCachedResult(const std::string& key);
    void clearCache();
};

/**
 * AI Reasoning Factory - Creates specialized reasoning engines for different domains
 */
class AIReasoningFactory {
public:
    static std::unique_ptr<AIReasoningEngine> createCRMReasoner(Database* db, SchemaModel* schema);
    static std::unique_ptr<AIReasoningEngine> createSalesReasoner(Database* db, SchemaModel* schema);
    static std::unique_ptr<AIReasoningEngine> createFinanceReasoner(Database* db, SchemaModel* schema);
    static std::unique_ptr<AIReasoningEngine> createHRReasoner(Database* db, SchemaModel* schema);
};
