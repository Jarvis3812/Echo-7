#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QStatusBar>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtGui/QFont>
#include <QtGui/QPalette>

// Forward declaration
class RileyCorpBrain;

class RileyInteractiveWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit RileyInteractiveWindow(QWidget *parent = nullptr);
    ~RileyInteractiveWindow() = default;

public slots:
    void sendCommand();
    void processVoiceCommand();
    void runAnalytics();
    void showSystemStatus();

private:
    void setupUI();
    void setupRiley();
    void setupConnections();
    void processRileyCommand(const QString& command);
    void addToChatLog(const QString& message, const QString& color);
    void updateInsightsTab();
    void applyDarkTheme();

    // UI Components
    QTextEdit *chatLog;
    QLineEdit *inputLine;
    QPushButton *sendButton;
    QPushButton *voiceButton;
    QTabWidget *tabWidget;
    QTextEdit *analyticsTab;
    QTextEdit *insightsTab;
    QTextEdit *memoryTab;
    QProgressBar *progressBar;

    // Riley AI System
    RileyCorpBrain *riley;
};
