# EN-OS Remote Assistant Creator 🤖

[![License: GPL-3.0](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](LICENSE)
[![Qt6](https://img.shields.io/badge/Qt-6.x-41CD52.svg)](https://www.qt.io/)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![Release](https://img.shields.io/github/v/release/Endscape-Coding/EN-OS-Remote-Assistant-Creator)](https://github.com/Endscape-Coding/EN-OS-Remote-Assistant-Creator/releases)

> **GUI-утилита для быстрой настройки и установки [EN-OS Remote Assistant](https://github.com/Endscape-Coding/EN-OS-Remote-Assistant)** - Telegram-бота для удалённого управления вашей системой.

![Logo](Assistant_Creator_Logo.png)

---

## 🚀 О проекте

**EN-OS Remote Assistant Creator** - это кроссплатформенное приложение на **Qt6**, которое упрощает процесс установки и настройки [EN-OS Remote Assistant](https://github.com/Endscape-Coding/EN-OS-Remote-Assistant).

Программа предоставляет интуитивно понятный графический интерфейс для:
- Ввода токена и идентификатора бота
- Выбора режима установки (готовый бинарник или локальная компиляция)
- Отслеживания прогресса установки в реальном времени
- Просмотра лога выполнения операций

Поддерживается работа на различных Linux-дистрибутивах: **Arch**, **Debian/Ubuntu**, **Fedora**.

---

## ✨ Возможности

| Функция | Описание |
|---------|----------|
| 🔐 **Авторизация** | Ввод Telegram Bot Token и User ID для настройки бота |
| 📦 **Два режима установки** | • `Binary`: быстрая установка готового бинарника <br> • `Compile`: локальная сборка из исходников с максимальной совместимостью |
| 🌐 **Мультиязычность** | Поддержка 8 языков: 🇷🇺 Русский, 🇬🇧 English, 🇺🇦 Українська, 🇩🇪 Deutsch, 🇫🇷 Français, 🇨🇳 中文, 🇰🇷 한국어, 🇯🇵 日本語 |
| 📊 **Прогресс-бар и лог** | Визуальное отслеживание этапов установки с подробным выводом |
| ⚙️ **Автонастройка системы** | Автоматическая установка зависимостей, настройка udev-правил, добавление в группы доступа |
| 🔄 **Автозагрузка** | Создание `.desktop`-файла для запуска ассистента при старте системы |
| 🎨 **Современный UI** | Интерфейс на Qt6 с поддержкой тем оформления и анимаций |

---

## 🖼️ Скриншоты

![Основное окно](https://github.com/user-attachments/assets/c2f4e253-1496-438f-8a35-861e258b134f)

> 💡 *Интерфейс адаптируется под системную локаль. Переключение языка доступно в выпадающем списке.*

---

## 📦 Требования

### Для запуска готовой версии:
- **ОС**: Linux (Arch, Debian/Ubuntu, Fedora или совместимые)
- **Зависимости**: `curl`, `bash`, `pkexec` (для прав администратора)
- **Права**: Доступ к `sudo`/`pkexec` для настройки udev-правил

### Для сборки из исходников:
```bash
# Базовые инструменты сборки
CMake >= 3.16
C++17-совместимый компилятор (g++, clang++)

# Qt6 модули
Qt6::Widgets
Qt6::LinguistTools

# Дополнительные
git, curl
```

---

## ⬇️ Установка

### 🔧 Сборка из исходников

```bash
# 1. Клонируйте репозиторий
git clone https://github.com/Endscape-Coding/EN-OS-Remote-Assistant-Creator.git
cd EN-OS-Remote-Assistant-Creator

# 2. Запустите скрипт сборки
chmod +x build.sh
./build.sh

# 3. Запустите приложение
./build/remote-assistant-creator
```

### 📥 Готовые бинарники

Готовые сборки доступны на странице [Releases](https://github.com/Endscape-Coding/EN-OS-Remote-Assistant-Creator/releases).

> ⚠️ **Внимание**: Для корректной работы требуется наличие библиотек Qt6 в системе, или скачиваете Appimage релиз.

---

## 🎮 Использование

1. **Запустите** `remote-assistant-creator`
2. **Введите данные**:
   - `Token` - токен вашего Telegram-бота (получить у [@BotFather](https://t.me/BotFather))
   - `ID` - ваш Telegram User ID (узнать у [@userinfobot](https://t.me/userinfobot))
   - Можете выбрать галочку `Local Compile` для сборки из исходников
4. **Нажмите "Начать установку"** и следите за прогрессом в логе
5. **Готово!** Ассистент запущен и готов к работе 🎉

### 📁 Конфигурация

После установки конфигурационный файл создаётся по пути:
```
~/.en-os/remote_assistant/.env
```

Содержимое:
```env
TELOXIDE_TOKEN=ваш_токен_бота
ID=ваш_user_id
```

### 🔄 Автозапуск

Приложение автоматически создаёт файл автозагрузки:
```
~/.config/autostart/en-os-remote-assistant.desktop
```

---

## 🌍 Локализация

Проект поддерживает многоязычный интерфейс через систему переводов Qt.

### Поддерживаемые языки:
| Код | Язык | Файл перевода |
|-----|------|---------------|
| `ru` | Русский | `assistant_ru.ts` |
| `en` | English | `assistant_en.ts` |
| `uk` | Українська | `assistant_uk.ts` |
| `de` | Deutsch | `assistant_de.ts` |
| `fr` | Français | `assistant_fr.ts` |
| `zh` | 中文 | `assistant_zh.ts` |
| `ko` | 한국어 | `assistant_ko.ts` |
| `ja` | 日本語 | `assistant_ja.ts` |

---

## 📜 Лицензия

Распространяется под лицензией **GNU General Public License v3.0**.  
См. файл [LICENSE](LICENSE) для подробностей.

---

## 🔗 Ссылки

- 🐙 [Исходный код на GitHub](https://github.com/Endscape-Coding/EN-OS-Remote-Assistant-Creator)
- 🤖 [EN-OS Remote Assistant (основной проект)](https://github.com/Endscape-Coding/EN-OS-Remote-Assistant)
- 🌐 [Официальный сайт](https://en-os.ru)
- 💬 [Telegram-канал](https://t.me/Linux_EN_OS)

---

> **Разработано с ❤️ в рамках проекта EN-OS**  
> *Автор: [Endscape](https://github.com/Endscape-Coding)*  

---
