#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <sstream>
#include "core/riley_corpbrain.h"

#pragma comment(lib, "comctl32.lib")

// Window controls IDs
#define ID_MAIN_MENU 2000
#define ID_SIDEBAR 2001
#define ID_TEAMS_TAB 2002
#define ID_CHAT_TAB 2003
#define ID_CALENDAR_TAB 2004
#define ID_CALLS_TAB 2005
#define ID_FILES_TAB 2006
#define ID_APPS_TAB 2007
#define ID_ACTIVITY_TAB 2008
#define ID_HELP_TAB 2009

// Content area IDs
#define ID_CONTENT_AREA 2100
#define ID_TEAMS_LIST 2101
#define ID_CHANNELS_LIST 2102
#define ID_CHAT_AREA 2103
#define ID_MESSAGE_INPUT 2104
#define ID_SEND_BUTTON 2105
#define ID_ATTACH_BUTTON 2106
#define ID_EMOJI_BUTTON 2107
#define ID_MEETING_BUTTON 2108

// Calendar IDs
#define ID_CALENDAR_VIEW 2200
#define ID_NEW_MEETING 2201
#define ID_TODAY_BUTTON 2202
#define ID_WEEK_VIEW 2203
#define ID_MONTH_VIEW 2204

// Files IDs
#define ID_FILES_TREE 2300
#define ID_UPLOAD_FILE 2301
#define ID_NEW_FOLDER 2302
#define ID_SEARCH_FILES 2303

// Calls IDs
#define ID_CALL_HISTORY 2400
#define ID_CONTACTS_LIST 2401
#define ID_DIAL_PAD 2402
#define ID_VIDEO_CALL 2403
#define ID_AUDIO_CALL 2404

// Global variables
HWND hMainWindow;
HWND hSidebar;
HWND hContentArea;
HWND hTeamsList;
HWND hChannelsList;
HWND hChatArea;
HWND hMessageInput;
HWND hCalendarView;
HWND hFilesList;
HWND hCallHistory;
RileyCorpBrain* riley = nullptr;

// Current view state
enum class ViewMode {
    TEAMS,
    CHAT,
    CALENDAR,
    CALLS,
    FILES,
    APPS,
    ACTIVITY,
    HELP
};

ViewMode currentView = ViewMode::TEAMS;

// Function declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateMainInterface(HWND hwnd);
void CreateSidebar(HWND parent);
void CreateContentArea(HWND parent);
void SwitchToView(ViewMode view);
void CreateTeamsView();
void CreateChatView();
void CreateCalendarView();
void CreateCallsView();
void CreateFilesView();
void InitializeRiley();
void AddMessageToChat(const std::string& sender, const std::string& message, const std::string& timestamp);

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        CreateMainInterface(hwnd);
        InitializeRiley();
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_TEAMS_TAB:
            SwitchToView(ViewMode::TEAMS);
            break;
        case ID_CHAT_TAB:
            SwitchToView(ViewMode::CHAT);
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
        case ID_ACTIVITY_TAB:
            SwitchToView(ViewMode::ACTIVITY);
            break;
        case ID_HELP_TAB:
            SwitchToView(ViewMode::HELP);
            break;
        case ID_SEND_BUTTON: {
            char buffer[1024];
            GetWindowTextA(hMessageInput, buffer, sizeof(buffer));
            std::string message(buffer);
            if (!message.empty()) {
                AddMessageToChat("You", message, "Now");
                SetWindowTextA(hMessageInput, "");
                
                // Process with Riley AI
                if (riley) {
                    std::string response = riley->processVoiceCommand(message);
                    AddMessageToChat("Riley AI", response, "Now");
                }
            }
            break;
        }
        case ID_NEW_MEETING:
            MessageBoxA(hwnd, "New Meeting feature - Integration with Riley's scheduling system", "New Meeting", MB_OK | MB_ICONINFORMATION);
            break;
        case ID_VIDEO_CALL:
            MessageBoxA(hwnd, "Video Call feature - Riley AI-enhanced communication", "Video Call", MB_OK | MB_ICONINFORMATION);
            break;
        case ID_UPLOAD_FILE:
            MessageBoxA(hwnd, "File Upload - Riley will analyze and index uploaded files", "Upload File", MB_OK | MB_ICONINFORMATION);
            break;
        }
        return 0;

    case WM_SIZE: {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        
        // Resize sidebar (left panel)
        MoveWindow(hSidebar, 0, 0, 80, height, TRUE);
        
        // Resize content area
        MoveWindow(hContentArea, 80, 0, width - 80, height, TRUE);
        
        return 0;
    }

    case WM_DESTROY:
        if (riley) delete riley;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CreateMainInterface(HWND hwnd) {
    // Create sidebar
    CreateSidebar(hwnd);
    
    // Create main content area
    CreateContentArea(hwnd);
    
    // Start with Teams view
    SwitchToView(ViewMode::TEAMS);
}

void CreateSidebar(HWND parent) {
    hSidebar = CreateWindowExA(
        0,
        "STATIC",
        "",
        WS_CHILD | WS_VISIBLE | SS_BLACKRECT,
        0, 0, 80, 600,
        parent, (HMENU)ID_SIDEBAR, GetModuleHandle(NULL), NULL
    );

    // Teams button
    CreateWindowA(
        "BUTTON",
        "Teams",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        5, 10, 70, 40,
        hSidebar, (HMENU)ID_TEAMS_TAB, GetModuleHandle(NULL), NULL
    );

    // Chat button
    CreateWindowA(
        "BUTTON",
        "Chat",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        5, 60, 70, 40,
        hSidebar, (HMENU)ID_CHAT_TAB, GetModuleHandle(NULL), NULL
    );

    // Calendar button
    CreateWindowA(
        "BUTTON",
        "Calendar",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        5, 110, 70, 40,
        hSidebar, (HMENU)ID_CALENDAR_TAB, GetModuleHandle(NULL), NULL
    );

    // Calls button
    CreateWindowA(
        "BUTTON",
        "Calls",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        5, 160, 70, 40,
        hSidebar, (HMENU)ID_CALLS_TAB, GetModuleHandle(NULL), NULL
    );

    // Files button
    CreateWindowA(
        "BUTTON",
        "Files",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        5, 210, 70, 40,
        hSidebar, (HMENU)ID_FILES_TAB, GetModuleHandle(NULL), NULL
    );

    // Apps button
    CreateWindowA(
        "BUTTON",
        "Apps",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        5, 260, 70, 40,
        hSidebar, (HMENU)ID_APPS_TAB, GetModuleHandle(NULL), NULL
    );

    // Activity button
    CreateWindowA(
        "BUTTON",
        "Activity",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        5, 310, 70, 40,
        hSidebar, (HMENU)ID_ACTIVITY_TAB, GetModuleHandle(NULL), NULL
    );

    // Help button
    CreateWindowA(
        "BUTTON",
        "Help",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        5, 360, 70, 40,
        hSidebar, (HMENU)ID_HELP_TAB, GetModuleHandle(NULL), NULL
    );
}

void CreateContentArea(HWND parent) {
    hContentArea = CreateWindowExA(
        0,
        "STATIC",
        "",
        WS_CHILD | WS_VISIBLE,
        80, 0, 1120, 700,
        parent, (HMENU)ID_CONTENT_AREA, GetModuleHandle(NULL), NULL
    );
}

void SwitchToView(ViewMode view) {
    currentView = view;
    
    // Hide all child windows in content area
    EnumChildWindows(hContentArea, [](HWND hwnd, LPARAM lParam) -> BOOL {
        ShowWindow(hwnd, SW_HIDE);
        return TRUE;
    }, 0);

    switch (view) {
    case ViewMode::TEAMS:
        CreateTeamsView();
        break;
    case ViewMode::CHAT:
        CreateChatView();
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
        MessageBoxA(hMainWindow, "Apps integration with Riley's AI capabilities", "Apps", MB_OK);
        break;
    case ViewMode::ACTIVITY:
        MessageBoxA(hMainWindow, "Activity feed powered by Riley's intelligence", "Activity", MB_OK);
        break;
    case ViewMode::HELP:
        MessageBoxA(hMainWindow, "Riley AI Assistant Help System", "Help", MB_OK);
        break;
    }
}

void CreateTeamsView() {
    // Teams list (left panel)
    hTeamsList = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 10, 250, 600,
        hContentArea, (HMENU)ID_TEAMS_LIST, GetModuleHandle(NULL), NULL
    );

    // Add sample teams
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Executive Team");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Engineering");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Sales & Marketing");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Customer Support");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Finance");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Human Resources");
    SendMessageA(hTeamsList, LB_ADDSTRING, 0, (LPARAM)"Riley AI Team");

    // Channels list (middle panel)
    hChannelsList = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "LISTBOX",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        270, 10, 250, 600,
        hContentArea, (HMENU)ID_CHANNELS_LIST, GetModuleHandle(NULL), NULL
    );

    // Add sample channels
    SendMessageA(hChannelsList, LB_ADDSTRING, 0, (LPARAM)"# General");
    SendMessageA(hChannelsList, LB_ADDSTRING, 0, (LPARAM)"# Announcements");
    SendMessageA(hChannelsList, LB_ADDSTRING, 0, (LPARAM)"# Project Updates");
    SendMessageA(hChannelsList, LB_ADDSTRING, 0, (LPARAM)"# Riley AI Insights");
    SendMessageA(hChannelsList, LB_ADDSTRING, 0, (LPARAM)"# Random");

    // Team info panel (right panel)
    CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "Welcome to Riley Corpbrain Teams!\n\nThis is your AI-powered collaboration platform.\n\nFeatures:\n- AI-enhanced messaging\n- Intelligent meeting scheduling\n- Predictive project management\n- Voice-controlled operations\n- Advanced security\n- Real-time business insights\n\nSelect a team and channel to start collaborating with Riley's intelligence.",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
        530, 10, 580, 600,
        hContentArea, NULL, GetModuleHandle(NULL), NULL
    );
}
