cmake_minimum_required(VERSION 3.22)
project(RileyCorpBrain)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fix for Qt6 with MSVC and add C++17 support
if(MSVC)
    add_compile_options(/Zc:__cplusplus /permissive- /std:c++17)
    add_compile_definitions(_USE_MATH_DEFINES)
else()
    add_compile_options(-std=c++17)
endif()

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable Qt MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Qt specific settings
set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Generated Files")

# Add core library
add_subdirectory(core)

# Create console version first (working)
add_executable(RileyCorpBrainConsole
    src/main.cpp
    src/core/app_core.cpp
)

# Link libraries for console version
target_link_libraries(RileyCorpBrainConsole
    PRIVATE
    core_build
)

# Create GUI version (experimental)
add_executable(RileyCorpBrain
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

# Link libraries for GUI version
target_link_libraries(RileyCorpBrain
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    core_build
)

# Set target properties
set_target_properties(RileyCorpBrain PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Create advanced demonstration program
add_executable(RileyDemo
    riley_demo.cpp
)

# Link libraries for demo
target_link_libraries(RileyDemo
    PRIVATE
    core_build
)

# Create Interactive GUI Window
add_executable(RileyInteractiveGUI
    riley_interactive_gui.cpp
    riley_interactive_gui.h
)

# Link libraries for Interactive GUI
target_link_libraries(RileyInteractiveGUI
    PRIVATE
    core_build
    Qt6::Core
    Qt6::Widgets
)

# Set Interactive GUI properties
set_target_properties(RileyInteractiveGUI PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Create Simple GUI Window (Working Version)
add_executable(RileyGUI
    riley_simple_gui.cpp
)

# Link libraries for Simple GUI
target_link_libraries(RileyGUI
    PRIVATE
    core_build
    Qt6::Core
    Qt6::Widgets
)

# Set Simple GUI properties
set_target_properties(RileyGUI PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Create Native Windows GUI
add_executable(RileyWindowsGUI
    riley_windows_gui.cpp
)

# Link libraries for Windows GUI
target_link_libraries(RileyWindowsGUI
    PRIVATE
    core_build
)

# Set Windows GUI properties
set_target_properties(RileyWindowsGUI PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Create the complete Riley Enterprise application (like Microsoft Teams)
add_executable(RileyEnterprise
    riley_enterprise_app.cpp
)

# Link libraries for Enterprise app
target_link_libraries(RileyEnterprise
    PRIVATE
    core_build
)

# Set Enterprise app properties
set_target_properties(RileyEnterprise PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Create the ULTRA MODERN Riley application with Direct2D graphics
add_executable(RileyUltraModern
    riley_ultra_modern.cpp
)

# Link libraries for Ultra Modern app
target_link_libraries(RileyUltraModern
    PRIVATE
    core_build
    d2d1
    dwrite
    windowscodecs
    dwmapi
    uxtheme
)

# Set Ultra Modern app properties
set_target_properties(RileyUltraModern PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Create the MICROSOFT TEAMS STYLE Riley application
add_executable(RileyTeamsStyle
    riley_teams_style.cpp
)

# Link libraries for Teams Style app
target_link_libraries(RileyTeamsStyle
    PRIVATE
    core_build
    d2d1
    dwrite
    windowscodecs
    dwmapi
    uxtheme
)

# Set Teams Style app properties
set_target_properties(RileyTeamsStyle PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Create the ULTIMATE MICROSOFT TEAMS CLONE with Riley AI
add_executable(RileyTeamsClone
    riley_teams_complete.cpp
)

# Create the STANDALONE MICROSOFT TEAMS CLONE with Riley AI
add_executable(RileyTeamsStandalone
    riley_teams_standalone.cpp
)

# Link libraries for Teams Clone
target_link_libraries(RileyTeamsClone
    PRIVATE
    core_build
    d2d1
    dwrite
    windowscodecs
    dwmapi
    uxtheme
    user32
    gdi32
    kernel32
    shell32
    ole32
    oleaut32
    uuid
    comctl32
)

# Link libraries for Teams Standalone
target_link_libraries(RileyTeamsStandalone
    PRIVATE
    d2d1
    dwrite
    windowscodecs
    dwmapi
    uxtheme
    user32
    gdi32
    kernel32
    shell32
    ole32
    oleaut32
    uuid
    comctl32
)

# Set Teams Clone properties
set_target_properties(RileyTeamsClone PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Set Teams Standalone properties
set_target_properties(RileyTeamsStandalone PROPERTIES
    WIN32_EXECUTABLE TRUE
)
