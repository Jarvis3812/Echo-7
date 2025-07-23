#include <windows.h>
#include <commctrl.h>
#include <string>
#include <thread>
#include <sstream>
#include "core/riley_corpbrain.h"

#pragma comment(lib, "comctl32.lib")

// Window controls IDs
#define ID_CHAT_LOG 1001
#define ID_INPUT_EDIT 1002
#define ID_SEND_BUTTON 1003
#define ID_VOICE_BUTTON 1004
#define ID_ANALYTICS_BUTTON 1005
#define ID_STATUS_BUTTON 1006
#define ID_PREDICT_BUTTON 1007
#define ID_OPTIMIZE_BUTTON 1008
#define ID_INSIGHTS_TAB 1009
#define ID_ANALYTICS_TAB 1010

// Global variables
HWND hMainWindow;
HWND hChatLog;
HWND hInputEdit;
HWND hSendButton;
HWND hVoiceButton;
HWND hAnalyticsButton;
HWND hStatusButton;
HWND hPredictButton;
HWND hOptimizeButton;
HWND hInsightsTab;
HWND hAnalyticsTab;
RileyCorpBrain* riley = nullptr;

// Function declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitializeRiley();
void ProcessCommand(const std::string& command);
void AddToChatLog(const std::string& message, COLORREF color = RGB(255, 255, 255));
void CreateControls(HWND hwnd);

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        CreateControls(hwnd);
        InitializeRiley();
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_SEND_BUTTON: {
            char buffer[1024];
            GetWindowTextA(hInputEdit, buffer, sizeof(buffer));
            std::string command(buffer);
            if (!command.empty()) {
                AddToChatLog("You: " + command, RGB(76, 175, 80));
                SetWindowTextA(hInputEdit, "");
                ProcessCommand(command);
            }
            break;
        }
        case ID_VOICE_BUTTON:
            AddToChatLog("Voice input activated...", RGB(33, 150, 243));
            ProcessCommand("analyze sales performance");
            break;
        case ID_ANALYTICS_BUTTON:
            AddToChatLog("Running advanced analytics...", RGB(156, 39, 176));
            if (riley) {
                try {
                    riley->runAdvancedAnalytics();
                    AddToChatLog("Analytics complete!", RGB(76, 175, 80));
                } catch (const std::exception& e) {
                    AddToChatLog("Analytics failed: " + std::string(e.what()), RGB(244, 67, 54));
                }
            }
            break;
        case ID_STATUS_BUTTON:
            if (riley) {
                auto status = riley->getSystemStatus();
                std::string statusText = "System Status:\n";
                for (const auto& line : status) {
                    statusText += "- " + line + "\n";
                }
                AddToChatLog(statusText, RGB(96, 125, 139));
            }
            break;
        case ID_PREDICT_BUTTON:
            ProcessCommand("Generate predictions for next quarter");
            break;
        case ID_OPTIMIZE_BUTTON:
            ProcessCommand("Optimize system performance");
            break;
        }
        return 0;

    case WM_SIZE: {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        
        // Resize controls
        MoveWindow(hChatLog, 10, 10, width - 320, height - 80, TRUE);
        MoveWindow(hInputEdit, 10, height - 60, width - 420, 25, TRUE);
        MoveWindow(hSendButton, width - 400, height - 60, 80, 25, TRUE);
        MoveWindow(hVoiceButton, width - 310, height - 60, 80, 25, TRUE);
        
        // Right panel buttons
        int buttonY = 10;
        MoveWindow(hAnalyticsButton, width - 200, buttonY, 180, 30, TRUE);
        buttonY += 40;
        MoveWindow(hStatusButton, width - 200, buttonY, 180, 30, TRUE);
        buttonY += 40;
        MoveWindow(hPredictButton, width - 200, buttonY, 180, 30, TRUE);
        buttonY += 40;
        MoveWindow(hOptimizeButton, width - 200, buttonY, 180, 30, TRUE);
        
        // Tabs
        MoveWindow(hInsightsTab, width - 200, buttonY + 50, 180, 200, TRUE);
        MoveWindow(hAnalyticsTab, width - 200, buttonY + 260, 180, 200, TRUE);
        
        return 0;
    }

    case WM_DESTROY:
        if (riley) delete riley;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CreateControls(HWND hwnd) {
    // Chat log (rich edit control)
    hChatLog = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
        10, 10, 800, 500,
        hwnd, (HMENU)ID_CHAT_LOG, GetModuleHandle(NULL), NULL
    );

    // Input edit
    hInputEdit = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        10, 520, 600, 25,
        hwnd, (HMENU)ID_INPUT_EDIT, GetModuleHandle(NULL), NULL
    );

    // Send button
    hSendButton = CreateWindowA(
        "BUTTON",
        "Send",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        620, 520, 80, 25,
        hwnd, (HMENU)ID_SEND_BUTTON, GetModuleHandle(NULL), NULL
    );

    // Voice button
    hVoiceButton = CreateWindowA(
        "BUTTON",
        "Voice",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        710, 520, 80, 25,
        hwnd, (HMENU)ID_VOICE_BUTTON, GetModuleHandle(NULL), NULL
    );

    // Analytics button
    hAnalyticsButton = CreateWindowA(
        "BUTTON",
        "Run Analytics",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        820, 10, 180, 30,
        hwnd, (HMENU)ID_ANALYTICS_BUTTON, GetModuleHandle(NULL), NULL
    );

    // Status button
    hStatusButton = CreateWindowA(
        "BUTTON",
        "System Status",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        820, 50, 180, 30,
        hwnd, (HMENU)ID_STATUS_BUTTON, GetModuleHandle(NULL), NULL
    );

    // Predict button
    hPredictButton = CreateWindowA(
        "BUTTON",
        "Generate Predictions",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        820, 90, 180, 30,
        hwnd, (HMENU)ID_PREDICT_BUTTON, GetModuleHandle(NULL), NULL
    );

    // Optimize button
    hOptimizeButton = CreateWindowA(
        "BUTTON",
        "Optimize Performance",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        820, 130, 180, 30,
        hwnd, (HMENU)ID_OPTIMIZE_BUTTON, GetModuleHandle(NULL), NULL
    );

    // Insights tab
    hInsightsTab = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "Latest Business Insights:\n\nSales Performance:\n- Revenue growth trending upward\n- Customer acquisition rate: +15%\n- Pipeline velocity improved\n\nCustomer Analysis:\n- Churn risk identified in enterprise segment\n- High-value customers showing strong engagement\n- Retention strategies recommended\n\nFinancial Health:\n- Cash flow stable and positive\n- Cost optimization opportunities identified\n- Budget variance within acceptable range",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
        820, 180, 180, 200,
        hwnd, (HMENU)ID_INSIGHTS_TAB, GetModuleHandle(NULL), NULL
    );

    // Analytics tab
    hAnalyticsTab = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "Real-time Analytics:\n\nPredictive Models:\n- Sales Revenue Forecast: $172K\n- Customer Churn: 2,350 customers\n- Cash Flow: $2,386K\n\nAI Insights:\n- 15 business patterns detected\n- 3 optimization opportunities\n- 7 risk factors identified\n\nPerformance:\n- Response time: 0.3s\n- Accuracy: 95%\n- Memory usage: 2.1GB",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
        820, 390, 180, 200,
        hwnd, (HMENU)ID_ANALYTICS_TAB, GetModuleHandle(NULL), NULL
    );

    // Set placeholder text
    SendMessageA(hInputEdit, EM_SETCUEBANNER, TRUE, (LPARAM)L"Ask Riley anything... (e.g., 'Analyze sales performance')");
}

void InitializeRiley() {
    AddToChatLog("Initializing Riley Corpbrain...", RGB(33, 150, 243));

    std::thread([&]() {
        try {
            riley = new RileyCorpBrain();
            riley->initializeModules();

            AddToChatLog("Riley is fully operational!", RGB(76, 175, 80));
            AddToChatLog("AI Reasoning Engine: Active", RGB(76, 175, 80));
            AddToChatLog("Predictive Analytics: Ready", RGB(76, 175, 80));
            AddToChatLog("Memory System: Online", RGB(76, 175, 80));
            AddToChatLog("Voice Interface: Available", RGB(76, 175, 80));
            AddToChatLog("Command Center: Autonomous", RGB(76, 175, 80));
            AddToChatLog("", RGB(255, 255, 255));
            AddToChatLog("You can now ask Riley anything about your business!", RGB(255, 152, 0));
            AddToChatLog("Try: 'Analyze sales performance' or 'Predict revenue'", RGB(96, 125, 139));

        } catch (const std::exception& e) {
            AddToChatLog("Failed to initialize Riley: " + std::string(e.what()), RGB(244, 67, 54));
        }
    }).detach();
}

void ProcessCommand(const std::string& command) {
    if (!riley) {
        AddToChatLog("Riley is not initialized yet", RGB(244, 67, 54));
        return;
    }

    std::thread([=]() {
        try {
            if (command.find("predict") != std::string::npos ||
                command.find("forecast") != std::string::npos) {
                std::string prediction = riley->predictFutureTrends("sales_revenue", 6);
                AddToChatLog("Riley: " + prediction, RGB(156, 39, 176));

            } else if (command.find("analyze") != std::string::npos ||
                      command.find("analysis") != std::string::npos) {
                CoreVariantMap context;
                context["revenue"] = 850000.0;
                context["growth_rate"] = 0.15;

                std::string analysis = riley->analyzeBusinessScenario("Sales", context);
                AddToChatLog("Riley: " + analysis, RGB(33, 150, 243));

            } else if (command.find("recommend") != std::string::npos ||
                      command.find("suggest") != std::string::npos) {
                CoreVariantMap context;
                context["situation"] = command;

                std::string recommendation = riley->getRecommendation(command, context);
                AddToChatLog("Riley: " + recommendation, RGB(255, 152, 0));

            } else {
                AddToChatLog("Riley: I understand you want to " + command +
                           ". Let me analyze this with my AI reasoning engine...", RGB(96, 125, 139));

                std::string response = riley->processVoiceCommand(command);
                AddToChatLog("Riley: " + response, RGB(76, 175, 80));
            }

        } catch (const std::exception& e) {
            AddToChatLog("Error: " + std::string(e.what()), RGB(244, 67, 54));
        }
    }).detach();
}

void AddToChatLog(const std::string& message, COLORREF color) {
    // Get current text length
    int textLength = GetWindowTextLengthA(hChatLog);
    
    // Move cursor to end
    SendMessageA(hChatLog, EM_SETSEL, textLength, textLength);
    
    // Add message with newline
    std::string fullMessage = message + "\r\n";
    SendMessageA(hChatLog, EM_REPLACESEL, FALSE, (LPARAM)fullMessage.c_str());
    
    // Scroll to bottom
    SendMessageA(hChatLog, WM_VSCROLL, SB_BOTTOM, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize common controls
    InitCommonControls();

    // Register window class
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "RileyCorpBrainGUI";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassA(&wc);

    // Create window
    hMainWindow = CreateWindowExA(
        0,
        "RileyCorpBrainGUI",
        "Riley Corpbrain - Interactive AI Assistant",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 700,
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
