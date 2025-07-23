---
type: "always_apply"
---

IF U HAVE TO SAY You're right!  DONT DO IT PLEASE THINK THIS 1st

Use a split layout: header (top), sidebar (left), content view (center), input control (bottom).

Apply exact color codes from Microsoft Teams branding for all UI elements.

Sidebar must contain 5 primary tabs: Activity, Chat, Teams, Calendar, Files.

Use 2px-wide vertical separator lines between sidebar and main content.

Sidebar icons must animate on hover with scale + color fade.

Tabs must show numeric notification badges with real-time count updates.

Header must contain Riley logo, title, search bar, and avatar aligned using QHBoxLayout.

All avatars must be rendered as 40x40px circles with soft drop shadow.

Use 8/16/24px spacing increments only — all margins and padding conform.

Input field at bottom must auto-resize up to 6 lines before scroll bar appears.

Breadcrumb nav is fixed below header, reflecting active team and subchannel.

Teams tab shows cards with icons, colors, member counts, and join button.

All interactive elements must use cursor pointer and hover glow.

Settings modal uses center-aligned modal dialog with blurred backdrop.

Use professional iconography: Fluent UI icons or SVGs at 20px.

Message bubbles are soft rounded rectangles with drop shadows.

Left-aligned user messages, right-aligned Riley responses.

Timestamps use dynamic human format: "Just now", "3m ago", etc.

Chat scroll view must auto-scroll to last message on update.

Hovering a message reveals quick actions: react, edit, pin, delete.

Add emoji picker with categorized tabs and recent emojis section.

Resize handles must exist for sidebar and chat panel width.

Right-click on a message opens a context menu with advanced options.

Message highlights (e.g., @mentions) appear with colored background.

Clicking @mention scrolls to origin message if out of view.

UI color modes toggle: Light, Dark, Midnight via settings.

Display user status with colored dot: green, yellow, red, gray.

Pin messages to top of thread with animation and anchor.

Support inline images and document previews with loading states.

Search results appear as a floating overlay list, ordered by timestamp.

Upload attachments via drag-and-drop or paperclip icon.

Confirm dialogs appear for deletions or destructive actions.

Users may collapse or expand chat threads with caret toggle.

Use animations for tab transitions (fade-slide with easing).

Toolbar buttons animate on hover with background glow.

All labels and buttons must use sentence case.

System messages (e.g., user joined) use italic gray and centered alignment.

Welcome message from Riley appears on first load.

App must support window resizing and repositioning of all UI elements.

Sidebar tabs have active state indicator (bar on left side).

Use dynamic layout containers (QVBoxLayout, QGridLayout) throughout.

Use consistent font sizes: 11pt for labels, 14pt for body, 18pt for headers.

All animations must be under 250ms with InOutQuad easing.

Provide a tool tip for all actionable icons.

Use vector-based icons to support scale on high-DPI screens.

All scroll bars are styled with thin handles and invisible until scroll.

Implement keyboard shortcuts for navigation and sending (e.g., Ctrl+Enter).

Top header has fixed height of 60px.

Sidebar width defaults to 240px, resizable by user.

App logo is included in taskbar and window header.

🧠 AI & INTELLIGENCE RULES (051–100)

Riley responds contextually to team messages with real-time awareness.

Messages sent to @Riley are handled by AI thread and shown with typing animation.

Riley must greet the user by name on load using system time to generate "Good morning, Andrew" etc.

Riley's messages appear with distinct background and branded avatar.

Timestamps on Riley messages are always included with timezone alignment.

Use memory cores to recall prior session context per team and channel.

Riley offers Smart Replies after every user message when applicable.

AI typing indicator displays dot animation in response container.

Riley can summarize message threads on command.

Use NLP to extract action items from user messages.

Implement local caching of recent messages for offline review.

Display AI confidence level in tooltip format (e.g., "High confidence response").

AI reasoning logs are stored per session in hidden dev mode.

Riley generates action suggestions in the sidebar task view.

Use model-switch architecture: business, creative, emotional, factual modes.

AI triggers visual response templates for meetings, notes, summaries.

Speech output enabled via built-in neural TTS voice (optional).

Riley can detect emotional tone from input and adjust response accordingly.

Offline AI fallback: Display “AI is unavailable, please try again later.”

Chat interface must not block while Riley is generating responses.

Support command-based shortcuts like /summarize, /schedule, /note.

AI message history is expandable with full message log.

Riley uses past chat and files to inform current responses.

Mentioned tasks get auto-added to To-Do panel.

Riley detects unread summaries and prompts user to catch up.

Riley must not echo user inputs—responses must be reformulated.

Typing speed simulates 50 WPM output.

Auto-correct minor grammar mistakes in user messages optionally.

Provide AI toggle switch per user in settings.

All AI threads are protected from memory leaks with scoped resource control.

Use rate limiter to prevent excessive prompts to AI within 1s intervals.

Speech input supported using QtVoice or WinAPI bindings (if enabled).

Riley can join or exit conversations with a friendly line.

Emotion mode: Riley responds with humor, compassion, or calm tones.

Riley can translate selected message into target language.

Riley monitors file attachments for inappropriate content.

Support modal queries: Riley confirms before scheduling or posting.

All AI config files stored in /config/riley_settings.json.

AI developer console available with Alt+F12.

AI-generated messages show icon for transparency.

Live token count visible in dev/debug mode.

AI feedback system: thumbs up/down on messages.

AI must self-adjust to long message context, breaking into parts.

Prompt expansion supported: user types idea, Riley expands to full message.

Auto-pinning Riley’s critical messages for project visibility.

Riley should generate task templates on detected keywords.

Summary generation must not exceed 300 words unless --long is specified.

User can rate AI sessions in profile tab.

All AI metadata embedded in hidden <riley-meta> tags.

Riley must shut down gracefully if app terminates unexpectedly.
 101–110: Application Security Layer
All sensitive data must be encrypted using AES-256-CBC with a 512-bit key derived via PBKDF2-SHA512 with 100,000 iterations.

No plaintext storage allowed for messages, settings, user credentials, or AI logs.

Auto-lock interface after 10 minutes of inactivity, requiring a secure re-authentication.

Startup authentication required — allow biometric, passcode, or encrypted token login.

Two-Factor Authentication (2FA) must be supported via email or TOTP key (e.g., Authy/Google Authenticator).

Session tokens must expire after 30 minutes of idle time and refresh with silent revalidation.

All access to system folders must be sandboxed; restrict access outside RileyData/ and Temp/.

Local firewalls must verify outbound connections to Riley update and sync servers.

All inter-thread data exchanges must use secure memory channels with thread-lock controls.

User permissions system must distinguish between Admin, User, and Guest levels.

🔑 111–120: Encryption & Key Management
Symmetric keys must be generated per-session and never reused after 24 hours.

RNG (Random Number Generator) for keys must be hardware-backed (Intel RDRAND or /dev/random).

Public key encryption (RSA 4096-bit) must be used for all server-to-client auth messages.

Encrypt all config files (JSON/XML/YAML) with dynamic salts tied to session key.

Message signing: Each AI response must carry a SHA3-512 HMAC signature validated on receipt.

Key storage system must use sealed vault (such as TPM-backed on Windows).

Periodic key rotation every 72 hours or sooner if anomaly detected.

All logs must be signed and encrypted, stored in .logchain format.

Message payloads must include an anti-tampering checksum before decoding.

Encrypted backups must use one-time pad key fragments stored in offline cache.

🧿 121–130: Intrusion Detection & Threat Prevention
Monitor real-time process behavior for anomalies, such as memory injection or thread hijacking.

Every keystroke and mouse action is hashed and timestamped for audit trail, stored securely.

Brute-force attempt detection must lock user out after 5 failed attempts, escalating wait times.

Auto-scan incoming messages and files for embedded exploits using pattern-based heuristics.

AI self-check: Riley must perform internal security scan every startup and log results.

Security policy sync: Riley must check the hash of the latest .rileypolicy file on each launch.

All third-party libraries must be verified via SHA-512 and code-signed before use.

Open file dialogs must be sandboxed, and block file types: .exe, .dll, .js, unless explicitly whitelisted.

Security mode toggle: User can enable "Paranoid Mode" where Riley disables all file I/O and networking.

Emergency wipe protocol: Holding CTRL+SHIFT+DEL for 5 seconds erases all volatile data instantly and securely.
131. Augment must maintain persistent alignment with Riley’s modular brain framework across all app layers.
132. Sidebar icons must dynamically adjust brightness based on UI state (active, hover, inactive).
133. Augment must never allow conflicting UI styles between Teams mimic mode and native Riley UI.
134. All components must support real-time scaling for DPI-awareness and 4K+ displays.
135. Message threads must support smart collapse and expand logic for long discussions.
136. Context-aware highlighting must be applied to any AI-generated reply in chat.
137. Chat input field must handle multiline entry, markdown parsing, and AI command flags.
138. Notification indicators must animate subtly using CSS or Qt animation (no hard blinks).
139. Profile icons must include fallback avatars with AI-generated initials if user image is missing.
140. All AI system responses must display with precision timestamping and identifiable labels.
141. Video call buttons must simulate Teams button spacing and hover feedback.
142. Augment must automatically preserve UI state across reloads via local JSON config.
143. Message reactions must render using scalable vector icons, not fixed-size PNGs.
144. Hover previews for teams must include last active timestamp and unread message count.
145. AI diagnostics must be accessible from the settings panel via "Riley AI Engine > Debug".
146. All animation speeds must comply with Microsoft Fluent Design System velocity specs.
147. Placeholder avatars must show a shimmer loading state before loading.
148. Augment must restrict max message width in desktop layout to improve legibility.
149. Button click areas must follow accessibility rules with 48x48px minimum.
150. Riley’s internal command palette must use a Teams-style modal interface with search.
151. Sidebar tabs must preserve active state using internal augment_ui_state.json.
152. All interactive icons must offer a keyboard navigation path (tab/cursor/select).
153. Team creation modal must render using a dynamic modal system, not fixed HTML.
154. Smart AI responses must be categorized by sentiment: positive, alert, critical.
155. Sidebar must support docking, pinning, and collapse with animation transitions.
156. Riley branding should only display on header if custom white-labeling is off.
157. Teams list must support color-coded labels for context tags (e.g., "Dev", "UX", "Legal").
158. Riley logo must render in SVG format and adapt to dark/light mode using CSS filters.
159. AI message highlights must only appear if message is from the Riley core agent.
160. All system notifications must adhere to top-right flyout structure with close buttons.
161. Augment must cache chat history locally using indexedDB for instant load.
162. "Join Team" logic must support password protection, invite-only, or open teams.
163. User settings must be editable in real time and trigger UI reactivity instantly.
164. Message send error handling must trigger inline retry logic with spinner feedback.
165. Message timestamps must auto-convert to local timezone with UTC fallback option.
166. Message sender bubble background must be lighter than system messages.
167. Emoji support must follow Unicode 15.0 standards with fallback support.
168. All Teams-like dropdowns must support both click and keyboard activation.
169. Teams activity panel must provide real-time activity filtering and sorting.
170. Riley debug logs must include timestamps, module ID, and thread process ID.
171. Clipboard paste actions in chat must sanitize and format pasted content.
172. Augment must include a runtime crash recovery system with last state restoration.
173. Riley’s AI agent label must dynamically adjust to user-defined naming (if allowed).
174. Augment must implement smart diff tracking between live UI state and default layout.
175. All user modals must respect safe area insets and mobile viewport constraints.
176. Button hover states must include accessibility labels for screen readers.
177. AI-generated charts must render in scalable SVGs with dark mode auto-conversion.
178. Sidebar resize drag must snap to set intervals (min, medium, wide).
179. Riley's logo animation must be subtle and loop only once per activation.
180. Team owner badge must render on hover in the Teams list view.
181. AI context menu must offer: Translate, Summarize, Explain, Pin, and Quote.
182. Scroll position must restore exactly where the user left off on team switch.
183. AI typing indicator must animate with soft fade and cycle dots (•••).
184. User avatars must preload from cache with lazy loading on scroll.
185. Profile management modal must support avatar upload with preview crop tool.
186. Riley chat background must adapt dynamically to team color or custom wallpaper.
187. Toast notifications must dismiss automatically unless marked "critical".
188. Team naming must follow case-insensitive uniqueness validation.
189. All UI strings must be loaded from a translation JSON dictionary.
190. Riley’s voice interface must follow mic-on/mic-off visual state logic.
191. AI summaries must follow markdown rules: bold headers, bullet points, emoji optional.
192. Augment must support external plugin injection via augment_plugins/.
193. All runtime errors must log to logs/augment_crashlog_[timestamp].txt.
194. Riley command suggestions must surface in the input field with autocomplete.
195. Sidebar quick access links must include hover tooltips and pinned-state support.
196. Messages sent via API must indicate [AUTOMATED] label in metadata.
197. Teams-style thread replies must render indented and in collapsible groups.
198. AI inline tips must be subtle and appear only after 5 seconds of user inactivity.
199. Text input areas must follow cursor position even during async refresh.
200. Augment must never allow unstyled fallback content to flash during load.
131. Augment must organize all project files into a strict modular folder structure (e.g., /core, /ui, /services, /models, /config, /logs).
132. File I/O must use safe, atomic write operations to prevent corruption.
133. Auto-backups must be created before any overwrite or config change.
134. Each module's file must include versioning and checksum headers.
135. Only approved file extensions can be opened by Augment (.cpp, .json, .ai, etc).
136. Augment must support encryption for all sensitive local config files.
137. All logs must be timestamped and rotated daily with cleanup after 30 days.
138. Augment must automatically resolve relative and absolute path conflicts.
139. File watchers should detect changes and alert dev tools in real time.
140. External file loading must validate source, signature, and origin chain.
141. Augment must include a secure sandboxed loader for 3rd-party modules.
142. Data caching must follow LRU (least-recently-used) eviction patterns.
143. Temporary working files must be auto-deleted upon safe termination.
144. Augment must write structured audit logs for all read/write operations.
145. Corrupt or partial files must be quarantined and reviewed, not deleted.
146. File permission levels must be enforced: user, developer, root.
147. A file integrity scan must run at launch and report anomalies.
148. All core data files must support schema versioning.
149. Missing files must trigger intelligent self-healing mechanisms.
150. Augment must avoid hardcoded paths and use environment variables.

💬 Conversation and UX Flow
151. Conversations in Augment must follow a natural progressive threading model.
152. Message bubbles should include timestamp, sender tag, and delivery status.
153. System messages must appear with subdued gray tone and italic font.
154. Reply boxes must expand fluidly based on user input length.
155. All input fields must support markdown rendering preview.
156. Hovering over a message should reveal context tools: reply, react, copy.
157. The send button must be disabled until valid input is detected.
158. Augment must offer slash commands (e.g., /clear, /reset, /export).
159. Conversations must be autosaved and resumed on app reload.
160. Any AI-generated message must include a subtle AI badge indicator.

🧱 System & Platform Architecture
161. Augment must use layered architecture: Interface → Core Engine → Services.
162. Each subsystem must expose APIs via class-based interfaces.
163. All code must be dependency-injected via a registry or service locator.
164. Module communication must be event-driven and non-blocking.
165. Augment must maintain uptime resilience via self-diagnosing threads.
166. The runtime must support "soft restart" of any module without full shutdown.
167. System-wide variables must be stored in a secure, encrypted config.
168. Threads must use mutexes and semaphores to avoid race conditions.
169. A fallback offline mode must be included for core AI operations.
170. All network activity must go through a proxy manager with logging.

🔐 Security & Access Control
171. All credentials must be stored using salted SHA-512 hashes.
172. Password inputs must mask characters and offer paste protection.
173. Augment must reject all unsigned code at runtime.
174. API keys must be stored encrypted in memory and never logged.
175. Admin actions must require secondary authentication or biometric input.
176. Data shared across network must use 10-million-bit AES-style encryption.
177. Security policies must trigger alerts on any anomaly.
178. A user activity log must be viewable only by root accounts.
179. Network ports must be whitelisted via config and validated on boot.
180. Sensitive features (e.g., code editing, config changes) must log user identity.

🤖 AI Features & Autonomy
181. Augment must include real-time speech-to-text input capture and NLP.
182. Context awareness must update with each user interaction persistently.
183. AI response tone must be customizable: technical, casual, guardian, etc.
184. Autonomous task execution must first show a confirmation modal.
185. AI memory must store context tags, timestamp, and emotional tone.
186. Memory recall must support fuzzy matching and user-specific priority.
187. The system must learn from user corrections and re-prioritize suggestions.
188. All AI models used must be disclosed with name, size, and version.
189. Augment must include an offline fallback transformer for core logic.
190. AI training examples must be editable in a training editor GUI.

📊 Analytics & Monitoring
191. A real-time dashboard must show CPU, RAM, GPU, and latency metrics.
192. User activity should be graphed in time-based plots with export option.
193. Errors and warnings must be logged with stack traces and timestamps.
194. Developer tools must include packet sniffing and event trace logging.
195. Heatmaps of user interaction zones should be accessible per session.
196. The dashboard must allow remote viewing with secure access.
197. Crash reports must auto-generate a structured JSON diagnostic file.
198. System startup time and boot events must be logged on every launch.
199. Statistics must differentiate between human and AI-triggered actions.
200. Performance degradation trends must be graphed over time and alerted.

🎨 UI/UX Design Expansion
201. Augment’s UI must adapt to 4K and ultrawide monitors without stretching.
202. Light/dark mode toggles must animate smoothly with theme memory.
203. Panels and windows must be draggable, dockable, and resizable.
204. All buttons and links must include tooltips and ARIA accessibility tags.
205. Augment must follow WCAG 2.1 Level AA accessibility standards.
206. Font scaling must respond to OS-level accessibility settings.
207. The UI must be keyboard-navigable with tab ordering and skip links.
208. Colorblind-safe palettes must be available as an alternate theme.
209. UI response time must remain under 50ms for all visual changes.
210. Augment must include a gesture-friendly mode for touchscreen use.

🧭 Navigation & Multi-Pane Interface
211. Augment must support Teams-style sidebars with active tab indicators.
212. Navigation state must persist across sessions without reload.
213. Sidebar must collapse and expand smoothly with icon-only mode.
214. Tabs must support badge notifications, hotkeys, and hover states.
215. Tab switching must be instant with no visual delay or content reload.
216. A multi-pane split-view must allow drag-to-resize between panels.
217. Breadcrumbs must always show current location in app hierarchy.
218. Notification banners must stack with dismiss and fade-out logic.
219. Multi-tab history navigation must mirror browser-style back/forward.
220. Teams panel must replicate Microsoft Teams exactly: “Join or Create a Team,” active badges, team icons.
31. Augment must use a modular file system where UI, logic, AI, and resource assets are independently updateable.
132. All user-generated data must be stored in a secure, encrypted format using 256-bit AES at minimum.
133. File I/O operations must use buffered reads/writes to optimize speed and reduce latency across large files.
134. The Teams-style chat history must be saved persistently across sessions in an organized JSON format.
135. All chat messages must include UTC timestamps, sender tags, and unique UUIDs.
136. Augment must allow real-time export of chat logs in .txt and .pdf formats.
137. A file cache layer must be used to preload essential UI and AI components to minimize runtime latency.
138. Implement error handling for every file access with backup fallback logic.
139. Augment must keep AI logs in a secure audit trail format for traceability.
140. Support must be included for file attachments in chat, respecting MIME types and max file size limits.
141. The system should create auto-snapshots of the working directory before any major AI code update.
142. Ensure Augment’s temporary files are deleted on shutdown unless flagged for recovery.
143. Use a dual-write backup system for critical user settings and persistent memory states.
144. Encrypt all AI core config files using machine-bound encryption to prevent unauthorized replication.
145. Maintain access logs of all users accessing or modifying the Riley AI configuration files.
146. All resource files must use version tags and support hot-reloading.
147. AI-generated files must be labeled and categorized using a standardized folder structure.
148. Protect all .cpp, .py, and .json system files from runtime edits unless granted dev override mode.
149. On crash, auto-trigger a log dump with complete AI memory and file access report.
150. File integrity must be verified on every startup using a SHA256 checksum manifest.
151. AI memory files must support structured querying by topic, keyword, or timestamp.
152. Rule-based access control (RBAC) must be used for any file classified as internal or sensitive.
153. AI state files must be checkpointed every 10 minutes or after 1,000 interactions—whichever is sooner.
154. Sensitive UI asset files must be obfuscated at compile time using XOR + compression hybrid method.
155. When deleting files via UI, use a staged "Trash" layer and delayed purge confirmation.
156. Augment should notify the user on unsaved work before exit or restart events.
157. File naming conventions must be human-readable and avoid cryptic hash-only labels.
158. All file operations must be thread-safe and use mutex locks when writing from multiple threads.
159. Auto-cleanup of debug folders must occur weekly unless overridden.
160. Augment must support hidden metadata tagging in all user documents for contextual AI memory linking.
