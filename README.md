# EN-OS Remote Assistant Creator 🤖

[](https://www.google.com/search?q=LICENSE)
[](https://www.qt.io/)
[](https://isocpp.org/)
[](https://github.com/Endscape-Coding/EN-OS-Remote-Assistant-Creator/releases)

> **A GUI utility for quick setup and installation of [EN-OS Remote Assistant](https://github.com/Endscape-Coding/EN-OS-Remote-Assistant)** - a Telegram bot for remote management of your system.

-----

## 🚀 About the Project

**EN-OS Remote Assistant Creator** is a cross-platform application built with **Qt6** that simplifies the installation and configuration process for [EN-OS Remote Assistant](https://github.com/Endscape-Coding/EN-OS-Remote-Assistant).

The program provides an intuitive graphical interface to:

  - Enter your bot token and user ID
  - Select installation mode (pre-built binary or local compilation)
  - Track installation progress in real-time
  - View detailed operation logs

Supports various Linux distributions: **Arch**, **Debian/Ubuntu**, **Fedora**.

-----

## ✨ Features

| Feature | Description |
|---------|----------|
| 🔐 **Authorization** | Enter Telegram Bot Token and User ID to configure the bot |
| 📦 **Two Install Modes** | • `Binary`: Fast installation of a pre-built binary <br> • `Compile`: Local build from source for maximum compatibility |
| 🌐 **Multilingual** | Support for 8 languages: 🇷🇺 Russian, 🇬🇧 English, 🇺🇦 Ukrainian, 🇩🇪 German, 🇫🇷 French, 🇨🇳 Chinese, 🇰🇷 Korean, 🇯🇵 Japanese |
| 📊 **Progress & Logs** | Visual tracking of installation stages with detailed output |
| ⚙️ **Auto-configuration** | Automatic dependency installation, udev rules setup, and permission group management |
| 🔄 **Autostart** | Creates a `.desktop` file to launch the assistant on system startup |
| 🎨 **Modern UI** | Qt6-based interface with theme support and animations |

-----

## 🖼️ Screenshots

> 💡 *The interface adapts to the system locale. Language switching is available via the dropdown menu.*

-----

## 📦 Requirements

### For Running the Pre-built Version:

  - **OS**: Linux (Arch, Debian/Ubuntu, Fedora, or compatible)
  - **Dependencies**: `curl`, `bash`, `pkexec` (for administrative rights)
  - **Permissions**: `sudo`/`pkexec` access for configuring udev rules

### For Building from Source:

```bash
# Basic build tools
CMake >= 3.16
C++17 compatible compiler (g++, clang++)

# Qt6 Modules
Qt6::Widgets
Qt6::LinguistTools

# Additional
git, curl
```

-----

## ⬇️ Installation

### 🔧 Building from Source

```bash
# 1. Clone the repository
git clone https://github.com/Endscape-Coding/EN-OS-Remote-Assistant-Creator.git
cd EN-OS-Remote-Assistant-Creator

# 2. Run the build script
chmod +x build.sh
./build.sh

# 3. Run the application
./build/remote-assistant-creator
```

### 📥 Pre-built Binaries

Ready-to-use builds are available on the [Releases](https://github.com/Endscape-Coding/EN-OS-Remote-Assistant-Creator/releases) page.

> ⚠️ **Note**: Requires Qt6 libraries to be installed on your system, or use the AppImage release.

-----

## 🎮 Usage

1.  **Launch** `remote-assistant-creator`
2.  **Enter Credentials**:
      - `Token` - your Telegram bot token (get it from [@BotFather](https://t.me/BotFather))
      - `ID` - your Telegram User ID (get it from [@userinfobot](https://t.me/userinfobot))
      - Check `Local Compile` if you wish to build from source
3.  **Click "Start Installation"** and monitor the progress in the log
4.  **Done\!** The assistant is installed and ready to work 🎉

### 📁 Configuration

After installation, the config file is created at:

```
~/.en-os/remote_assistant/.env
```

Content:

```env
TELOXIDE_TOKEN=your_bot_token
ID=your_user_id
```

### 🔄 Autostart

The application automatically creates an autostart file:

```
~/.config/autostart/en-os-remote-assistant.desktop
```

-----

## 🌍 Localization

The project supports a multi-language interface via the Qt translation system.

### Supported Languages:

| Code | Language | Translation File |
|-----|------|---------------|
| `ru` | Russian | `assistant_ru.ts` |
| `en` | English | `assistant_en.ts` |
| `uk` | Ukrainian | `assistant_uk.ts` |
| `de` | German | `assistant_de.ts` |
| `fr` | French | `assistant_fr.ts` |
| `zh` | Chinese | `assistant_zh.ts` |
| `ko` | Korean | `assistant_ko.ts` |
| `ja` | Japanese | `assistant_ja.ts` |

-----

## 📜 License

Distributed under the **GNU General Public License v3.0**.  
See the [LICENSE](https://www.google.com/search?q=LICENSE) file for details.

-----

## 🔗 Links

  - 🐙 [Source Code on GitHub](https://github.com/Endscape-Coding/EN-OS-Remote-Assistant-Creator)
  - 🤖 [EN-OS Remote Assistant (Main Project)](https://github.com/Endscape-Coding/EN-OS-Remote-Assistant)
  - 🌐 [Official Website](https://en-os.ru)
  - 💬 [Telegram Channel](https://t.me/Linux_EN_OS)

-----

> **Developed with ❤️ as part of the EN-OS project** \> *Author: [Endscape](https://github.com/Endscape-Coding)*
