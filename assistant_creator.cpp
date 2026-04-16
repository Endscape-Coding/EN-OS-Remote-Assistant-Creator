#include "assistant_creator.h"
#include "./ui_assistant_creator.h"
#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include <QScrollBar>
#include <QRegularExpression>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMovie>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>

Assistant_Creator::Assistant_Creator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Assistant_Creator)
{
    ui->setupUi(this);
    installProcess = new QProcess(this);

    ui->comboBox_lang->clear();
    ui->comboBox_lang->addItem("Русский", "ru");
    ui->comboBox_lang->addItem("English", "en");
    ui->comboBox_lang->addItem("Українська", "uk");
    ui->comboBox_lang->addItem("Deutsch", "de");
    ui->comboBox_lang->addItem("Français", "fr");
    ui->comboBox_lang->addItem("中文", "zh");
    ui->comboBox_lang->addItem("한국어", "ko");
    ui->comboBox_lang->addItem("日本語", "ja");

    connect(installProcess, &QProcess::readyReadStandardOutput, this, &Assistant_Creator::onProcessOutput);
    connect(installProcess, &QProcess::readyReadStandardError, this, &Assistant_Creator::onProcessOutput);
    connect(installProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Assistant_Creator::onProcessFinished);
}

Assistant_Creator::~Assistant_Creator()
{
    if (installProcess->state() != QProcess::NotRunning) {
        installProcess->terminate();
        installProcess->waitForFinished(1000);
    }
    delete ui;
}

void Assistant_Creator::on_pushButton_clicked()
{
    if (installProcess->state() != QProcess::NotRunning) {
        appendLog(tr("Процесс уже запущен..."));
        return;
    }

    QString token = ui->lineEdit_token->text().trimmed();
    QString id = ui->lineEdit_id->text().trimmed();
    if (token.isEmpty() || id.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Введите Token и ID"));
        return;
    }

    ui->progressBar->setValue(0);
    appendLog(tr("Запуск установки..."));

    bool useCompile = ui->checkBox_2->isChecked();

    QString scriptUrl;
    if (useCompile) {
        scriptUrl = "https://raw.githubusercontent.com/Endscape-Coding/EN-OS-Remote-Assistant-Creator/main/scripts/install_compile.sh";
        appendLog(tr("Режим: Local Compile (локальная компиляция)"));
    } else {
        scriptUrl = "https://raw.githubusercontent.com/Endscape-Coding/EN-OS-Remote-Assistant-Creator/main/scripts/install_no_compile.sh";
    }

    QString proxyConfigPath = "~/.en-os/remote_assistant/.env";
    appendLog(tr("Конфиг прокси находится по пути: %1").arg(proxyConfigPath));

    QString compileFlag = useCompile ? "1" : "0";

    QString shellCommand = QString("curl -sL %1 | bash -s -- \"%2\" \"%3\"")
                               .arg(scriptUrl, token, id);

    QStringList args;
    args << "-c" << shellCommand;
    installProcess->start("bash", args);
}

void Assistant_Creator::on_infobutton_clicked()
{
    QDialog aboutDialog(this);
    aboutDialog.setWindowTitle(tr("О программе"));
    aboutDialog.setFixedSize(480, 320);
    aboutDialog.setWindowIcon(QIcon(":/icon.svg"));

    QVBoxLayout *mainLayout = new QVBoxLayout(&aboutDialog);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);

    QVBoxLayout *leftColumn = new QVBoxLayout();
    leftColumn->setSpacing(6);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(10);

    QLabel *appIconLabel = new QLabel();
    QPixmap appIcon(":/icon.svg");
    appIconLabel->setPixmap(appIcon.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    headerLayout->addWidget(appIconLabel, 0, Qt::AlignTop);

    QVBoxLayout *titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(1);
    titleLayout->setContentsMargins(0, 2, 0, 0);

    QLabel *nameLabel = new QLabel("<b>EN-OS Remote Assistant Creator</b>");
    QLabel *versionLabel = new QLabel(tr("Версия: 0.2.0"));
    QLabel *licenseLabel = new QLabel(tr("Лицензия: GPL v3"));

    titleLayout->addWidget(nameLabel);
    titleLayout->addWidget(versionLabel);
    titleLayout->addWidget(licenseLabel);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    headerLayout->addLayout(titleLayout, 1);

    leftColumn->addLayout(headerLayout);
    leftColumn->addSpacing(8);

    QLabel *devLabel = new QLabel(tr("Разработчик: <b>Endscape</b>"));
    devLabel->setAlignment(Qt::AlignLeft);
    leftColumn->addWidget(devLabel);
    leftColumn->addSpacing(12);

    QVBoxLayout *linksLayout = new QVBoxLayout();
    linksLayout->setSpacing(20);

    auto createLinkLabel = [](const QString &text, const QString &url) {
        QLabel *label = new QLabel(QString("<a href='%1' style='text-decoration: none; color: #2196F3;'>%2</a>")
                                        .arg(url, text));
        label->setOpenExternalLinks(true);
        label->setCursor(Qt::PointingHandCursor);
        return label;
    };

    linksLayout->addWidget(createLinkLabel("GitHub", "https://github.com/Endscape-Coding/EN-OS-Remote-Assistant-Creator"));
    linksLayout->addWidget(createLinkLabel("Telegram", "https://t.me/Linux_EN_OS"));
    linksLayout->addWidget(createLinkLabel("Site", "https://en-os.ru"));

    leftColumn->addLayout(linksLayout);
    leftColumn->addStretch();
    contentLayout->addLayout(leftColumn, 1);

    QVBoxLayout *rightColumn = new QVBoxLayout();
    QLabel *gifLabel = new QLabel();
    gifLabel->setAlignment(Qt::AlignCenter);

    QMovie *movie = new QMovie(":/assets/cat.gif");
    movie->setParent(&aboutDialog);

    if (movie->isValid()) {
        gifLabel->setMovie(movie);
        movie->start();
        gifLabel->setFixedSize(150, 150);
        gifLabel->setScaledContents(true);
    } else {
        gifLabel->setText(tr("[GIF]"));
        gifLabel->setFixedSize(100, 100);
        gifLabel->setStyleSheet("color: gray; font-style: italic; background: #f5f5f5; border-radius: 6px;");
        gifLabel->setAlignment(Qt::AlignCenter);
    }
    rightColumn->addWidget(gifLabel, 0, Qt::AlignVCenter);
    contentLayout->addLayout(rightColumn, 0);

    mainLayout->addLayout(contentLayout, 1);

    mainLayout->addSpacing(5);

    QHBoxLayout *footerLayout = new QHBoxLayout();

    QLabel *copyrightLabel = new QLabel(tr("© 2026–%1 Endscape-Coding").arg(QDate::currentDate().year()));
    copyrightLabel->setStyleSheet("color: #666; font-size: 11px;");
    footerLayout->addWidget(copyrightLabel, 0, Qt::AlignLeft);

    QPushButton *okBtn = new QPushButton(tr("Закрыть"));
    okBtn->setCursor(Qt::PointingHandCursor);
    okBtn->setFixedSize(80, 28);
    connect(okBtn, &QPushButton::clicked, &aboutDialog, &QDialog::accept);
    footerLayout->addWidget(okBtn, 0, Qt::AlignRight);

    mainLayout->addLayout(footerLayout);

    aboutDialog.exec();
}

void Assistant_Creator::onProcessOutput()
{
    QByteArray outputData = installProcess->readAllStandardOutput();
    if (!outputData.isEmpty()) {
        QString output = QString::fromLocal8Bit(outputData);
        QStringList lines = output.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
        for (const QString &line : lines) {
            if (line.startsWith("PROGRESS:")) {
                bool ok;
                int value = line.section(':', 1, 1).toInt(&ok);
                if (ok) {
                    ui->progressBar->setValue(value);
                }
            } else {
                appendLog(line.trimmed());
            }
        }
    }

    QByteArray errorData = installProcess->readAllStandardError();
    if (!errorData.isEmpty()) {
        appendLog("⚠ " + QString::fromLocal8Bit(errorData).trimmed());
    }
}

void Assistant_Creator::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        ui->progressBar->setValue(100);
        appendLog(tr("Успешно завершено."));
        showInstallationResult(true, tr("Настройка завершена!"));
    } else {
        appendLog(tr("Ошибка! Код: ") + QString::number(exitCode));
        showInstallationResult(false, tr("Ошибка! Код: %1").arg(exitCode));
    }
}

void Assistant_Creator::appendLog(const QString &message)
{
    QString timestamp = QTime::currentTime().toString("HH:mm:ss");
    ui->textEdit_log->append(QString("[%1] %2").arg(timestamp, message));
    ui->textEdit_log->verticalScrollBar()->setValue(ui->textEdit_log->verticalScrollBar()->maximum());
}

void Assistant_Creator::showInstallationResult(bool success, const QString &message)
{
    if (success) QMessageBox::information(this, tr("Успех"), message);
    else QMessageBox::critical(this, tr("Ошибка"), message);
}

void Assistant_Creator::on_comboBox_lang_currentIndexChanged(int index)
{
    QString locale = ui->comboBox_lang->itemData(index).toString();
    qApp->removeTranslator(&appTranslator);
    if (appTranslator.load(QString(":/i18n/assistant_%1.qm").arg(locale))) {
        qApp->installTranslator(&appTranslator);
    }
}

void Assistant_Creator::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    QMainWindow::changeEvent(event);
}
