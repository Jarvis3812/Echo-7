#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <dwmapi.h>
#include <uxtheme.h>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include "core/riley_corpbrain.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")

// Riley Enterprise Color Scheme
#define TEAMS_PURPLE RGB(98, 100, 167)
#define TEAMS_DARK_PURPLE RGB(70, 72, 120)
#define TEAMS_LIGHT_GRAY RGB(243, 242, 241)
#define TEAMS_MEDIUM_GRAY RGB(237, 235, 233)
#define TEAMS_DARK_GRAY RGB(50, 49, 48)
#define TEAMS_WHITE RGB(255, 255, 255)
#define TEAMS_BLUE RGB(0, 120, 212)
#define TEAMS_GREEN RGB(16, 124, 16)
#define TEAMS_RED RGB(196, 43, 28)

// Global variables
HWND g_hwnd = nullptr;
RileyCorpBrain* riley = nullptr;

// Direct2D resources
ID2D1Factory* pD2DFactory = nullptr;
ID2D1HwndRenderTarget* pRenderTarget = nullptr;
IDWriteFactory* pDWriteFactory = nullptr;
IDWriteTextFormat* pTextFormat = nullptr;
IDWriteTextFormat* pTitleFormat = nullptr;
IDWriteTextFormat* pSmallFormat = nullptr;

// Brushes for Teams colors
ID2D1SolidColorBrush* pPurpleBrush = nullptr;
ID2D1SolidColorBrush* pLightGrayBrush = nullptr;
ID2D1SolidColorBrush* pDarkGrayBrush = nullptr;
ID2D1SolidColorBrush* pWhiteBrush = nullptr;
ID2D1SolidColorBrush* pBlueBrush = nullptr;
ID2D1SolidColorBrush* pGreenBrush = nullptr;
ID2D1SolidColorBrush* pHoverBrush = nullptr;

// UI State
int currentTab = 0; // 0=Teams, 1=Chat, 2=Calendar, 3=Calls, 4=Files
int hoveredButton = -1;
std::vector<std::wstring> teamsList;
std::vector<std::wstring> chatMessages;

// Control IDs
#define ID_TEAMS_TAB 1001
#define ID_CHAT_TAB 1002
#define ID_CALENDAR_TAB 1003
#define ID_CALLS_TAB 1004
#define ID_FILES_TAB 1005
#define ID_NEW_TEAM 1006
#define ID_NEW_CHAT 1007
#define ID_NEW_MEETING 1008

struct TeamsButton {
    D2D1_RECT_F rect;
    std::wstring text;
    std::wstring icon;
    int id;
    bool isActive = false;
    bool isHovered = false;
};

std::vector<TeamsButton> sidebarButtons;

// Function declarations
void DrawTeamsContent();
void DrawChatContent();
void DrawCalendarContent();
void DrawCallsContent();
void DrawFilesContent();
void Render();
void OnMouseMove(int x, int y);
int OnMouseClick(int x, int y);

bool InitializeD2D(HWND hwnd) {
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

    // Create text formats (Riley Enterprise fonts)
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
        18.0f,
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
    pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(243/255.0f, 242/255.0f, 241/255.0f), &pLightGrayBrush);
    pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(50/255.0f, 49/255.0f, 48/255.0f), &pDarkGrayBrush);
    pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &pWhiteBrush);
    pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0/255.0f, 120/255.0f, 212/255.0f), &pBlueBrush);
    pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(16/255.0f, 124/255.0f, 16/255.0f), &pGreenBrush);
    pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(237/255.0f, 235/255.0f, 233/255.0f), &pHoverBrush);

    return true;
}

void CreateSidebarButtons() {
    sidebarButtons.clear();
    
    // Teams tab
    TeamsButton teamsBtn;
    teamsBtn.rect = D2D1::RectF(10, 80, 70, 130);
    teamsBtn.text = L"Teams";
    teamsBtn.icon = L"T";
    teamsBtn.id = ID_TEAMS_TAB;
    teamsBtn.isActive = (currentTab == 0);
    sidebarButtons.push_back(teamsBtn);

    // Chat tab
    TeamsButton chatBtn;
    chatBtn.rect = D2D1::RectF(10, 140, 70, 190);
    chatBtn.text = L"Chat";
    chatBtn.icon = L"C";
    chatBtn.id = ID_CHAT_TAB;
    chatBtn.isActive = (currentTab == 1);
    sidebarButtons.push_back(chatBtn);

    // Calendar tab
    TeamsButton calBtn;
    calBtn.rect = D2D1::RectF(10, 200, 70, 250);
    calBtn.text = L"Calendar";
    calBtn.icon = L"Cal";
    calBtn.id = ID_CALENDAR_TAB;
    calBtn.isActive = (currentTab == 2);
    sidebarButtons.push_back(calBtn);

    // Calls tab
    TeamsButton callsBtn;
    callsBtn.rect = D2D1::RectF(10, 260, 70, 310);
    callsBtn.text = L"Calls";
    callsBtn.icon = L"Call";
    callsBtn.id = ID_CALLS_TAB;
    callsBtn.isActive = (currentTab == 3);
    sidebarButtons.push_back(callsBtn);

    // Files tab
    TeamsButton filesBtn;
    filesBtn.rect = D2D1::RectF(10, 320, 70, 370);
    filesBtn.text = L"Files";
    filesBtn.icon = L"F";
    filesBtn.id = ID_FILES_TAB;
    filesBtn.isActive = (currentTab == 4);
    sidebarButtons.push_back(filesBtn);
}

void DrawTeamsButton(const TeamsButton& button) {
    ID2D1SolidColorBrush* bgBrush = pLightGrayBrush;
    ID2D1SolidColorBrush* textBrush = pDarkGrayBrush;
    
    if (button.isActive) {
        bgBrush = pPurpleBrush;
        textBrush = pWhiteBrush;
    } else if (button.isHovered) {
        bgBrush = pWhiteBrush;
        textBrush = pDarkGrayBrush;
    }

    // Draw button background with rounded corners
    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(button.rect, 4.0f, 4.0f);
    pRenderTarget->FillRoundedRectangle(roundedRect, bgBrush);

    // Draw icon
    D2D1_RECT_F iconRect = button.rect;
    iconRect.bottom = iconRect.top + 20;
    iconRect.top += 5;
    
    pRenderTarget->DrawText(
        button.icon.c_str(),
        button.icon.length(),
        pTextFormat,
        iconRect,
        textBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // Draw text
    D2D1_RECT_F textRect = button.rect;
    textRect.top += 25;
    
    pRenderTarget->DrawText(
        button.text.c_str(),
        button.text.length(),
        pSmallFormat,
        textRect,
        textBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );
}

void DrawTopBar() {
    // Draw top bar background
    D2D1_RECT_F topBar = D2D1::RectF(0, 0, 1600, 50);
    pRenderTarget->FillRectangle(topBar, pPurpleBrush);

    // Draw Riley logo and title
    D2D1_RECT_F titleRect = D2D1::RectF(100, 10, 600, 40);
    std::wstring title = L"Riley Enterprise - Team Collaboration";
    pRenderTarget->DrawText(
        title.c_str(),
        title.length(),
        pTitleFormat,
        titleRect,
        pWhiteBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // Draw user profile area
    D2D1_RECT_F profileRect = D2D1::RectF(1400, 10, 1580, 40);
    std::wstring profile = L"Andrew Riley";
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
    // Draw sidebar background
    D2D1_RECT_F sidebar = D2D1::RectF(0, 50, 80, 1000);
    pRenderTarget->FillRectangle(sidebar, pLightGrayBrush);

    // Draw all sidebar buttons
    for (const auto& button : sidebarButtons) {
        DrawTeamsButton(button);
    }
}

void DrawTeamsContent() {
    // Draw main content area background (exact Teams white)
    D2D1_RECT_F contentArea = D2D1::RectF(80, 50, 1600, 1000);
    pRenderTarget->FillRectangle(contentArea, pWhiteBrush);

    // Draw teams list panel (exact Teams light gray)
    D2D1_RECT_F teamsPanel = D2D1::RectF(80, 50, 320, 1000);
    pRenderTarget->FillRectangle(teamsPanel, pLightGrayBrush);

    // Draw vertical separator line
    pRenderTarget->DrawLine(
        D2D1::Point2F(320, 50),
        D2D1::Point2F(320, 1000),
        pDarkGrayBrush,
        0.5f
    );

    // Teams panel header with search and new team button
    D2D1_RECT_F headerRect = D2D1::RectF(90, 60, 310, 85);
    std::wstring header = L"Teams";
    pRenderTarget->DrawText(
        header.c_str(),
        header.length(),
        pTitleFormat,
        headerRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // Add "Join or create a team" button
    D2D1_RECT_F joinButtonRect = D2D1::RectF(90, 90, 310, 115);
    pRenderTarget->FillRoundedRectangle(
        D2D1::RoundedRect(joinButtonRect, 4.0f, 4.0f),
        pHoverBrush
    );

    D2D1_RECT_F joinTextRect = D2D1::RectF(95, 95, 305, 110);
    std::wstring joinText = L"+ Join or create a team";
    pRenderTarget->DrawText(
        joinText.c_str(),
        joinText.length(),
        pSmallFormat,
        joinTextRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // Draw team items with Teams-style hover effects
    std::vector<std::wstring> teams = {
        L"Executive Leadership",
        L"Engineering & Development",
        L"Sales & Marketing",
        L"Customer Success",
        L"Finance & Operations",
        L"Human Resources",
        L"Legal & Compliance",
        L"Riley AI Team",
        L"Security & IT"
    };

    float yPos = 130;
    for (size_t i = 0; i < teams.size(); ++i) {
        D2D1_RECT_F teamBgRect = D2D1::RectF(85, yPos - 2, 315, yPos + 28);

        // Add hover effect (simulate for first team)
        if (i == 0) {
            pRenderTarget->FillRoundedRectangle(
                D2D1::RoundedRect(teamBgRect, 4.0f, 4.0f),
                pHoverBrush
            );
        }

        D2D1_RECT_F teamRect = D2D1::RectF(90, yPos, 310, yPos + 25);
        pRenderTarget->DrawText(
            teams[i].c_str(),
            teams[i].length(),
            pTextFormat,
            teamRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Add notification dots for some teams
        if (i == 0 || i == 7) { // Executive and Riley AI teams
            D2D1_ELLIPSE notificationDot = D2D1::Ellipse(D2D1::Point2F(305, yPos + 12), 4, 4);
            pRenderTarget->FillEllipse(notificationDot, pBlueBrush);
        }

        yPos += 35;
    }

    // Draw main chat/content area
    D2D1_RECT_F chatArea = D2D1::RectF(320, 50, 1600, 1000);
    pRenderTarget->FillRectangle(chatArea, pWhiteBrush);

    // Chat header with Teams-style toolbar
    D2D1_RECT_F chatHeaderBg = D2D1::RectF(320, 50, 1600, 100);
    pRenderTarget->FillRectangle(chatHeaderBg, pLightGrayBrush);

    // Draw separator line
    pRenderTarget->DrawLine(
        D2D1::Point2F(320, 100),
        D2D1::Point2F(1600, 100),
        pDarkGrayBrush,
        0.5f
    );

    // Chat header with breadcrumb navigation
    D2D1_RECT_F chatHeaderRect = D2D1::RectF(340, 60, 1000, 85);
    std::wstring chatHeader = L"Riley AI Team - General";
    pRenderTarget->DrawText(
        chatHeader.c_str(),
        chatHeader.length(),
        pTitleFormat,
        chatHeaderRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // Add Teams-style action buttons in header
    std::vector<std::wstring> headerButtons = {L"Video", L"Call", L"Pin", L"Star", L"Settings"};
    float buttonX = 1400;
    for (const auto& button : headerButtons) {
        D2D1_RECT_F buttonRect = D2D1::RectF(buttonX, 65, buttonX + 30, 90);
        pRenderTarget->FillRoundedRectangle(
            D2D1::RoundedRect(buttonRect, 4.0f, 4.0f),
            pHoverBrush
        );

        D2D1_RECT_F buttonTextRect = D2D1::RectF(buttonX + 5, 68, buttonX + 25, 88);
        pRenderTarget->DrawText(
            button.c_str(),
            button.length(),
            pTextFormat,
            buttonTextRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );
        buttonX += 35;
    }

    // Teams-style chat messages area
    D2D1_RECT_F messagesArea = D2D1::RectF(340, 120, 1580, 850);

    // Draw chat messages with Teams-style bubbles
    std::vector<std::pair<std::wstring, std::wstring>> messages = {
        {L"Riley AI", L"Good morning team! I've analyzed overnight data and identified 3 key optimization opportunities for today's sprint."},
        {L"Andrew Riley", L"Thanks Riley! Can you share the details on the sales pipeline insights?"},
        {L"Riley AI", L"Certainly! Pipeline conversion rate increased 12% with AI-assisted lead scoring. I've updated the dashboard with detailed metrics."},
        {L"Sarah Johnson", L"The predictive analytics are impressive. ROI projections look very strong for Q4."},
        {L"Riley AI", L"I've scheduled follow-up meetings based on priority scores. Your calendars have been updated automatically."}
    };

    float messageY = 130;
    for (const auto& message : messages) {
        // Draw message bubble background
        D2D1_RECT_F messageBg = D2D1::RectF(350, messageY, 1570, messageY + 60);
        if (message.first == L"Riley AI") {
            pRenderTarget->FillRoundedRectangle(
                D2D1::RoundedRect(messageBg, 8.0f, 8.0f),
                pLightGrayBrush
            );
        }

        // Draw sender name
        D2D1_RECT_F senderRect = D2D1::RectF(360, messageY + 5, 500, messageY + 20);
        pRenderTarget->DrawText(
            message.first.c_str(),
            message.first.length(),
            pSmallFormat,
            senderRect,
            pBlueBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Draw message content
        D2D1_RECT_F contentRect = D2D1::RectF(360, messageY + 22, 1560, messageY + 55);
        pRenderTarget->DrawText(
            message.second.c_str(),
            message.second.length(),
            pTextFormat,
            contentRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        messageY += 70;
    }

    // Draw message input area at bottom
    D2D1_RECT_F inputArea = D2D1::RectF(340, 860, 1580, 920);
    pRenderTarget->FillRoundedRectangle(
        D2D1::RoundedRect(inputArea, 8.0f, 8.0f),
        pLightGrayBrush
    );

    D2D1_RECT_F inputTextRect = D2D1::RectF(350, 875, 1500, 905);
    std::wstring inputPlaceholder = L"Type a message to Riley AI or the team...";
    pRenderTarget->DrawText(
        inputPlaceholder.c_str(),
        inputPlaceholder.length(),
        pTextFormat,
        inputTextRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // Send button
    D2D1_RECT_F sendButton = D2D1::RectF(1520, 870, 1570, 910);
    pRenderTarget->FillRoundedRectangle(
        D2D1::RoundedRect(sendButton, 6.0f, 6.0f),
        pBlueBrush
    );

    D2D1_RECT_F sendTextRect = D2D1::RectF(1535, 880, 1555, 900);
    std::wstring sendText = L"Send";
    pRenderTarget->DrawText(
        sendText.c_str(),
        sendText.length(),
        pTextFormat,
        sendTextRect,
        pWhiteBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );
}

void DrawChatContent() {
    // Draw main chat area background
    D2D1_RECT_F contentArea = D2D1::RectF(80, 50, 1600, 1000);
    pRenderTarget->FillRectangle(contentArea, pWhiteBrush);

    // Draw recent chats panel
    D2D1_RECT_F chatsPanel = D2D1::RectF(80, 50, 320, 1000);
    pRenderTarget->FillRectangle(chatsPanel, pLightGrayBrush);

    // Draw separator line
    pRenderTarget->DrawLine(
        D2D1::Point2F(320, 50),
        D2D1::Point2F(320, 1000),
        pDarkGrayBrush,
        0.5f
    );

    // Chat panel header
    D2D1_RECT_F headerRect = D2D1::RectF(90, 60, 310, 85);
    std::wstring header = L"Chat";
    pRenderTarget->DrawText(
        header.c_str(),
        header.length(),
        pTitleFormat,
        headerRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // Recent chats list
    std::vector<std::pair<std::wstring, std::wstring>> recentChats = {
        {L"🤖 Riley AI", L"I've completed the quarterly analysis..."},
        {L"👤 John Smith (CEO)", L"Great work on the presentation"},
        {L"👤 Sarah Johnson (CTO)", L"The new features look promising"},
        {L"👥 Engineering Team", L"Sprint planning meeting at 2 PM"},
        {L"👤 Mike Wilson (CFO)", L"Budget approval needed"},
        {L"👥 Sales Team", L"Q4 targets exceeded!"}
    };

    float chatY = 100;
    for (const auto& chat : recentChats) {
        D2D1_RECT_F chatBgRect = D2D1::RectF(85, chatY - 2, 315, chatY + 48);

        // Highlight first chat (Riley AI)
        if (chatY == 100) {
            pRenderTarget->FillRoundedRectangle(
                D2D1::RoundedRect(chatBgRect, 4.0f, 4.0f),
                pHoverBrush
            );
        }

        // Draw chat name
        D2D1_RECT_F nameRect = D2D1::RectF(90, chatY, 310, chatY + 18);
        pRenderTarget->DrawText(
            chat.first.c_str(),
            chat.first.length(),
            pTextFormat,
            nameRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Draw last message preview
        D2D1_RECT_F previewRect = D2D1::RectF(90, chatY + 20, 310, chatY + 38);
        pRenderTarget->DrawText(
            chat.second.c_str(),
            chat.second.length(),
            pSmallFormat,
            previewRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        chatY += 55;
    }

    // Draw main chat conversation area
    D2D1_RECT_F chatArea = D2D1::RectF(320, 50, 1600, 1000);
    pRenderTarget->FillRectangle(chatArea, pWhiteBrush);

    // Chat header
    D2D1_RECT_F chatHeaderBg = D2D1::RectF(320, 50, 1600, 100);
    pRenderTarget->FillRectangle(chatHeaderBg, pLightGrayBrush);

    D2D1_RECT_F chatHeaderRect = D2D1::RectF(340, 65, 1000, 85);
    std::wstring chatHeader = L"🤖 Riley AI - Personal Assistant";
    pRenderTarget->DrawText(
        chatHeader.c_str(),
        chatHeader.length(),
        pTitleFormat,
        chatHeaderRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // Chat messages with Riley AI conversation
    std::vector<std::pair<std::wstring, std::wstring>> messages = {
        {L"Riley AI", L"🌅 Good morning Andrew! I've analyzed overnight data and prepared your daily briefing. Revenue is up 3.2% from yesterday."},
        {L"You", L"That's great! What about the team productivity metrics?"},
        {L"Riley AI", L"📊 Team productivity increased 8% this week. Engineering team completed 23 tasks, Sales closed 5 new deals worth $340K total."},
        {L"You", L"Can you schedule a team meeting for this afternoon?"},
        {L"Riley AI", L"✅ I've scheduled 'Team Performance Review' for 3:00 PM today. All team leads have been notified and calendar invites sent."},
        {L"You", L"Perfect! Any security alerts I should know about?"},
        {L"Riley AI", L"🔒 All systems secure. Completed 3 security scans overnight - no threats detected. Firewall blocked 12 suspicious attempts."}
    };

    float messageY = 120;
    for (const auto& message : messages) {
        bool isRiley = (message.first == L"Riley AI");

        // Message alignment
        float msgX = isRiley ? 340 : 1200;
        float msgWidth = 350;

        if (!isRiley) msgX = 1250; // Right align user messages

        // Message bubble
        D2D1_RECT_F messageBg = D2D1::RectF(msgX, messageY, msgX + msgWidth, messageY + 80);
        ID2D1SolidColorBrush* bgBrush = isRiley ? pLightGrayBrush : pBlueBrush;
        pRenderTarget->FillRoundedRectangle(
            D2D1::RoundedRect(messageBg, 12.0f, 12.0f),
            bgBrush
        );

        // Sender name
        D2D1_RECT_F senderRect = D2D1::RectF(msgX + 10, messageY + 8, msgX + msgWidth - 10, messageY + 25);
        ID2D1SolidColorBrush* senderBrush = isRiley ? pBlueBrush : pWhiteBrush;
        pRenderTarget->DrawText(
            message.first.c_str(),
            message.first.length(),
            pSmallFormat,
            senderRect,
            senderBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Message content
        D2D1_RECT_F contentRect = D2D1::RectF(msgX + 10, messageY + 28, msgX + msgWidth - 10, messageY + 75);
        ID2D1SolidColorBrush* textBrush = isRiley ? pDarkGrayBrush : pWhiteBrush;
        pRenderTarget->DrawText(
            message.second.c_str(),
            message.second.length(),
            pTextFormat,
            contentRect,
            textBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        messageY += 90;
    }

    // Message input area
    D2D1_RECT_F inputArea = D2D1::RectF(340, 860, 1580, 920);
    pRenderTarget->FillRoundedRectangle(
        D2D1::RoundedRect(inputArea, 8.0f, 8.0f),
        pLightGrayBrush
    );

    D2D1_RECT_F inputTextRect = D2D1::RectF(350, 875, 1500, 905);
    std::wstring inputPlaceholder = L"💬 Ask Riley anything about your business...";
    pRenderTarget->DrawText(
        inputPlaceholder.c_str(),
        inputPlaceholder.length(),
        pTextFormat,
        inputTextRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );
}

void Render() {
    if (!pRenderTarget) return;

    pRenderTarget->BeginDraw();

    // Clear background
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    // Draw UI components
    DrawTopBar();
    DrawSidebar();

    switch (currentTab) {
    case 0:
        DrawTeamsContent();
        break;
    case 1:
        DrawChatContent();
        break;
    case 2:
        DrawCalendarContent();
        break;
    case 3:
        DrawCallsContent();
        break;
    case 4:
        DrawFilesContent();
        break;
    }

    pRenderTarget->EndDraw();
}

void DrawCalendarContent() {
    // Draw calendar background
    D2D1_RECT_F contentArea = D2D1::RectF(80, 50, 1600, 1000);
    pRenderTarget->FillRectangle(contentArea, pWhiteBrush);

    // Calendar header
    D2D1_RECT_F headerBg = D2D1::RectF(80, 50, 1600, 100);
    pRenderTarget->FillRectangle(headerBg, pLightGrayBrush);

    D2D1_RECT_F headerRect = D2D1::RectF(100, 65, 600, 85);
    std::wstring header = L"📅 Calendar - Riley AI Scheduling";
    pRenderTarget->DrawText(
        header.c_str(),
        header.length(),
        pTitleFormat,
        headerRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // Today's schedule
    D2D1_RECT_F scheduleRect = D2D1::RectF(100, 120, 800, 140);
    std::wstring todayHeader = L"📋 Today's Schedule - AI Optimized";
    pRenderTarget->DrawText(
        todayHeader.c_str(),
        todayHeader.length(),
        pTitleFormat,
        scheduleRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // Meeting items
    std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> meetings = {
        {L"9:00 AM", L"🤖 Daily Riley AI Briefing", L"Automated - 15 min"},
        {L"10:30 AM", L"👥 Team Standup", L"Conference Room A - 30 min"},
        {L"2:00 PM", L"📊 Q4 Performance Review", L"Riley AI Analysis - 60 min"},
        {L"3:30 PM", L"🎯 Client Strategy Meeting", L"Video Call - 45 min"},
        {L"5:00 PM", L"🔍 Riley Analytics Review", L"AI Insights - 30 min"}
    };

    float meetingY = 160;
    for (const auto& meeting : meetings) {
        // Meeting time block
        D2D1_RECT_F timeBg = D2D1::RectF(100, meetingY, 180, meetingY + 60);
        pRenderTarget->FillRoundedRectangle(
            D2D1::RoundedRect(timeBg, 6.0f, 6.0f),
            pBlueBrush
        );

        D2D1_RECT_F timeRect = D2D1::RectF(105, meetingY + 20, 175, meetingY + 40);
        pRenderTarget->DrawText(
            std::get<0>(meeting).c_str(),
            std::get<0>(meeting).length(),
            pSmallFormat,
            timeRect,
            pWhiteBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Meeting details
        D2D1_RECT_F titleRect = D2D1::RectF(190, meetingY + 5, 750, meetingY + 25);
        pRenderTarget->DrawText(
            std::get<1>(meeting).c_str(),
            std::get<1>(meeting).length(),
            pTextFormat,
            titleRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        D2D1_RECT_F detailsRect = D2D1::RectF(190, meetingY + 28, 750, meetingY + 48);
        pRenderTarget->DrawText(
            std::get<2>(meeting).c_str(),
            std::get<2>(meeting).length(),
            pSmallFormat,
            detailsRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        meetingY += 70;
    }

    // Riley AI insights panel
    D2D1_RECT_F insightsPanel = D2D1::RectF(850, 120, 1580, 600);
    pRenderTarget->FillRoundedRectangle(
        D2D1::RoundedRect(insightsPanel, 8.0f, 8.0f),
        pLightGrayBrush
    );

    D2D1_RECT_F insightsHeaderRect = D2D1::RectF(870, 140, 1560, 165);
    std::wstring insightsHeader = L"🤖 Riley AI Calendar Intelligence";
    pRenderTarget->DrawText(
        insightsHeader.c_str(),
        insightsHeader.length(),
        pTitleFormat,
        insightsHeaderRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    D2D1_RECT_F insightsContentRect = D2D1::RectF(870, 180, 1560, 580);
    std::wstring insightsContent = L"📊 Schedule Optimization:\n"
                                  L"• 3 potential conflicts resolved automatically\n"
                                  L"• Travel time calculated and buffer added\n"
                                  L"• Meeting prep time scheduled\n\n"
                                  L"🎯 Productivity Insights:\n"
                                  L"• Peak focus time: 10 AM - 12 PM\n"
                                  L"• Recommended break at 3:15 PM\n"
                                  L"• Energy levels optimal for strategy meeting\n\n"
                                  L"⚡ AI Recommendations:\n"
                                  L"• Move 5 PM meeting to tomorrow for better focus\n"
                                  L"• Add 15-min prep time before client meeting\n"
                                  L"• Schedule team lunch for Friday\n\n"
                                  L"📈 This Week's Performance:\n"
                                  L"• 94% meeting attendance rate\n"
                                  L"• Average meeting efficiency: 87%\n"
                                  L"• Time saved by AI optimization: 2.3 hours";

    pRenderTarget->DrawText(
        insightsContent.c_str(),
        insightsContent.length(),
        pTextFormat,
        insightsContentRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );
}

void DrawCallsContent() {
    // Draw calls background
    D2D1_RECT_F contentArea = D2D1::RectF(80, 50, 1600, 1000);
    pRenderTarget->FillRectangle(contentArea, pWhiteBrush);

    // Calls header
    D2D1_RECT_F headerBg = D2D1::RectF(80, 50, 1600, 100);
    pRenderTarget->FillRectangle(headerBg, pLightGrayBrush);

    D2D1_RECT_F headerRect = D2D1::RectF(100, 65, 600, 85);
    std::wstring header = L"📞 Calls - AI Enhanced Communication";
    pRenderTarget->DrawText(
        header.c_str(),
        header.length(),
        pTitleFormat,
        headerRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // Call history panel
    D2D1_RECT_F historyPanel = D2D1::RectF(100, 120, 500, 800);
    pRenderTarget->FillRoundedRectangle(
        D2D1::RoundedRect(historyPanel, 8.0f, 8.0f),
        pLightGrayBrush
    );

    D2D1_RECT_F historyHeaderRect = D2D1::RectF(120, 140, 480, 165);
    std::wstring historyHeader = L"📋 Recent Calls";
    pRenderTarget->DrawText(
        historyHeader.c_str(),
        historyHeader.length(),
        pTitleFormat,
        historyHeaderRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // Call history items
    std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> callHistory = {
        {L"📹", L"John Smith (CEO)", L"2:30 PM - 45 min"},
        {L"📞", L"Sarah Johnson (CTO)", L"1:15 PM - 23 min"},
        {L"📹", L"Client Strategy Call", L"11:00 AM - 60 min"},
        {L"📞", L"Mike Wilson (CFO)", L"Yesterday - 15 min"},
        {L"📹", L"Team All-Hands", L"Yesterday - 90 min"}
    };

    float callY = 180;
    for (const auto& call : callHistory) {
        D2D1_RECT_F callRect = D2D1::RectF(120, callY, 480, callY + 50);

        // Call type icon
        D2D1_RECT_F iconRect = D2D1::RectF(125, callY + 15, 145, callY + 35);
        pRenderTarget->DrawText(
            std::get<0>(call).c_str(),
            std::get<0>(call).length(),
            pTextFormat,
            iconRect,
            pBlueBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        // Call details
        D2D1_RECT_F nameRect = D2D1::RectF(155, callY + 8, 470, callY + 28);
        pRenderTarget->DrawText(
            std::get<1>(call).c_str(),
            std::get<1>(call).length(),
            pTextFormat,
            nameRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        D2D1_RECT_F timeRect = D2D1::RectF(155, callY + 30, 470, callY + 45);
        pRenderTarget->DrawText(
            std::get<2>(call).c_str(),
            std::get<2>(call).length(),
            pSmallFormat,
            timeRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );

        callY += 60;
    }

    // Riley AI call features panel
    D2D1_RECT_F featuresPanel = D2D1::RectF(550, 120, 1580, 800);
    pRenderTarget->FillRoundedRectangle(
        D2D1::RoundedRect(featuresPanel, 8.0f, 8.0f),
        pLightGrayBrush
    );

    D2D1_RECT_F featuresHeaderRect = D2D1::RectF(570, 140, 1560, 165);
    std::wstring featuresHeader = L"🤖 Riley AI Call Intelligence";
    pRenderTarget->DrawText(
        featuresHeader.c_str(),
        featuresHeader.length(),
        pTitleFormat,
        featuresHeaderRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    D2D1_RECT_F featuresContentRect = D2D1::RectF(570, 180, 1560, 780);
    std::wstring featuresContent = L"🎥 AI-Enhanced Video Calls:\n"
                                  L"• Real-time transcription and translation\n"
                                  L"• Automatic meeting notes and action items\n"
                                  L"• Background noise cancellation\n"
                                  L"• Smart camera framing and lighting\n\n"
                                  L"📊 Call Analytics:\n"
                                  L"• Speaking time analysis\n"
                                  L"• Sentiment analysis during calls\n"
                                  L"• Engagement level monitoring\n"
                                  L"• Follow-up recommendations\n\n"
                                  L"🔍 Smart Features:\n"
                                  L"• Automatic call scheduling\n"
                                  L"• Contact suggestions based on context\n"
                                  L"• Integration with calendar and tasks\n"
                                  L"• Call quality optimization\n\n"
                                  L"📈 Performance Metrics:\n"
                                  L"• Average call duration: 32 minutes\n"
                                  L"• Call success rate: 96%\n"
                                  L"• Customer satisfaction: 4.8/5\n"
                                  L"• Action items completion: 89%\n\n"
                                  L"🚀 Start a new call with AI assistance:\n"
                                  L"• Video call with screen sharing\n"
                                  L"• Audio call with transcription\n"
                                  L"• Conference call with up to 100 participants";

    pRenderTarget->DrawText(
        featuresContent.c_str(),
        featuresContent.length(),
        pTextFormat,
        featuresContentRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );
}

void DrawFilesContent() {
    // Draw files background
    D2D1_RECT_F contentArea = D2D1::RectF(80, 50, 1600, 1000);
    pRenderTarget->FillRectangle(contentArea, pWhiteBrush);

    // Files header
    D2D1_RECT_F headerBg = D2D1::RectF(80, 50, 1600, 100);
    pRenderTarget->FillRectangle(headerBg, pLightGrayBrush);

    D2D1_RECT_F headerRect = D2D1::RectF(100, 65, 600, 85);
    std::wstring header = L"📁 Files - AI Powered Document Management";
    pRenderTarget->DrawText(
        header.c_str(),
        header.length(),
        pTitleFormat,
        headerRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // File tree panel
    D2D1_RECT_F treePanel = D2D1::RectF(100, 120, 400, 800);
    pRenderTarget->FillRoundedRectangle(
        D2D1::RoundedRect(treePanel, 8.0f, 8.0f),
        pLightGrayBrush
    );

    D2D1_RECT_F treeHeaderRect = D2D1::RectF(120, 140, 380, 165);
    std::wstring treeHeader = L"📂 File Structure";
    pRenderTarget->DrawText(
        treeHeader.c_str(),
        treeHeader.length(),
        pTitleFormat,
        treeHeaderRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    // File structure
    std::vector<std::pair<std::wstring, int>> fileStructure = {
        {L"📁 Corporate Documents", 0},
        {L"  📄 Annual Report 2024.pdf", 1},
        {L"  📄 Strategic Plan.docx", 1},
        {L"📁 Project Files", 0},
        {L"  📁 Project Alpha", 1},
        {L"    📄 Requirements.docx", 2},
        {L"    📊 Timeline.xlsx", 2},
        {L"📁 Riley AI Data", 0},
        {L"  📄 Analytics Report.pdf", 1},
        {L"  📊 Performance Metrics.xlsx", 1},
        {L"📁 Team Resources", 0},
        {L"  📄 Employee Handbook.pdf", 1},
        {L"  📊 Budget 2024.xlsx", 1}
    };

    float fileY = 180;
    for (const auto& file : fileStructure) {
        float indent = 120 + (file.second * 15);
        D2D1_RECT_F fileRect = D2D1::RectF(indent, fileY, 380, fileY + 20);
        pRenderTarget->DrawText(
            file.first.c_str(),
            file.first.length(),
            pSmallFormat,
            fileRect,
            pDarkGrayBrush,
            D2D1_DRAW_TEXT_OPTIONS_NONE
        );
        fileY += 25;
    }

    // Riley AI file intelligence panel
    D2D1_RECT_F intelligencePanel = D2D1::RectF(450, 120, 1580, 800);
    pRenderTarget->FillRoundedRectangle(
        D2D1::RoundedRect(intelligencePanel, 8.0f, 8.0f),
        pLightGrayBrush
    );

    D2D1_RECT_F intelligenceHeaderRect = D2D1::RectF(470, 140, 1560, 165);
    std::wstring intelligenceHeader = L"🤖 Riley AI File Intelligence";
    pRenderTarget->DrawText(
        intelligenceHeader.c_str(),
        intelligenceHeader.length(),
        pTitleFormat,
        intelligenceHeaderRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    D2D1_RECT_F intelligenceContentRect = D2D1::RectF(470, 180, 1560, 780);
    std::wstring intelligenceContent = L"🔍 Smart File Management:\n"
                                      L"• Automatic file categorization and tagging\n"
                                      L"• AI-powered search across all documents\n"
                                      L"• Content analysis and key insights extraction\n"
                                      L"• Duplicate detection and cleanup\n\n"
                                      L"📊 File Analytics:\n"
                                      L"• Total files: 2,847 documents\n"
                                      L"• Storage used: 15.7 GB\n"
                                      L"• Files analyzed by Riley: 2,203 (77%)\n"
                                      L"• Average file access time: 0.3 seconds\n\n"
                                      L"🔒 Security & Compliance:\n"
                                      L"• All files scanned for security threats\n"
                                      L"• Compliance checking for regulations\n"
                                      L"• Access control and permissions\n"
                                      L"• Version control and audit trails\n\n"
                                      L"💡 AI Insights:\n"
                                      L"• Most accessed: Strategic Plan.docx\n"
                                      L"• Trending topic: Q4 Performance\n"
                                      L"• Collaboration hotspot: Project Alpha\n"
                                      L"• Recommended cleanup: 23 duplicate files\n\n"
                                      L"🚀 Smart Features:\n"
                                      L"• Auto-generate summaries\n"
                                      L"• Extract action items from documents\n"
                                      L"• Smart file sharing recommendations\n"
                                      L"• Integration with Teams and Calendar";

    pRenderTarget->DrawText(
        intelligenceContent.c_str(),
        intelligenceContent.length(),
        pTextFormat,
        intelligenceContentRect,
        pDarkGrayBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );
}

void OnMouseMove(int x, int y) {
    bool needsRedraw = false;
    
    for (auto& button : sidebarButtons) {
        bool wasHovered = button.isHovered;
        button.isHovered = (x >= button.rect.left && x <= button.rect.right &&
                           y >= button.rect.top && y <= button.rect.bottom);
        
        if (wasHovered != button.isHovered) {
            needsRedraw = true;
        }
    }
    
    if (needsRedraw) {
        InvalidateRect(g_hwnd, nullptr, FALSE);
    }
}

int OnMouseClick(int x, int y) {
    for (const auto& button : sidebarButtons) {
        if (x >= button.rect.left && x <= button.rect.right &&
            y >= button.rect.top && y <= button.rect.bottom) {
            return button.id;
        }
    }
    return -1;
}

// Window procedure
LRESULT CALLBACK TeamsWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        g_hwnd = hwnd;

        // Enable modern window composition
        BOOL darkMode = TRUE;
        DwmSetWindowAttribute(hwnd, 20, &darkMode, sizeof(darkMode)); // DWMWA_USE_IMMERSIVE_DARK_MODE

        // Initialize Direct2D
        if (!InitializeD2D(hwnd)) {
            MessageBoxA(hwnd, "Failed to initialize Direct2D", "Error", MB_OK);
            return -1;
        }

        // Create sidebar buttons
        CreateSidebarButtons();

        // Initialize Riley AI
        try {
            riley = new RileyCorpBrain();
            riley->initializeModules();

            // Start Riley's background processing
            std::thread rileyThread([&]() {
                while (true) {
                    if (riley) {
                        riley->runAdvancedAnalytics();
                        riley->analyzeScheduleHealth();
                    }
                    std::this_thread::sleep_for(std::chrono::minutes(3));
                }
            });
            rileyThread.detach();

        } catch (const std::exception& e) {
            MessageBoxA(hwnd, e.what(), "Riley AI Initialization", MB_OK);
        }

        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        Render();
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_MOUSEMOVE: {
        OnMouseMove(LOWORD(lParam), HIWORD(lParam));
        return 0;
    }

    case WM_LBUTTONDOWN: {
        int buttonId = OnMouseClick(LOWORD(lParam), HIWORD(lParam));
        if (buttonId != -1) {
            SendMessage(hwnd, WM_COMMAND, buttonId, 0);
        }
        return 0;
    }

    case WM_COMMAND: {
        int buttonId = LOWORD(wParam);

        switch (buttonId) {
        case ID_TEAMS_TAB: {
            currentTab = 0;
            CreateSidebarButtons();

            if (riley) {
                std::string taskId = riley->createTask("Team collaboration optimization", "Team Lead", 6);
                riley->runAdvancedAnalytics();

                std::wstring message = L"✅ TEAMS ACTIVATED!\n\n";
                message += L"🤖 Riley AI Team Features:\n";
                message += L"• Intelligent team formation\n";
                message += L"• Real-time collaboration insights\n";
                message += L"• Automated task distribution\n";
                message += L"• Performance analytics\n";
                message += L"• Smart resource allocation\n\n";
                message += L"Task ID: " + std::wstring(taskId.begin(), taskId.end()) + L"\n";
                message += L"🎯 Riley is optimizing team productivity";

                MessageBoxW(hwnd, message.c_str(), L"Riley Teams Intelligence", MB_OK);
            }

            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        }

        case ID_CHAT_TAB: {
            currentTab = 1;
            CreateSidebarButtons();

            if (riley) {
                riley->runAdvancedAnalytics();
                auto insights = riley->generateDataInsights("communication");

                std::wstring message = L"💬 AI-ENHANCED CHAT ACTIVATED!\n\n";
                message += L"🧠 Riley Chat Intelligence:\n";
                message += L"• Real-time sentiment analysis\n";
                message += L"• Smart response suggestions\n";
                message += L"• Context-aware recommendations\n";
                message += L"• Business intelligence integration\n";
                message += L"• Automatic meeting scheduling\n";
                message += L"• File sharing with AI analysis\n\n";

                message += L"💡 Current Business Insights:\n";
                for (const auto& insight : insights) {
                    message += L"• " + std::wstring(insight.begin(), insight.end()) + L"\n";
                }

                MessageBoxW(hwnd, message.c_str(), L"Riley Chat Intelligence", MB_OK);
            }

            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        }

        case ID_CALENDAR_TAB: {
            currentTab = 2;
            CreateSidebarButtons();

            if (riley) {
                riley->analyzeScheduleHealth();
                auto tasks = riley->getNextRecommendedTasks("Current User");

                std::wstring message = L"📅 SMART CALENDAR ACTIVATED!\n\n";
                message += L"🤖 Riley Scheduling Intelligence:\n";
                message += L"• Optimal meeting time calculation\n";
                message += L"• Conflict detection and resolution\n";
                message += L"• Resource allocation optimization\n";
                message += L"• Travel time consideration\n";
                message += L"• Preparation time scheduling\n";
                message += L"• AI-powered agenda creation\n\n";

                message += L"🎯 Recommended Tasks:\n";
                for (size_t i = 0; i < tasks.size() && i < 5; ++i) {
                    message += L"• " + std::wstring(tasks[i].begin(), tasks[i].end()) + L"\n";
                }

                MessageBoxW(hwnd, message.c_str(), L"Riley Calendar Intelligence", MB_OK);
            }

            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        }

        case ID_CALLS_TAB: {
            currentTab = 3;
            CreateSidebarButtons();

            std::wstring message = L"📞 AI-ENHANCED CALLS READY!\n\n";
            message += L"Riley Enterprise Call Features:\n";
            message += L"• HD Video & Audio Calls\n";
            message += L"• Real-time transcription\n";
            message += L"• AI meeting insights\n";
            message += L"• Smart call routing\n";
            message += L"• Background noise cancellation\n";
            message += L"• Meeting recording & analysis\n";
            message += L"• Screen sharing with annotations\n\n";
            message += L"🤖 Riley provides real-time insights during calls";

            MessageBoxW(hwnd, message.c_str(), L"Riley Call Intelligence", MB_OK);

            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        }

        case ID_FILES_TAB: {
            currentTab = 4;
            CreateSidebarButtons();

            if (riley) {
                std::wstring message = L"📁 INTELLIGENT FILE SYSTEM!\n\n";
                message += L"🧠 Riley AI File Features:\n";
                message += L"• Automatic file organization\n";
                message += L"• AI-powered search & indexing\n";
                message += L"• Content analysis & summarization\n";
                message += L"• Version control & tracking\n";
                message += L"• Security scanning & compliance\n";
                message += L"• Smart collaboration features\n";
                message += L"• SharePoint integration\n\n";
                message += L"📊 File Analytics:\n";
                message += L"• 2,847 files analyzed by Riley\n";
                message += L"• Storage optimized by 23%\n";
                message += L"• All security scans: CLEAR\n";
                message += L"• Collaboration efficiency: +34%";

                MessageBoxW(hwnd, message.c_str(), L"Riley File Intelligence", MB_OK);
            }

            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        }
        }
        return 0;
    }

    case WM_SIZE: {
        if (pRenderTarget) {
            RECT rc;
            GetClientRect(hwnd, &rc);
            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
            pRenderTarget->Resize(size);
        }
        return 0;
    }

    case WM_DESTROY:
        if (riley) delete riley;

        // Cleanup Direct2D resources
        if (pGreenBrush) pGreenBrush->Release();
        if (pBlueBrush) pBlueBrush->Release();
        if (pWhiteBrush) pWhiteBrush->Release();
        if (pDarkGrayBrush) pDarkGrayBrush->Release();
        if (pLightGrayBrush) pLightGrayBrush->Release();
        if (pPurpleBrush) pPurpleBrush->Release();
        if (pSmallFormat) pSmallFormat->Release();
        if (pTitleFormat) pTitleFormat->Release();
        if (pTextFormat) pTextFormat->Release();
        if (pDWriteFactory) pDWriteFactory->Release();
        if (pRenderTarget) pRenderTarget->Release();
        if (pD2DFactory) pD2DFactory->Release();

        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize COM
    CoInitialize(nullptr);

    // Register window class
    WNDCLASSW wc = {};
    wc.lpfnWndProc = TeamsWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"RileyTeamsStyle";
    wc.hbrBackground = nullptr;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClassW(&wc);

    // Create main window with Teams-like styling
    HWND hwnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        L"RileyTeamsStyle",
        L"Riley Enterprise - Team Collaboration",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        100, 50, 1600, 1000,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd) {
        MessageBoxA(nullptr, "Failed to create window", "Error", MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();
    return (int)msg.wParam;
}
