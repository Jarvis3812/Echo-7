#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <functional>

class AutonomousScheduler {
public:
    enum class TaskStatus {
        NOT_STARTED,
        IN_PROGRESS,
        COMPLETED,
        DELAYED,
        BLOCKED,
        CANCELLED
    };

    enum class Priority {
        LOW = 1,
        MEDIUM = 2,
        HIGH = 3,
        CRITICAL = 4,
        EMERGENCY = 5
    };

    struct Task {
        std::string task_id;
        std::string name;
        std::string description;
        TaskStatus status;
        Priority priority;
        std::string assigned_to;
        std::string project_id;
        std::chrono::system_clock::time_point start_date;
        std::chrono::system_clock::time_point due_date;
        std::chrono::system_clock::time_point estimated_completion;
        std::vector<std::string> dependencies;
        std::vector<std::string> resources_required;
        double progress_percentage;
        int estimated_hours;
        int actual_hours;
        std::string notes;
    };

    struct Project {
        std::string project_id;
        std::string name;
        std::string description;
        TaskStatus status;
        Priority priority;
        std::string project_manager;
        std::chrono::system_clock::time_point start_date;
        std::chrono::system_clock::time_point due_date;
        std::vector<std::string> task_ids;
        double budget;
        double spent_budget;
        std::map<std::string, std::string> milestones;
    };

    struct ScheduleAlert {
        std::string alert_id;
        std::string timestamp;
        std::string type; // "delay_warning", "completion", "resource_conflict", "dependency_issue"
        std::string message;
        Priority severity;
        std::string affected_task_id;
        std::string recommended_action;
        bool is_resolved;
    };

    struct ResourceAllocation {
        std::string resource_id;
        std::string resource_name;
        std::string resource_type; // "person", "equipment", "budget"
        std::vector<std::string> allocated_tasks;
        double utilization_percentage;
        std::chrono::system_clock::time_point available_from;
        std::chrono::system_clock::time_point available_until;
    };

    AutonomousScheduler();
    ~AutonomousScheduler() = default;

    // Task Management
    std::string createTask(const Task& task);
    bool updateTask(const std::string& task_id, const Task& updated_task);
    bool deleteTask(const std::string& task_id);
    Task getTask(const std::string& task_id);
    std::vector<Task> getAllTasks();
    std::vector<Task> getTasksByProject(const std::string& project_id);
    std::vector<Task> getTasksByAssignee(const std::string& assignee);

    // Project Management
    std::string createProject(const Project& project);
    bool updateProject(const std::string& project_id, const Project& updated_project);
    Project getProject(const std::string& project_id);
    std::vector<Project> getAllProjects();

    // Autonomous Scheduling Intelligence
    bool analyzeScheduleHealth();
    std::vector<ScheduleAlert> getActiveAlerts();
    bool optimizeSchedule();
    std::vector<Task> getNextRecommendedTasks(const std::string& assignee);
    bool autoAssignTasks();

    // Timeline Prediction & Management
    std::chrono::system_clock::time_point predictTaskCompletion(const std::string& task_id);
    std::chrono::system_clock::time_point predictProjectCompletion(const std::string& project_id);
    bool detectPotentialDelays();
    std::vector<std::string> identifyBottlenecks();
    bool rebalanceWorkload();

    // Dependency Management
    bool addTaskDependency(const std::string& task_id, const std::string& dependency_id);
    bool removeTaskDependency(const std::string& task_id, const std::string& dependency_id);
    std::vector<std::string> getTaskDependencies(const std::string& task_id);
    bool validateDependencyChain(const std::string& task_id);
    std::vector<Task> getReadyTasks(); // Tasks with no blocking dependencies

    // Resource Management
    bool allocateResource(const std::string& resource_id, const std::string& task_id);
    bool deallocateResource(const std::string& resource_id, const std::string& task_id);
    std::vector<ResourceAllocation> getResourceAllocations();
    bool detectResourceConflicts();
    std::vector<std::string> suggestResourceReallocation();

    // Proactive Intelligence (10 Steps Ahead)
    std::vector<Task> getFutureTaskQueue(int days_ahead = 30);
    std::vector<ScheduleAlert> predictFutureIssues();
    bool prepareForUpcomingDeadlines();
    std::map<std::string, std::vector<Task>> generateWeeklySchedule();
    bool autoCreateFollowupTasks();

    // Performance Analytics
    std::map<std::string, double> getTeamPerformanceMetrics();
    std::map<std::string, double> getProjectHealthMetrics();
    double calculateScheduleAdherence();
    std::vector<std::string> identifyHighPerformers();
    std::vector<std::string> identifyAtRiskProjects();

    // Notifications & Communication
    bool notifyTaskCompletion(const std::string& task_id);
    bool notifyUpcomingDeadlines();
    bool sendScheduleUpdates();
    bool alertStakeholders(const ScheduleAlert& alert);

    // Learning & Adaptation
    bool learnFromCompletedTasks();
    bool updateEstimationModels();
    bool adaptToTeamPatterns();
    double getEstimationAccuracy();

private:
    std::map<std::string, Task> tasks_;
    std::map<std::string, Project> projects_;
    std::vector<ScheduleAlert> alerts_;
    std::map<std::string, ResourceAllocation> resources_;
    std::map<std::string, std::vector<double>> performance_history_;
    
    // AI Learning Models
    std::map<std::string, double> task_completion_patterns_;
    std::map<std::string, double> team_velocity_metrics_;
    std::map<std::string, int> historical_estimates_;
    
    // Helper methods
    std::string generateTaskId();
    std::string generateProjectId();
    std::string getCurrentTimestamp();
    bool isTaskReady(const std::string& task_id);
    double calculateTaskComplexity(const Task& task);
    Priority calculateDynamicPriority(const Task& task);
    bool shouldRaiseAlert(const Task& task);
    std::vector<Task> sortTasksByPriority(const std::vector<Task>& tasks);

    // Time conversion helpers
    std::string timePointToString(const std::chrono::system_clock::time_point& tp);
    std::chrono::system_clock::time_point stringToTimePoint(const std::string& str);
};
