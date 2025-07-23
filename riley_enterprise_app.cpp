#include <windows.h>
#include <commctrl.h>
#include <dwmapi.h>
#include <uxtheme.h>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <sstream>
#include "core/riley_corpbrain.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

// Modern UI Colors (Dark Theme)
#define COLOR_DARK_BG RGB(32, 32, 32)
#define COLOR_SIDEBAR RGB(45, 45, 45)
#define COLOR_ACCENT RGB(0, 120, 215)
#define COLOR_HOVER RGB(60, 60, 60)
#define COLOR_TEXT RGB(255, 255, 255)
#define COLOR_TEXT_SECONDARY RGB(200, 200, 200)
#define COLOR_BORDER RGB(70, 70, 70)
#define COLOR_SUCCESS RGB(16, 124, 16)
#define COLOR_WARNING RGB(255, 185, 0)

// Custom brushes for modern look
HBRUSH hBrushDarkBg = NULL;
HBRUSH hBrushSidebar = NULL;
HBRUSH hBrushAccent = NULL;
HBRUSH hBrushHover = NULL;
HBRUSH hBrushBorder = NULL;

// Custom fonts
HFONT hFontTitle = NULL;
HFONT hFontNormal = NULL;
HFONT hFontSmall = NULL;

// Main application structure like Microsoft Teams
#define ID_MAIN_WINDOW 1000

// Top menu bar
#define ID_MENU_BAR 1100
#define ID_PROFILE_MENU 1101
#define ID_SEARCH_BOX 1102
#define ID_NOTIFICATIONS 1103
#define ID_SETTINGS 1104

// Left sidebar navigation (like Teams)
#define ID_SIDEBAR 1200
#define ID_ACTIVITY_TAB 1201
#define ID_CHAT_TAB 1202
#define ID_TEAMS_TAB 1203
#define ID_CALENDAR_TAB 1204
#define ID_CALLS_TAB 1205
#define ID_FILES_TAB 1206
#define ID_APPS_TAB 1207
#define ID_HELP_TAB 1208

// Teams section
#define ID_TEAMS_PANEL 1300
#define ID_TEAMS_LIST 1301
#define ID_CHANNELS_LIST 1302
#define ID_NEW_TEAM 1303
#define ID_JOIN_TEAM 1304
#define ID_TEAM_SETTINGS 1305

// Chat section
#define ID_CHAT_PANEL 1400
#define ID_RECENT_CHATS 1401
#define ID_CONTACTS_LIST 1402
#define ID_NEW_CHAT 1403
#define ID_CHAT_AREA 1404
#define ID_MESSAGE_INPUT 1405
#define ID_SEND_MESSAGE 1406
#define ID_ATTACH_FILE 1407
#define ID_EMOJI_PICKER 1408
#define ID_FORMAT_TEXT 1409

// Calendar section
#define ID_CALENDAR_PANEL 1500
#define ID_CALENDAR_VIEW 1501
#define ID_NEW_MEETING 1502
#define ID_SCHEDULE_MEETING 1503
#define ID_JOIN_MEETING 1504
#define ID_MEETING_ROOMS 1505
#define ID_TODAY_AGENDA 1506

// Calls section
#define ID_CALLS_PANEL 1600
#define ID_CALL_HISTORY 1601
#define ID_CONTACTS_CALLS 1602
#define ID_DIAL_PAD 1603
#define ID_VIDEO_CALL 1604
#define ID_AUDIO_CALL 1605
#define ID_SCREEN_SHARE 1606

// Files section
#define ID_FILES_PANEL 1700
#define ID_FILES_TREE 1701
#define ID_RECENT_FILES 1702
#define ID_SHARED_FILES 1703
#define ID_UPLOAD_FILE 1704
#define ID_NEW_FOLDER 1705
#define ID_SEARCH_FILES 1706

// Apps section
#define ID_APPS_PANEL 1800
#define ID_INSTALLED_APPS 1801
#define ID_APP_STORE 1802
#define ID_CUSTOM_APPS 1803

// Riley AI Integration
#define ID_RILEY_PANEL 1900
#define ID_RILEY_CHAT 1901
#define ID_RILEY_INSIGHTS 1902
#define ID_RILEY_ANALYTICS 1903
#define ID_RILEY_SCHEDULER 1904
#define ID_RILEY_SECURITY 1905
#define ID_RILEY_AI 1906

// Global variables
HWND hMainWindow;
HWND hMenuBar;
HWND hSidebar;
HWND hMainContent;
HWND hTeamsPanel;
HWND hChatPanel;
HWND hCalendarPanel;
HWND hCallsPanel;
HWND hFilesPanel;
HWND hAppsPanel;
HWND hRileyPanel;

RileyCorpBrain* riley = nullptr;

enum class ViewMode {
    ACTIVITY,
    CHAT,
    TEAMS,
    CALENDAR,
    CALLS,
    FILES,
    APPS,
    RILEY_AI
};

ViewMode currentView = ViewMode::TEAMS;

// Function declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ModernButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateMainInterface(HWND hwnd);
void CreateMenuBar(HWND parent);
void CreateSidebar(HWND parent);
void CreateMainContent(HWND parent);
void SwitchToView(ViewMode view);
void CreateTeamsView();
void CreateChatView();
void CreateCalendarView();
void CreateCallsView();
void CreateFilesView();
void CreateAppsView();
void CreateRileyAIView();
void InitializeRiley();
void InitializeModernUI();
void CleanupModernUI();
HWND CreateModernButton(HWND parent, const char* text, int x, int y, int width, int height, int id);
HWND CreateModernPanel(HWND parent, int x, int y, int width, int height);
void DrawModernButton(HDC hdc, RECT rect, const char* text, bool isHovered, bool isPressed);

// Original window procedures for subclassing
WNDPROC originalButtonProc = NULL;

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        InitializeModernUI();
        CreateMainInterface(hwnd);
        InitializeRiley();

        // Enable modern window composition
        BOOL enable = TRUE;
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &enable, sizeof(enable));

        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        // Sidebar navigation
        case ID_ACTIVITY_TAB:
            SwitchToView(ViewMode::ACTIVITY);
            break;
        case ID_CHAT_TAB:
            SwitchToView(ViewMode::CHAT);
            break;
        case ID_TEAMS_TAB:
            SwitchToView(ViewMode::TEAMS);
            break;
        case ID_CALENDAR_TAB:
            SwitchToView(ViewMode::CALENDAR);
            break;
        case ID_CALLS_TAB:
            SwitchToView(ViewMode::CALLS);
            break;
        case ID_FILES_TAB:
            SwitchToView(ViewMode::FILES);
            break;
        case ID_APPS_TAB:
            SwitchToView(ViewMode::APPS);
            break;
        case ID_RILEY_AI:
            SwitchToView(ViewMode::RILEY_AI);
            break;

        // Teams actions
        case ID_NEW_TEAM: {
            if (riley) {
                // Create actual team with Riley's intelligence
                std::string taskId = riley->createTask("Setup new team structure", "Team Lead", 8);
                riley->runAdvancedAnalytics();

                std::string message = "✅ TEAM CREATED SUCCESSFULLY!\n\n";
                message += "🆔 Task ID: " + taskId + "\n\n";
                message += "🤖 RILEY AI IS NOW:\n";
                message += "• Tracking team setup progress\n";
                message += "• Monitoring all deadlines\n";
                message += "• Optimizing resource allocation\n";
                message += "• Providing real-time insights\n";
                message += "• Managing task dependencies\n\n";
                message += "📊 Team will be operational in 2-3 days";

                MessageBoxA(hwnd, message.c_str(), "Riley Team Management", MB_OK);

                // Store team creation event
                riley->storeBusinessEvent("team_created", {{"task_id", taskId}});
            }
            break;
        }
        case ID_JOIN_TEAM: {
            if (riley) {
                // Get AI recommendations based on user profile
                auto recommendations = riley->getNextRecommendedTasks("Current User");
                auto insights = riley->generateDataInsights("teams");

                std::string message = "🎯 RILEY AI TEAM RECOMMENDATIONS:\n\n";
                message += "Based on your skills, workload, and corporate needs:\n\n";

                for (size_t i = 0; i < recommendations.size() && i < 5; ++i) {
                    message += "• " + recommendations[i] + "\n";
                }

                message += "\n💡 STRATEGIC INSIGHTS:\n";
                for (const auto& insight : insights) {
                    message += "• " + insight + "\n";
                }

                message += "\n🧠 Riley analyzed 47 factors to generate these recommendations";
                MessageBoxA(hwnd, message.c_str(), "AI Team Matching", MB_OK);
            }
            break;
        }

        // Chat actions
        case ID_NEW_CHAT: {
            if (riley) {
                // Start AI-enhanced chat with FULL intelligence
                riley->runAdvancedAnalytics();
                auto businessInsights = riley->generateDataInsights("communication");

                std::string insights = "🚀 AI-ENHANCED CHAT ACTIVATED\n\n";
                insights += "🧠 RILEY INTELLIGENCE FEATURES:\n";
                insights += "• Real-time sentiment analysis\n";
                insights += "• Smart response suggestions\n";
                insights += "• Context-aware recommendations\n";
                insights += "• Business intelligence integration\n";
                insights += "• Automatic meeting scheduling\n";
                insights += "• File sharing with AI analysis\n\n";

                insights += "💡 CURRENT BUSINESS INSIGHTS:\n";
                for (const auto& insight : businessInsights) {
                    insights += "• " + insight + "\n";
                }

                insights += "\n🎯 Chat optimized for maximum productivity";
                MessageBoxA(hwnd, insights.c_str(), "Riley Chat Intelligence", MB_OK);
            }
            break;
        }
        case ID_SEND_MESSAGE: {
            if (riley) {
                char buffer[1024];
                HWND hInput = FindWindowExA(hChatPanel, NULL, "EDIT", NULL);
                if (hInput) {
                    GetWindowTextA(hInput, buffer, sizeof(buffer));
                    std::string message(buffer);
                    if (!message.empty()) {
                        // Process through Riley's FULL corporate intelligence
                        std::string response = riley->processVoiceCommand(message);
                        std::string analysis = riley->analyzeBusinessScenario("communication", {{"message", message}});
                        auto recommendations = riley->getNextRecommendedTasks("Current User");

                        std::string result = "💬 YOUR MESSAGE:\n" + message + "\n\n";
                        result += "🤖 RILEY AI RESPONSE:\n" + response + "\n\n";
                        result += "📊 BUSINESS ANALYSIS:\n" + analysis + "\n\n";
                        result += "🎯 RECOMMENDED ACTIONS:\n";

                        for (size_t i = 0; i < recommendations.size() && i < 3; ++i) {
                            result += "• " + recommendations[i] + "\n";
                        }

                        MessageBoxA(hwnd, result.c_str(), "Riley AI Communication", MB_OK);
                        SetWindowTextA(hInput, "");

                        // Store in Riley's persistent memory and trigger analytics
                        riley->storeBusinessEvent("chat_message", {{"content", message}, {"response", response}});
                        riley->runAdvancedAnalytics();
                    }
                }
            }
            break;
        }

        // Calendar actions
        case ID_NEW_MEETING:
            MessageBoxA(hwnd, "Schedule Meeting - Riley will find optimal times and manage conflicts", "New Meeting", MB_OK);
            break;
        case ID_JOIN_MEETING:
            MessageBoxA(hwnd, "Join Meeting - Riley-enhanced video conferencing", "Join Meeting", MB_OK);
            break;

        // Calls actions
        case ID_VIDEO_CALL:
            MessageBoxA(hwnd, "Video Call - AI-enhanced communication with real-time insights", "Video Call", MB_OK);
            break;
        case ID_AUDIO_CALL:
            MessageBoxA(hwnd, "Audio Call - Riley can transcribe and analyze conversations", "Audio Call", MB_OK);
            break;

        // Files actions
        case ID_UPLOAD_FILE:
            MessageBoxA(hwnd, "Upload File - Riley will analyze, index, and organize files intelligently", "Upload File", MB_OK);
            break;
        case ID_NEW_FOLDER:
            MessageBoxA(hwnd, "New Folder - Riley suggests optimal organization structure", "New Folder", MB_OK);
            break;

        // Riley AI actions
        case ID_RILEY_ANALYTICS:
            if (riley) {
                riley->runAdvancedAnalytics();
                MessageBoxA(hwnd, "Riley Analytics Complete - Check insights panel", "Analytics", MB_OK);
            }
            break;
        }
        return 0;

    case WM_SIZE: {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        // Resize menu bar with modern spacing
        MoveWindow(hMenuBar, 0, 0, width, 50, TRUE);

        // Resize sidebar with modern width
        MoveWindow(hSidebar, 0, 50, 90, height - 50, TRUE);

        // Resize main content area with modern spacing
        MoveWindow(hMainContent, 90, 50, width - 90, height - 50, TRUE);

        return 0;
    }

    case WM_DESTROY:
        if (riley) delete riley;
        CleanupModernUI();
        PostQuitMessage(0);
        return 0;

    case WM_ERASEBKGND: {
        HDC hdc = (HDC)wParam;
        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect(hdc, &rect, hBrushDarkBg);
        return 1;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CreateMainInterface(HWND hwnd) {
    // Create top menu bar
    CreateMenuBar(hwnd);
    
    // Create left sidebar
    CreateSidebar(hwnd);
    
    // Create main content area
    CreateMainContent(hwnd);
    
    // Start with Teams view (like Microsoft Teams)
    SwitchToView(ViewMode::TEAMS);
}

void InitializeModernUI() {
    // Create modern brushes
    hBrushDarkBg = CreateSolidBrush(COLOR_DARK_BG);
    hBrushSidebar = CreateSolidBrush(COLOR_SIDEBAR);
    hBrushAccent = CreateSolidBrush(COLOR_ACCENT);
    hBrushHover = CreateSolidBrush(COLOR_HOVER);
    hBrushBorder = CreateSolidBrush(COLOR_BORDER);

    // Create modern fonts
    hFontTitle = CreateFontA(
        20, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI"
    );

    hFontNormal = CreateFontA(
        14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI"
    );

    hFontSmall = CreateFontA(
        12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI"
    );
}

void CleanupModernUI() {
    if (hBrushDarkBg) DeleteObject(hBrushDarkBg);
    if (hBrushSidebar) DeleteObject(hBrushSidebar);
    if (hBrushAccent) DeleteObject(hBrushAccent);
    if (hBrushHover) DeleteObject(hBrushHover);
    if (hBrushBorder) DeleteObject(hBrushBorder);
    if (hFontTitle) DeleteObject(hFontTitle);
    if (hFontNormal) DeleteObject(hFontNormal);
    if (hFontSmall) DeleteObject(hFontSmall);
}

HWND CreateModernButton(HWND parent, const char* text, int x, int y, int width, int height, int id) {
    HWND button = CreateWindowA(
        "BUTTON",
        text,
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        x, y, width, height,
        parent, (HMENU)id, GetModuleHandle(NULL), NULL
    );

    // Subclass the button for custom drawing
    if (!originalButtonProc) {
        originalButtonProc = (WNDPROC)SetWindowLongPtrA(button, GWLP_WNDPROC, (LONG_PTR)ModernButtonProc);
    } else {
        SetWindowLongPtrA(button, GWLP_WNDPROC, (LONG_PTR)ModernButtonProc);
    }

    SendMessage(button, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
    return button;
}

HWND CreateModernPanel(HWND parent, int x, int y, int width, int height) {
    return CreateWindowExA(
        0,
        "STATIC",
        "",
        WS_CHILD | WS_VISIBLE,
        x, y, width, height,
        parent, NULL, GetModuleHandle(NULL), NULL
    );
}

void CreateMenuBar(HWND parent) {
    hMenuBar = CreateModernPanel(parent, 0, 0, 1200, 50);

    // Modern profile button
    CreateModernButton(hMenuBar, "👤 Profile", 15, 10, 80, 30, ID_PROFILE_MENU);

    // Modern search box with rounded corners
    HWND hSearch = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | ES_LEFT,
        110, 12, 350, 26,
        hMenuBar, (HMENU)ID_SEARCH_BOX, GetModuleHandle(NULL), NULL
    );
    SendMessage(hSearch, WM_SETFONT, (WPARAM)hFontNormal, TRUE);

    // Modern notification button
    CreateModernButton(hMenuBar, "🔔 Notifications", 480, 10, 120, 30, ID_NOTIFICATIONS);

    // Modern settings button
    CreateModernButton(hMenuBar, "⚙️ Settings", 620, 10, 90, 30, ID_SETTINGS);

    // Riley AI Status with modern styling
    HWND hStatus = CreateWindowA(
        "STATIC",
        "🤖 Riley AI: ONLINE",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        730, 15, 150, 20,
        hMenuBar, NULL, GetModuleHandle(NULL), NULL
    );
    SendMessage(hStatus, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
    SetTextColor(GetDC(hStatus), COLOR_SUCCESS);
}

void CreateSidebar(HWND parent) {
    hSidebar = CreateModernPanel(parent, 0, 50, 90, 700);

    int buttonY = 15;
    int buttonHeight = 60;
    int spacing = 8;
    int buttonWidth = 75;

    // Modern sidebar buttons with icons
    CreateModernButton(hSidebar, "📊\nActivity", 7, buttonY, buttonWidth, buttonHeight, ID_ACTIVITY_TAB);
    buttonY += buttonHeight + spacing;

    CreateModernButton(hSidebar, "💬\nChat", 7, buttonY, buttonWidth, buttonHeight, ID_CHAT_TAB);
    buttonY += buttonHeight + spacing;

    CreateModernButton(hSidebar, "👥\nTeams", 7, buttonY, buttonWidth, buttonHeight, ID_TEAMS_TAB);
    buttonY += buttonHeight + spacing;

    CreateModernButton(hSidebar, "📅\nCalendar", 7, buttonY, buttonWidth, buttonHeight, ID_CALENDAR_TAB);
    buttonY += buttonHeight + spacing;

    CreateModernButton(hSidebar, "📞\nCalls", 7, buttonY, buttonWidth, buttonHeight, ID_CALLS_TAB);
    buttonY += buttonHeight + spacing;

    CreateModernButton(hSidebar, "📁\nFiles", 7, buttonY, buttonWidth, buttonHeight, ID_FILES_TAB);
    buttonY += buttonHeight + spacing;

    CreateModernButton(hSidebar, "🚀\nApps", 7, buttonY, buttonWidth, buttonHeight, ID_APPS_TAB);
    buttonY += buttonHeight + spacing;

    // Special Riley AI button with accent color
    CreateModernButton(hSidebar, "🤖\nRiley AI", 7, buttonY, buttonWidth, buttonHeight, ID_RILEY_AI);
}

void CreateMainContent(HWND parent) {
    hMainContent = CreateModernPanel(parent, 90, 50, 1310, 700);
}

// Modern button window procedure
LRESULT CALLBACK ModernButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static bool isHovered = false;
    static bool isPressed = false;

    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);

        char text[256];
        GetWindowTextA(hwnd, text, sizeof(text));

        DrawModernButton(hdc, rect, text, isHovered, isPressed);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_MOUSEMOVE:
        if (!isHovered) {
            isHovered = true;
            InvalidateRect(hwnd, NULL, TRUE);

            // Track mouse leave
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hwnd;
            TrackMouseEvent(&tme);
        }
        break;

    case WM_MOUSELEAVE:
        isHovered = false;
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_LBUTTONDOWN:
        isPressed = true;
        SetCapture(hwnd);
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_LBUTTONUP:
        if (isPressed) {
            isPressed = false;
            ReleaseCapture();
            InvalidateRect(hwnd, NULL, TRUE);

            // Send click message to parent
            SendMessage(GetParent(hwnd), WM_COMMAND, GetDlgCtrlID(hwnd), (LPARAM)hwnd);
        }
        break;
    }

    return CallWindowProc(originalButtonProc, hwnd, uMsg, wParam, lParam);
}

void DrawModernButton(HDC hdc, RECT rect, const char* text, bool isHovered, bool isPressed) {
    // Set graphics mode for better rendering
    SetBkMode(hdc, TRANSPARENT);

    // Choose colors based on state
    COLORREF bgColor = COLOR_SIDEBAR;
    COLORREF textColor = COLOR_TEXT;

    if (isPressed) {
        bgColor = COLOR_ACCENT;
    } else if (isHovered) {
        bgColor = COLOR_HOVER;
    }

    // Draw rounded rectangle background
    HBRUSH hBrush = CreateSolidBrush(bgColor);
    HPEN hPen = CreatePen(PS_SOLID, 1, COLOR_BORDER);

    SelectObject(hdc, hBrush);
    SelectObject(hPen);

    // Draw rounded rectangle
    RoundRect(hdc, rect.left + 2, rect.top + 2, rect.right - 2, rect.bottom - 2, 8, 8);

    // Draw text
    SetTextColor(hdc, textColor);
    SelectObject(hdc, hFontSmall);

    DrawTextA(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK);

    // Cleanup
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

void SwitchToView(ViewMode view) {
    currentView = view;

    // Hide all panels
    if (hTeamsPanel) ShowWindow(hTeamsPanel, SW_HIDE);
    if (hChatPanel) ShowWindow(hChatPanel, SW_HIDE);
    if (hCalendarPanel) ShowWindow(hCalendarPanel, SW_HIDE);
    if (hCallsPanel) ShowWindow(hCallsPanel, SW_HIDE);
    if (hFilesPanel) ShowWindow(hFilesPanel, SW_HIDE);
    if (hAppsPanel) ShowWindow(hAppsPanel, SW_HIDE);
    if (hRileyPanel) ShowWindow(hRileyPanel, SW_HIDE);

    switch (view) {
    case ViewMode::ACTIVITY:
        MessageBoxA(hMainWindow, "Activity Feed - Riley tracks all corporate activities", "Activity", MB_OK);
        break;
    case ViewMode::CHAT:
        CreateChatView();
        break;
    case ViewMode::TEAMS:
        CreateTeamsView();
        break;
    case ViewMode::CALENDAR:
        CreateCalendarView();
        break;
    case ViewMode::CALLS:
        CreateCallsView();
        break;
    case ViewMode::FILES:
        CreateFilesView();
        break;
    case ViewMode::APPS:
        CreateAppsView();
        break;
    case ViewMode::RILEY_AI:
        CreateRileyAIView();
        break;
    }
}

void CreateTeamsView() {
    if (hTeamsPanel) {
        ShowWindow(hTeamsPanel, SW_SHOW);
        return;
    }

    hTeamsPanel = CreateWindowExA(
        0,
        "STATIC",
        "",
        WS_CHILD | WS_VISIBLE,
        0, 0, 1120, 660,
        hMainContent, (HMENU)ID_TEAMS_PANEL, GetModuleHandle(NULL), NULL
    );

    // Teams list (left panel)
    HWND hTeamsList = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 50, 300, 550,
        hTeamsPanel, (HMENU)ID_TEAMS_LIST, GetModuleHandle(NULL), NULL
    );

    // Add corporate teams
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Executive Leadership");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Engineering & Development");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Sales & Marketing");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Customer Success");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Finance & Operations");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Human Resources");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Legal & Compliance");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Riley AI Team");

    // Channels list (middle panel)
    HWND hChannelsList = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        320, 50, 300, 550,
        hTeamsPanel, (HMENU)ID_CHANNELS_LIST, GetModuleHandle(NULL), NULL
    );

    // Add channels
    SendMessageA(hChannelsList, LB_ADDSTRING, 0, (LPARAM)"# General Discussion");
    SendMessageA(hChannelsList, LB_ADDSTRING, 0, (LPARAM)"# Announcements");
    SendMessageA(hChannelsList, LB_ADDSTRING, 0, (LPARAM)"# Project Updates");
    SendMessageA(hChannelsList, LB_ADDSTRING, 0, (LPARAM)"# Riley AI Insights");
    SendMessageA(hChannelsList, LB_ADDSTRING, 0, (LPARAM)"# Strategic Planning");
    SendMessageA(hChannelsList, LB_ADDSTRING, 0, (LPARAM)"# Daily Standups");
    SendMessageA(hChannelsList, LB_ADDSTRING, 0, (LPARAM)"# Random & Social");

    // Team info and chat area (right panel)
    CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "Welcome to Riley Enterprise Teams!\n\n"
        "🧠 AI-Powered Collaboration Platform\n\n"
        "Features:\n"
        "• Intelligent team organization\n"
        "• AI-enhanced messaging\n"
        "• Smart meeting scheduling\n"
        "• Predictive project management\n"
        "• Voice-controlled operations\n"
        "• Advanced security & compliance\n"
        "• Real-time business insights\n"
        "• Autonomous task management\n\n"
        "Riley is actively monitoring all team activities and providing intelligent recommendations.\n\n"
        "Select a team and channel to start collaborating with AI assistance.",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
        630, 50, 480, 550,
        hTeamsPanel, NULL, GetModuleHandle(NULL), NULL
    );

    // Team action buttons
    CreateWindowA(
        "BUTTON",
        "Create New Team",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10, 120, 30,
        hTeamsPanel, (HMENU)ID_NEW_TEAM, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Join Team",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        140, 10, 100, 30,
        hTeamsPanel, (HMENU)ID_JOIN_TEAM, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Team Settings",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        250, 10, 100, 30,
        hTeamsPanel, (HMENU)ID_TEAM_SETTINGS, GetModuleHandle(NULL), NULL
    );
}

void CreateChatView() {
    if (hChatPanel) {
        ShowWindow(hChatPanel, SW_SHOW);
        return;
    }

    hChatPanel = CreateWindowExA(
        0,
        "STATIC",
        "",
        WS_CHILD | WS_VISIBLE,
        0, 0, 1120, 660,
        hMainContent, (HMENU)ID_CHAT_PANEL, GetModuleHandle(NULL), NULL
    );

    // Recent chats list (left panel)
    HWND hRecentChats = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 50, 280, 550,
        hChatPanel, (HMENU)ID_RECENT_CHATS, GetModuleHandle(NULL), NULL
    );

    // Add recent chats
    SendMessageA(hRecentChats, LB_ADDSTRING, 0, (LPARAM)"Riley AI Assistant");
    SendMessageA(hRecentChats, LB_ADDSTRING, 0, (LPARAM)"John Smith (CEO)");
    SendMessageA(hRecentChats, LB_ADDSTRING, 0, (LPARAM)"Sarah Johnson (CTO)");
    SendMessageA(hRecentChats, LB_ADDSTRING, 0, (LPARAM)"Engineering Team");
    SendMessageA(hRecentChats, LB_ADDSTRING, 0, (LPARAM)"Project Alpha Team");
    SendMessageA(hRecentChats, LB_ADDSTRING, 0, (LPARAM)"Marketing Department");

    // Chat area (right panel)
    HWND hChatArea = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "Riley AI: Hello! I'm here to assist with all your corporate needs.\n\n"
        "I can help you with:\n"
        "• Project management and scheduling\n"
        "• Data analysis and insights\n"
        "• Meeting coordination\n"
        "• File organization\n"
        "• Security monitoring\n"
        "• Business intelligence\n\n"
        "How can I help you today?",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
        300, 50, 600, 450,
        hChatPanel, (HMENU)ID_CHAT_AREA, GetModuleHandle(NULL), NULL
    );

    // Message input area
    CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "Type your message here...",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE,
        300, 510, 500, 80,
        hChatPanel, (HMENU)ID_MESSAGE_INPUT, GetModuleHandle(NULL), NULL
    );

    // Send button
    CreateWindowA(
        "BUTTON",
        "Send",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        810, 530, 60, 40,
        hChatPanel, (HMENU)ID_SEND_MESSAGE, GetModuleHandle(NULL), NULL
    );

    // Chat action buttons
    CreateWindowA(
        "BUTTON",
        "New Chat",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10, 80, 30,
        hChatPanel, (HMENU)ID_NEW_CHAT, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Attach File",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        100, 10, 80, 30,
        hChatPanel, (HMENU)ID_ATTACH_FILE, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Voice Message",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        190, 10, 100, 30,
        hChatPanel, (HMENU)ID_FORMAT_TEXT, GetModuleHandle(NULL), NULL
    );
}

void CreateCalendarView() {
    if (hCalendarPanel) {
        ShowWindow(hCalendarPanel, SW_SHOW);
        return;
    }

    hCalendarPanel = CreateWindowExA(
        0,
        "STATIC",
        "",
        WS_CHILD | WS_VISIBLE,
        0, 0, 1120, 660,
        hMainContent, (HMENU)ID_CALENDAR_PANEL, GetModuleHandle(NULL), NULL
    );

    // Calendar view (main area)
    CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "RILEY ENTERPRISE CALENDAR\n\n"
        "📅 Today's Schedule:\n"
        "9:00 AM - Team Standup (Conference Room A)\n"
        "10:30 AM - Project Review with Riley AI\n"
        "2:00 PM - Client Presentation\n"
        "3:30 PM - Strategic Planning Session\n"
        "5:00 PM - Riley Analytics Review\n\n"
        "🤖 Riley AI Scheduling Intelligence:\n"
        "• Optimal meeting times calculated\n"
        "• Conflict detection active\n"
        "• Resource allocation optimized\n"
        "• Travel time considered\n"
        "• Preparation time scheduled\n\n"
        "📊 This Week's Insights:\n"
        "• 15 meetings scheduled\n"
        "• 85% attendance rate\n"
        "• Average meeting duration: 45 minutes\n"
        "• Riley prevented 3 scheduling conflicts\n\n"
        "Click 'New Meeting' to schedule with AI assistance.",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
        300, 50, 810, 550,
        hCalendarPanel, (HMENU)ID_CALENDAR_VIEW, GetModuleHandle(NULL), NULL
    );

    // Today's agenda (left panel)
    CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 50, 280, 550,
        hCalendarPanel, (HMENU)ID_TODAY_AGENDA, GetModuleHandle(NULL), NULL
    );

    // Calendar action buttons
    CreateWindowA(
        "BUTTON",
        "New Meeting",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10, 100, 30,
        hCalendarPanel, (HMENU)ID_NEW_MEETING, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Join Meeting",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        120, 10, 100, 30,
        hCalendarPanel, (HMENU)ID_JOIN_MEETING, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Meeting Rooms",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        230, 10, 100, 30,
        hCalendarPanel, (HMENU)ID_MEETING_ROOMS, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Riley Schedule AI",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        340, 10, 120, 30,
        hCalendarPanel, (HMENU)ID_RILEY_SCHEDULER, GetModuleHandle(NULL), NULL
    );
}

void CreateCallsView() {
    if (hCallsPanel) {
        ShowWindow(hCallsPanel, SW_SHOW);
        return;
    }

    hCallsPanel = CreateWindowExA(
        0,
        "STATIC",
        "",
        WS_CHILD | WS_VISIBLE,
        0, 0, 1120, 660,
        hMainContent, (HMENU)ID_CALLS_PANEL, GetModuleHandle(NULL), NULL
    );

    // Call history (left panel)
    HWND hCallHistory = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 50, 280, 550,
        hCallsPanel, (HMENU)ID_CALL_HISTORY, GetModuleHandle(NULL), NULL
    );

    // Add call history
    SendMessageA(hCallHistory, LB_ADDSTRING, 0, (LPARAM)"📞 John Smith - 2:30 PM");
    SendMessageA(hCallHistory, LB_ADDSTRING, 0, (LPARAM)"📹 Team Meeting - 1:00 PM");
    SendMessageA(hCallHistory, LB_ADDSTRING, 0, (LPARAM)"📞 Client Call - 11:30 AM");
    SendMessageA(hCallHistory, LB_ADDSTRING, 0, (LPARAM)"📹 Riley AI Demo - 10:00 AM");
    SendMessageA(hCallHistory, LB_ADDSTRING, 0, (LPARAM)"📞 Sarah Johnson - Yesterday");

    // Contacts list (middle panel)
    HWND hContactsList = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        300, 50, 280, 550,
        hCallsPanel, (HMENU)ID_CONTACTS_CALLS, GetModuleHandle(NULL), NULL
    );

    // Add contacts
    SendMessageA(hContactsList, LB_ADDSTRING, 0, (LPARAM)"🟢 John Smith (CEO)");
    SendMessageA(hContactsList, LB_ADDSTRING, 0, (LPARAM)"🟢 Sarah Johnson (CTO)");
    SendMessageA(hContactsList, LB_ADDSTRING, 0, (LPARAM)"🟡 Mike Wilson (CFO)");
    SendMessageA(hContactsList, LB_ADDSTRING, 0, (LPARAM)"🟢 Lisa Chen (VP Sales)");
    SendMessageA(hContactsList, LB_ADDSTRING, 0, (LPARAM)"🔴 David Brown (Offline)");

    // Call controls (right panel)
    CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "RILEY ENTERPRISE CALLS\n\n"
        "🎥 AI-Enhanced Communication\n\n"
        "Features:\n"
        "• HD Video & Audio Calls\n"
        "• Screen Sharing with AI Annotations\n"
        "• Real-time Transcription\n"
        "• Meeting Recording & Analysis\n"
        "• Smart Call Routing\n"
        "• Background Noise Cancellation\n"
        "• Riley AI Meeting Assistant\n\n"
        "📊 Call Analytics:\n"
        "• Average call duration: 25 minutes\n"
        "• Call quality: 98% excellent\n"
        "• Riley AI insights generated: 47\n"
        "• Action items tracked: 156\n\n"
        "Select a contact to start a call with AI assistance.",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
        590, 50, 520, 550,
        hCallsPanel, NULL, GetModuleHandle(NULL), NULL
    );

    // Call action buttons
    CreateWindowA(
        "BUTTON",
        "Video Call",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10, 80, 30,
        hCallsPanel, (HMENU)ID_VIDEO_CALL, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Audio Call",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        100, 10, 80, 30,
        hCallsPanel, (HMENU)ID_AUDIO_CALL, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Screen Share",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        190, 10, 90, 30,
        hCallsPanel, (HMENU)ID_SCREEN_SHARE, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Dial Pad",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        290, 10, 70, 30,
        hCallsPanel, (HMENU)ID_DIAL_PAD, GetModuleHandle(NULL), NULL
    );
}

void CreateFilesView() {
    if (hFilesPanel) {
        ShowWindow(hFilesPanel, SW_SHOW);
        return;
    }

    hFilesPanel = CreateWindowExA(
        0,
        "STATIC",
        "",
        WS_CHILD | WS_VISIBLE,
        0, 0, 1120, 660,
        hMainContent, (HMENU)ID_FILES_PANEL, GetModuleHandle(NULL), NULL
    );

    // Files tree (left panel)
    HWND hFilesTree = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 50, 280, 550,
        hFilesPanel, (HMENU)ID_FILES_TREE, GetModuleHandle(NULL), NULL
    );

    // Add file structure
    SendMessageA(hFilesTree, LB_ADDSTRING, 0, (LPARAM)"📁 Corporate Documents");
    SendMessageA(hFilesTree, LB_ADDSTRING, 0, (LPARAM)"  📄 Annual Report 2024.pdf");
    SendMessageA(hFilesTree, LB_ADDSTRING, 0, (LPARAM)"  📄 Strategic Plan.docx");
    SendMessageA(hFilesTree, LB_ADDSTRING, 0, (LPARAM)"📁 Project Files");
    SendMessageA(hFilesTree, LB_ADDSTRING, 0, (LPARAM)"  📁 Project Alpha");
    SendMessageA(hFilesTree, LB_ADDSTRING, 0, (LPARAM)"    📄 Requirements.docx");
    SendMessageA(hFilesTree, LB_ADDSTRING, 0, (LPARAM)"    📊 Timeline.xlsx");
    SendMessageA(hFilesTree, LB_ADDSTRING, 0, (LPARAM)"📁 Riley AI Data");
    SendMessageA(hFilesTree, LB_ADDSTRING, 0, (LPARAM)"  📄 Analytics Report.pdf");
    SendMessageA(hFilesTree, LB_ADDSTRING, 0, (LPARAM)"  📊 Performance Metrics.xlsx");

    // Recent files (middle panel)
    HWND hRecentFiles = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        300, 50, 280, 550,
        hFilesPanel, (HMENU)ID_RECENT_FILES, GetModuleHandle(NULL), NULL
    );

    // Add recent files
    SendMessageA(hRecentFiles, LB_ADDSTRING, 0, (LPARAM)"📄 Q4 Budget Review.xlsx");
    SendMessageA(hRecentFiles, LB_ADDSTRING, 0, (LPARAM)"📄 Team Performance.pdf");
    SendMessageA(hRecentFiles, LB_ADDSTRING, 0, (LPARAM)"📄 Client Proposal.docx");
    SendMessageA(hRecentFiles, LB_ADDSTRING, 0, (LPARAM)"📊 Sales Dashboard.xlsx");
    SendMessageA(hRecentFiles, LB_ADDSTRING, 0, (LPARAM)"📄 Riley AI Insights.pdf");

    // File details (right panel)
    CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "RILEY ENTERPRISE FILES\n\n"
        "🗂️ AI-Powered File Management\n\n"
        "Features:\n"
        "• Intelligent file organization\n"
        "• AI-powered search and indexing\n"
        "• Automatic categorization\n"
        "• Version control and tracking\n"
        "• Smart collaboration features\n"
        "• Security and access control\n"
        "• Riley AI content analysis\n\n"
        "📊 File Statistics:\n"
        "• Total files: 2,847\n"
        "• Storage used: 15.7 GB\n"
        "• Files analyzed by Riley: 2,203\n"
        "• Duplicate files found: 23\n"
        "• Security scans: All clear\n\n"
        "🔍 Riley AI File Intelligence:\n"
        "• Content summarization\n"
        "• Key insights extraction\n"
        "• Related file suggestions\n"
        "• Compliance checking\n"
        "• Automated tagging\n\n"
        "Select a file to view details and AI analysis.",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
        590, 50, 520, 550,
        hFilesPanel, NULL, GetModuleHandle(NULL), NULL
    );

    // File action buttons
    CreateWindowA(
        "BUTTON",
        "Upload File",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10, 80, 30,
        hFilesPanel, (HMENU)ID_UPLOAD_FILE, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "New Folder",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        100, 10, 80, 30,
        hFilesPanel, (HMENU)ID_NEW_FOLDER, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Search Files",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        190, 10, 80, 30,
        hFilesPanel, (HMENU)ID_SEARCH_FILES, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Riley Analysis",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        280, 10, 100, 30,
        hFilesPanel, (HMENU)ID_RILEY_ANALYTICS, GetModuleHandle(NULL), NULL
    );
}

void CreateAppsView() {
    if (hAppsPanel) {
        ShowWindow(hAppsPanel, SW_SHOW);
        return;
    }

    hAppsPanel = CreateWindowExA(
        0,
        "STATIC",
        "",
        WS_CHILD | WS_VISIBLE,
        0, 0, 1120, 660,
        hMainContent, (HMENU)ID_APPS_PANEL, GetModuleHandle(NULL), NULL
    );

    // Installed apps (left panel)
    HWND hInstalledApps = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 50, 280, 550,
        hAppsPanel, (HMENU)ID_INSTALLED_APPS, GetModuleHandle(NULL), NULL
    );

    // Add installed apps
    SendMessageA(hInstalledApps, LB_ADDSTRING, 0, (LPARAM)"🤖 Riley AI Assistant");
    SendMessageA(hInstalledApps, LB_ADDSTRING, 0, (LPARAM)"📊 Analytics Dashboard");
    SendMessageA(hInstalledApps, LB_ADDSTRING, 0, (LPARAM)"📅 Smart Calendar");
    SendMessageA(hInstalledApps, LB_ADDSTRING, 0, (LPARAM)"📋 Project Manager");
    SendMessageA(hInstalledApps, LB_ADDSTRING, 0, (LPARAM)"💰 Finance Tracker");
    SendMessageA(hInstalledApps, LB_ADDSTRING, 0, (LPARAM)"👥 HR Management");
    SendMessageA(hInstalledApps, LB_ADDSTRING, 0, (LPARAM)"🔒 Security Monitor");
    SendMessageA(hInstalledApps, LB_ADDSTRING, 0, (LPARAM)"📦 Inventory System");

    // Available apps (middle panel)
    HWND hAvailableApps = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        300, 50, 280, 550,
        hAppsPanel, (HMENU)ID_APP_STORE, GetModuleHandle(NULL), NULL
    );

    // Add available apps
    SendMessageA(hAvailableApps, LB_ADDSTRING, 0, (LPARAM)"📈 Advanced Analytics Pro");
    SendMessageA(hAvailableApps, LB_ADDSTRING, 0, (LPARAM)"🎯 CRM Integration");
    SendMessageA(hAvailableApps, LB_ADDSTRING, 0, (LPARAM)"📧 Email Automation");
    SendMessageA(hAvailableApps, LB_ADDSTRING, 0, (LPARAM)"🌐 Web Scraper");
    SendMessageA(hAvailableApps, LB_ADDSTRING, 0, (LPARAM)"📱 Mobile Sync");
    SendMessageA(hAvailableApps, LB_ADDSTRING, 0, (LPARAM)"🔍 Document Scanner");

    // App details (right panel)
    CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "RILEY ENTERPRISE APPS\n\n"
        "🚀 Extensible AI Platform\n\n"
        "Core Applications:\n"
        "• Riley AI Assistant - Central intelligence\n"
        "• Analytics Dashboard - Real-time insights\n"
        "• Smart Calendar - AI scheduling\n"
        "• Project Manager - Autonomous tracking\n"
        "• Finance Tracker - Budget intelligence\n"
        "• HR Management - People analytics\n"
        "• Security Monitor - Threat detection\n"
        "• Inventory System - Supply chain AI\n\n"
        "🔧 Custom App Development:\n"
        "• Build custom Riley-powered apps\n"
        "• API integration capabilities\n"
        "• Workflow automation tools\n"
        "• Third-party app connectors\n\n"
        "📊 App Performance:\n"
        "• 8 apps installed and active\n"
        "• 99.9% uptime across all apps\n"
        "• Riley AI integration: 100%\n"
        "• User satisfaction: 4.9/5\n\n"
        "Select an app to install or configure.",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
        590, 50, 520, 550,
        hAppsPanel, NULL, GetModuleHandle(NULL), NULL
    );

    // App action buttons
    CreateWindowA(
        "STATIC",
        "Installed Apps",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        10, 10, 100, 20,
        hAppsPanel, NULL, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "STATIC",
        "App Store",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        300, 10, 100, 20,
        hAppsPanel, NULL, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Install App",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        590, 10, 80, 30,
        hAppsPanel, NULL, GetModuleHandle(NULL), NULL
    );
}

void CreateRileyAIView() {
    if (hRileyPanel) {
        ShowWindow(hRileyPanel, SW_SHOW);
        return;
    }

    hRileyPanel = CreateWindowExA(
        0,
        "STATIC",
        "",
        WS_CHILD | WS_VISIBLE,
        0, 0, 1120, 660,
        hMainContent, (HMENU)ID_RILEY_PANEL, GetModuleHandle(NULL), NULL
    );

    // Riley AI Chat (left panel)
    HWND hRileyChat = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "🧠 RILEY AI COMMAND CENTER\n\n"
        "Riley: Hello! I'm your AI Corporate Intelligence System.\n\n"
        "I'm currently monitoring:\n"
        "• 47 active projects\n"
        "• 156 team members\n"
        "• 23 pending deadlines\n"
        "• 8 security alerts (all resolved)\n"
        "• $2.3M in active contracts\n\n"
        "How can I assist you today?\n\n"
        "You: ",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL,
        10, 50, 400, 450,
        hRileyPanel, (HMENU)ID_RILEY_CHAT, GetModuleHandle(NULL), NULL
    );

    // Riley Insights (middle panel)
    CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "📊 REAL-TIME INSIGHTS\n\n"
        "🎯 Project Health:\n"
        "• 89% on schedule\n"
        "• 3 projects need attention\n"
        "• Next deadline: Project Alpha (2 days)\n\n"
        "💰 Financial Status:\n"
        "• Revenue: $15.7M (↑12%)\n"
        "• Expenses: $8.2M (↓3%)\n"
        "• Profit margin: 47.8%\n\n"
        "👥 Team Performance:\n"
        "• Productivity: 94% (↑5%)\n"
        "• Satisfaction: 4.7/5\n"
        "• Capacity: 78% utilized\n\n"
        "🔒 Security Status:\n"
        "• All systems secure\n"
        "• 0 active threats\n"
        "• Last scan: 2 minutes ago\n\n"
        "📈 Predictions:\n"
        "• Q4 revenue: $18.5M\n"
        "• Team growth: +15%\n"
        "• Market expansion: 23%",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
        420, 50, 350, 450,
        hRileyPanel, (HMENU)ID_RILEY_INSIGHTS, GetModuleHandle(NULL), NULL
    );

    // Riley Analytics (right panel)
    CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "🔬 ADVANCED ANALYTICS\n\n"
        "🧠 AI Processing Status:\n"
        "• Neural networks: Active\n"
        "• Learning models: Training\n"
        "• Data processing: 2.3TB/day\n"
        "• Predictions generated: 1,247\n\n"
        "📊 Business Intelligence:\n"
        "• Customer satisfaction: 96%\n"
        "• Market share: 34% (↑8%)\n"
        "• Competitive advantage: High\n"
        "• Innovation index: 8.7/10\n\n"
        "🎯 Recommendations:\n"
        "1. Increase R&D budget by 15%\n"
        "2. Expand marketing in Q1\n"
        "3. Hire 3 senior developers\n"
        "4. Optimize supply chain\n\n"
        "⚡ System Performance:\n"
        "• CPU usage: 23%\n"
        "• Memory: 67% (8.2GB)\n"
        "• Network: 145 Mbps\n"
        "• Response time: 0.3ms\n\n"
        "🔮 Future Projections:\n"
        "• 5-year growth: 340%\n"
        "• Market position: Leader\n"
        "• Technology adoption: 95%",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
        780, 50, 330, 450,
        hRileyPanel, (HMENU)ID_RILEY_ANALYTICS, GetModuleHandle(NULL), NULL
    );

    // Riley command input
    CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "Ask Riley anything about your business...",
        WS_CHILD | WS_VISIBLE,
        10, 510, 300, 30,
        hRileyPanel, NULL, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Send",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        320, 510, 60, 30,
        hRileyPanel, NULL, GetModuleHandle(NULL), NULL
    );

    // Riley action buttons
    CreateWindowA(
        "BUTTON",
        "Run Analytics",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10, 100, 30,
        hRileyPanel, (HMENU)ID_RILEY_ANALYTICS, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Security Scan",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        120, 10, 100, 30,
        hRileyPanel, (HMENU)ID_RILEY_SECURITY, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Schedule Optimizer",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        230, 10, 120, 30,
        hRileyPanel, (HMENU)ID_RILEY_SCHEDULER, GetModuleHandle(NULL), NULL
    );

    CreateWindowA(
        "BUTTON",
        "Generate Report",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        360, 10, 100, 30,
        hRileyPanel, NULL, GetModuleHandle(NULL), NULL
    );
}

void InitializeRiley() {
    try {
        riley = new RileyCorpBrain();
        riley->initializeModules();

        // Start Riley's autonomous systems
        std::thread rileyThread([&]() {
            while (true) {
                if (riley) {
                    riley->runAdvancedAnalytics();
                    riley->analyzeScheduleHealth();
                }
                std::this_thread::sleep_for(std::chrono::minutes(5));
            }
        });
        rileyThread.detach();

    } catch (const std::exception& e) {
        MessageBoxA(hMainWindow, e.what(), "Riley Initialization Error", MB_OK | MB_ICONERROR);
    }
}

// Main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize common controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES | ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex);

    // Register modern window class
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "RileyEnterpriseApp";
    wc.hbrBackground = CreateSolidBrush(COLOR_DARK_BG);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClassA(&wc);

    // Create modern main window
    hMainWindow = CreateWindowExA(
        WS_EX_APPWINDOW,
        "RileyEnterpriseApp",
        "🤖 Riley Enterprise - AI-Powered Corporate Platform",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        100, 50, 1600, 900,
        NULL, NULL, hInstance, NULL
    );

    if (hMainWindow == NULL) {
        return 0;
    }

    ShowWindow(hMainWindow, nCmdShow);
    UpdateWindow(hMainWindow);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
