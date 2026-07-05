# NexusChat

**NexusChat v1.0** — Universal console-based AI chatbot platform with multi-provider support, file-based authentication, and per-user chat history.

```
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ~ ~ ~ CHAT AI ASSISTANT ~ ~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
```

## Overview

NexusChat is a terminal-based AI chatbot application that connects to multiple AI providers and automatically detects which one to use based on the API key you paste in. It supports user authentication, persistent configuration, per-user chat history, and an admin panel for managing users and system settings.

## Features

- **Multi-Provider AI Support** — Works with Groq, Gemini, Anthropic, OpenAI, and custom APIs
- **Automatic Key Detection** — Paste any supported API key and the provider is auto-detected
- **File-Based Authentication** — User accounts stored and managed via `nexus_users.txt`
- **Per-User Chat History** — Each user's conversations are logged separately (`history_<user>.txt`)
- **Persistent Configuration** — API keys and settings persist across restarts (`nexus_config.txt`)
- **Admin Panel** — System administrator menu for managing users and viewing logs
- **In-Chat Commands** — `/history`, `/clear`, `/exit` supported during a chat session

## Admin Panel

The admin panel provides the following options:

| Option | Description |
|--------|-------------|
| 1 | Start Chat (own session) |
| 2 | List All Users |
| 3 | Delete a User |
| 4 | View System Logs |
| 5 | System Information |
| 6 | Configure Global AI Key |
| 7 | Settings / My Profile |
| 0 | Logout |

## Files Used

| File | Purpose |
|------|---------|
| `nexus_users.txt` | Stores registered user accounts |
| `nexus_config.txt` | Stores API keys and configuration, persists across restarts |
| `history_<user>.txt` | Stores per-user chat history |

## Chat Session Commands

Once connected to the AI, the following commands are available:

- `/history` — View past conversation history
- `/clear` — Clear the current chat history
- `/exit` — End the chat session

## Currently Supported AI Providers

- Groq (e.g. `llama-3.3-70b`)
- Gemini
- Anthropic
- OpenAI
- Custom (user-defined endpoint)

## Notes

- On Windows, an HTTP backend may be required for full functionality. If unavailable, use the Windows build.
- Make sure `nexus_config.txt` and `nexus_users.txt` are excluded from version control (see `.gitignore`) to avoid committing API keys or user credentials.

## License

Add your preferred license here (e.g. MIT).
