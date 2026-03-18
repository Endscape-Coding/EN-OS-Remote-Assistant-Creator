#ifndef ASSISTANT_CREATOR_H
#define ASSISTANT_CREATOR_H

#include <QMainWindow>
#include <QProcess>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui { class Assistant_Creator; }
QT_END_NAMESPACE

class Assistant_Creator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Assistant_Creator(QWidget *parent = nullptr);
    ~Assistant_Creator();

protected:
    void changeEvent(QEvent *event) override;

private slots:
    void on_pushButton_clicked();
    void onProcessOutput();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void on_comboBox_lang_currentIndexChanged(int index);

private:
    Ui::Assistant_Creator *ui;
    QProcess *installProcess;
    void appendLog(const QString &message);
    void showInstallationResult(bool success, const QString &message);
    QTranslator appTranslator;
    QTranslator qtTranslator;
};

#endif
