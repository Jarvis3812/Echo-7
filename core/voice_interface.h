#pragma once
#include "common_types.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <thread>
#include <atomic>

// Forward declarations
class Database;
class AIReasoningEngine;
class MemoryEngine;

/**
 * Voice Interface System - Advanced voice interaction and consciousness simulation
 * Provides natural language understanding, emotional intelligence, and autonomous conversation
 */
class VoiceInterface {
public:
    // Voice command structure
    struct VoiceCommand {
        std::string raw_text;
        std::string intent;
        std::map<std::string, std::string> entities;
        double confidence;
        std::string domain;
        std::vector<std::string> parameters;
    };

    // Voice response structure
    struct VoiceResponse {
        std::string text;
        std::string emotion;          // neutral, excited, concerned, confident, etc.
        double urgency;               // 0.0 to 1.0
        std::string voice_style;      // professional, casual, technical, empathetic
        bool requires_action;
        std::vector<std::string> suggested_actions;
    };

    // Conversation context
    struct ConversationContext {
        std::string user_id;
        std::string session_id;
        std::vector<std::string> conversation_history;
        std::map<std::string, std::string> user_preferences;
        std::string current_topic;
        std::string mood;
        double engagement_level;
    };

    // Voice personality configuration
    struct PersonalityConfig {
        std::string name;             // "Riley", "Assistant", etc.
        std::string personality_type; // professional, friendly, technical, creative
        double formality_level;       // 0.0 (casual) to 1.0 (formal)
        double proactivity;          // How often to offer suggestions
        std::vector<std::string> expertise_areas;
        std::map<std::string, std::string> speech_patterns;
    };

public:
    VoiceInterface(Database* db, AIReasoningEngine* reasoning, MemoryEngine* memory);
    ~VoiceInterface();

    // Core voice operations
    bool initialize();
    void shutdown();
    bool startListening();
    void stopListening();
    
    // Voice processing
    VoiceCommand processVoiceInput(const std::string& audio_data);
    VoiceResponse generateResponse(const VoiceCommand& command, const ConversationContext& context);
    void speakResponse(const VoiceResponse& response);
    
    // Natural language understanding
    std::string extractIntent(const std::string& text);
    std::map<std::string, std::string> extractEntities(const std::string& text, const std::string& intent);
    double calculateConfidence(const std::string& text, const std::string& intent);
    
    // Conversation management
    void startConversation(const std::string& user_id);
    void endConversation();
    void updateConversationContext(const std::string& user_input, const std::string& response);
    ConversationContext getCurrentContext() const { return current_context_; }
    
    // Personality and emotional intelligence
    void setPersonality(const PersonalityConfig& config);
    std::string adaptResponseToMood(const std::string& base_response, const std::string& user_mood);
    std::string detectUserEmotion(const std::string& text, const std::string& voice_tone = "");
    
    // Advanced capabilities
    void enableProactiveMode(bool enabled);
    void suggestActions(const ConversationContext& context);
    std::string generateCreativeResponse(const std::string& topic, const std::string& style);
    
    // Business intelligence integration
    VoiceResponse handleBusinessQuery(const std::string& query, const std::string& domain);
    std::string explainDataInsights(const std::map<std::string, double>& metrics, const std::string& context);
    VoiceResponse provideRecommendations(const std::string& situation, const CoreVariantMap& data);
    
    // Learning and adaptation
    void learnFromInteraction(const VoiceCommand& command, const VoiceResponse& response, double user_satisfaction);
    void updateLanguageModel(const std::vector<std::string>& training_data);
    void personalizeForUser(const std::string& user_id, const std::map<std::string, std::string>& preferences);
    
    // Voice synthesis customization
    void setVoiceParameters(const std::string& voice_id, double speed, double pitch, double volume);
    void addCustomPronunciation(const std::string& word, const std::string& pronunciation);
    void enableEmotionalSynthesis(bool enabled);
    
    // Multi-language support
    void setLanguage(const std::string& language_code);
    std::vector<std::string> getSupportedLanguages();
    std::string translateText(const std::string& text, const std::string& target_language);
    
    // Callbacks and events
    void setVoiceCommandCallback(std::function<void(const VoiceCommand&)> callback);
    void setConversationEventCallback(std::function<void(const std::string&, const ConversationContext&)> callback);

private:
    Database* db_;
    AIReasoningEngine* reasoning_;
    MemoryEngine* memory_;
    
    // Voice processing state
    std::atomic<bool> is_listening_;
    std::atomic<bool> is_speaking_;
    std::thread listening_thread_;
    std::thread processing_thread_;
    
    // Conversation state
    ConversationContext current_context_;
    PersonalityConfig personality_;
    std::map<std::string, ConversationContext> user_contexts_;
    
    // Language processing
    std::map<std::string, std::vector<std::string>> intent_patterns_;
    std::map<std::string, std::vector<std::string>> entity_patterns_;
    std::map<std::string, std::string> response_templates_;
    
    // Voice synthesis
    std::string current_voice_id_;
    double voice_speed_;
    double voice_pitch_;
    double voice_volume_;
    std::map<std::string, std::string> custom_pronunciations_;
    
    // Callbacks
    std::function<void(const VoiceCommand&)> command_callback_;
    std::function<void(const std::string&, const ConversationContext&)> conversation_callback_;
    
    // Helper methods
    void initializeIntentPatterns();
    void initializeResponseTemplates();
    std::string processAudioToText(const std::string& audio_data);
    void textToSpeech(const std::string& text, const std::string& emotion = "neutral");
    std::string generatePersonalizedGreeting(const ConversationContext& context);
    std::string generateSystemStatusReport();
    std::string determineDomain(const std::string& text);
    
    // Natural language processing helpers
    std::vector<std::string> tokenizeText(const std::string& text);
    std::string normalizeText(const std::string& text);
    double calculateSemanticSimilarity(const std::string& text1, const std::string& text2);
    
    // Response generation helpers
    std::string selectResponseTemplate(const std::string& intent, const std::string& emotion);
    std::string fillResponseTemplate(const std::string& template_text, const std::map<std::string, std::string>& entities);
    std::string addPersonalityToResponse(const std::string& base_response);
    
    // Conversation flow helpers
    void updateUserMood(const std::string& user_input);
    void trackEngagementLevel(const ConversationContext& context);
    bool shouldOfferProactiveSuggestion();
    
    // Learning helpers
    void updateIntentModel(const std::string& text, const std::string& correct_intent);
    void improveResponseQuality(const VoiceResponse& response, double satisfaction_score);
    void adaptToUserPreferences(const std::string& user_id);
};

/**
 * Voice Command Registry - Manages available voice commands and their handlers
 */
class VoiceCommandRegistry {
public:
    using CommandHandler = std::function<VoiceInterface::VoiceResponse(const VoiceInterface::VoiceCommand&)>;
    
    void registerCommand(const std::string& intent, CommandHandler handler);
    void unregisterCommand(const std::string& intent);
    CommandHandler getHandler(const std::string& intent);
    std::vector<std::string> getAvailableCommands();
    
private:
    std::map<std::string, CommandHandler> command_handlers_;
};

/**
 * Voice Analytics - Provides insights into voice interaction patterns
 */
class VoiceAnalytics {
public:
    VoiceAnalytics(VoiceInterface* voice_interface);
    
    // Usage analytics
    std::map<std::string, int> getCommandFrequency();
    std::map<std::string, double> getUserSatisfactionScores();
    std::vector<std::string> getMostUsedIntents(int count = 10);
    
    // Performance metrics
    double getAverageResponseTime();
    double getVoiceRecognitionAccuracy();
    double getConversationEngagementScore();
    
    // Improvement suggestions
    std::vector<std::string> suggestPersonalityImprovements();
    std::vector<std::string> identifyMissingCommands();
    std::map<std::string, std::string> getOptimizationRecommendations();

private:
    VoiceInterface* voice_interface_;
    std::map<std::string, int> command_usage_;
    std::map<std::string, std::vector<double>> satisfaction_history_;
};
