#!/bin/bash
TOKEN=$1
ID=$2
BIN_URL="https://github.com/Endscape-Coding/EN-OS-Remote-Assistant/releases/download/v/en-os-remote-assistant"
TARGET_DIR="$HOME/.en-os/remote_assistant"
UDEV_RULE="/etc/udev/rules.d/99-uinput.rules"
UDEV_CONTENT='KERNEL=="uinput", MODE="0660", GROUP="input", OPTIONS+="static_node=uinput"'

if [[ "$LANG" == *_RU* ]]; then
    MSG_DL="Загрузка программы..."
    MSG_CONF="Настройка окружения..."
    MSG_AUTH="Запрос прав для udev..."
    MSG_DONE="Установка завершена! Ассистент запущен."
else
    MSG_DL="Downloading binary..."
    MSG_CONF="Configuring environment..."
    MSG_AUTH="Requesting udev permissions..."
    MSG_DONE="Installation complete! Assistant started."
fi

echo "PROGRESS:10"
mkdir -p "$TARGET_DIR"
cd "$TARGET_DIR"

echo "PROGRESS:20"
echo "$MSG_DL"
pkill -f "en-os-remote-assistant" || true
curl -L "$BIN_URL" -o en-os-remote-assistant
chmod +x en-os-remote-assistant

echo "PROGRESS:50"
echo "$MSG_CONF"
# Перезаписываем конфиг
echo "TELOXIDE_TOKEN=$TOKEN" > .env
echo "ID=$ID" >> .env

echo "PROGRESS:70"
if [ ! -f "$UDEV_RULE" ]; then
    echo "$MSG_AUTH"
    echo "$UDEV_CONTENT" | pkexec tee "$UDEV_RULE" > /dev/null
    pkexec udevadm control --reload-rules && pkexec udevadm trigger
fi

if ! groups $USER | grep &>/dev/null "\binput\b"; then
    pkexec gpasswd -a $USER input
fi

echo "PROGRESS:85"
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
# Запуск в фоне
nohup ./en-os-remote-assistant > /dev/null 2>&1 & disown

echo "PROGRESS:100"
echo "$MSG_DONE"
