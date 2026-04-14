#!/bin/bash
TOKEN=$1
ID=$2
REPO_URL="https://github.com/Endscape-Coding/EN-OS-Remote-Assistant"
TARGET_DIR="$HOME/.en-os/remote_assistant"
UDEV_RULE="/etc/udev/rules.d/99-uinput.rules"
UDEV_CONTENT='KERNEL=="uinput", MODE="0660", GROUP="input", OPTIONS+="static_node=uinput"'

if [[ "$LANG" == *_RU* ]]; then
    MSG_OS="Определение дистрибутива..."
    MSG_PREP="Установка системных зависимостей..."
    MSG_BUILD="Сборка бинарника из исходников..."
    MSG_AUTH="Настройка прав доступа (udev/input)..."
    MSG_DONE="Готово! Ассистент запущен."
else
    MSG_OS="Detecting OS..."
    MSG_PREP="Installing system dependencies..."
    MSG_BUILD="Building binary from source..."
    MSG_AUTH="Configuring permissions (udev/input)..."
    MSG_DONE="Done! Assistant started."
fi

echo "PROGRESS:5"
echo "$MSG_OS"

install_pkgs() {
    if command -v pacman &> /dev/null; then
        pkexec pacman -S --noconfirm git base-devel ydotool curl
    elif command -v apt-get &> /dev/null; then
        pkexec apt-get update && pkexec apt-get install -y git build-essential ydotool curl
    elif command -v dnf &> /dev/null; then
        pkexec dnf install -y git gcc-c++ make ydotool curl
    else
        echo "Unknown package manager. Please install git, ydotool and build tools manually."
    fi
}

echo "PROGRESS:10"
echo "$MSG_PREP"
install_pkgs

echo "PROGRESS:20"
if ! command -v cargo &> /dev/null; then
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
    source "$HOME/.cargo/env"
else
    source "$HOME/.cargo/env" 2>/dev/null || true
fi

echo "PROGRESS:30"
echo "$MSG_BUILD"
pkill -f "en-os-remote-assistant" || true
mkdir -p "$TARGET_DIR"

if [ -d "$TARGET_DIR/.git" ]; then
    cd "$TARGET_DIR"
    git pull
else
    git clone "$REPO_URL" "$TARGET_DIR"
    cd "$TARGET_DIR"
fi

cargo build --release

echo "PROGRESS:75"
cp target/release/en-os-remote-assistant ./en-os-remote-assistant
chmod +x en-os-remote-assistant

echo "TELOXIDE_TOKEN=$TOKEN" > .env
echo "ID=$ID" >> .env

echo "PROGRESS:85"
echo "$MSG_AUTH"
if [ ! -f "$UDEV_RULE" ]; then
    echo "$UDEV_CONTENT" | pkexec tee "$UDEV_RULE" > /dev/null
    pkexec udevadm control --reload-rules && pkexec udevadm trigger
fi

for grp in input uinput; do
    if getent group "$grp" > /dev/null; then
        if ! groups $USER | grep &>/dev/null "\b$grp\b"; then
            pkexec gpasswd -a $USER $grp
        fi
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
# Запуск (демон ydotoold поднимется самим ассистентом, если это прописано в коде ассистента)
nohup ./en-os-remote-assistant > /dev/null 2>&1 & disown

echo "PROGRESS:100"
echo "$MSG_DONE"
