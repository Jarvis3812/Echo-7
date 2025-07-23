#include "mainwindow.h"
#include <QApplication>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTimer>
#include <QStatusBar>
#include <QMenuBar>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , corpBrain(nullptr)
    , centralWidget(nullptr)
    , currentModule("CRM")
{
    try {
        // Initialize the core brain system
        corpBrain = std::make_unique<RileyCorpBrain>();
        
        // Setup the UI
        setupUI();
        loadStyleSheet();
        
        // Initialize the core system
        corpBrain->initializeModules();
        
        // Setup status updates
        statusTimer = new QTimer(this);
        connect(statusTimer, &QTimer::timeout, this, &MainWindow::updateStatusDisplay);
        statusTimer->start(5000); // Update every 5 seconds
        
        // Set window properties
        setWindowTitle("🧠 Riley Corpbrain - Enterprise Intelligence System");
        setMinimumSize(1200, 800);
        resize(1400, 900);
        
        // Show CRM module by default
        showCRMModule();
        
        std::cout << "✅ MainWindow initialized successfully" << std::endl;
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Initialization Error", 
                            QString("Failed to initialize Riley Corpbrain: %1").arg(e.what()));
        std::cerr << "❌ MainWindow initialization failed: " << e.what() << std::endl;
    }
}

MainWindow::~MainWindow()
{
    std::cout << "🔄 MainWindow shutting down..." << std::endl;
}

void MainWindow::setupUI()
{
    // Create central widget and main layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Setup components
    setupSideNavigation();
    setupCentralArea();
    setupStatusBar();
}

void MainWindow::setupSideNavigation()
{
    // Create side navigation frame
    sideNavFrame = new QFrame();
    sideNavFrame->setObjectName("sideNavFrame");
    sideNavFrame->setFixedWidth(250);
    sideNavFrame->setFrameStyle(QFrame::StyledPanel);
    
    sideNavLayout = new QVBoxLayout(sideNavFrame);
    sideNavLayout->setContentsMargins(10, 20, 10, 20);
    sideNavLayout->setSpacing(10);
    
    // Add title
    QLabel *titleLabel = new QLabel("🧠 RILEY CORPBRAIN");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    sideNavLayout->addWidget(titleLabel);
    
    // Add separator
    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setObjectName("separator");
    sideNavLayout->addWidget(separator);
    
    // Create navigation buttons for Teams-like sections
    activityButton = new QPushButton("🏠 Activity");
    chatButton = new QPushButton("� Chat");
    teamsButton = new QPushButton("👥 Teams");
    calendarButton = new QPushButton("� Calendar");
    callsButton = new QPushButton("� Calls");
    filesButton = new QPushButton("� Files");
    appsButton = new QPushButton("🧩 Apps");
    helpButton = new QPushButton("❓ Help");
    analyticsButton = new QPushButton("🧠 AI Analytics");

    // Set object names for styling
    activityButton->setObjectName("navButton");
    chatButton->setObjectName("navButton");
    teamsButton->setObjectName("navButton");
    calendarButton->setObjectName("navButton");
    callsButton->setObjectName("navButton");
    filesButton->setObjectName("navButton");
    appsButton->setObjectName("navButton");
    helpButton->setObjectName("navButton");
    analyticsButton->setObjectName("analyticsButton");

    // Connect signals
    connect(activityButton, &QPushButton::clicked, this, &MainWindow::showActivitySection);
    connect(chatButton, &QPushButton::clicked, this, &MainWindow::showChatSection);
    connect(teamsButton, &QPushButton::clicked, this, &MainWindow::showTeamsSection);
    connect(calendarButton, &QPushButton::clicked, this, &MainWindow::showCalendarSection);
    connect(callsButton, &QPushButton::clicked, this, &MainWindow::showCallsSection);
    connect(filesButton, &QPushButton::clicked, this, &MainWindow::showFilesSection);
    connect(appsButton, &QPushButton::clicked, this, &MainWindow::showAppsSection);
    connect(helpButton, &QPushButton::clicked, this, &MainWindow::showHelpSection);
    connect(analyticsButton, &QPushButton::clicked, this, &MainWindow::runAnalytics);

    // Add buttons to layout
    sideNavLayout->addWidget(activityButton);
    sideNavLayout->addWidget(chatButton);
    sideNavLayout->addWidget(teamsButton);
    sideNavLayout->addWidget(calendarButton);
    sideNavLayout->addWidget(callsButton);
    sideNavLayout->addWidget(filesButton);
    sideNavLayout->addWidget(appsButton);
    sideNavLayout->addWidget(helpButton);

    // Add separator before analytics
    QFrame *separator2 = new QFrame();
    separator2->setFrameShape(QFrame::HLine);
    separator2->setObjectName("separator");
    sideNavLayout->addWidget(separator2);

    sideNavLayout->addWidget(analyticsButton);

    // Add stretch to push everything to top
    sideNavLayout->addStretch();

    // Add AI status label
    aiStatusLabel = new QLabel("🤖 AI: Ready");
    aiStatusLabel->setObjectName("aiStatusLabel");
    aiStatusLabel->setAlignment(Qt::AlignCenter);
    sideNavLayout->addWidget(aiStatusLabel);

    // Add to main layout
    mainLayout->addWidget(sideNavFrame);
}

void MainWindow::setupCentralArea()
{
    // Create stacked widget for Teams-like sections
    sectionStack = new QStackedWidget();
    sectionStack->setObjectName("sectionStack");

    // Create section widgets
    createSectionWidget("Activity", "See what's happening across your teams and channels.");
    createSectionWidget("Chat", "Chat with individuals and groups, share files, and collaborate in real time.");
    createSectionWidget("Teams", "Organize your teams, channels, and conversations.");
    createSectionWidget("Calendar", "View and schedule meetings and events.");
    createSectionWidget("Calls", "Make voice and video calls, see call history, and manage contacts.");
    createSectionWidget("Files", "Browse, share, and manage your files.");
    createSectionWidget("Apps", "Discover and use apps to enhance your workflow.");
    createSectionWidget("Help", "Get help, tips, and support.");

    mainLayout->addWidget(sectionStack);
}

void MainWindow::setupStatusBar()
{
    statusLabel = new QLabel("🟢 System Ready");
    statusLabel->setObjectName("statusLabel");
    statusBar()->addWidget(statusLabel);
    
    // Add permanent widgets to status bar
    QLabel *versionLabel = new QLabel("Riley Corpbrain v1.0");
    versionLabel->setObjectName("versionLabel");
    statusBar()->addPermanentWidget(versionLabel);
}

void MainWindow::createSectionWidget(const QString& sectionName, const QString& description)
{
    QWidget *widget = new QWidget();
    widget->setObjectName("sectionWidget");

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    // Section title
    QLabel *titleLabel = new QLabel(sectionName);
    titleLabel->setObjectName("sectionTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Section description
    QLabel *descLabel = new QLabel(description);
    descLabel->setObjectName("sectionDescription");
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setWordWrap(true);
    layout->addWidget(descLabel);

    // Placeholder content
    QLabel *contentLabel = new QLabel(QString("🚀 %1 section is ready for implementation.\n\n"
                                            "This section will provide comprehensive %2 functionality "
                                            "with AI-powered insights and real-time collaboration.")
                                    .arg(sectionName).arg(description.toLower()));
    contentLabel->setObjectName("sectionContent");
    contentLabel->setAlignment(Qt::AlignCenter);
    contentLabel->setWordWrap(true);
    layout->addWidget(contentLabel);

    layout->addStretch();

    sectionStack->addWidget(widget);

    // Store widget references
    if (sectionName == "Activity") activityWidget = widget;
    else if (sectionName == "Chat") chatWidget = widget;
    else if (sectionName == "Teams") teamsWidget = widget;
    else if (sectionName == "Calendar") calendarWidget = widget;
    else if (sectionName == "Calls") callsWidget = widget;
    else if (sectionName == "Files") filesWidget = widget;
    else if (sectionName == "Apps") appsWidget = widget;
    else if (sectionName == "Help") helpWidget = widget;
}

void MainWindow::loadStyleSheet()
{
    QFile file("assets/styles.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString styleSheet = stream.readAll();
        setStyleSheet(styleSheet);
        std::cout << "✅ Loaded custom stylesheet" << std::endl;
    } else {
        // Fallback dark theme
        setStyleSheet(
            "QMainWindow { background-color: #181A20; color: #E0E0E0; }"
            "QFrame#sideNavFrame { background-color: #23263A; border-right: 2px solid #00FFD0; }"
            "QPushButton#navButton { background-color: #2A2D3A; color: #00FFD0; border: 1px solid #00FFD0; "
            "border-radius: 6px; padding: 12px; margin: 2px; font-weight: bold; }"
            "QPushButton#navButton:hover { background-color: #00FFD0; color: #181A20; }"
            "QPushButton#analyticsButton { background-color: #FF6B35; color: white; border: 1px solid #FF6B35; "
            "border-radius: 6px; padding: 12px; margin: 2px; font-weight: bold; }"
            "QLabel#titleLabel { color: #00FFD0; font-size: 16px; font-weight: bold; }"
            "QLabel#moduleTitle { color: #00FFD0; font-size: 24px; font-weight: bold; }"
            "QLabel#moduleDescription { color: #B0B0B0; font-size: 14px; }"
            "QLabel#moduleContent { color: #E0E0E0; font-size: 12px; }"
            "QFrame#separator { color: #00FFD0; }"
            "QStackedWidget#moduleStack { background-color: #1E2028; }"
            "QStatusBar { background-color: #23263A; color: #E0E0E0; border-top: 1px solid #00FFD0; }"
        );
        std::cout << "⚠️ Using fallback stylesheet" << std::endl;
    }
}


// Section navigation slots
void MainWindow::showActivitySection() {
    sectionStack->setCurrentWidget(activityWidget);
    currentSection = "Activity";
    statusLabel->setText("🟢 Activity Section Active");
}

void MainWindow::showChatSection() {
    sectionStack->setCurrentWidget(chatWidget);
    currentSection = "Chat";
    statusLabel->setText("🟢 Chat Section Active");
}

void MainWindow::showTeamsSection() {
    sectionStack->setCurrentWidget(teamsWidget);
    currentSection = "Teams";
    statusLabel->setText("🟢 Teams Section Active");
}

void MainWindow::showCalendarSection() {
    sectionStack->setCurrentWidget(calendarWidget);
    currentSection = "Calendar";
    statusLabel->setText("🟢 Calendar Section Active");
}

void MainWindow::showCallsSection() {
    sectionStack->setCurrentWidget(callsWidget);
    currentSection = "Calls";
    statusLabel->setText("🟢 Calls Section Active");
}

void MainWindow::showFilesSection() {
    sectionStack->setCurrentWidget(filesWidget);
    currentSection = "Files";
    statusLabel->setText("🟢 Files Section Active");
}

void MainWindow::showAppsSection() {
    sectionStack->setCurrentWidget(appsWidget);
    currentSection = "Apps";
    statusLabel->setText("🟢 Apps Section Active");
}

void MainWindow::showHelpSection() {
    sectionStack->setCurrentWidget(helpWidget);
    currentSection = "Help";
    statusLabel->setText("🟢 Help Section Active");
}

void MainWindow::runAnalytics() {
    if (corpBrain) {
        statusLabel->setText("🧠 Running AI Analytics...");
        aiStatusLabel->setText("🤖 AI: Processing...");
        
        try {
            corpBrain->runAIAnalytics();
            statusLabel->setText("✅ AI Analytics Complete");
            aiStatusLabel->setText("🤖 AI: Insights Ready");
        } catch (const std::exception& e) {
            statusLabel->setText("❌ Analytics Failed");
            aiStatusLabel->setText("🤖 AI: Error");
            QMessageBox::warning(this, "Analytics Error", 
                                QString("AI Analytics failed: %1").arg(e.what()));
        }
    }
}

void MainWindow::updateStatusDisplay() {
    // Update AI status and system metrics
    if (corpBrain) {
        aiStatusLabel->setText("🤖 AI: Active");
    }
}


