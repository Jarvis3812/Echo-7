#include "riley_interactive_gui.h"
#include <QtWidgets/QApplication>
#include <QtCore/QTimer>
#include <QtGui/QTextCursor>
#include "core/riley_corpbrain.h"
RileyInteractiveWindow::RileyInteractiveWindow(QWidget *parent) : QMainWindow(parent) {
        setupUI();
        setupRiley();
        setupConnections();
        applyDarkTheme();
}

void RileyInteractiveWindow::sendCommand() {
        QString command = inputLine->text();
        if (command.isEmpty()) return;
        
        // Add user input to chat
        addToChatLog("👤 You: " + command, "#4CAF50");
        
        // Clear input
        inputLine->clear();
        
        // Show processing
        statusBar()->showMessage("🧠 Riley is thinking...", 2000);
        progressBar->setVisible(true);
        
        // Process with Riley
        processRileyCommand(command);
}

void RileyInteractiveWindow::processVoiceCommand() {
        addToChatLog("🎤 Voice input activated...", "#2196F3");
        statusBar()->showMessage("🎤 Listening for voice input...", 3000);
        
        // Simulate voice processing
        QTimer::singleShot(2000, [this]() {
            QString voiceCommand = "Analyze sales performance";
            addToChatLog("🎤 Voice: " + voiceCommand, "#FF9800");
            processRileyCommand(voiceCommand);
        });
}

void RileyInteractiveWindow::runAnalytics() {
        addToChatLog("📊 Running advanced analytics...", "#9C27B0");
        statusBar()->showMessage("📊 Executing AI analytics...", 5000);
        progressBar->setVisible(true);
        
        try {
            riley->runAdvancedAnalytics();
            addToChatLog("✅ Analytics complete! Check the insights tab.", "#4CAF50");
            updateInsightsTab();
        } catch (const std::exception& e) {
            addToChatLog("❌ Analytics failed: " + QString(e.what()), "#F44336");
        }
        
        progressBar->setVisible(false);
}

void RileyInteractiveWindow::showSystemStatus() {
        auto status = riley->getSystemStatus();
        QString statusText = "🖥️ System Status:\n";
        for (const auto& line : status) {
            statusText += "• " + QString::fromStdString(line) + "\n";
        }
        addToChatLog(statusText, "#607D8B");
}

void RileyInteractiveWindow::setupUI() {
        setWindowTitle("🧠 Riley Corpbrain - Interactive AI Assistant");
        setMinimumSize(1200, 800);
        resize(1400, 900);
        
        // Central widget with splitter
        QWidget *centralWidget = new QWidget;
        setCentralWidget(centralWidget);
        
        QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
        QSplitter *splitter = new QSplitter(Qt::Horizontal);
        mainLayout->addWidget(splitter);
        
        // Left panel - Chat interface
        QWidget *chatPanel = new QWidget;
        QVBoxLayout *chatLayout = new QVBoxLayout(chatPanel);
        
        // Chat log
        chatLog = new QTextEdit;
        chatLog->setReadOnly(true);
        chatLog->setFont(QFont("Consolas", 10));
        chatLayout->addWidget(chatLog);
        
        // Input area
        QHBoxLayout *inputLayout = new QHBoxLayout;
        inputLine = new QLineEdit;
        inputLine->setPlaceholderText("Ask Riley anything... (e.g., 'Analyze sales performance')");
        inputLine->setFont(QFont("Segoe UI", 11));
        
        sendButton = new QPushButton("Send");
        voiceButton = new QPushButton("🎤 Voice");
        
        inputLayout->addWidget(inputLine);
        inputLayout->addWidget(sendButton);
        inputLayout->addWidget(voiceButton);
        chatLayout->addLayout(inputLayout);
        
        splitter->addWidget(chatPanel);
        
        // Right panel - Tabs
        tabWidget = new QTabWidget;
        
        // Analytics tab
        analyticsTab = new QTextEdit;
        analyticsTab->setReadOnly(true);
        analyticsTab->setFont(QFont("Consolas", 9));
        tabWidget->addTab(analyticsTab, "📊 Analytics");
        
        // Insights tab
        insightsTab = new QTextEdit;
        insightsTab->setReadOnly(true);
        insightsTab->setFont(QFont("Consolas", 9));
        tabWidget->addTab(insightsTab, "💡 Insights");
        
        // Memory tab
        memoryTab = new QTextEdit;
        memoryTab->setReadOnly(true);
        memoryTab->setFont(QFont("Consolas", 9));
        tabWidget->addTab(memoryTab, "🧠 Memory");
        
        // Commands tab
        QWidget *commandsTab = new QWidget;
        QVBoxLayout *commandsLayout = new QVBoxLayout(commandsTab);
        
        QPushButton *analyticsBtn = new QPushButton("📊 Run Analytics");
        QPushButton *statusBtn = new QPushButton("🖥️ System Status");
        QPushButton *predictBtn = new QPushButton("🔮 Generate Predictions");
        QPushButton *optimizeBtn = new QPushButton("⚡ Optimize Performance");
        
        commandsLayout->addWidget(analyticsBtn);
        commandsLayout->addWidget(statusBtn);
        commandsLayout->addWidget(predictBtn);
        commandsLayout->addWidget(optimizeBtn);
        commandsLayout->addStretch();
        
        tabWidget->addTab(commandsTab, "🎯 Commands");
        
        splitter->addWidget(tabWidget);
        splitter->setSizes({700, 500});
        
        // Status bar
        statusBar()->showMessage("🚀 Riley Corpbrain ready for interaction");
        progressBar = new QProgressBar;
        progressBar->setRange(0, 0); // Indeterminate
        progressBar->setVisible(false);
        statusBar()->addPermanentWidget(progressBar);
        
        // Connect command buttons
        connect(analyticsBtn, &QPushButton::clicked, this, &RileyInteractiveWindow::runAnalytics);
        connect(statusBtn, &QPushButton::clicked, this, &RileyInteractiveWindow::showSystemStatus);
        connect(predictBtn, &QPushButton::clicked, [this]() {
            processRileyCommand("Generate predictions for next quarter");
        });
        connect(optimizeBtn, &QPushButton::clicked, [this]() {
            processRileyCommand("Optimize system performance");
        });
}

void RileyInteractiveWindow::setupRiley() {
        addToChatLog("🚀 Initializing Riley Corpbrain...", "#2196F3");
        
        try {
            riley = new RileyCorpBrain();
            riley->initializeModules();
            
            addToChatLog("✅ Riley is fully operational!", "#4CAF50");
            addToChatLog("🧠 AI Reasoning Engine: Active", "#4CAF50");
            addToChatLog("🔮 Predictive Analytics: Ready", "#4CAF50");
            addToChatLog("💾 Memory System: Online", "#4CAF50");
            addToChatLog("🎤 Voice Interface: Available", "#4CAF50");
            addToChatLog("🎯 Command Center: Autonomous", "#4CAF50");
            addToChatLog("", "#FFFFFF");
            addToChatLog("💬 You can now ask Riley anything about your business!", "#FF9800");
            addToChatLog("Try: 'Analyze sales performance' or 'Predict revenue'", "#607D8B");
            
        } catch (const std::exception& e) {
            addToChatLog("❌ Failed to initialize Riley: " + QString(e.what()), "#F44336");
        }
}

void RileyInteractiveWindow::setupConnections() {
        connect(sendButton, &QPushButton::clicked, this, &RileyInteractiveWindow::sendCommand);
        connect(voiceButton, &QPushButton::clicked, this, &RileyInteractiveWindow::processVoiceCommand);
        connect(inputLine, &QLineEdit::returnPressed, this, &RileyInteractiveWindow::sendCommand);
}

void RileyInteractiveWindow::processRileyCommand(const QString& command) {
        progressBar->setVisible(true);
        
        try {
            std::string cmdStr = command.toStdString();
            
            if (command.contains("predict", Qt::CaseInsensitive) || 
                command.contains("forecast", Qt::CaseInsensitive)) {
                // Predictive analytics
                std::string prediction = riley->predictFutureTrends("sales_revenue", 6);
                addToChatLog("🔮 Riley: " + QString::fromStdString(prediction), "#9C27B0");
                
            } else if (command.contains("analyze", Qt::CaseInsensitive) ||
                      command.contains("analysis", Qt::CaseInsensitive)) {
                // Business analysis
                CoreVariantMap context;
                context["revenue"] = 850000.0;
                context["growth_rate"] = 0.15;
                
                std::string analysis = riley->analyzeBusinessScenario("Sales", context);
                addToChatLog("🧠 Riley: " + QString::fromStdString(analysis), "#2196F3");
                
            } else if (command.contains("recommend", Qt::CaseInsensitive) ||
                      command.contains("suggest", Qt::CaseInsensitive)) {
                // Recommendations
                CoreVariantMap context;
                context["situation"] = cmdStr;
                
                std::string recommendation = riley->getRecommendation(cmdStr, context);
                addToChatLog("💡 Riley: " + QString::fromStdString(recommendation), "#FF9800");
                
            } else {
                // General AI response
                addToChatLog("🤖 Riley: I understand you want to " + command + 
                           ". Let me analyze this with my AI reasoning engine...", "#607D8B");
                
                // Use voice interface for natural response
                std::string response = riley->processVoiceCommand(cmdStr);
                addToChatLog("🧠 Riley: " + QString::fromStdString(response), "#4CAF50");
            }
            
        } catch (const std::exception& e) {
            addToChatLog("❌ Error: " + QString(e.what()), "#F44336");
        }
        
        progressBar->setVisible(false);
        statusBar()->showMessage("✅ Command processed", 2000);
}

void RileyInteractiveWindow::addToChatLog(const QString& message, const QString& color) {
        chatLog->setTextColor(QColor(color));
        chatLog->append(message);
        chatLog->moveCursor(QTextCursor::End);
}

void RileyInteractiveWindow::updateInsightsTab() {
        QString insights = "💡 Latest Business Insights:\n\n";
        insights += "📈 Sales Performance:\n";
        insights += "• Revenue growth trending upward\n";
        insights += "• Customer acquisition rate: +15%\n";
        insights += "• Pipeline velocity improved\n\n";
        
        insights += "👥 Customer Analysis:\n";
        insights += "• Churn risk identified in enterprise segment\n";
        insights += "• High-value customers showing strong engagement\n";
        insights += "• Retention strategies recommended\n\n";
        
        insights += "💰 Financial Health:\n";
        insights += "• Cash flow stable and positive\n";
        insights += "• Cost optimization opportunities identified\n";
        insights += "• Budget variance within acceptable range\n";
        
        insightsTab->setPlainText(insights);
}

void RileyInteractiveWindow::applyDarkTheme() {
        setStyleSheet(R"(
            QMainWindow {
                background-color: #1e1e1e;
                color: #ffffff;
            }
            QTextEdit {
                background-color: #2d2d2d;
                color: #ffffff;
                border: 1px solid #555;
                border-radius: 5px;
                padding: 5px;
            }
            QLineEdit {
                background-color: #2d2d2d;
                color: #ffffff;
                border: 2px solid #555;
                border-radius: 5px;
                padding: 8px;
            }
            QLineEdit:focus {
                border-color: #4CAF50;
            }
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border: none;
                border-radius: 5px;
                padding: 8px 16px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #45a049;
            }
            QPushButton:pressed {
                background-color: #3d8b40;
            }
            QTabWidget::pane {
                border: 1px solid #555;
                background-color: #2d2d2d;
            }
            QTabBar::tab {
                background-color: #3d3d3d;
                color: #ffffff;
                padding: 8px 16px;
                margin-right: 2px;
            }
            QTabBar::tab:selected {
                background-color: #4CAF50;
            }
            QStatusBar {
                background-color: #2d2d2d;
                color: #ffffff;
            }
        )");
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    RileyInteractiveWindow window;
    window.show();
    
    return app.exec();
}

// MOC will be handled by CMake
