#include "assistant_creator.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <cstdlib>

int main(int argc, char *argv[])
{
    setenv("XDG_RUNTIME_DIR", "/tmp/runtime-root", 1);

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "EN-OS-Remote-Assistant-Creator_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    a.setWindowIcon(QIcon(":/icon.png"));
    Assistant_Creator w;
    w.show();
    return a.exec();
}
