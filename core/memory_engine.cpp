#include "memory_engine.h"
#include "database.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>

MemoryEngine::MemoryEngine(Database* db) : db_(db) {
    std::cout << "🧠 Initializing Memory Engine..." << std::endl;
    
    // Initialize importance weights for different domains
    importance_weights_["CRM"] = 0.9;
    importance_weights_["Sales"] = 0.85;
    importance_weights_["Finance"] = 0.95;
    importance_weights_["HR"] = 0.8;
    importance_weights_["Projects"] = 0.75;
    importance_weights_["Support"] = 0.7;
    
    last_consolidation_ = std::chrono::system_clock::now();
    
    // Load existing memories from database
    loadAssociationsFromDB();
    
    std::cout << "✅ Memory Engine initialized" << std::endl;
}

MemoryEngine::~MemoryEngine() {
    std::cout << "🔄 Shutting down Memory Engine..." << std::endl;
    
    // Save current state to database
    saveAssociationsToDB();
    
    std::cout << "✅ Memory Engine shutdown complete" << std::endl;
}

std::string MemoryEngine::storeMemory(const MemoryEntry& entry) {
    std::string memory_id = entry.id.empty() ? generateMemoryId() : entry.id;
    
    MemoryEntry stored_entry = entry;
    stored_entry.id = memory_id;
    stored_entry.timestamp = std::chrono::system_clock::now();
    stored_entry.access_frequency = 0.0;
    
    // Calculate importance if not set
    if (stored_entry.importance_score <= 0.0) {
        stored_entry.importance_score = calculateImportance(stored_entry);
    }
    
    // Extract tags if not provided
    if (stored_entry.tags.empty()) {
        stored_entry.tags = extractTags(stored_entry.content);
    }
    
    // Store in memory
    memory_store_[memory_id] = stored_entry;
    
    // Update tag index
    for (const auto& tag : stored_entry.tags) {
        tag_index_[tag].push_back(memory_id);
    }
    
    // Save to database
    saveMemoryToDB(stored_entry);
    
    std::cout << "💾 Stored memory: " << memory_id << " (type: " << static_cast<int>(stored_entry.type) << ")" << std::endl;
    return memory_id;
}

MemoryEngine::MemoryEntry MemoryEngine::retrieveMemory(const std::string& memory_id) {
    updateAccessFrequency(memory_id);
    
    auto it = memory_store_.find(memory_id);
    if (it != memory_store_.end()) {
        return it->second;
    }
    
    // Try loading from database
    return loadMemoryFromDB(memory_id);
}

std::vector<MemoryEngine::MemoryEntry> MemoryEngine::searchMemories(const MemoryQuery& query) {
    std::cout << "🔍 Searching memories: " << query.query_text << std::endl;
    
    std::vector<MemoryEntry> results;
    
    // Search through all memories
    for (const auto& [id, memory] : memory_store_) {
        bool matches = true;
        
        // Check memory type
        if (query.preferred_type != MemoryType::SHORT_TERM && memory.type != query.preferred_type) {
            continue;
        }
        
        // Check importance threshold
        if (memory.importance_score < query.min_importance) {
            continue;
        }
        
        // Check required tags
        for (const auto& required_tag : query.required_tags) {
            if (std::find(memory.tags.begin(), memory.tags.end(), required_tag) == memory.tags.end()) {
                matches = false;
                break;
            }
        }
        
        if (!matches) continue;
        
        // Check time range
        if (query.time_range_start != std::chrono::system_clock::time_point{} &&
            memory.timestamp < query.time_range_start) {
            continue;
        }
        
        if (query.time_range_end != std::chrono::system_clock::time_point{} &&
            memory.timestamp > query.time_range_end) {
            continue;
        }
        
        // Check content relevance (simple text search)
        if (!query.query_text.empty()) {
            std::string content_lower = memory.content;
            std::string query_lower = query.query_text;
            std::transform(content_lower.begin(), content_lower.end(), content_lower.begin(), ::tolower);
            std::transform(query_lower.begin(), query_lower.end(), query_lower.begin(), ::tolower);
            
            if (content_lower.find(query_lower) == std::string::npos) {
                continue;
            }
        }
        
        results.push_back(memory);
    }
    
    // Rank by relevance
    results = rankMemoriesByRelevance(results, query);
    
    // Limit results
    if (query.max_results > 0 && results.size() > static_cast<size_t>(query.max_results)) {
        results.resize(query.max_results);
    }
    
    std::cout << "✅ Found " << results.size() << " matching memories" << std::endl;
    return results;
}

std::string MemoryEngine::storeEvent(const std::string& event_description, const CoreVariantMap& context, MemoryType type) {
    MemoryEntry entry;
    entry.type = type;
    entry.content = event_description;
    entry.importance_score = 0.5; // Default importance
    
    // Add context as metadata
    for (const auto& [key, value] : context) {
        if (std::holds_alternative<std::string>(value)) {
            entry.metadata[key] = std::get<std::string>(value);
        } else if (std::holds_alternative<double>(value)) {
            entry.metadata[key] = std::to_string(std::get<double>(value));
        }
    }
    
    // Add event-specific tags
    entry.tags.push_back("event");
    entry.tags.push_back("episodic");
    
    return storeMemory(entry);
}

std::string MemoryEngine::storeFact(const std::string& fact, const std::string& domain, double confidence) {
    MemoryEntry entry;
    entry.type = MemoryType::SEMANTIC;
    entry.content = fact;
    entry.importance_score = confidence;
    entry.metadata["domain"] = domain;
    entry.metadata["confidence"] = std::to_string(confidence);
    
    entry.tags.push_back("fact");
    entry.tags.push_back("semantic");
    entry.tags.push_back(domain);
    
    return storeMemory(entry);
}

void MemoryEngine::learnFromExperience(const LearningContext& context) {
    std::cout << "📚 Learning from experience in domain: " << context.domain << std::endl;
    
    // Store the experience as an episodic memory
    MemoryEntry experience;
    experience.type = MemoryType::EPISODIC;
    experience.content = "Experience: " + context.event_type + " in " + context.domain;
    experience.importance_score = context.success_score;
    experience.metadata["domain"] = context.domain;
    experience.metadata["event_type"] = context.event_type;
    experience.metadata["outcome"] = context.outcome;
    experience.metadata["success_score"] = std::to_string(context.success_score);
    
    // Add context data as metadata
    for (const auto& [key, value] : context.data) {
        if (std::holds_alternative<std::string>(value)) {
            experience.metadata[key] = std::get<std::string>(value);
        } else if (std::holds_alternative<double>(value)) {
            experience.metadata[key] = std::to_string(std::get<double>(value));
        }
    }
    
    experience.tags.push_back("experience");
    experience.tags.push_back("learning");
    experience.tags.push_back(context.domain);
    experience.tags.push_back(context.event_type);
    
    std::string experience_id = storeMemory(experience);
    
    // Store lessons learned as semantic memories
    for (const auto& lesson : context.lessons_learned) {
        MemoryEntry lesson_memory;
        lesson_memory.type = MemoryType::SEMANTIC;
        lesson_memory.content = lesson;
        lesson_memory.importance_score = context.success_score * 0.8; // Slightly lower than experience
        lesson_memory.metadata["domain"] = context.domain;
        lesson_memory.metadata["source_experience"] = experience_id;
        
        lesson_memory.tags.push_back("lesson");
        lesson_memory.tags.push_back("knowledge");
        lesson_memory.tags.push_back(context.domain);
        
        std::string lesson_id = storeMemory(lesson_memory);
        
        // Associate lesson with experience
        associateMemories(experience_id, lesson_id, "lesson_from");
    }
    
    std::cout << "✅ Learning complete: stored experience and " << context.lessons_learned.size() << " lessons" << std::endl;
}

std::vector<MemoryEngine::MemoryEntry> MemoryEngine::recallSimilarSituations(const std::string& current_situation, const CoreVariantMap& context) {
    std::cout << "🔄 Recalling similar situations to: " << current_situation << std::endl;
    
    MemoryQuery query;
    query.query_text = current_situation;
    query.preferred_type = MemoryType::EPISODIC;
    query.min_importance = 0.3;
    query.max_results = 10;
    
    // Add context-based tags
    for (const auto& [key, value] : context) {
        if (std::holds_alternative<std::string>(value)) {
            query.required_tags.push_back(std::get<std::string>(value));
        }
    }
    
    auto similar_memories = searchMemories(query);
    
    std::cout << "✅ Found " << similar_memories.size() << " similar situations" << std::endl;
    return similar_memories;
}

void MemoryEngine::consolidateMemories() {
    std::cout << "🔄 Consolidating memories..." << std::endl;
    
    auto now = std::chrono::system_clock::now();
    auto time_since_last = std::chrono::duration_cast<std::chrono::hours>(now - last_consolidation_).count();
    
    if (time_since_last < 24) {
        std::cout << "⏰ Consolidation not needed yet (last: " << time_since_last << " hours ago)" << std::endl;
        return;
    }
    
    int consolidated_count = 0;
    
    // Move important short-term memories to long-term
    for (auto& [id, memory] : memory_store_) {
        if (memory.type == MemoryType::SHORT_TERM && memory.importance_score > 0.7) {
            memory.type = MemoryType::LONG_TERM;
            consolidated_count++;
        }
    }
    
    last_consolidation_ = now;
    
    std::cout << "✅ Consolidated " << consolidated_count << " memories to long-term storage" << std::endl;
}

std::string MemoryEngine::generateMemoryId() {
    static int counter = 0;
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    std::stringstream ss;
    ss << "mem_" << timestamp << "_" << (++counter);
    return ss.str();
}

double MemoryEngine::calculateImportance(const MemoryEntry& entry) {
    double importance = 0.5; // Base importance
    
    // Adjust based on memory type
    switch (entry.type) {
        case MemoryType::LONG_TERM:
            importance += 0.3;
            break;
        case MemoryType::SEMANTIC:
            importance += 0.2;
            break;
        case MemoryType::PROCEDURAL:
            importance += 0.25;
            break;
        default:
            break;
    }
    
    // Adjust based on domain
    auto domain_it = entry.metadata.find("domain");
    if (domain_it != entry.metadata.end()) {
        auto weight_it = importance_weights_.find(domain_it->second);
        if (weight_it != importance_weights_.end()) {
            importance *= weight_it->second;
        }
    }
    
    // Adjust based on content length (longer content might be more important)
    importance += std::min(entry.content.length() / 1000.0, 0.2);
    
    return std::min(importance, 1.0);
}

void MemoryEngine::updateAccessFrequency(const std::string& memory_id) {
    auto it = memory_store_.find(memory_id);
    if (it != memory_store_.end()) {
        it->second.access_frequency += 1.0;
    }
}

std::vector<std::string> MemoryEngine::extractTags(const std::string& content) {
    std::vector<std::string> tags;
    
    // Simple keyword extraction (in a real implementation, this would be more sophisticated)
    std::vector<std::string> keywords = {
        "client", "customer", "sales", "revenue", "project", "task", "employee", "finance",
        "support", "ticket", "deal", "contract", "meeting", "decision", "problem", "solution"
    };
    
    std::string content_lower = content;
    std::transform(content_lower.begin(), content_lower.end(), content_lower.begin(), ::tolower);
    
    for (const auto& keyword : keywords) {
        if (content_lower.find(keyword) != std::string::npos) {
            tags.push_back(keyword);
        }
    }
    
    return tags;
}

void MemoryEngine::associateMemories(const std::string& memory_id1, const std::string& memory_id2, const std::string& relationship_type) {
    memory_associations_[memory_id1].push_back(memory_id2);
    memory_associations_[memory_id2].push_back(memory_id1);
    
    std::cout << "🔗 Associated memories: " << memory_id1 << " <-> " << memory_id2 << " (" << relationship_type << ")" << std::endl;
}

std::vector<MemoryEngine::MemoryEntry> MemoryEngine::rankMemoriesByRelevance(const std::vector<MemoryEntry>& memories, const MemoryQuery& query) {
    std::vector<MemoryEntry> ranked_memories = memories;
    
    // Sort by relevance score (combination of importance, access frequency, and recency)
    std::sort(ranked_memories.begin(), ranked_memories.end(), [&](const MemoryEntry& a, const MemoryEntry& b) {
        double score_a = a.importance_score * 0.5 + a.access_frequency * 0.3;
        double score_b = b.importance_score * 0.5 + b.access_frequency * 0.3;
        
        // Add recency bonus
        auto now = std::chrono::system_clock::now();
        auto age_a = std::chrono::duration_cast<std::chrono::hours>(now - a.timestamp).count();
        auto age_b = std::chrono::duration_cast<std::chrono::hours>(now - b.timestamp).count();
        
        score_a += (1.0 / (1.0 + age_a / 24.0)) * 0.2; // Recency bonus
        score_b += (1.0 / (1.0 + age_b / 24.0)) * 0.2;
        
        return score_a > score_b;
    });
    
    return ranked_memories;
}

void MemoryEngine::saveMemoryToDB(const MemoryEntry& entry) {
    // In a real implementation, this would save to the database
    // For now, just log the operation
    std::cout << "💾 Saving memory to DB: " << entry.id << std::endl;
}

MemoryEngine::MemoryEntry MemoryEngine::loadMemoryFromDB(const std::string& memory_id) {
    // In a real implementation, this would load from the database
    // For now, return an empty entry
    std::cout << "📖 Loading memory from DB: " << memory_id << std::endl;
    return MemoryEntry{};
}

void MemoryEngine::saveAssociationsToDB() {
    std::cout << "💾 Saving memory associations to DB" << std::endl;
}

void MemoryEngine::loadAssociationsFromDB() {
    std::cout << "📖 Loading memory associations from DB" << std::endl;
}

std::map<std::string, int> MemoryEngine::getMemoryStatistics() {
    std::map<std::string, int> stats;
    stats["total_memories"] = static_cast<int>(memory_store_.size());
    stats["associations"] = static_cast<int>(memory_associations_.size());
    stats["tags"] = static_cast<int>(tag_index_.size());
    return stats;
}
