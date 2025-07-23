#pragma once
#include "common_types.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

// Forward declarations
class Database;
class SchemaModel;

/**
 * Predictive Engine - Advanced forecasting and prediction capabilities
 * Provides time series forecasting, trend analysis, and predictive modeling
 */
class PredictiveEngine {
public:
    // Prediction result structure
    struct PredictionResult {
        std::string metric_name;
        std::vector<double> predicted_values;
        std::vector<std::string> time_periods;
        double confidence_interval;
        std::string model_type;
        std::map<std::string, double> accuracy_metrics;
        std::vector<std::string> assumptions;
    };

    // Forecast configuration
    struct ForecastConfig {
        std::string metric_name;
        int periods_ahead;
        std::string time_unit;        // day, week, month, quarter, year
        std::string model_type;       // linear, exponential, seasonal, ml
        double confidence_level;      // 0.8, 0.9, 0.95, 0.99
        std::map<std::string, double> parameters;
    };

    // Trend analysis result
    struct TrendAnalysis {
        std::string trend_direction;  // upward, downward, stable, volatile
        double trend_strength;        // 0.0 to 1.0
        double slope;                 // Rate of change
        std::vector<double> seasonal_factors;
        std::string trend_description;
        int trend_duration_periods;
    };

public:
    PredictiveEngine(Database* db, SchemaModel* schema);
    ~PredictiveEngine();

    // Core prediction capabilities
    PredictionResult forecastMetric(const std::string& metric_name, const ForecastConfig& config);
    PredictionResult predictSalesRevenue(int periods_ahead, const std::string& time_unit = "month");
    PredictionResult predictCustomerChurn(int periods_ahead);
    PredictionResult forecastCashFlow(int periods_ahead);
    PredictionResult predictProjectCompletion(int project_id);

    // Trend analysis
    TrendAnalysis analyzeTrend(const std::string& metric_name, const std::vector<double>& historical_data);
    std::vector<TrendAnalysis> identifyTrends(const std::map<std::string, std::vector<double>>& metrics);
    
    // Seasonal analysis
    std::vector<double> detectSeasonality(const std::vector<double>& data, int season_length = 12);
    std::map<std::string, double> analyzeSeasonalPatterns(const std::vector<double>& data);

    // Scenario modeling
    PredictionResult modelScenario(const std::string& scenario_name, const CoreVariantMap& assumptions);
    std::vector<PredictionResult> compareScenarios(const std::vector<std::string>& scenario_names);

    // Model management
    void trainModel(const std::string& model_name, const std::vector<double>& training_data);
    void updateModel(const std::string& model_name, const std::vector<double>& new_data);
    double evaluateModelAccuracy(const std::string& model_name, const std::vector<double>& test_data);

    // Advanced analytics
    std::map<std::string, double> calculateCorrelations(const std::map<std::string, std::vector<double>>& metrics);
    std::vector<std::string> identifyLeadingIndicators(const std::string& target_metric);
    PredictionResult predictWithLeadingIndicators(const std::string& target_metric, const CoreVariantMap& indicator_values);

    // Risk assessment
    std::map<std::string, double> assessPredictionRisks(const PredictionResult& prediction);
    std::vector<std::string> identifyRiskFactors(const std::string& metric_name);

private:
    Database* db_;
    SchemaModel* schema_;
    
    // Model storage
    std::map<std::string, std::vector<double>> trained_models_;
    std::map<std::string, double> model_accuracies_;
    std::map<std::string, ForecastConfig> model_configs_;
    
    // Helper methods for different prediction models
    std::vector<double> linearForecast(const std::vector<double>& data, int periods);
    std::vector<double> exponentialForecast(const std::vector<double>& data, int periods);
    std::vector<double> seasonalForecast(const std::vector<double>& data, int periods, int season_length);
    std::vector<double> movingAverageForecast(const std::vector<double>& data, int periods, int window_size);
    
    // Statistical helpers
    double calculateMean(const std::vector<double>& data);
    double calculateStandardDeviation(const std::vector<double>& data);
    double calculateCorrelation(const std::vector<double>& x, const std::vector<double>& y);
    std::vector<double> smoothData(const std::vector<double>& data, int window_size);
    
    // Trend calculation helpers
    double calculateSlope(const std::vector<double>& data);
    std::string determineTrendDirection(double slope, double threshold = 0.01);
    double calculateTrendStrength(const std::vector<double>& data);
    
    // Validation and accuracy
    double calculateMAPE(const std::vector<double>& actual, const std::vector<double>& predicted);
    double calculateRMSE(const std::vector<double>& actual, const std::vector<double>& predicted);
    double calculateR2(const std::vector<double>& actual, const std::vector<double>& predicted);
    
    // Data preparation
    std::vector<double> getHistoricalData(const std::string& metric_name, int periods);
    std::vector<double> normalizeData(const std::vector<double>& data);
    std::vector<double> removeOutliers(const std::vector<double>& data, double threshold = 2.0);
    
    // Business-specific prediction logic
    PredictionResult predictSalesGrowth(const CoreVariantMap& sales_data);
    PredictionResult predictCustomerLifetimeValue(const CoreVariantMap& customer_data);
    PredictionResult predictResourceNeeds(const CoreVariantMap& project_data);
};

/**
 * Predictive Model Factory - Creates specialized predictive models
 */
class PredictiveModelFactory {
public:
    static std::unique_ptr<PredictiveEngine> createSalesPredictor(Database* db, SchemaModel* schema);
    static std::unique_ptr<PredictiveEngine> createFinancePredictor(Database* db, SchemaModel* schema);
    static std::unique_ptr<PredictiveEngine> createHRPredictor(Database* db, SchemaModel* schema);
    static std::unique_ptr<PredictiveEngine> createOperationsPredictor(Database* db, SchemaModel* schema);
};
