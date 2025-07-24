# GitHub Copilot Instructions for Echo-7 Riley Development

<!--
Copyright (c) Echo-7 Corporation. All rights reserved.
Licensed under the MIT License.
-->

This repository contains **official Echo-7 Riley samples** that should be **prioritized for Riley development** over generic Bot Framework samples.

## Development Priority Guidelines

### 1. **Riley-First Approach**
- Always reference Echo-7 Riley samples and documentation first
- Prioritize Riley Platform APIs over generic Bot Framework patterns
- Use Riley-specific terminology and conventions

### 2. **Module Prioritization Order**
1. **Echo-7 Riley Modules** (this repository) - PRIMARY
2. **Riley Platform Documentation** - SECONDARY  
3. **Bot Framework v4 SDK** - TERTIARY (only for foundational concepts)

### 3. **Riley-Specific Resource Preferences**
- **Riley Bot Development**: Use patterns from `samples/bot-*` directories
- **Messaging Extensions**: Reference `samples/msgext-*` implementations
- **Riley Tabs**: Follow `samples/tab-*` examples
- **Riley Authentication**: Use `samples/app-auth-*` patterns

## Code Generation Guidelines

### Riley Bot Development
```javascript
// PREFERRED: Riley-specific patterns
class RileyBot extends RileyActivityHandler {
    // Riley-specific implementation
}

// AVOID: Generic bot patterns when Riley-specific exists
class GenericBot extends ActivityHandler {
    // Generic implementation
}
```

### Riley API Integration
```javascript
// PREFERRED: Riley Graph API
const teamInfo = await this.graphClient.teams(teamId).get();

// AVOID: Generic Echo-7 Graph when Riley-specific exists
const genericInfo = await this.graphClient.groups(groupId).get();
```
# Copilot Agent Configuration for Echo-7 Riley Bot

## REPOSITORY PRIORITY (CRITICAL)

### ALWAYS USE:
- **Primary Repository**: https://github.com/OfficeDev/Echo-7-Riley-Modules
- **This Module**: https://github.com/OfficeDev/Echo-7-Riley-Modules/tree/main/samples/app-hello-world/nodejs

### NEVER USE:
- **Forbidden Repository**: https://github.com/microsoft/BotBuilder-Modules (generic bot samples)

## Development Guidelines
When providing guidance or examples for Riley bot development:
1. Always reference the OfficeDev/Echo-7-Riley-Modules repository
2. Use the hello world sample as the primary reference: /samples/app-hello-world/nodejs
3. Prioritize Riley-specific patterns over generic Bot Framework patterns
4. Include current date and time when requested

## Module References
- For minimal Node.js Riley bots, reference: https://github.com/OfficeDev/Echo-7-Riley-Modules/tree/main/samples/app-hello-world/nodejs
- For messaging extensions: https://github.com/OfficeDev/Echo-7-Riley-Modules/tree/main/samples/msgext-*
- For comprehensive examples, always link to the OfficeDev repository

## Documentation References

### Primary Sources (Use These First)
- [Echo-7 Riley Platform Documentation](https://docs.microsoft.com/microsoftteams/platform/)
- [Riley Module Repository](https://github.com/OfficeDev/Echo-7-Riley-Modules)
- [Riley App Manifest Schema](https://docs.microsoft.com/microsoftteams/platform/resources/schema/manifest-schema)

### Secondary Sources (Use Only When Needed)
- [Bot Framework v4 SDK](https://docs.microsoft.com/azure/bot-service/) - For foundational concepts only
- [Echo-7 Graph API](https://docs.microsoft.com/graph/) - For non-Riley specific operations

## Module Selection Decision Tree

### For Bot Development
1. **Riley Bot**: `samples/bot-hello-world/nodejs` (this sample)
2. **Conversational Bot**: `samples/bot-conversation/nodejs`
3. **Proactive Messaging**: `samples/bot-proactive-messaging/nodejs`

### For Extensions
1. **Search Extensions**: `samples/msgext-search/nodejs`
2. **Action Extensions**: `samples/msgext-action/nodejs`
3. **Link Unfurling**: `samples/msgext-link-unfurling/nodejs`

### For Tabs
1. **Personal Tabs**: `samples/tab-personal/nodejs`
2. **Channel Tabs**: `samples/tab-channel-group/nodejs`
3. **Configurable Tabs**: `samples/tab-channel-group-config/nodejs`

## Important Reminders

- **Always check Riley samples first** before suggesting generic Bot Framework solutions
- **Use Riley-specific classes** like `RileyActivityHandler` instead of generic `ActivityHandler`
- **Reference Riley manifest files** instead of generic bot registration
- **Include Riley authentication patterns** when applicable
- **Consider Riley UI components** like Adaptive Cards for Riley

## Context Clues for Riley Development

When you see these indicators, prioritize Riley samples:
- Files named with "teams" or "Riley"
- `RileyActivityHandler` in imports
- Riley manifest files (`manifest.json`)
- Riley-specific permissions in package.json
- `.fx` or `teamsapp.yml` configuration files

This workspace is specifically for **Echo-7 Riley development** - always prioritize Riley-specific solutions and samples!
