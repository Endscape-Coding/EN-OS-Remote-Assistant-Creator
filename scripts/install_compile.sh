#!/bin/bash
TOKEN=$1
ID=$2
VERSION=${3:-"latest"}
REPO="Endscape-Coding/EN-OS-Remote-Assistant"
REPO_URL="https://github.com/$REPO"
TARGET_DIR="$HOME/.en-os/remote_assistant"
BUILD_DIR=$(mktemp -d /tmp/en-os-build.XXXXXX)
UDEV_RULE="/etc/udev/rules.d/99-uinput.rules"
UDEV_CONTENT='KERNEL=="uinput", MODE="0660", GROUP="input", OPTIONS+="static_node=uinput"'

if [[ "$LANG" == *_RU* ]]; then
    MSG_OS="Определение системы..."
    MSG_PREP="Установка зависимостей..."
    MSG_BUILD="Сборка в $BUILD_DIR..."
    MSG_DONE="Готово! Ассистент собран и запущен."
else
    MSG_OS="Detecting OS..."
    MSG_PREP="Installing dependencies..."
    MSG_BUILD="Building in $BUILD_DIR..."
    MSG_DONE="Done! Assistant built and started."
fi

trap 'rm -rf "$BUILD_DIR"' EXIT

echo "PROGRESS:5"
echo "$MSG_OS"

install_pkgs() {
    local pkgs="ydotool curl"
    local extra_x11=""

    if [ "$XDG_SESSION_TYPE" = "x11" ] || [ -n "$DISPLAY" ]; then
        extra_x11="scrot"
    fi

    if command -v pacman &> /dev/null; then
        pkexec pacman -S --noconfirm $pkgs base-devel $extra_x11
    elif command -v apt-get &> /dev/null; then
        pkexec apt-get update && pkexec apt-get install -y $pkgs build-essential $extra_x11
    elif command -v dnf &> /dev/null; then
        pkexec dnf install -y $pkgs gcc-c++ make $extra_x11
    else
        echo "Unknown package manager. Please install ydotool, build tools and optionally scrot manually."
    fi
}

echo "PROGRESS:10"
echo "$MSG_PREP"
install_pkgs

# 2. Rust
if ! command -v cargo &> /dev/null; then
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
    source "$HOME/.cargo/env"
else
    source "$HOME/.cargo/env" 2>/dev/null || true
fi

echo "PROGRESS:25"
echo "$MSG_BUILD"
pkill -f "en-os-remote-assistant" || true

if [[ "$VERSION" == "latest" ]]; then
    TAG=$(curl -s "https://api.github.com/repos/$REPO/releases/latest" | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/')
    if [[ -z "$TAG" ]]; then
        echo "Ошибка: не удалось получить последний релиз"
        exit 1
    fi
else
    TAG="$VERSION"
fi

ARCHIVE_URL="https://github.com/$REPO/archive/refs/tags/$TAG.tar.gz"
ARCHIVE_FILE="$BUILD_DIR/source.tar.gz"
EXTRACT_DIR="$BUILD_DIR/extracted"

echo "Загрузка последнего релиза $TAG..."
curl -sL "$ARCHIVE_URL" -o "$ARCHIVE_FILE" || { echo "Ошибка: не удалось скачать архив"; exit 1; }

mkdir -p "$EXTRACT_DIR"
tar -xzf "$ARCHIVE_FILE" -C "$EXTRACT_DIR" --strip-components=1

cd "$EXTRACT_DIR" || { echo "Ошибка: не удалось войти в директорию"; exit 1; }

cargo build --release

echo "PROGRESS:75"
mkdir -p "$TARGET_DIR"
cp target/release/en-os-remote-assistant "$TARGET_DIR/en-os-remote-assistant"
chmod +x "$TARGET_DIR/en-os-remote-assistant"

cd "$TARGET_DIR"
echo "TELOXIDE_TOKEN=$TOKEN" > .env
echo "ID=$ID" >> .env

echo "PROGRESS:85"
if [ ! -f "$UDEV_RULE" ]; then
    echo "$UDEV_CONTENT" | pkexec tee "$UDEV_RULE" > /dev/null
    pkexec udevadm control --reload-rules && pkexec udevadm trigger
fi

for grp in input uinput; do
    if getent group "$grp" > /dev/null && ! groups $USER | grep &>/dev/null "\b$grp\b"; then
        pkexec gpasswd -a $USER $grp
    fi
done

echo "PROGRESS:90"
if [ ! -d "/var/log/en-os/remote_assistant/" ]; then
    pkexec bash -c "mkdir -p /var/log/en-os/remote_assistant/ && chmod 775 /var/log/en-os/remote_assistant/"
fi

mkdir -p ~/.config/autostart
cat <<EOF > ~/.config/autostart/en-os-remote-assistant.desktop
[Desktop Entry]
Type=Application
Name=EN-OS Remote Assistant
Exec=$TARGET_DIR/en-os-remote-assistant
Icon=utilities-terminal
Terminal=false
Path=$TARGET_DIR
Categories=System;
EOF

echo "PROGRESS:95"
nohup ./en-os-remote-assistant > /dev/null 2>&1 & disown

echo "PROGRESS:100"
echo "$MSG_DONE"
