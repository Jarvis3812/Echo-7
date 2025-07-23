#include "app_core.h"
#include "../../core/riley_corpbrain.h"
#include <iostream>
#include <memory>

void AppCore::run() {
    std::cout << "🧠 RILEY CORPBRAIN - ENTERPRISE INTELLIGENCE SYSTEM" << std::endl;
    std::cout << "=================================================" << std::endl;

    try {
        // Initialize the core brain system
        auto corpBrain = std::make_unique<RileyCorpBrain>();

        std::cout << "🚀 Initializing modules..." << std::endl;
        corpBrain->initializeModules();

        std::cout << "🧠 Running AI analytics..." << std::endl;
        corpBrain->runAIAnalytics();

        std::cout << "✅ Riley Corpbrain initialized successfully!" << std::endl;
        std::cout << "📊 Enterprise management system is ready." << std::endl;
        std::cout << "🎯 All business modules are operational." << std::endl;

        // Keep the console open
        std::cout << "\nPress Enter to exit..." << std::endl;
        std::cin.get();

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
    }
}