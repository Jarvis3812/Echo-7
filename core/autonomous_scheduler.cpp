#include "autonomous_scheduler.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <random>

AutonomousScheduler::AutonomousScheduler() {
    std::cout << "Autonomous Scheduling Engine initialized - 10 steps ahead mode ACTIVE" << std::endl;
}

std::string AutonomousScheduler::createTask(const Task& task) {
    std::string task_id = generateTaskId();
    Task new_task = task;
    new_task.task_id = task_id;
    
    // Set default values if not provided
    if (new_task.status == TaskStatus::NOT_STARTED) {
        new_task.estimated_completion = std::chrono::system_clock::now() + 
            std::chrono::hours(new_task.estimated_hours);
    }
    
    tasks_[task_id] = new_task;
    
    std::cout << "Task created: " << new_task.name << " (ID: " << task_id << ")" << std::endl;
    
    // Immediately analyze impact on schedule
    analyzeScheduleHealth();
    
    return task_id;
}

bool AutonomousScheduler::analyzeScheduleHealth() {
    std::cout << "Analyzing schedule health and predicting future issues..." << std::endl;
    
    int at_risk_tasks = 0;
    int overdue_tasks = 0;
    int resource_conflicts = 0;
    
    auto now = std::chrono::system_clock::now();
    
    for (const auto& pair : tasks_) {
        const Task& task = pair.second;
        
        // Check for overdue tasks
        if (task.due_date < now && task.status != TaskStatus::COMPLETED) {
            overdue_tasks++;
            
            ScheduleAlert alert;
            alert.alert_id = "ALERT_" + std::to_string(alerts_.size() + 1);
            alert.timestamp = getCurrentTimestamp();
            alert.type = "overdue_task";
            alert.message = "Task '" + task.name + "' is overdue";
            alert.severity = Priority::HIGH;
            alert.affected_task_id = task.task_id;
            alert.recommended_action = "Reassign resources or extend deadline";
            alert.is_resolved = false;
            
            alerts_.push_back(alert);
        }
        
        // Check for at-risk tasks (due within 24 hours with <50% progress)
        auto hours_until_due = std::chrono::duration_cast<std::chrono::hours>(task.due_date - now).count();
        if (hours_until_due <= 24 && hours_until_due > 0 && task.progress_percentage < 50.0) {
            at_risk_tasks++;
            
            ScheduleAlert alert;
            alert.alert_id = "ALERT_" + std::to_string(alerts_.size() + 1);
            alert.timestamp = getCurrentTimestamp();
            alert.type = "delay_warning";
            alert.message = "Task '" + task.name + "' at risk of missing deadline";
            alert.severity = Priority::MEDIUM;
            alert.affected_task_id = task.task_id;
            alert.recommended_action = "Increase resources or adjust timeline";
            alert.is_resolved = false;
            
            alerts_.push_back(alert);
        }
    }
    
    std::cout << "Schedule Health Analysis Complete:" << std::endl;
    std::cout << "- Overdue tasks: " << overdue_tasks << std::endl;
    std::cout << "- At-risk tasks: " << at_risk_tasks << std::endl;
    std::cout << "- Resource conflicts: " << resource_conflicts << std::endl;
    
    return true;
}

std::vector<AutonomousScheduler::Task> AutonomousScheduler::getNextRecommendedTasks(const std::string& assignee) {
    std::cout << "Calculating next recommended tasks for: " << assignee << std::endl;
    
    std::vector<Task> recommended_tasks;
    std::vector<Task> ready_tasks = getReadyTasks();
    
    // Filter tasks for the specific assignee
    for (const Task& task : ready_tasks) {
        if (task.assigned_to == assignee || task.assigned_to.empty()) {
            recommended_tasks.push_back(task);
        }
    }
    
    // Sort by priority and due date
    std::sort(recommended_tasks.begin(), recommended_tasks.end(), 
        [](const Task& a, const Task& b) {
            if (a.priority != b.priority) {
                return static_cast<int>(a.priority) > static_cast<int>(b.priority);
            }
            return a.due_date < b.due_date;
        });
    
    // Limit to top 5 recommendations
    if (recommended_tasks.size() > 5) {
        recommended_tasks.resize(5);
    }
    
    std::cout << "Recommended " << recommended_tasks.size() << " tasks for " << assignee << std::endl;
    return recommended_tasks;
}

std::vector<AutonomousScheduler::Task> AutonomousScheduler::getReadyTasks() {
    std::vector<Task> ready_tasks;
    
    for (const auto& pair : tasks_) {
        const Task& task = pair.second;
        
        if (task.status == TaskStatus::NOT_STARTED && isTaskReady(task.task_id)) {
            ready_tasks.push_back(task);
        }
    }
    
    return ready_tasks;
}

bool AutonomousScheduler::isTaskReady(const std::string& task_id) {
    if (tasks_.find(task_id) == tasks_.end()) {
        return false;
    }
    
    const Task& task = tasks_[task_id];
    
    // Check if all dependencies are completed
    for (const std::string& dep_id : task.dependencies) {
        if (tasks_.find(dep_id) == tasks_.end()) {
            continue; // Skip missing dependencies
        }
        
        if (tasks_[dep_id].status != TaskStatus::COMPLETED) {
            return false; // Dependency not completed
        }
    }
    
    return true;
}

std::chrono::system_clock::time_point AutonomousScheduler::predictTaskCompletion(const std::string& task_id) {
    if (tasks_.find(task_id) == tasks_.end()) {
        return std::chrono::system_clock::now();
    }
    
    const Task& task = tasks_[task_id];
    auto now = std::chrono::system_clock::now();
    
    // Calculate remaining work
    double remaining_work = (100.0 - task.progress_percentage) / 100.0;
    int remaining_hours = static_cast<int>(task.estimated_hours * remaining_work);
    
    // Apply learning from historical data
    std::string task_type = task.name.substr(0, task.name.find(' ')); // Simple categorization
    if (task_completion_patterns_.find(task_type) != task_completion_patterns_.end()) {
        double adjustment_factor = task_completion_patterns_[task_type];
        remaining_hours = static_cast<int>(remaining_hours * adjustment_factor);
    }
    
    // Account for assignee velocity
    if (team_velocity_metrics_.find(task.assigned_to) != team_velocity_metrics_.end()) {
        double velocity = team_velocity_metrics_[task.assigned_to];
        remaining_hours = static_cast<int>(remaining_hours / velocity);
    }
    
    auto predicted_completion = now + std::chrono::hours(remaining_hours);
    
    std::cout << "Predicted completion for '" << task.name << "': " 
              << timePointToString(predicted_completion) << std::endl;
    
    return predicted_completion;
}

std::vector<AutonomousScheduler::Task> AutonomousScheduler::getFutureTaskQueue(int days_ahead) {
    std::cout << "Generating future task queue for next " << days_ahead << " days..." << std::endl;
    
    std::vector<Task> future_tasks;
    auto cutoff_date = std::chrono::system_clock::now() + std::chrono::hours(24 * days_ahead);
    
    for (const auto& pair : tasks_) {
        const Task& task = pair.second;
        
        if (task.due_date <= cutoff_date && 
            (task.status == TaskStatus::NOT_STARTED || task.status == TaskStatus::IN_PROGRESS)) {
            future_tasks.push_back(task);
        }
    }
    
    // Sort by due date and priority
    std::sort(future_tasks.begin(), future_tasks.end(), 
        [](const Task& a, const Task& b) {
            if (a.due_date != b.due_date) {
                return a.due_date < b.due_date;
            }
            return static_cast<int>(a.priority) > static_cast<int>(b.priority);
        });
    
    std::cout << "Future queue contains " << future_tasks.size() << " tasks" << std::endl;
    return future_tasks;
}

bool AutonomousScheduler::detectPotentialDelays() {
    std::cout << "Scanning for potential delays using predictive analysis..." << std::endl;
    
    bool delays_detected = false;
    
    for (const auto& pair : tasks_) {
        const Task& task = pair.second;
        
        if (task.status == TaskStatus::IN_PROGRESS || task.status == TaskStatus::NOT_STARTED) {
            auto predicted_completion = predictTaskCompletion(task.task_id);
            
            if (predicted_completion > task.due_date) {
                delays_detected = true;
                
                ScheduleAlert alert;
                alert.alert_id = "DELAY_" + std::to_string(alerts_.size() + 1);
                alert.timestamp = getCurrentTimestamp();
                alert.type = "predicted_delay";
                alert.message = "Task '" + task.name + "' predicted to miss deadline";
                alert.severity = Priority::HIGH;
                alert.affected_task_id = task.task_id;
                alert.recommended_action = "Reallocate resources or adjust scope";
                alert.is_resolved = false;
                
                alerts_.push_back(alert);
                
                std::cout << "DELAY PREDICTED: " << task.name << " - Expected: " 
                          << timePointToString(predicted_completion) << ", Due: " 
                          << timePointToString(task.due_date) << std::endl;
            }
        }
    }
    
    if (!delays_detected) {
        std::cout << "No potential delays detected - schedule is healthy" << std::endl;
    }
    
    return delays_detected;
}

bool AutonomousScheduler::notifyTaskCompletion(const std::string& task_id) {
    if (tasks_.find(task_id) == tasks_.end()) {
        return false;
    }
    
    Task& task = tasks_[task_id];
    task.status = TaskStatus::COMPLETED;
    task.progress_percentage = 100.0;
    
    std::cout << "TASK COMPLETED: " << task.name << std::endl;
    std::cout << "Analyzing impact and queuing next tasks..." << std::endl;
    
    // Find dependent tasks that are now ready
    std::vector<Task> newly_ready_tasks;
    for (const auto& pair : tasks_) {
        const Task& dependent_task = pair.second;
        
        if (dependent_task.status == TaskStatus::NOT_STARTED) {
            auto deps = dependent_task.dependencies;
            if (std::find(deps.begin(), deps.end(), task_id) != deps.end()) {
                if (isTaskReady(dependent_task.task_id)) {
                    newly_ready_tasks.push_back(dependent_task);
                }
            }
        }
    }
    
    // Notify about newly available tasks
    for (const Task& ready_task : newly_ready_tasks) {
        std::cout << "TASK NOW READY: " << ready_task.name 
                  << " (Assignee: " << ready_task.assigned_to << ")" << std::endl;
    }
    
    // Update learning models
    learnFromCompletedTasks();
    
    return true;
}

bool AutonomousScheduler::learnFromCompletedTasks() {
    std::cout << "Learning from completed tasks to improve future estimates..." << std::endl;
    
    for (const auto& pair : tasks_) {
        const Task& task = pair.second;
        
        if (task.status == TaskStatus::COMPLETED && task.actual_hours > 0) {
            // Calculate accuracy of estimate
            double accuracy = static_cast<double>(task.estimated_hours) / task.actual_hours;
            
            // Update task type patterns
            std::string task_type = task.name.substr(0, task.name.find(' '));
            if (task_completion_patterns_.find(task_type) == task_completion_patterns_.end()) {
                task_completion_patterns_[task_type] = accuracy;
            } else {
                // Running average
                task_completion_patterns_[task_type] = 
                    (task_completion_patterns_[task_type] + accuracy) / 2.0;
            }
            
            // Update team velocity
            if (!task.assigned_to.empty()) {
                double velocity = static_cast<double>(task.estimated_hours) / task.actual_hours;
                if (team_velocity_metrics_.find(task.assigned_to) == team_velocity_metrics_.end()) {
                    team_velocity_metrics_[task.assigned_to] = velocity;
                } else {
                    team_velocity_metrics_[task.assigned_to] = 
                        (team_velocity_metrics_[task.assigned_to] + velocity) / 2.0;
                }
            }
        }
    }
    
    std::cout << "Learning complete - estimation models updated" << std::endl;
    return true;
}

std::string AutonomousScheduler::generateTaskId() {
    static int counter = 1;
    return "TASK_" + std::to_string(counter++);
}

std::string AutonomousScheduler::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string AutonomousScheduler::timePointToString(const std::chrono::system_clock::time_point& tp) {
    auto time_t = std::chrono::system_clock::to_time_t(tp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
