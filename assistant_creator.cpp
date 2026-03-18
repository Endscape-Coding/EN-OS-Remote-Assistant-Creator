#include "assistant_creator.h"
#include "./ui_assistant_creator.h"
#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include <QScrollBar>
#include <QRegularExpression>

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

    QString scriptUrl = "https://raw.githubusercontent.com/Endscape-Coding/EN-OS-Remote-Assistant-Creator/main/scripts/install_no_compile.sh";
    QString shellCommand = QString("curl -sL %1 | bash -s -- \"%2\" \"%3\"")
                               .arg(scriptUrl, token, id);

    QStringList args;
    args << "-c" << shellCommand;

    installProcess->start("bash", args);
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
