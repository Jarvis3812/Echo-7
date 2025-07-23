#pragma once
#include "common_types.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>

// Forward declarations
class Database;

/**
 * Memory Engine - Advanced memory management and knowledge retention
 * Provides intelligent storage, retrieval, and learning from historical data
 */
class MemoryEngine {
public:
    // Memory types
    enum class MemoryType {
        SHORT_TERM,    // Recent events, temporary data
        LONG_TERM,     // Persistent knowledge, patterns
        EPISODIC,      // Specific events and experiences
        SEMANTIC,      // Facts, rules, and relationships
        PROCEDURAL     // How-to knowledge, processes
    };

    // Memory entry structure
    struct MemoryEntry {
        std::string id;
        MemoryType type;
        std::string content;
        std::map<std::string, std::string> metadata;
        std::chrono::system_clock::time_point timestamp;
        double importance_score;
        double access_frequency;
        std::vector<std::string> tags;
        std::vector<std::string> related_entries;
    };

    // Memory query structure
    struct MemoryQuery {
        std::string query_text;
        MemoryType preferred_type;
        std::vector<std::string> required_tags;
        std::chrono::system_clock::time_point time_range_start;
        std::chrono::system_clock::time_point time_range_end;
        double min_importance;
        int max_results;
    };

    // Learning context
    struct LearningContext {
        std::string domain;           // CRM, Sales, Finance, etc.
        std::string event_type;       // decision, outcome, pattern, etc.
        CoreVariantMap data;
        std::string outcome;
        double success_score;
        std::vector<std::string> lessons_learned;
    };

public:
    MemoryEngine(Database* db);
    ~MemoryEngine();

    // Core memory operations
    std::string storeMemory(const MemoryEntry& entry);
    MemoryEntry retrieveMemory(const std::string& memory_id);
    std::vector<MemoryEntry> searchMemories(const MemoryQuery& query);
    bool updateMemory(const std::string& memory_id, const MemoryEntry& updated_entry);
    bool deleteMemory(const std::string& memory_id);

    // Intelligent storage
    std::string storeEvent(const std::string& event_description, const CoreVariantMap& context, MemoryType type = MemoryType::EPISODIC);
    std::string storeFact(const std::string& fact, const std::string& domain, double confidence = 1.0);
    std::string storePattern(const std::string& pattern_description, const CoreVariantMap& pattern_data);
    std::string storeProcedure(const std::string& procedure_name, const std::vector<std::string>& steps);

    // Learning and adaptation
    void learnFromExperience(const LearningContext& context);
    void reinforceMemory(const std::string& memory_id, double reinforcement_strength = 1.0);
    void associateMemories(const std::string& memory_id1, const std::string& memory_id2, const std::string& relationship_type);

    // Knowledge retrieval
    std::vector<MemoryEntry> recallSimilarSituations(const std::string& current_situation, const CoreVariantMap& context);
    std::vector<std::string> getRelevantFacts(const std::string& domain, const std::string& topic);
    std::vector<std::string> getApplicableProcedures(const std::string& task_type);
    std::vector<MemoryEntry> getRelatedMemories(const std::string& memory_id, int max_depth = 2);

    // Pattern recognition in memory
    std::vector<std::string> identifyRecurringPatterns(const std::string& domain);
    std::map<std::string, int> analyzeMemoryFrequency(const std::vector<std::string>& tags);
    std::vector<MemoryEntry> findAnomalousMemories(const std::string& domain);

    // Memory management
    void consolidateMemories(); // Move important short-term to long-term
    void forgetIrrelevantMemories(double importance_threshold = 0.1);
    void optimizeMemoryStorage();
    std::map<std::string, int> getMemoryStatistics();

    // Context-aware retrieval
    std::vector<MemoryEntry> getContextualMemories(const std::string& current_context, const CoreVariantMap& situation);
    std::vector<std::string> suggestActions(const std::string& situation, const CoreVariantMap& context);
    double calculateMemoryRelevance(const MemoryEntry& memory, const std::string& current_context);

    // Knowledge graph operations
    void buildKnowledgeGraph();
    std::vector<std::string> findShortestPath(const std::string& concept1, const std::string& concept2);
    std::vector<std::string> getConnectedConcepts(const std::string& concept, int max_distance = 2);

private:
    Database* db_;
    
    // Memory storage
    std::map<std::string, MemoryEntry> memory_store_;
    std::map<std::string, std::vector<std::string>> memory_associations_;
    std::map<std::string, std::vector<std::string>> tag_index_;
    
    // Memory management state
    std::chrono::system_clock::time_point last_consolidation_;
    std::map<std::string, double> importance_weights_;
    
    // Helper methods
    std::string generateMemoryId();
    double calculateImportance(const MemoryEntry& entry);
    void updateAccessFrequency(const std::string& memory_id);
    std::vector<std::string> extractTags(const std::string& content);
    
    // Similarity and matching
    double calculateSimilarity(const MemoryEntry& memory1, const MemoryEntry& memory2);
    double calculateTextSimilarity(const std::string& text1, const std::string& text2);
    std::vector<MemoryEntry> rankMemoriesByRelevance(const std::vector<MemoryEntry>& memories, const MemoryQuery& query);
    
    // Learning algorithms
    void updateImportanceScores();
    void identifyMemoryPatterns();
    void createMemoryAssociations();
    
    // Storage optimization
    void compressOldMemories();
    void mergeRedundantMemories();
    void archiveInactiveMemories();
    
    // Persistence
    void saveMemoryToDB(const MemoryEntry& entry);
    MemoryEntry loadMemoryFromDB(const std::string& memory_id);
    void saveAssociationsToDB();
    void loadAssociationsFromDB();
};

/**
 * Memory Analytics - Provides insights into memory usage and patterns
 */
class MemoryAnalytics {
public:
    MemoryAnalytics(MemoryEngine* memory_engine);
    
    // Memory usage analysis
    std::map<std::string, int> getMemoryDistribution();
    std::vector<std::string> getMostAccessedMemories(int count = 10);
    std::vector<std::string> getRecentLearnings(int days = 7);
    
    // Pattern analysis
    std::map<std::string, std::vector<std::string>> getMemoryPatterns();
    std::vector<std::string> getEmergingTrends();
    std::map<std::string, double> getKnowledgeGaps();
    
    // Performance metrics
    double getMemoryEfficiency();
    double getKnowledgeRetention();
    std::map<std::string, double> getLearningMetrics();

private:
    MemoryEngine* memory_engine_;
};
