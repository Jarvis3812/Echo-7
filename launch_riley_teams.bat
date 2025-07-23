@echo off
echo.
echo ========================================
echo   🤖 Riley Enterprise Teams Launcher
echo ========================================
echo.
echo Starting Riley Teams Application...
echo.

REM Check if the executable exists
if exist "build\Release\RileyTeamsStandalone.exe" (
    echo ✅ Found Riley Teams executable
    echo 🚀 Launching Riley Enterprise Teams...
    echo.
    start "" "build\Release\RileyTeamsStandalone.exe"
    echo ✨ Riley Teams is now running!
    echo.
    echo Features available:
    echo   🔔 Activity Feed with AI insights
    echo   💬 Chat with Riley AI
    echo   👥 Teams management
    echo   📅 Calendar integration
    echo   📞 Voice calls
    echo   📁 File management
    echo   🤖 Riley AI dashboard
    echo.
    echo Press F1 in the app for help
    echo Press ESC in the app to exit
    echo.
) else (
    echo ❌ Riley Teams executable not found!
    echo.
    echo Please build the application first:
    echo   cmake --build build --config Release --target RileyTeamsStandalone
    echo.
)

pause
