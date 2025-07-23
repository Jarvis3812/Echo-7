#include "ai_reasoning.h"
#include "database.h"
#include "schema_model.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>
#include <sstream>

AIReasoningEngine::AIReasoningEngine(Database* db, SchemaModel* schema) 
    : db_(db), schema_(schema) {
    std::cout << "🧠 Initializing AI Reasoning Engine..." << std::endl;
    
    // Initialize default business rules
    addBusinessRule("CRM", "High value clients require immediate attention", 0.9);
    addBusinessRule("CRM", "Clients with declining engagement need intervention", 0.8);
    addBusinessRule("Sales", "Deals stalled for >30 days need review", 0.7);
    addBusinessRule("Sales", "Large deals require multiple touchpoints", 0.8);
    addBusinessRule("Finance", "Cash flow below 30 days requires immediate action", 0.95);
    addBusinessRule("HR", "Employee satisfaction below 3.0 needs intervention", 0.85);
    
    std::cout << "✅ AI Reasoning Engine initialized with " << business_rules_.size() << " rule domains" << std::endl;
}

AIReasoningEngine::~AIReasoningEngine() {
    std::cout << "🔄 Shutting down AI Reasoning Engine..." << std::endl;
    clearCache();
}

AIReasoningEngine::ReasoningResult AIReasoningEngine::analyzeBusinessScenario(const DecisionContext& context) {
    std::cout << "🎯 Analyzing business scenario in domain: " << context.domain << std::endl;
    
    ReasoningResult result;
    result.confidence = 0.0;
    
    // Gather evidence from the context
    auto evidence = gatherEvidence(context.domain, context.data);
    result.evidence = evidence;
    
    // Apply business rules
    auto applicable_rules = getApplicableRules(context.domain, context.data);
    
    // Calculate confidence based on evidence strength and rule coverage
    result.confidence = calculateConfidence(evidence, context.data);
    
    // Generate conclusion based on objective and evidence
    if (context.objective == "optimize_performance") {
        result.conclusion = "Performance optimization opportunities identified";
        result.recommendation = "Focus on high-impact areas with immediate ROI potential";
    } else if (context.objective == "risk_assessment") {
        result.conclusion = "Risk factors analyzed and prioritized";
        result.recommendation = "Implement mitigation strategies for high-priority risks";
    } else {
        result.conclusion = "Analysis completed with actionable insights";
        result.recommendation = "Review findings and implement suggested actions";
    }
    
    // Add metrics based on domain
    if (context.domain == "CRM") {
        result.metrics["churn_risk"] = assessChurnRisk(context.data);
        result.metrics["engagement_score"] = 0.75; // Simulated
    } else if (context.domain == "Sales") {
        result.metrics["conversion_probability"] = 0.68; // Simulated
        result.metrics["deal_velocity"] = 0.82; // Simulated
    }
    
    std::cout << "✅ Business scenario analysis complete (confidence: " << result.confidence << ")" << std::endl;
    return result;
}

std::vector<AIReasoningEngine::Pattern> AIReasoningEngine::detectPatterns(const std::string& domain, const CoreVariantMap& data) {
    std::cout << "🔍 Detecting patterns in domain: " << domain << std::endl;
    
    std::vector<Pattern> patterns;
    
    // Simulate pattern detection based on domain
    if (domain == "Sales") {
        Pattern trend_pattern;
        trend_pattern.type = "trend";
        trend_pattern.description = "Upward sales trend detected in Q4";
        trend_pattern.strength = 0.85;
        trend_pattern.parameters["growth_rate"] = 0.15;
        trend_pattern.parameters["duration_weeks"] = 8.0;
        trend_pattern.affected_entities = {"sales_team_1", "product_line_A"};
        patterns.push_back(trend_pattern);
        
        Pattern correlation_pattern;
        correlation_pattern.type = "correlation";
        correlation_pattern.description = "Strong correlation between marketing spend and lead generation";
        correlation_pattern.strength = 0.92;
        correlation_pattern.parameters["correlation_coefficient"] = 0.92;
        correlation_pattern.affected_entities = {"marketing_budget", "lead_pipeline"};
        patterns.push_back(correlation_pattern);
    } else if (domain == "CRM") {
        Pattern behavior_pattern;
        behavior_pattern.type = "behavior";
        behavior_pattern.description = "Customer engagement declining in enterprise segment";
        behavior_pattern.strength = 0.78;
        behavior_pattern.parameters["decline_rate"] = -0.12;
        behavior_pattern.affected_entities = {"enterprise_clients", "support_tickets"};
        patterns.push_back(behavior_pattern);
    }
    
    std::cout << "✅ Pattern detection complete: " << patterns.size() << " patterns found" << std::endl;
    return patterns;
}

std::string AIReasoningEngine::generateRecommendation(const std::string& situation, const CoreVariantMap& context) {
    std::cout << "💡 Generating recommendation for situation: " << situation << std::endl;
    
    std::string recommendation;
    
    if (situation == "declining_sales") {
        recommendation = "1. Analyze sales funnel bottlenecks\n2. Increase lead generation activities\n3. Review pricing strategy\n4. Enhance sales team training";
    } else if (situation == "high_churn_risk") {
        recommendation = "1. Implement customer success program\n2. Increase engagement touchpoints\n3. Offer retention incentives\n4. Conduct exit interviews";
    } else if (situation == "project_delays") {
        recommendation = "1. Reassess project scope and timeline\n2. Allocate additional resources\n3. Improve communication protocols\n4. Implement risk mitigation strategies";
    } else {
        recommendation = "1. Gather more data for analysis\n2. Consult with domain experts\n3. Implement monitoring systems\n4. Review and adjust strategy regularly";
    }
    
    std::cout << "✅ Recommendation generated" << std::endl;
    return recommendation;
}

AIReasoningEngine::ReasoningResult AIReasoningEngine::analyzeCustomerChurn(int clientId) {
    std::cout << "🎯 Analyzing churn risk for client ID: " << clientId << std::endl;
    
    ReasoningResult result;
    
    // Simulate client data analysis
    CoreVariantMap clientData;
    clientData["last_interaction_days"] = 45.0;
    clientData["support_tickets_count"] = 3.0;
    clientData["payment_delays"] = 1.0;
    clientData["engagement_score"] = 0.3;
    
    double churn_risk = assessChurnRisk(clientData);
    
    result.confidence = 0.87;
    result.conclusion = churn_risk > 0.7 ? "High churn risk detected" : "Moderate churn risk";
    result.metrics["churn_probability"] = churn_risk;
    result.metrics["engagement_score"] = 0.3;
    result.evidence = {
        "Last interaction 45 days ago",
        "Multiple support tickets",
        "Payment delay history",
        "Low engagement metrics"
    };
    
    if (churn_risk > 0.7) {
        result.recommendation = "Immediate intervention required: Schedule executive call, offer retention package, assign dedicated success manager";
    } else {
        result.recommendation = "Monitor closely: Increase touchpoint frequency, provide value-added services";
    }
    
    std::cout << "✅ Churn analysis complete (risk: " << churn_risk << ")" << std::endl;
    return result;
}

double AIReasoningEngine::assessChurnRisk(const CoreVariantMap& clientData) {
    double risk_score = 0.0;
    
    // Analyze various risk factors
    auto it = clientData.find("last_interaction_days");
    if (it != clientData.end()) {
        double days = std::get<double>(it->second);
        risk_score += std::min(days / 60.0, 1.0) * 0.3; // 30% weight
    }
    
    it = clientData.find("support_tickets_count");
    if (it != clientData.end()) {
        double tickets = std::get<double>(it->second);
        risk_score += std::min(tickets / 5.0, 1.0) * 0.2; // 20% weight
    }
    
    it = clientData.find("engagement_score");
    if (it != clientData.end()) {
        double engagement = std::get<double>(it->second);
        risk_score += (1.0 - engagement) * 0.4; // 40% weight
    }
    
    it = clientData.find("payment_delays");
    if (it != clientData.end()) {
        double delays = std::get<double>(it->second);
        risk_score += std::min(delays / 3.0, 1.0) * 0.1; // 10% weight
    }
    
    return std::min(risk_score, 1.0);
}

void AIReasoningEngine::addBusinessRule(const std::string& domain, const std::string& rule, double weight) {
    business_rules_[domain].push_back(rule);
    rule_weights_[domain + ":" + rule] = weight;
    std::cout << "📋 Added business rule to " << domain << ": " << rule << std::endl;
}

std::vector<std::string> AIReasoningEngine::getApplicableRules(const std::string& domain, const CoreVariantMap& context) {
    auto it = business_rules_.find(domain);
    if (it != business_rules_.end()) {
        return it->second;
    }
    return {};
}

double AIReasoningEngine::calculateConfidence(const std::vector<std::string>& evidence, const CoreVariantMap& data) {
    // Simple confidence calculation based on evidence count and data completeness
    double evidence_score = std::min(evidence.size() / 5.0, 1.0); // Normalize to 5 pieces of evidence
    double data_score = std::min(data.size() / 10.0, 1.0); // Normalize to 10 data points
    
    return (evidence_score * 0.6 + data_score * 0.4);
}

std::vector<std::string> AIReasoningEngine::gatherEvidence(const std::string& domain, const CoreVariantMap& data) {
    std::vector<std::string> evidence;
    
    // Gather evidence based on available data
    for (const auto& [key, value] : data) {
        if (std::holds_alternative<double>(value)) {
            double val = std::get<double>(value);
            evidence.push_back(key + " value: " + std::to_string(val));
        } else if (std::holds_alternative<std::string>(value)) {
            evidence.push_back(key + ": " + std::get<std::string>(value));
        }
    }
    
    return evidence;
}

void AIReasoningEngine::clearCache() {
    reasoning_cache_.clear();
    std::cout << "🧹 Reasoning cache cleared" << std::endl;
}
