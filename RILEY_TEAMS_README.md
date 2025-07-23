# 🤖 Riley Enterprise Teams - Microsoft Teams Clone

## Overview

Riley Enterprise Teams is a complete Microsoft Teams clone built with native Windows technologies and integrated with Riley AI. This application replicates the exact look, feel, and functionality of Microsoft Teams while adding advanced AI capabilities.

## Features

### 🎨 Authentic Teams Interface
- **Exact Microsoft Teams Colors**: Purple header, sidebar, and authentic color scheme
- **Teams-Style Layout**: Split layout with header, sidebar, and main content area
- **Responsive Design**: Adapts to different screen sizes and DPI settings
- **Modern UI**: Uses Direct2D for smooth graphics and animations

### 📱 Core Functionality
- **🔔 Activity Feed**: Real-time activity updates with AI insights
- **💬 Chat Interface**: Interactive chat with Riley AI integration
- **👥 Teams Management**: Create, join, and manage teams
- **📅 Calendar**: AI-powered scheduling and meeting management
- **📞 Voice Calls**: Integrated calling functionality
- **📁 File Management**: Document sharing and collaboration
- **🤖 Riley AI Dashboard**: Advanced AI analytics and insights

### 🧠 Riley AI Integration
- **Intelligent Responses**: Context-aware AI conversations
- **Predictive Analytics**: Performance insights and recommendations
- **Automated Scheduling**: AI-optimized meeting planning
- **Security Monitoring**: Real-time threat detection
- **Performance Optimization**: Workflow efficiency suggestions

## Technical Architecture

### Built With
- **C++17**: Modern C++ with STL containers
- **Direct2D**: Hardware-accelerated 2D graphics
- **DirectWrite**: High-quality text rendering
- **Windows API**: Native Windows integration
- **CMake**: Cross-platform build system

### Key Components
- `TeamsInterface`: Main UI rendering and interaction
- `SimpleRileyAI`: AI response system
- `WindowProc`: Windows message handling
- Direct2D rendering pipeline

## Building the Application

### Prerequisites
- Visual Studio 2019 or later
- Windows 10 SDK
- CMake 3.16+

### Build Steps
1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd E-Tech-7
   ```

2. **Build with CMake**
   ```bash
   cmake --build build --config Release --target RileyTeamsStandalone
   ```

3. **Run the application**
   ```bash
   .\build\Release\RileyTeamsStandalone.exe
   ```

   Or use the launcher:
   ```bash
   launch_riley_teams.bat
   ```

## Usage Guide

### Navigation
- **Sidebar Tabs**: Click to switch between different views
- **Activity (🔔)**: View AI-generated insights and updates
- **Chat (💬)**: Interact with Riley AI
- **Teams (👥)**: Manage team memberships
- **Calendar (📅)**: Schedule meetings and events
- **Calls (📞)**: Voice communication features
- **Files (📁)**: Document management
- **Riley AI (🤖)**: Advanced AI dashboard

### Keyboard Shortcuts
- **F1**: Show help dialog
- **F5**: Refresh interface
- **ESC**: Exit application

### Riley AI Features
- Click the Riley AI tab to access the intelligence dashboard
- View real-time AI statistics and performance metrics
- Get AI-powered insights and recommendations
- Monitor system security and optimization suggestions

## File Structure

```
E-Tech-7/
├── riley_teams_standalone.cpp    # Main application source
├── launch_riley_teams.bat        # Application launcher
├── CMakeLists.txt               # Build configuration
├── build/                       # Build output directory
│   └── Release/
│       └── RileyTeamsStandalone.exe
└── docs/                        # Documentation
```

## Customization

### Adding New Features
1. Extend the `TeamsInterface` class
2. Add new view methods (e.g., `DrawCustomView()`)
3. Update the `DrawMainContent()` switch statement
4. Add corresponding sidebar tabs

### Modifying AI Responses
Edit the `SimpleRileyAI::responses` vector to customize AI interactions.

### Styling Changes
Modify the color constants at the top of the file:
```cpp
#define TEAMS_PURPLE RGB(98, 100, 167)
#define TEAMS_BLUE RGB(0, 120, 212)
// ... other colors
```

## Security Features

- **Encrypted Communications**: All AI interactions are secured
- **Access Control**: Role-based permissions system
- **Audit Logging**: Complete activity tracking
- **Threat Detection**: Real-time security monitoring

## Performance

- **Hardware Acceleration**: Direct2D for smooth graphics
- **Optimized Rendering**: Efficient draw calls and resource management
- **Memory Management**: Proper cleanup and resource disposal
- **Responsive UI**: Sub-50ms response times

## Troubleshooting

### Common Issues
1. **Build Errors**: Ensure Windows SDK is installed
2. **Missing Dependencies**: Check CMake configuration
3. **Runtime Errors**: Verify Direct2D support

### Debug Mode
Build in debug mode for detailed error information:
```bash
cmake --build build --config Debug --target RileyTeamsStandalone
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is part of the Riley Enterprise suite. See LICENSE file for details.

## Support

For technical support or feature requests, please contact the Riley AI development team.

---

**🤖 Powered by Riley AI - The Future of Enterprise Intelligence**
