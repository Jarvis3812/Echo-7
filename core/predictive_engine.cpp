#include "predictive_engine.h"
#include "database.h"
#include "schema_model.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

PredictiveEngine::PredictiveEngine(Database* db, SchemaModel* schema) 
    : db_(db), schema_(schema) {
    std::cout << "🔮 Initializing Predictive Engine..." << std::endl;
    
    // Initialize default model configurations
    ForecastConfig sales_config;
    sales_config.metric_name = "sales_revenue";
    sales_config.periods_ahead = 12;
    sales_config.time_unit = "month";
    sales_config.model_type = "seasonal";
    sales_config.confidence_level = 0.95;
    model_configs_["sales_revenue"] = sales_config;
    
    ForecastConfig churn_config;
    churn_config.metric_name = "customer_churn";
    churn_config.periods_ahead = 6;
    churn_config.time_unit = "month";
    churn_config.model_type = "exponential";
    churn_config.confidence_level = 0.90;
    model_configs_["customer_churn"] = churn_config;
    
    std::cout << "✅ Predictive Engine initialized with " << model_configs_.size() << " default models" << std::endl;
}

PredictiveEngine::~PredictiveEngine() {
    std::cout << "🔄 Shutting down Predictive Engine..." << std::endl;
}

PredictiveEngine::PredictionResult PredictiveEngine::forecastMetric(const std::string& metric_name, const ForecastConfig& config) {
    std::cout << "🔮 Forecasting " << metric_name << " for " << config.periods_ahead << " " << config.time_unit << "(s)" << std::endl;
    
    PredictionResult result;
    result.metric_name = metric_name;
    result.model_type = config.model_type;
    result.confidence_interval = config.confidence_level;
    
    // Get historical data
    std::vector<double> historical_data = getHistoricalData(metric_name, 24); // Get 24 periods of history
    
    // Apply the specified forecasting model
    std::vector<double> predictions;
    if (config.model_type == "linear") {
        predictions = linearForecast(historical_data, config.periods_ahead);
    } else if (config.model_type == "exponential") {
        predictions = exponentialForecast(historical_data, config.periods_ahead);
    } else if (config.model_type == "seasonal") {
        predictions = seasonalForecast(historical_data, config.periods_ahead, 12);
    } else {
        predictions = movingAverageForecast(historical_data, config.periods_ahead, 3);
    }
    
    result.predicted_values = predictions;
    
    // Generate time periods
    for (int i = 1; i <= config.periods_ahead; ++i) {
        result.time_periods.push_back(config.time_unit + "_" + std::to_string(i));
    }
    
    // Calculate accuracy metrics (simulated)
    result.accuracy_metrics["mape"] = 0.12; // 12% Mean Absolute Percentage Error
    result.accuracy_metrics["rmse"] = 0.08; // Root Mean Square Error
    result.accuracy_metrics["r2"] = 0.85;   // R-squared
    
    // Add assumptions
    result.assumptions = {
        "Historical patterns continue",
        "No major market disruptions",
        "Current business conditions remain stable",
        "Seasonal patterns repeat"
    };
    
    std::cout << "✅ Forecast complete for " << metric_name << " (avg predicted: " 
              << calculateMean(predictions) << ")" << std::endl;
    
    return result;
}

PredictiveEngine::PredictionResult PredictiveEngine::predictSalesRevenue(int periods_ahead, const std::string& time_unit) {
    std::cout << "💰 Predicting sales revenue for " << periods_ahead << " " << time_unit << "(s)" << std::endl;
    
    ForecastConfig config = model_configs_["sales_revenue"];
    config.periods_ahead = periods_ahead;
    config.time_unit = time_unit;
    
    auto result = forecastMetric("sales_revenue", config);
    
    // Add sales-specific insights
    result.assumptions.push_back("Sales team performance remains consistent");
    result.assumptions.push_back("Marketing spend continues at current levels");
    result.assumptions.push_back("No major competitor actions");
    
    return result;
}

PredictiveEngine::TrendAnalysis PredictiveEngine::analyzeTrend(const std::string& metric_name, const std::vector<double>& historical_data) {
    std::cout << "📈 Analyzing trend for " << metric_name << std::endl;
    
    TrendAnalysis analysis;
    
    if (historical_data.empty()) {
        analysis.trend_direction = "unknown";
        analysis.trend_strength = 0.0;
        return analysis;
    }
    
    // Calculate slope and trend direction
    analysis.slope = calculateSlope(historical_data);
    analysis.trend_direction = determineTrendDirection(analysis.slope);
    analysis.trend_strength = calculateTrendStrength(historical_data);
    
    // Detect seasonality
    if (historical_data.size() >= 12) {
        analysis.seasonal_factors = detectSeasonality(historical_data, 12);
    }
    
    // Generate description
    if (analysis.trend_strength > 0.7) {
        analysis.trend_description = "Strong " + analysis.trend_direction + " trend detected";
    } else if (analysis.trend_strength > 0.4) {
        analysis.trend_description = "Moderate " + analysis.trend_direction + " trend detected";
    } else {
        analysis.trend_description = "Weak or no clear trend";
    }
    
    analysis.trend_duration_periods = static_cast<int>(historical_data.size());
    
    std::cout << "✅ Trend analysis complete: " << analysis.trend_description << std::endl;
    return analysis;
}

std::vector<double> PredictiveEngine::linearForecast(const std::vector<double>& data, int periods) {
    if (data.empty()) return {};
    
    double slope = calculateSlope(data);
    double last_value = data.back();
    
    std::vector<double> forecast;
    for (int i = 1; i <= periods; ++i) {
        forecast.push_back(last_value + slope * i);
    }
    
    return forecast;
}

std::vector<double> PredictiveEngine::exponentialForecast(const std::vector<double>& data, int periods) {
    if (data.empty()) return {};
    
    // Simple exponential smoothing
    double alpha = 0.3; // Smoothing parameter
    double smoothed = data[0];
    
    // Calculate smoothed values
    for (size_t i = 1; i < data.size(); ++i) {
        smoothed = alpha * data[i] + (1 - alpha) * smoothed;
    }
    
    // Forecast future periods
    std::vector<double> forecast;
    for (int i = 0; i < periods; ++i) {
        forecast.push_back(smoothed);
    }
    
    return forecast;
}

std::vector<double> PredictiveEngine::seasonalForecast(const std::vector<double>& data, int periods, int season_length) {
    if (data.size() < season_length) {
        return linearForecast(data, periods); // Fall back to linear if insufficient data
    }
    
    // Calculate seasonal indices
    std::vector<double> seasonal_indices = detectSeasonality(data, season_length);
    
    // Calculate trend
    double trend = calculateSlope(data);
    double base_level = calculateMean(data);
    
    std::vector<double> forecast;
    for (int i = 0; i < periods; ++i) {
        int season_index = i % season_length;
        double seasonal_factor = seasonal_indices.empty() ? 1.0 : seasonal_indices[season_index];
        double predicted_value = (base_level + trend * (data.size() + i)) * seasonal_factor;
        forecast.push_back(predicted_value);
    }
    
    return forecast;
}

std::vector<double> PredictiveEngine::detectSeasonality(const std::vector<double>& data, int season_length) {
    if (data.size() < season_length * 2) {
        return {}; // Need at least 2 seasons
    }
    
    std::vector<double> seasonal_indices(season_length, 0.0);
    std::vector<int> counts(season_length, 0);
    
    // Calculate average for each season position
    for (size_t i = 0; i < data.size(); ++i) {
        int season_pos = i % season_length;
        seasonal_indices[season_pos] += data[i];
        counts[season_pos]++;
    }
    
    // Normalize by count and overall average
    double overall_avg = calculateMean(data);
    for (int i = 0; i < season_length; ++i) {
        if (counts[i] > 0) {
            seasonal_indices[i] = (seasonal_indices[i] / counts[i]) / overall_avg;
        } else {
            seasonal_indices[i] = 1.0;
        }
    }
    
    return seasonal_indices;
}

double PredictiveEngine::calculateMean(const std::vector<double>& data) {
    if (data.empty()) return 0.0;
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double PredictiveEngine::calculateSlope(const std::vector<double>& data) {
    if (data.size() < 2) return 0.0;
    
    double n = static_cast<double>(data.size());
    double sum_x = n * (n - 1) / 2; // Sum of indices 0, 1, 2, ..., n-1
    double sum_y = std::accumulate(data.begin(), data.end(), 0.0);
    double sum_xy = 0.0;
    double sum_x2 = n * (n - 1) * (2 * n - 1) / 6; // Sum of squares
    
    for (size_t i = 0; i < data.size(); ++i) {
        sum_xy += i * data[i];
    }
    
    double slope = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    return slope;
}

std::string PredictiveEngine::determineTrendDirection(double slope, double threshold) {
    if (slope > threshold) return "upward";
    if (slope < -threshold) return "downward";
    return "stable";
}

double PredictiveEngine::calculateTrendStrength(const std::vector<double>& data) {
    if (data.size() < 3) return 0.0;
    
    // Calculate R-squared for linear trend
    double slope = calculateSlope(data);
    double mean_y = calculateMean(data);
    
    double ss_tot = 0.0; // Total sum of squares
    double ss_res = 0.0; // Residual sum of squares
    
    for (size_t i = 0; i < data.size(); ++i) {
        double predicted = mean_y + slope * (static_cast<double>(i) - (data.size() - 1) / 2.0);
        ss_tot += (data[i] - mean_y) * (data[i] - mean_y);
        ss_res += (data[i] - predicted) * (data[i] - predicted);
    }
    
    if (ss_tot == 0.0) return 0.0;
    return 1.0 - (ss_res / ss_tot);
}

std::vector<double> PredictiveEngine::getHistoricalData(const std::string& metric_name, int periods) {
    // Simulate historical data - in real implementation, this would query the database
    std::vector<double> data;
    std::random_device rd;
    std::mt19937 gen(rd());
    
    if (metric_name == "sales_revenue") {
        // Simulate sales data with growth trend and seasonality
        std::normal_distribution<> dis(100000, 10000);
        for (int i = 0; i < periods; ++i) {
            double base = 100000 + i * 2000; // Growth trend
            double seasonal = 1.0 + 0.2 * std::sin(2 * M_PI * i / 12); // Seasonal pattern
            data.push_back(base * seasonal + dis(gen) * 0.1);
        }
    } else {
        // Generic data
        std::normal_distribution<> dis(1000, 100);
        for (int i = 0; i < periods; ++i) {
            data.push_back(1000 + i * 10 + dis(gen));
        }
    }
    
    return data;
}

std::vector<double> PredictiveEngine::movingAverageForecast(const std::vector<double>& data, int periods, int window_size) {
    if (data.empty() || window_size <= 0) return {};

    std::vector<double> forecast;

    // Calculate moving average for the last window_size points
    double sum = 0.0;
    int count = std::min(window_size, static_cast<int>(data.size()));

    for (int i = data.size() - count; i < data.size(); ++i) {
        sum += data[i];
    }

    double avg = sum / count;

    // Use this average for all forecast periods
    for (int i = 0; i < periods; ++i) {
        forecast.push_back(avg);
    }

    return forecast;
}
