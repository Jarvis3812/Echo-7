#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include "schema_model.h"

class DatabaseIntelligence {
public:
    struct DataSource {
        std::string name;
        std::string type; // "csv", "excel", "sql", "json", "api"
        std::string connection_string;
        std::map<std::string, std::string> schema_mapping;
        bool is_active;
        std::string last_sync;
    };

    struct QueryResult {
        std::vector<std::map<std::string, std::string>> rows;
        std::vector<std::string> columns;
        int total_rows;
        std::string query_time;
        bool success;
        std::string error_message;
    };

    struct SchemaField {
        std::string name;
        std::string type;
        std::string description;
        bool is_key;
        bool is_nullable;
        std::vector<std::string> sample_values;
    };

    DatabaseIntelligence(SchemaModel* schema);
    ~DatabaseIntelligence() = default;

    // Data Ingestion
    bool ingestCSV(const std::string& file_path, const std::string& table_name);
    bool ingestExcel(const std::string& file_path, const std::string& table_name);
    bool ingestJSON(const std::string& file_path, const std::string& table_name);
    bool connectToSQL(const std::string& connection_string, const std::string& source_name);
    bool syncFromAPI(const std::string& api_endpoint, const std::string& table_name);

    // Schema Intelligence
    std::vector<SchemaField> analyzeSchema(const std::string& table_name);
    bool mapForeignSchema(const std::string& source_table, const std::string& target_table);
    std::map<std::string, std::string> detectFieldTypes(const std::vector<std::map<std::string, std::string>>& sample_data);
    bool normalizeData(const std::string& table_name);

    // Natural Language Querying
    QueryResult processNaturalLanguageQuery(const std::string& question);
    std::string translateToSQL(const std::string& natural_query);
    std::vector<std::string> suggestQueries(const std::string& context);

    // Live Synchronization
    bool startLiveSync(const std::string& source_name, int interval_minutes);
    bool stopLiveSync(const std::string& source_name);
    std::vector<std::string> getActiveSyncSources();
    bool updateFromSource(const std::string& source_name);

    // Data Analysis
    std::map<std::string, double> calculateTableStatistics(const std::string& table_name);
    std::vector<std::string> detectAnomalies(const std::string& table_name);
    std::map<std::string, std::vector<std::string>> findRelationships();
    
    // Corporate Intelligence
    QueryResult getBusinessMetrics(const std::string& metric_type);
    std::vector<std::string> generateInsights(const std::string& domain);
    bool flagDataQualityIssues(const std::string& table_name);

    // Data Sources Management
    bool addDataSource(const DataSource& source);
    bool removeDataSource(const std::string& source_name);
    std::vector<DataSource> getDataSources();
    bool testConnection(const std::string& source_name);

private:
    SchemaModel* schema_;
    std::map<std::string, DataSource> data_sources_;
    std::map<std::string, bool> sync_status_;
    
    // Helper methods
    std::string getCurrentTimestamp();
    bool validateTableExists(const std::string& table_name);
    std::vector<std::string> parseCSVLine(const std::string& line);
    std::string sanitizeSQL(const std::string& input);
    bool executeQuery(const std::string& query);
    
    // NLP helpers
    std::vector<std::string> extractKeywords(const std::string& query);
    std::string identifyQueryIntent(const std::string& query);
    std::map<std::string, std::string> mapFieldsToQuery(const std::vector<std::string>& keywords);
};
