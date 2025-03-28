#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateTimer();
    void startTimer();
    void stopTimer();
    void resetTimer();
    void addMinute();
    void subtractMinute();
    void add10Seconds();
    void subtract10Seconds();
    void resetToInitialTime();
    void checkWarningTime();
    void updateTimerFontSize();
    void updateButtonFontSizes();
    void updateWarningFontSizes();
    void updateTimeDisplay();
    void updateWarningTime();
    void updateWarningDisplay();
    void addWarningMinute();
    void subtractWarningMinute();
    void addWarningSeconds();
    void subtractWarningSeconds();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    bool isRunning;
    int initialSeconds;
    int remainingSeconds;
    int warningSeconds;
    int warningMinutes;
    int warningSecondsOnly;
};

#endif // MAINWINDOW_H 