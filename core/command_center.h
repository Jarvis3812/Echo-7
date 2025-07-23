#pragma once
#include "common_types.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <queue>
#include <thread>
#include <atomic>
#include <chrono>

// Forward declarations
class Database;
class AIReasoningEngine;
class PredictiveEngine;
class MemoryEngine;
class VoiceInterface;

/**
 * Command Center - Central orchestration hub for Riley's consciousness
 * Manages autonomous operations, task coordination, and intelligent decision making
 */
class CommandCenter {
public:
    // Command types
    enum class CommandType {
        VOICE_COMMAND,
        UI_ACTION,
        SYSTEM_EVENT,
        AUTONOMOUS_TASK,
        SCHEDULED_OPERATION,
        EMERGENCY_RESPONSE
    };

    // Command priority levels
    enum class Priority {
        LOW = 1,
        NORMAL = 2,
        HIGH = 3,
        URGENT = 4,
        CRITICAL = 5
    };

    // Command structure
    struct Command {
        std::string id;
        CommandType type;
        Priority priority;
        std::string source;
        std::string action;
        CoreVariantMap parameters;
        std::chrono::system_clock::time_point timestamp;
        std::chrono::system_clock::time_point scheduled_time;
        std::string status; // pending, executing, completed, failed
        std::vector<std::string> dependencies;
        std::function<void(const Command&)> callback;
    };

    // Autonomous agent configuration
    struct AgentConfig {
        std::string name;
        std::string domain;           // CRM, Sales, Finance, etc.
        double autonomy_level;        // 0.0 to 1.0
        std::vector<std::string> capabilities;
        std::map<std::string, double> decision_thresholds;
        bool proactive_mode;
        int max_concurrent_tasks;
    };

    // System health metrics
    struct SystemHealth {
        double cpu_usage;
        double memory_usage;
        double response_time;
        int active_commands;
        int completed_commands;
        int failed_commands;
        std::string overall_status;
        std::vector<std::string> alerts;
    };

public:
    CommandCenter(Database* db, AIReasoningEngine* reasoning, PredictiveEngine* predictive, 
                  MemoryEngine* memory, VoiceInterface* voice);
    ~CommandCenter();

    // Core operations
    bool initialize();
    void shutdown();
    void startAutonomousMode();
    void stopAutonomousMode();
    
    // Command management
    std::string submitCommand(const Command& command);
    bool cancelCommand(const std::string& command_id);
    Command getCommandStatus(const std::string& command_id);
    std::vector<Command> getActiveCommands();
    
    // Autonomous operations
    void enableAutonomousAgent(const std::string& domain, const AgentConfig& config);
    void disableAutonomousAgent(const std::string& domain);
    void setAutonomyLevel(const std::string& domain, double level);
    
    // Task orchestration
    void scheduleRecurringTask(const std::string& task_name, const std::string& schedule,
                              std::function<void()> task_function);
    void executeWorkflow(const std::string& workflow_name, const CoreVariantMap& parameters);
    void createTaskDependency(const std::string& task_id, const std::string& dependency_id);
    
    // Intelligent decision making
    void enableProactiveMode(bool enabled);
    void setDecisionThreshold(const std::string& decision_type, double threshold);
    std::vector<std::string> generateProactiveSuggestions();
    
    // System monitoring
    SystemHealth getSystemHealth();
    void setHealthThreshold(const std::string& metric, double threshold);
    void enableSystemAlerts(bool enabled);
    
    // Event handling
    void registerEventHandler(const std::string& event_type, std::function<void(const CoreVariantMap&)> handler);
    void triggerEvent(const std::string& event_type, const CoreVariantMap& event_data);
    
    // Performance optimization
    void optimizeResourceAllocation();
    void balanceWorkload();
    void prioritizeCommands();
    
    // Learning and adaptation
    void learnFromCommandExecution(const Command& command, bool success, double execution_time);
    void adaptToUserBehavior(const std::string& user_id, const std::map<std::string, double>& behavior_metrics);
    void updateDecisionModels();
    
    // Integration points
    void handleVoiceCommand(const std::string& command_text, const CoreVariantMap& context);
    void handleUIAction(const std::string& action, const CoreVariantMap& parameters);
    void handleSystemEvent(const std::string& event_type, const CoreVariantMap& event_data);
    
    // Advanced capabilities
    void enableInventionMode(bool enabled); // Riley's creative problem-solving
    std::vector<std::string> generateInnovativeSolutions(const std::string& problem, const CoreVariantMap& constraints);
    void simulateScenarios(const std::vector<std::string>& scenarios);

private:
    // Core components
    Database* db_;
    AIReasoningEngine* reasoning_;
    PredictiveEngine* predictive_;
    MemoryEngine* memory_;
    VoiceInterface* voice_;
    
    // Command comparison for priority queue
    struct CommandComparator {
        bool operator()(const Command& a, const Command& b) const {
            return static_cast<int>(a.priority) < static_cast<int>(b.priority);
        }
    };

    // Command processing
    std::priority_queue<Command, std::vector<Command>, CommandComparator> command_queue_;
    std::map<std::string, Command> active_commands_;
    std::map<std::string, Command> command_history_;
    std::thread command_processor_;
    std::atomic<bool> processing_active_;
    
    // Autonomous agents
    std::map<std::string, AgentConfig> autonomous_agents_;
    std::map<std::string, std::thread> agent_threads_;
    std::atomic<bool> autonomous_mode_;
    
    // System state
    SystemHealth current_health_;
    std::map<std::string, double> health_thresholds_;
    std::atomic<bool> system_alerts_enabled_;
    
    // Event system
    std::map<std::string, std::vector<std::function<void(const CoreVariantMap&)>>> event_handlers_;
    
    // Scheduling
    std::map<std::string, std::function<void()>> scheduled_tasks_;
    std::thread scheduler_thread_;
    
    // Performance metrics
    std::map<std::string, std::vector<double>> execution_times_;
    std::map<std::string, int> command_success_counts_;
    std::map<std::string, int> command_failure_counts_;
    
    // Helper methods
    void processCommandQueue();
    void executeCommand(const Command& command);
    void runAutonomousAgent(const std::string& domain);
    void monitorSystemHealth();
    void runScheduler();
    
    // Command execution helpers
    bool validateCommand(const Command& command);
    void logCommandExecution(const Command& command, bool success, const std::string& result);
    void notifyCommandCompletion(const Command& command);
    
    // Autonomous decision making
    bool shouldExecuteAutonomously(const std::string& action, const CoreVariantMap& context);
    double calculateActionConfidence(const std::string& action, const CoreVariantMap& context);
    std::vector<std::string> generateAutonomousActions(const std::string& domain);
    
    // System optimization
    void rebalanceCommandPriorities();
    void optimizeAgentPerformance();
    void cleanupCompletedCommands();
    
    // Learning algorithms
    void updateCommandSuccessRates();
    void improveDecisionAccuracy();
    void adaptToSystemLoad();
    
    // Utility methods
    std::string generateCommandId();
    Priority calculateCommandPriority(const Command& command);
    bool checkCommandDependencies(const Command& command);
};

/**
 * Command Registry - Manages available commands and their implementations
 */
class CommandRegistry {
public:
    using CommandExecutor = std::function<bool(const CommandCenter::Command&)>;
    
    void registerCommand(const std::string& action, CommandExecutor executor);
    void unregisterCommand(const std::string& action);
    CommandExecutor getExecutor(const std::string& action);
    std::vector<std::string> getAvailableCommands();
    
    // Command validation
    bool validateCommandSyntax(const std::string& action, const CoreVariantMap& parameters);
    std::vector<std::string> getRequiredParameters(const std::string& action);
    
private:
    std::map<std::string, CommandExecutor> command_executors_;
    std::map<std::string, std::vector<std::string>> required_parameters_;
};

/**
 * Workflow Engine - Manages complex multi-step operations
 */
class WorkflowEngine {
public:
    struct WorkflowStep {
        std::string id;
        std::string action;
        CoreVariantMap parameters;
        std::vector<std::string> dependencies;
        std::function<bool(const CoreVariantMap&)> condition;
        std::function<void(const CoreVariantMap&)> on_success;
        std::function<void(const CoreVariantMap&)> on_failure;
    };
    
    struct Workflow {
        std::string name;
        std::vector<WorkflowStep> steps;
        CoreVariantMap global_parameters;
        std::string status;
        std::chrono::system_clock::time_point start_time;
    };
    
    void defineWorkflow(const std::string& name, const std::vector<WorkflowStep>& steps);
    bool executeWorkflow(const std::string& name, const CoreVariantMap& parameters);
    Workflow getWorkflowStatus(const std::string& name);
    void cancelWorkflow(const std::string& name);
    
private:
    std::map<std::string, Workflow> workflows_;
    std::map<std::string, std::thread> workflow_threads_;
    
    void runWorkflow(const std::string& name);
    bool executeWorkflowStep(const WorkflowStep& step, const CoreVariantMap& context);
};
