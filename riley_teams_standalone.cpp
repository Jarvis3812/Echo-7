#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <dwmapi.h>
#include <uxtheme.h>
#include <commctrl.h>
#include <shellapi.h>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <memory>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")

// Microsoft Teams Exact Colors
#define TEAMS_PURPLE RGB(98, 100, 167)
#define TEAMS_DARK_PURPLE RGB(70, 72, 120)
#define TEAMS_LIGHT_GRAY RGB(243, 242, 241)
#define TEAMS_MEDIUM_GRAY RGB(237, 235, 233)
#define TEAMS_DARK_GRAY RGB(50, 49, 48)
#define TEAMS_WHITE RGB(255, 255, 255)
#define TEAMS_BLUE RGB(0, 120, 212)
#define TEAMS_GREEN RGB(16, 124, 16)
#define TEAMS_RED RGB(196, 43, 28)
#define TEAMS_SIDEBAR RGB(247, 246, 243)
#define TEAMS_HOVER RGB(237, 235, 233)

// Simple Riley AI Mock Class
class SimpleRileyAI {
public:
    std::vector<std::wstring> responses = {
        L"Riley AI: Hello! I'm analyzing your team's performance data.",
        L"Riley AI: I've detected 3 optimization opportunities in your workflow.",
        L"Riley AI: Your team productivity has increased by 15% this quarter!",
        L"Riley AI: I recommend scheduling a strategy meeting for next week.",
        L"Riley AI: Security scan complete - all systems are secure."
    };
    
    int responseIndex = 0;
    
    std::wstring GetResponse() {
        std::wstring response = responses[responseIndex];
        responseIndex = (responseIndex + 1) % responses.size();
        return response;
    }
    
    void ProcessCommand(const std::string& command) {
        // Simple command processing
    }
};

// Complete Microsoft Teams Interface
class TeamsInterface {
public:
    HWND mainWindow;

    // Direct2D resources
    ID2D1Factory* pD2DFactory = nullptr;
    ID2D1HwndRenderTarget* pRenderTarget = nullptr;
    IDWriteFactory* pDWriteFactory = nullptr;
    IDWriteTextFormat* pTextFormat = nullptr;
    IDWriteTextFormat* pTitleFormat = nullptr;
    IDWriteTextFormat* pSmallFormat = nullptr;
    
    // Teams brushes
    ID2D1SolidColorBrush* pPurpleBrush = nullptr;
    ID2D1SolidColorBrush* pLightGrayBrush = nullptr;
    ID2D1SolidColorBrush* pDarkGrayBrush = nullptr;
    ID2D1SolidColorBrush* pWhiteBrush = nullptr;
    ID2D1SolidColorBrush* pBlueBrush = nullptr;
    ID2D1SolidColorBrush* pSidebarBrush = nullptr;
    ID2D1SolidColorBrush* pHoverBrush = nullptr;
    
    // UI State
    int currentTab = 0; // 0=Activity, 1=Chat, 2=Teams, 3=Calendar, 4=Calls, 5=Files, 6=Riley AI
    int hoveredButton = -1;
    
    // Teams data
    std::vector<std::wstring> teamsList;
    std::vector<std::wstring> channelsList;
    std::vector<std::wstring> chatMessages;
    std::vector<std::wstring> activityFeed;
    struct TeamsTab {
        D2D1_RECT_F rect;
        std::wstring name;
        std::wstring icon;
        int id;
        bool isActive = false;
        bool isHovered = false;
        bool hasNotification = false;
        int notificationCount = 0;
    };
    
    std::vector<TeamsTab> sidebarTabs;
    SimpleRileyAI* riley = nullptr;
    
    bool Initialize(HWND hwnd) {
        mainWindow = hwnd;
        
        // Initialize Riley AI
        riley = new SimpleRileyAI();
        
        // Initialize Direct2D
        HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
        if (FAILED(hr)) return false;
        
        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pDWriteFactory), 
                                reinterpret_cast<IUnknown**>(&pDWriteFactory));
        if (FAILED(hr)) return false;
        
        RECT rc;
        GetClientRect(hwnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
        
        hr = pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            &pRenderTarget
        );
        if (FAILED(hr)) return false;
        
        CreateResources();
        CreateSidebarTabs();
        InitializeTeamsData();
        
        return true;
    }
    
    void CreateResources() {
        // Create Teams fonts
        pDWriteFactory->CreateTextFormat(
            L"Segoe UI",
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            14.0f,
            L"",
            &pTextFormat
        );
        
        pDWriteFactory->CreateTextFormat(
            L"Segoe UI",
            nullptr,
            DWRITE_FONT_WEIGHT_SEMI_BOLD,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            20.0f,
            L"",
            &pTitleFormat
        );
        
        pDWriteFactory->CreateTextFormat(
            L"Segoe UI",
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            12.0f,
            L"",
            &pSmallFormat
        );
        
        // Create Teams color brushes
        pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(98/255.0f, 100/255.0f, 167/255.0f), &pPurpleBrush);
        pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(247/255.0f, 246/255.0f, 243/255.0f), &pSidebarBrush);
        pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(50/255.0f, 49/255.0f, 48/255.0f), &pDarkGrayBrush);
        pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &pWhiteBrush);
        pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0/255.0f, 120/255.0f, 212/255.0f), &pBlueBrush);
        pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(237/255.0f, 235/255.0f, 233/255.0f), &pHoverBrush);
        pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(243/255.0f, 242/255.0f, 241/255.0f), &pLightGrayBrush);
    }
    
    void CreateSidebarTabs() {
        sidebarTabs.clear();
        
        // Activity tab
        TeamsTab activityTab;
        activityTab.rect = D2D1::RectF(8, 60, 72, 110);
        activityTab.name = L"Activity";
        activityTab.icon = L"🔔";
        activityTab.id = 1001;
        activityTab.isActive = (currentTab == 0);
        activityTab.hasNotification = true;
        activityTab.notificationCount = 3;
        sidebarTabs.push_back(activityTab);
        
        // Chat tab
        TeamsTab chatTab;
        chatTab.rect = D2D1::RectF(8, 120, 72, 170);
        chatTab.name = L"Chat";
        chatTab.icon = L"💬";
        chatTab.id = 1002;
        chatTab.isActive = (currentTab == 1);
        chatTab.hasNotification = true;
        chatTab.notificationCount = 7;
        sidebarTabs.push_back(chatTab);
        
        // Teams tab
        TeamsTab teamsTab;
        teamsTab.rect = D2D1::RectF(8, 180, 72, 230);
        teamsTab.name = L"Teams";
        teamsTab.icon = L"👥";
        teamsTab.id = 1003;
        teamsTab.isActive = (currentTab == 2);
        sidebarTabs.push_back(teamsTab);
        
        // Calendar tab
        TeamsTab calendarTab;
        calendarTab.rect = D2D1::RectF(8, 240, 72, 290);
        calendarTab.name = L"Calendar";
        calendarTab.icon = L"📅";
        calendarTab.id = 1004;
        calendarTab.isActive = (currentTab == 3);
        sidebarTabs.push_back(calendarTab);
        
        // Calls tab
        TeamsTab callsTab;
        callsTab.rect = D2D1::RectF(8, 300, 72, 350);
        callsTab.name = L"Calls";
        callsTab.icon = L"📞";
        callsTab.id = 1005;
        callsTab.isActive = (currentTab == 4);
        sidebarTabs.push_back(callsTab);
        
        // Files tab
        TeamsTab filesTab;
        filesTab.rect = D2D1::RectF(8, 360, 72, 410);
        filesTab.name = L"Files";
        filesTab.icon = L"📁";
        filesTab.id = 1006;
        filesTab.isActive = (currentTab == 5);
        sidebarTabs.push_back(filesTab);
        
        // Riley AI tab (special)
        TeamsTab rileyTab;
        rileyTab.rect = D2D1::RectF(8, 450, 72, 500);
        rileyTab.name = L"Riley AI";
        rileyTab.icon = L"🤖";
        rileyTab.id = 1007;
        rileyTab.isActive = (currentTab == 6);
        sidebarTabs.push_back(rileyTab);
    }
    
    void InitializeTeamsData() {
        // Initialize teams
        teamsList = {
            L"🏢 Executive Leadership",
            L"💻 Engineering & Development", 
            L"📈 Sales & Marketing",
            L"🎯 Customer Success",
            L"💰 Finance & Operations",
            L"👥 Human Resources",
            L"⚖️ Legal & Compliance",
            L"🤖 Riley AI Team",
            L"🔒 Security & IT",
            L"📊 Business Intelligence"
        };
        
        // Initialize activity feed
        activityFeed = {
            L"🤖 Riley AI analyzed quarterly performance - 15% growth detected",
            L"👥 New team member joined Engineering & Development",
            L"📅 Upcoming meeting: Q4 Strategy Review in 30 minutes",
            L"📊 Weekly analytics report is ready for review",
            L"🎯 Customer Success team exceeded targets by 23%",
            L"💡 Innovation Lab submitted 3 new patent applications",
            L"🔒 Security scan completed - All systems secure"
        };
        
        // Initialize chat messages
        chatMessages = {
            L"Riley AI: Good morning! I've analyzed overnight data and identified 3 optimization opportunities.",
            L"John Smith: Thanks Riley! Can you elaborate on the sales pipeline insights?",
            L"Riley AI: Certainly! Pipeline conversion rate increased 12% with AI-assisted lead scoring.",
            L"Sarah Johnson: The new predictive analytics are impressive. ROI projections look strong.",
            L"Riley AI: I've scheduled follow-up meetings based on priority scores. Calendar updated."
        };
    }

    void DrawTopBar() {
        // Draw Teams purple top bar
        D2D1_RECT_F topBar = D2D1::RectF(0, 0, 1600, 48);
        pRenderTarget->FillRectangle(topBar, pPurpleBrush);

        // Draw Teams logo and title
        D2D1_RECT_F logoRect = D2D1::RectF(90, 8, 400, 40);
        std::wstring title = L"🤖 Riley Enterprise Teams";
        pRenderTarget->DrawText(
            title.c_str(),
            title.length(),
            pTitleFormat,
            logoRect,
            pWhiteBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Draw search box
        D2D1_RECT_F searchRect = D2D1::RectF(450, 10, 750, 38);
        pRenderTarget->FillRoundedRectangle(
            D2D1::RoundedRect(searchRect, 4.0f, 4.0f),
            pWhiteBrush
        );

        D2D1_RECT_F searchTextRect = D2D1::RectF(460, 15, 740, 35);
        std::wstring searchText = L"🔍 Search Riley AI or type a command";
        pRenderTarget->DrawText(
            searchText.c_str(),
            searchText.length(),
            pTextFormat,
            searchTextRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Draw user profile
        D2D1_RECT_F profileRect = D2D1::RectF(1400, 8, 1580, 40);
        std::wstring profile = L"👤 Andrew Riley";
        pRenderTarget->DrawText(
            profile.c_str(),
            profile.length(),
            pTextFormat,
            profileRect,
            pWhiteBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );
    }

    void DrawSidebar() {
        // Draw Teams sidebar background
        D2D1_RECT_F sidebar = D2D1::RectF(0, 48, 80, 1000);
        pRenderTarget->FillRectangle(sidebar, pSidebarBrush);

        // Draw all sidebar tabs
        for (const auto& tab : sidebarTabs) {
            DrawTeamsTab(tab);
        }
    }

    void DrawTeamsTab(const TeamsTab& tab) {
        ID2D1SolidColorBrush* bgBrush = nullptr;
        ID2D1SolidColorBrush* textBrush = pDarkGrayBrush;

        if (tab.isActive) {
            bgBrush = pBlueBrush;
            textBrush = pWhiteBrush;
        } else if (tab.isHovered) {
            bgBrush = pHoverBrush;
        }

        // Draw background if needed
        if (bgBrush) {
            D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(tab.rect, 4.0f, 4.0f);
            pRenderTarget->FillRoundedRectangle(roundedRect, bgBrush);
        }

        // Draw icon
        D2D1_RECT_F iconRect = tab.rect;
        iconRect.bottom = iconRect.top + 24;
        iconRect.top += 8;

        pRenderTarget->DrawText(
            tab.icon.c_str(),
            tab.icon.length(),
            pTextFormat,
            iconRect,
            textBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Draw text
        D2D1_RECT_F textRect = tab.rect;
        textRect.top += 28;

        pRenderTarget->DrawText(
            tab.name.c_str(),
            tab.name.length(),
            pSmallFormat,
            textRect,
            textBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Draw notification badge
        if (tab.hasNotification && tab.notificationCount > 0) {
            D2D1_ELLIPSE badge = D2D1::Ellipse(D2D1::Point2F(65, 65), 8, 8);
            pRenderTarget->FillEllipse(badge, pBlueBrush);

            // Draw notification count
            D2D1_RECT_F countRect = D2D1::RectF(60, 60, 70, 70);
            std::wstring count = std::to_wstring(tab.notificationCount);
            pRenderTarget->DrawText(
                count.c_str(),
                count.length(),
                pSmallFormat,
                countRect,
                pWhiteBrush,
                D2D1_DRAW_TEXT_OPTIONS_NONE
            );
        }
    }

    void DrawMainContent() {
        // Draw main content area background
        D2D1_RECT_F contentArea = D2D1::RectF(80, 48, 1600, 1000);
        pRenderTarget->FillRectangle(contentArea, pWhiteBrush);

        switch (currentTab) {
            case 0: DrawActivityView(); break;
            case 1: DrawChatView(); break;
            case 2: DrawTeamsView(); break;
            case 3: DrawCalendarView(); break;
            case 4: DrawCallsView(); break;
            case 5: DrawFilesView(); break;
            case 6: DrawRileyAIView(); break;
        }
    }

    void DrawActivityView() {
        // Draw activity feed header
        D2D1_RECT_F headerRect = D2D1::RectF(100, 70, 1580, 110);
        std::wstring header = L"🔔 Activity Feed - Riley AI Insights";
        pRenderTarget->DrawText(
            header.c_str(),
            header.length(),
            pTitleFormat,
            headerRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Draw activity items
        float yPos = 130;
        for (const auto& activity : activityFeed) {
            D2D1_RECT_F activityRect = D2D1::RectF(120, yPos, 1560, yPos + 60);

            // Draw activity background
            D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(activityRect, 8.0f, 8.0f);
            pRenderTarget->FillRoundedRectangle(roundedRect, pLightGrayBrush);

            // Draw activity text
            D2D1_RECT_F textRect = D2D1::RectF(140, yPos + 15, 1540, yPos + 45);
            pRenderTarget->DrawText(
                activity.c_str(),
                activity.length(),
                pTextFormat,
                textRect,
                pDarkGrayBrush,
                D2D1_DRAW_TEXT_OPTIONS_NONE
            );

            yPos += 80;
        }
    }

    void DrawChatView() {
        // Draw chat header
        D2D1_RECT_F headerRect = D2D1::RectF(100, 70, 1580, 110);
        std::wstring header = L"💬 Chat with Riley AI";
        pRenderTarget->DrawText(
            header.c_str(),
            header.length(),
            pTitleFormat,
            headerRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Draw chat messages
        float yPos = 130;
        for (const auto& message : chatMessages) {
            bool isRiley = message.find(L"Riley AI:") == 0;

            D2D1_RECT_F messageRect;
            if (isRiley) {
                // Riley messages on the left
                messageRect = D2D1::RectF(120, yPos, 800, yPos + 60);
            } else {
                // User messages on the right
                messageRect = D2D1::RectF(900, yPos, 1560, yPos + 60);
            }

            // Draw message background
            D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(messageRect, 12.0f, 12.0f);
            ID2D1SolidColorBrush* bgBrush = isRiley ? pBlueBrush : pLightGrayBrush;
            ID2D1SolidColorBrush* textBrush = isRiley ? pWhiteBrush : pDarkGrayBrush;

            pRenderTarget->FillRoundedRectangle(roundedRect, bgBrush);

            // Draw message text
            D2D1_RECT_F textRect = D2D1::RectF(
                messageRect.left + 15,
                messageRect.top + 15,
                messageRect.right - 15,
                messageRect.bottom - 15
            );
            pRenderTarget->DrawText(
                message.c_str(),
                message.length(),
                pTextFormat,
                textRect,
                textBrush,
                D2D1_DRAW_TEXT_OPTIONS_NONE
            );

            yPos += 80;
        }

        // Draw input area
        D2D1_RECT_F inputRect = D2D1::RectF(120, 850, 1560, 900);
        D2D1_ROUNDED_RECT inputRounded = D2D1::RoundedRect(inputRect, 8.0f, 8.0f);
        pRenderTarget->FillRoundedRectangle(inputRounded, pLightGrayBrush);

        D2D1_RECT_F inputTextRect = D2D1::RectF(140, 865, 1540, 885);
        std::wstring inputPlaceholder = L"Type a message to Riley AI...";
        pRenderTarget->DrawText(
            inputPlaceholder.c_str(),
            inputPlaceholder.length(),
            pTextFormat,
            inputTextRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );
    }

    void DrawTeamsView() {
        // Draw teams header
        D2D1_RECT_F headerRect = D2D1::RectF(100, 70, 1580, 110);
        std::wstring header = L"👥 Teams - Riley Enterprise";
        pRenderTarget->DrawText(
            header.c_str(),
            header.length(),
            pTitleFormat,
            headerRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Draw "Join or create a team" button
        D2D1_RECT_F joinButtonRect = D2D1::RectF(1300, 75, 1560, 105);
        D2D1_ROUNDED_RECT joinButtonRounded = D2D1::RoundedRect(joinButtonRect, 6.0f, 6.0f);
        pRenderTarget->FillRoundedRectangle(joinButtonRounded, pBlueBrush);

        D2D1_RECT_F joinTextRect = D2D1::RectF(1310, 82, 1550, 98);
        std::wstring joinText = L"Join or create a team";
        pRenderTarget->DrawText(
            joinText.c_str(),
            joinText.length(),
            pSmallFormat,
            joinTextRect,
            pWhiteBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Draw teams grid
        float xPos = 120;
        float yPos = 130;
        int teamsPerRow = 3;
        int teamIndex = 0;

        for (const auto& team : teamsList) {
            D2D1_RECT_F teamRect = D2D1::RectF(xPos, yPos, xPos + 400, yPos + 120);

            // Draw team card background
            D2D1_ROUNDED_RECT teamRounded = D2D1::RoundedRect(teamRect, 12.0f, 12.0f);
            pRenderTarget->FillRoundedRectangle(teamRounded, pLightGrayBrush);

            // Draw team border on hover
            pRenderTarget->DrawRoundedRectangle(teamRounded, pDarkGrayBrush, 1.0f);

            // Draw team name
            D2D1_RECT_F teamNameRect = D2D1::RectF(xPos + 20, yPos + 20, xPos + 380, yPos + 50);
            pRenderTarget->DrawText(
                team.c_str(),
                team.length(),
                pTextFormat,
                teamNameRect,
                pDarkGrayBrush,
                D2D1_DRAW_TEXT_OPTIONS_NONE
            );

            // Draw member count
            D2D1_RECT_F memberRect = D2D1::RectF(xPos + 20, yPos + 60, xPos + 380, yPos + 80);
            std::wstring memberCount = L"👤 " + std::to_wstring(15 + teamIndex * 3) + L" members";
            pRenderTarget->DrawText(
                memberCount.c_str(),
                memberCount.length(),
                pSmallFormat,
                memberRect,
                pDarkGrayBrush,
                D2D1_DRAW_TEXT_OPTIONS_NONE
            );

            // Draw join button
            D2D1_RECT_F teamJoinRect = D2D1::RectF(xPos + 300, yPos + 85, xPos + 380, yPos + 105);
            D2D1_ROUNDED_RECT teamJoinRounded = D2D1::RoundedRect(teamJoinRect, 4.0f, 4.0f);
            pRenderTarget->FillRoundedRectangle(teamJoinRounded, pBlueBrush);

            D2D1_RECT_F joinBtnTextRect = D2D1::RectF(xPos + 315, yPos + 90, xPos + 365, yPos + 100);
            std::wstring joinBtnText = L"Join";
            pRenderTarget->DrawText(
                joinBtnText.c_str(),
                joinBtnText.length(),
                pSmallFormat,
                joinBtnTextRect,
                pWhiteBrush,
                D2D1_DRAW_TEXT_OPTIONS_NONE
            );

            xPos += 420;
            teamIndex++;

            if (teamIndex % teamsPerRow == 0) {
                xPos = 120;
                yPos += 140;
            }
        }
    }

    void DrawCalendarView() {
        D2D1_RECT_F headerRect = D2D1::RectF(100, 70, 1580, 110);
        std::wstring header = L"📅 Calendar - Riley AI Scheduling";
        pRenderTarget->DrawText(header.c_str(), header.length(), pTitleFormat, headerRect, pDarkGrayBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
    }

    void DrawCallsView() {
        D2D1_RECT_F headerRect = D2D1::RectF(100, 70, 1580, 110);
        std::wstring header = L"📞 Calls - Riley AI Voice Interface";
        pRenderTarget->DrawText(header.c_str(), header.length(), pTitleFormat, headerRect, pDarkGrayBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
    }

    void DrawFilesView() {
        D2D1_RECT_F headerRect = D2D1::RectF(100, 70, 1580, 110);
        std::wstring header = L"📁 Files - Riley AI Document Management";
        pRenderTarget->DrawText(header.c_str(), header.length(), pTitleFormat, headerRect, pDarkGrayBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
    }

    void DrawRileyAIView() {
        // Draw Riley AI header
        D2D1_RECT_F headerRect = D2D1::RectF(100, 70, 1580, 110);
        std::wstring header = L"🤖 Riley AI - Advanced Intelligence Dashboard";
        pRenderTarget->DrawText(
            header.c_str(),
            header.length(),
            pTitleFormat,
            headerRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Draw AI status panel
        D2D1_RECT_F statusRect = D2D1::RectF(120, 130, 760, 300);
        D2D1_ROUNDED_RECT statusRounded = D2D1::RoundedRect(statusRect, 12.0f, 12.0f);
        pRenderTarget->FillRoundedRectangle(statusRounded, pBlueBrush);

        D2D1_RECT_F statusTitleRect = D2D1::RectF(140, 150, 740, 180);
        std::wstring statusTitle = L"🧠 Riley AI Status: ACTIVE";
        pRenderTarget->DrawText(
            statusTitle.c_str(),
            statusTitle.length(),
            pTitleFormat,
            statusTitleRect,
            pWhiteBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        std::vector<std::wstring> aiStats = {
            L"⚡ Processing Speed: 2.3 THz",
            L"🧮 Active Calculations: 847,293",
            L"📊 Data Points Analyzed: 15.7M",
            L"🎯 Prediction Accuracy: 97.3%",
            L"🔄 Learning Rate: Optimal"
        };

        float statY = 190;
        for (const auto& stat : aiStats) {
            D2D1_RECT_F statRect = D2D1::RectF(140, statY, 740, statY + 20);
            pRenderTarget->DrawText(
                stat.c_str(),
                stat.length(),
                pTextFormat,
                statRect,
                pWhiteBrush,
                D2D1_DRAW_TEXT_OPTIONS_NONE
            );
            statY += 25;
        }

        // Draw AI insights panel
        D2D1_RECT_F insightsRect = D2D1::RectF(780, 130, 1560, 300);
        D2D1_ROUNDED_RECT insightsRounded = D2D1::RoundedRect(insightsRect, 12.0f, 12.0f);
        pRenderTarget->FillRoundedRectangle(insightsRounded, pLightGrayBrush);

        D2D1_RECT_F insightsTitleRect = D2D1::RectF(800, 150, 1540, 180);
        std::wstring insightsTitle = L"💡 AI Insights & Recommendations";
        pRenderTarget->DrawText(
            insightsTitle.c_str(),
            insightsTitle.length(),
            pTitleFormat,
            insightsTitleRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        std::vector<std::wstring> insights = {
            L"• Optimize team meeting schedules for 15% efficiency gain",
            L"• Sales pipeline shows 23% growth opportunity in Q1",
            L"• Recommend AI training for customer service team",
            L"• Security protocols updated - no threats detected"
        };

        float insightY = 190;
        for (const auto& insight : insights) {
            D2D1_RECT_F insightRect = D2D1::RectF(800, insightY, 1540, insightY + 20);
            pRenderTarget->DrawText(
                insight.c_str(),
                insight.length(),
                pTextFormat,
                insightRect,
                pDarkGrayBrush,
                D2D1_DRAW_TEXT_OPTIONS_NONE
            );
            insightY += 25;
        }
    }

    void Render() {
        if (!pRenderTarget) return;

        pRenderTarget->BeginDraw();
        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        DrawTopBar();
        DrawSidebar();
        DrawMainContent();

        HRESULT hr = pRenderTarget->EndDraw();
        if (FAILED(hr)) {
            // Handle render failure
        }
    }

    bool HandleClick(int x, int y) {
        // Check sidebar tab clicks
        for (int i = 0; i < sidebarTabs.size(); i++) {
            const auto& tab = sidebarTabs[i];
            if (x >= tab.rect.left && x <= tab.rect.right &&
                y >= tab.rect.top && y <= tab.rect.bottom) {

                // Update active tab
                for (auto& t : sidebarTabs) {
                    t.isActive = false;
                }
                sidebarTabs[i].isActive = true;
                currentTab = i;

                // Add Riley AI response if AI tab clicked
                if (i == 6 && riley) {
                    std::wstring response = riley->GetResponse();
                    chatMessages.push_back(response);
                }

                return true;
            }
        }

        return false;
    }

    void HandleMouseMove(int x, int y) {
        // Update hover states for sidebar tabs
        for (auto& tab : sidebarTabs) {
            tab.isHovered = (x >= tab.rect.left && x <= tab.rect.right &&
                           y >= tab.rect.top && y <= tab.rect.bottom);
        }
    }

    void Cleanup() {
        if (pPurpleBrush) pPurpleBrush->Release();
        if (pLightGrayBrush) pLightGrayBrush->Release();
        if (pDarkGrayBrush) pDarkGrayBrush->Release();
        if (pWhiteBrush) pWhiteBrush->Release();
        if (pBlueBrush) pBlueBrush->Release();
        if (pSidebarBrush) pSidebarBrush->Release();
        if (pHoverBrush) pHoverBrush->Release();
        if (pTextFormat) pTextFormat->Release();
        if (pTitleFormat) pTitleFormat->Release();
        if (pSmallFormat) pSmallFormat->Release();
        if (pRenderTarget) pRenderTarget->Release();
        if (pDWriteFactory) pDWriteFactory->Release();
        if (pD2DFactory) pD2DFactory->Release();
        if (riley) delete riley;
    }
};

// Global variables
TeamsInterface* g_teamsInterface = nullptr;

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Initialize Teams interface
            g_teamsInterface = new TeamsInterface();

            if (!g_teamsInterface->Initialize(hwnd)) {
                MessageBoxW(hwnd, L"Failed to initialize Teams interface", L"Error", MB_OK);
                return -1;
            }

            // Set window properties for Teams-like appearance
            SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (g_teamsInterface) {
                g_teamsInterface->Render();
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_SIZE: {
            // Handle window resize
            if (g_teamsInterface) {
                InvalidateRect(hwnd, nullptr, TRUE);
            }
            return 0;
        }

        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            if (g_teamsInterface) {
                if (g_teamsInterface->HandleClick(x, y)) {
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            }
            return 0;
        }

        case WM_MOUSEMOVE: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            if (g_teamsInterface) {
                g_teamsInterface->HandleMouseMove(x, y);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }

        case WM_KEYDOWN: {
            switch (wParam) {
                case VK_F1:
                    MessageBoxW(hwnd, L"Riley Enterprise Teams - F1 Help\n\nClick tabs to navigate:\n🔔 Activity\n💬 Chat\n👥 Teams\n📅 Calendar\n📞 Calls\n📁 Files\n🤖 Riley AI", L"Help", MB_OK);
                    break;
                case VK_F5:
                    if (g_teamsInterface) {
                        InvalidateRect(hwnd, nullptr, TRUE);
                    }
                    break;
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
            return 0;
        }

        case WM_DESTROY: {
            if (g_teamsInterface) {
                g_teamsInterface->Cleanup();
                delete g_teamsInterface;
                g_teamsInterface = nullptr;
            }

            PostQuitMessage(0);
            return 0;
        }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// Application entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize COM
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    // Register window class
    const wchar_t CLASS_NAME[] = L"RileyTeamsWindow";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);

    RegisterClassW(&wc);

    // Create window with Teams-like dimensions
    HWND hwnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        CLASS_NAME,
        L"🤖 Riley Enterprise - Microsoft Teams Clone",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1600, 1000,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr) {
        MessageBoxW(nullptr, L"Failed to create window", L"Error", MB_OK);
        return 0;
    }

    // Show window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup COM
    CoUninitialize();

    return 0;
}
