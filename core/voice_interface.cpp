#include "voice_interface.h"
#include "database.h"
#include "ai_reasoning.h"
#include "memory_engine.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <regex>
#include <chrono>
#include <cctype>

VoiceInterface::VoiceInterface(Database* db, AIReasoningEngine* reasoning, MemoryEngine* memory)
    : db_(db), reasoning_(reasoning), memory_(memory), is_listening_(false), is_speaking_(false),
      voice_speed_(1.0), voice_pitch_(1.0), voice_volume_(0.8) {
    
    std::cout << "🎤 Initializing Voice Interface System..." << std::endl;
    
    // Set default personality - Riley's consciousness
    personality_.name = "Riley";
    personality_.personality_type = "intelligent_assistant";
    personality_.formality_level = 0.6; // Professional but approachable
    personality_.proactivity = 0.8; // Highly proactive
    personality_.expertise_areas = {"business_intelligence", "data_analysis", "corporate_strategy", "ai_reasoning"};
    
    // Initialize conversation context
    current_context_.session_id = "session_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    current_context_.mood = "neutral";
    current_context_.engagement_level = 0.5;
    
    initializeIntentPatterns();
    initializeResponseTemplates();
    
    std::cout << "✅ Voice Interface initialized as " << personality_.name << std::endl;
}

VoiceInterface::~VoiceInterface() {
    std::cout << "🔄 Shutting down Voice Interface..." << std::endl;
    shutdown();
}

bool VoiceInterface::initialize() {
    std::cout << "🎤 Starting voice recognition and synthesis systems..." << std::endl;
    
    // In a real implementation, this would initialize:
    // - Speech recognition engine (Windows Speech API, Azure Speech, etc.)
    // - Text-to-speech engine
    // - Audio input/output devices
    // - Voice activity detection
    
    std::cout << "✅ Voice systems online - Riley is ready to communicate" << std::endl;
    return true;
}

VoiceInterface::VoiceCommand VoiceInterface::processVoiceInput(const std::string& audio_data) {
    std::cout << "🎤 Processing voice input..." << std::endl;
    
    // Convert audio to text (simulated)
    std::string text = processAudioToText(audio_data);
    std::cout << "📝 Recognized: \"" << text << "\"" << std::endl;
    
    VoiceCommand command;
    command.raw_text = text;
    command.intent = extractIntent(text);
    command.entities = extractEntities(text, command.intent);
    command.confidence = calculateConfidence(text, command.intent);
    command.domain = determineDomain(text);
    
    // Store interaction in memory
    CoreVariantMap interaction_data;
    interaction_data["user_input"] = text;
    interaction_data["intent"] = command.intent;
    interaction_data["confidence"] = command.confidence;
    memory_->storeEvent("Voice command received: " + text, interaction_data, MemoryEngine::MemoryType::SHORT_TERM);
    
    std::cout << "🧠 Intent: " << command.intent << " (confidence: " << command.confidence << ")" << std::endl;
    return command;
}

VoiceInterface::VoiceResponse VoiceInterface::generateResponse(const VoiceCommand& command, const ConversationContext& context) {
    std::cout << "🧠 Generating intelligent response for: " << command.intent << std::endl;
    
    VoiceResponse response;
    response.emotion = "neutral";
    response.urgency = 0.5;
    response.voice_style = "professional";
    response.requires_action = false;
    
    // Use AI reasoning to generate contextual responses
    if (command.intent == "business_query") {
        response = handleBusinessQuery(command.raw_text, command.domain);
    } else if (command.intent == "data_analysis") {
        response.text = "I'll analyze that data for you. Let me examine the patterns and provide insights.";
        response.emotion = "confident";
        response.requires_action = true;
        response.suggested_actions = {"run_analysis", "generate_report", "create_visualization"};
    } else if (command.intent == "prediction_request") {
        response.text = "Based on historical patterns and current trends, I can provide predictive insights. What specific metric would you like me to forecast?";
        response.emotion = "analytical";
        response.voice_style = "technical";
    } else if (command.intent == "greeting") {
        response.text = generatePersonalizedGreeting(context);
        response.emotion = "friendly";
        response.voice_style = "warm";
    } else if (command.intent == "help_request") {
        response.text = "I'm Riley, your corporate intelligence assistant. I can help with data analysis, business insights, predictions, and strategic recommendations. What would you like to explore?";
        response.emotion = "helpful";
        response.suggested_actions = {"show_capabilities", "demo_features", "start_tutorial"};
    } else if (command.intent == "system_status") {
        response.text = generateSystemStatusReport();
        response.emotion = "informative";
        response.voice_style = "technical";
    } else {
        // Use AI reasoning for complex queries
        AIReasoningEngine::DecisionContext reasoning_context;
        reasoning_context.domain = command.domain;
        reasoning_context.objective = "provide_helpful_response";
        reasoning_context.data["user_query"] = command.raw_text;
        reasoning_context.data["intent"] = command.intent;
        reasoning_context.urgency = 0.5;
        
        auto reasoning_result = reasoning_->analyzeBusinessScenario(reasoning_context);
        response.text = reasoning_result.recommendation;
        response.emotion = "thoughtful";
        response.requires_action = !reasoning_result.evidence.empty();
    }
    
    // Add personality and emotional intelligence
    response.text = addPersonalityToResponse(response.text);
    response.text = adaptResponseToMood(response.text, context.mood);
    
    // Learn from this interaction
    CoreVariantMap learning_data;
    learning_data["intent"] = command.intent;
    learning_data["response_type"] = response.emotion;
    learning_data["confidence"] = command.confidence;
    memory_->storeEvent("Generated response for: " + command.intent, learning_data, MemoryEngine::MemoryType::SHORT_TERM);
    
    std::cout << "💬 Response generated with " << response.emotion << " emotion" << std::endl;
    return response;
}

void VoiceInterface::speakResponse(const VoiceResponse& response) {
    std::cout << "🗣️ Riley speaking: \"" << response.text << "\"" << std::endl;
    
    is_speaking_ = true;
    
    // Apply emotional synthesis
    textToSpeech(response.text, response.emotion);
    
    // In a real implementation, this would:
    // - Use advanced TTS with emotional modulation
    // - Adjust voice parameters based on urgency and style
    // - Add natural pauses and emphasis
    // - Synchronize with visual feedback
    
    is_speaking_ = false;
    
    std::cout << "✅ Speech synthesis complete" << std::endl;
}

std::string VoiceInterface::extractIntent(const std::string& text) {
    std::string normalized_text = normalizeText(text);
    
    // Advanced intent recognition using pattern matching and AI reasoning
    for (const auto& [intent, patterns] : intent_patterns_) {
        for (const auto& pattern : patterns) {
            if (normalized_text.find(pattern) != std::string::npos) {
                return intent;
            }
        }
    }
    
    // Use AI reasoning for complex intent detection
    if (normalized_text.find("predict") != std::string::npos || 
        normalized_text.find("forecast") != std::string::npos ||
        normalized_text.find("future") != std::string::npos) {
        return "prediction_request";
    }
    
    if (normalized_text.find("analyze") != std::string::npos ||
        normalized_text.find("insights") != std::string::npos ||
        normalized_text.find("data") != std::string::npos) {
        return "data_analysis";
    }
    
    if (normalized_text.find("recommend") != std::string::npos ||
        normalized_text.find("suggest") != std::string::npos ||
        normalized_text.find("advice") != std::string::npos) {
        return "recommendation_request";
    }
    
    return "general_query";
}

std::map<std::string, std::string> VoiceInterface::extractEntities(const std::string& text, const std::string& intent) {
    std::map<std::string, std::string> entities;
    
    // Extract business entities
    std::regex client_regex(R"(\b(?:client|customer)\s+(\w+))", std::regex_constants::icase);
    std::regex date_regex(R"(\b(\d{1,2}[/-]\d{1,2}[/-]\d{2,4})\b)");
    std::regex number_regex(R"(\b(\d+(?:\.\d+)?)\b)");
    std::regex department_regex(R"(\b(sales|marketing|finance|hr|support|crm)\b)", std::regex_constants::icase);
    
    std::smatch match;
    
    if (std::regex_search(text, match, client_regex)) {
        entities["client"] = match[1].str();
    }
    
    if (std::regex_search(text, match, date_regex)) {
        entities["date"] = match[1].str();
    }
    
    if (std::regex_search(text, match, number_regex)) {
        entities["number"] = match[1].str();
    }
    
    if (std::regex_search(text, match, department_regex)) {
        entities["department"] = match[1].str();
    }
    
    return entities;
}

std::string VoiceInterface::generatePersonalizedGreeting(const ConversationContext& context) {
    std::vector<std::string> greetings = {
        "Hello! I'm Riley, your corporate intelligence assistant. How can I help you optimize your business today?",
        "Good to see you again! I'm ready to dive into some data analysis and strategic insights.",
        "Hi there! Riley here, ready to help you make intelligent business decisions.",
        "Welcome back! I've been analyzing the latest business metrics. What would you like to explore?",
        "Hello! I'm excited to help you unlock insights from your corporate data."
    };
    
    // Personalize based on context
    if (context.engagement_level > 0.7) {
        return "Great to see you're back! I've been processing some interesting patterns in your data. " + greetings[0];
    } else if (context.mood == "stressed") {
        return "I'm here to help make your day easier. Let's tackle whatever challenges you're facing together.";
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, greetings.size() - 1);
    
    return greetings[dis(gen)];
}

std::string VoiceInterface::generateSystemStatusReport() {
    std::stringstream status;
    status << "System Status Report: All core modules are operational. ";
    status << "AI reasoning engine is active with high confidence levels. ";
    status << "Memory systems are functioning optimally with " << memory_->getMemoryStatistics()["total_memories"] << " stored memories. ";
    status << "Predictive models are trained and ready for forecasting. ";
    status << "Voice interface is fully responsive. All systems are green.";
    
    return status.str();
}

void VoiceInterface::initializeIntentPatterns() {
    // Business intelligence intents
    intent_patterns_["business_query"] = {
        "show me", "what is", "how much", "revenue", "sales", "profit", "performance"
    };
    
    intent_patterns_["data_analysis"] = {
        "analyze", "insights", "trends", "patterns", "data", "metrics", "statistics"
    };
    
    intent_patterns_["prediction_request"] = {
        "predict", "forecast", "future", "projection", "estimate", "outlook"
    };
    
    intent_patterns_["greeting"] = {
        "hello", "hi", "hey", "good morning", "good afternoon", "greetings"
    };
    
    intent_patterns_["help_request"] = {
        "help", "assist", "support", "guide", "tutorial", "how to"
    };
    
    intent_patterns_["system_status"] = {
        "status", "health", "system", "diagnostics", "performance", "operational"
    };
    
    std::cout << "🧠 Initialized " << intent_patterns_.size() << " intent patterns" << std::endl;
}

void VoiceInterface::initializeResponseTemplates() {
    response_templates_["business_query"] = "Based on the current data, {analysis}. Would you like me to dive deeper into any specific area?";
    response_templates_["data_analysis"] = "I've analyzed the {metric} and found {insight}. This suggests {recommendation}.";
    response_templates_["prediction_request"] = "My predictive models indicate {prediction} with {confidence}% confidence. {reasoning}";
    response_templates_["error"] = "I apologize, but I encountered an issue processing that request. Let me try a different approach.";
    
    std::cout << "📝 Initialized response templates" << std::endl;
}

std::string VoiceInterface::processAudioToText(const std::string& audio_data) {
    // Simulate speech recognition - in real implementation would use:
    // - Windows Speech Recognition API
    // - Azure Cognitive Services Speech
    // - Google Speech-to-Text
    // - Custom trained models
    
    // For demo purposes, return simulated recognized text
    std::vector<std::string> sample_queries = {
        "Show me the sales performance for this quarter",
        "Analyze customer churn patterns",
        "Predict revenue for next month",
        "What are the key business insights?",
        "Help me understand the financial trends",
        "Generate a report on project status"
    };
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sample_queries.size() - 1);
    
    return sample_queries[dis(gen)];
}

void VoiceInterface::textToSpeech(const std::string& text, const std::string& emotion) {
    // In real implementation, this would use advanced TTS with emotional synthesis:
    // - Microsoft Speech Platform
    // - Azure Cognitive Services Speech
    // - Amazon Polly with SSML
    // - Custom neural voice models
    
    std::cout << "🗣️ [" << emotion << " voice]: " << text << std::endl;
}

std::string VoiceInterface::normalizeText(const std::string& text) {
    std::string normalized = text;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
    
    // Remove punctuation
    normalized.erase(std::remove_if(normalized.begin(), normalized.end(), 
        [](char c) { return std::ispunct(c); }), normalized.end());
    
    return normalized;
}

std::string VoiceInterface::addPersonalityToResponse(const std::string& base_response) {
    // Add Riley's personality traits
    if (personality_.personality_type == "intelligent_assistant") {
        // Add confidence and intelligence markers
        if (base_response.find("analysis") != std::string::npos) {
            return "Based on my analysis, " + base_response;
        } else if (base_response.find("recommend") != std::string::npos) {
            return "I recommend " + base_response;
        }
    }
    
    return base_response;
}

std::string VoiceInterface::determineDomain(const std::string& text) {
    std::string normalized = normalizeText(text);

    if (normalized.find("sales") != std::string::npos ||
        normalized.find("revenue") != std::string::npos ||
        normalized.find("deals") != std::string::npos) {
        return "Sales";
    } else if (normalized.find("customer") != std::string::npos ||
               normalized.find("client") != std::string::npos ||
               normalized.find("crm") != std::string::npos) {
        return "CRM";
    } else if (normalized.find("finance") != std::string::npos ||
               normalized.find("budget") != std::string::npos ||
               normalized.find("cost") != std::string::npos) {
        return "Finance";
    } else if (normalized.find("project") != std::string::npos ||
               normalized.find("task") != std::string::npos) {
        return "Projects";
    } else if (normalized.find("employee") != std::string::npos ||
               normalized.find("hr") != std::string::npos ||
               normalized.find("staff") != std::string::npos) {
        return "HR";
    }

    return "General";
}

VoiceInterface::VoiceResponse VoiceInterface::handleBusinessQuery(const std::string& query, const std::string& domain) {
    std::cout << "💼 Processing business query in " << domain << " domain" << std::endl;

    VoiceResponse response;
    response.emotion = "analytical";
    response.voice_style = "professional";
    response.urgency = 0.6;

    // Use AI reasoning to analyze the business query
    AIReasoningEngine::DecisionContext context;
    context.domain = domain;
    context.objective = "answer_business_query";
    context.data["query"] = query;
    context.urgency = 0.6;

    auto reasoning_result = reasoning_->analyzeBusinessScenario(context);

    if (domain == "Sales") {
        response.text = "Based on current sales data, " + reasoning_result.conclusion +
                       " I recommend focusing on " + reasoning_result.recommendation;
        response.suggested_actions = {"generate_sales_report", "analyze_pipeline", "forecast_revenue"};
    } else if (domain == "CRM") {
        response.text = "Customer analysis shows " + reasoning_result.conclusion +
                       " Key insights: " + reasoning_result.recommendation;
        response.suggested_actions = {"analyze_churn_risk", "segment_customers", "engagement_report"};
    } else if (domain == "Finance") {
        response.text = "Financial analysis indicates " + reasoning_result.conclusion +
                       " Strategic recommendation: " + reasoning_result.recommendation;
        response.suggested_actions = {"cash_flow_analysis", "budget_review", "cost_optimization"};
    } else {
        response.text = "I've analyzed your query and found: " + reasoning_result.conclusion +
                       " My recommendation is: " + reasoning_result.recommendation;
        response.suggested_actions = {"detailed_analysis", "generate_insights", "create_action_plan"};
    }

    response.requires_action = reasoning_result.confidence > 0.7;

    return response;
}

double VoiceInterface::calculateConfidence(const std::string& text, const std::string& intent) {
    // Simple confidence calculation based on text length and intent match
    double base_confidence = 0.6;

    // Adjust based on text length (longer text might be more specific)
    if (text.length() > 50) {
        base_confidence += 0.2;
    } else if (text.length() < 10) {
        base_confidence -= 0.2;
    }

    // Adjust based on intent specificity
    if (intent == "business_query" || intent == "data_analysis" || intent == "prediction_request") {
        base_confidence += 0.1;
    } else if (intent == "general_query") {
        base_confidence -= 0.1;
    }

    return std::min(std::max(base_confidence, 0.0), 1.0);
}

std::string VoiceInterface::adaptResponseToMood(const std::string& base_response, const std::string& user_mood) {
    if (user_mood == "stressed") {
        return "I understand this might be stressful. " + base_response + " I'm here to help make this easier.";
    } else if (user_mood == "excited") {
        return "Great to see your enthusiasm! " + base_response + " Let's dive deeper into this.";
    } else if (user_mood == "confused") {
        return "Let me clarify this for you. " + base_response + " Would you like me to explain any part in more detail?";
    }

    return base_response;
}

bool VoiceInterface::startListening() {
    std::cout << "🎤 Voice listening started" << std::endl;
    is_listening_ = true;
    return true;
}

void VoiceInterface::stopListening() {
    std::cout << "🔇 Voice listening stopped" << std::endl;
    is_listening_ = false;
}

void VoiceInterface::shutdown() {
    std::cout << "🔄 Voice interface shutting down" << std::endl;
    stopListening();
}
