#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTimer>
#include <memory>
#include "core/riley_corpbrain.h"
#include "core/common_types.h"

QT_BEGIN_NAMESPACE
class QStackedWidget;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QFrame;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showActivitySection();
    void showChatSection();
    void showTeamsSection();
    void showCalendarSection();
    void showCallsSection();
    void showFilesSection();
    void showAppsSection();
    void showHelpSection();
    void runAnalytics();
    void updateStatusDisplay();

private:
    void setupUI();
    void setupSideNavigation();
    void setupCentralArea();
    void setupStatusBar();
    void loadStyleSheet();
    void createSectionWidget(const QString& sectionName, const QString& description);
    
    // Core system
    std::unique_ptr<RileyCorpBrain> corpBrain;
    
    // UI Components
    QWidget *centralWidget;
    QHBoxLayout *mainLayout;
    QFrame *sideNavFrame;
    QVBoxLayout *sideNavLayout;
    QStackedWidget *sectionStack;
    // Navigation buttons for Teams-like sections
    QPushButton *activityButton;
    QPushButton *chatButton;
    QPushButton *teamsButton;
    QPushButton *calendarButton;
    QPushButton *callsButton;
    QPushButton *filesButton;
    QPushButton *appsButton;
    QPushButton *helpButton;
    QPushButton *analyticsButton;

    // Section widgets
    QWidget *activityWidget;
    QWidget *chatWidget;
    QWidget *teamsWidget;
    QWidget *calendarWidget;
    QWidget *callsWidget;
    QWidget *filesWidget;
    QWidget *appsWidget;
    QWidget *helpWidget;

    // Status and info
    QLabel *statusLabel;
    QLabel *aiStatusLabel;
    QTimer *statusTimer;

    // Current active section
    QString currentSection;
};

#endif // MAINWINDOW_H
