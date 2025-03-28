#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QPushButton>
#include <QResizeEvent>
#include <QFontMetrics>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
    , isRunning(false)
    , initialSeconds(0)
    , remainingSeconds(0)
    , warningSeconds(0)
    , warningMinutes(0)
    , warningSecondsOnly(0)
{
    ui->setupUi(this);

    updateWarningDisplay();
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    timer->setInterval(1000);

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startTimer);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopTimer);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::resetTimer);
    connect(ui->addMinuteButton, &QPushButton::clicked, this, &MainWindow::addMinute);
    connect(ui->subtractMinuteButton, &QPushButton::clicked, this, &MainWindow::subtractMinute);
    connect(ui->add10SecondsButton, &QPushButton::clicked, this, &MainWindow::add10Seconds);
    connect(ui->subtract10SecondsButton, &QPushButton::clicked, this, &MainWindow::subtract10Seconds);
    connect(ui->resetToInitialButton, &QPushButton::clicked, this, &MainWindow::resetToInitialTime);
    connect(ui->addWarningMinutesButton, &QPushButton::clicked, this, &MainWindow::addWarningMinute);
    connect(ui->subtractWarningMinutesButton, &QPushButton::clicked, this, &MainWindow::subtractWarningMinute);
    connect(ui->addWarningSecondsButton, &QPushButton::clicked, this, &MainWindow::addWarningSeconds);
    connect(ui->subtractWarningSecondsButton, &QPushButton::clicked, this, &MainWindow::subtractWarningSeconds);

    QFont timerFont = ui->timerLabel->font();
    timerFont.setFamily("Arial");
    timerFont.setBold(true);
    ui->timerLabel->setFont(timerFont);
    
    updateTimerFontSize();
    updateButtonFontSizes();
    updateWarningFontSizes();

    resetTimer();
    ui->stopButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateTimerFontSize();
    updateButtonFontSizes();
    updateWarningFontSizes();
}

void MainWindow::updateTimerFontSize()
{
    if (!ui->timerLabel)
        return;
        
    int labelWidth = ui->timerLabel->width();
    int labelHeight = ui->timerLabel->height();
    int fontSize = labelHeight * 0.7;
    
    QFont font = ui->timerLabel->font();
    font.setPointSize(fontSize);
    
    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance("00:00");
    
    if (textWidth > labelWidth * 0.9) {
        fontSize = fontSize * labelWidth * 0.9 / textWidth;
        font.setPointSize(fontSize);
    }
    
    ui->timerLabel->setFont(font);
    
    if (isRunning && remainingSeconds <= warningSeconds) {
        ui->timerLabel->setStyleSheet("color: red;");
    }
}

void MainWindow::updateButtonFontSizes()
{
    QList<QPushButton*> buttons = {
        ui->startButton, ui->stopButton, ui->resetButton,
        ui->addMinuteButton, ui->subtractMinuteButton,
        ui->add10SecondsButton, ui->subtract10SecondsButton,
        ui->resetToInitialButton,
        ui->addWarningMinutesButton, ui->subtractWarningMinutesButton,
        ui->addWarningSecondsButton, ui->subtractWarningSecondsButton
    };
    
    for (QPushButton* button : buttons) {
        if (button) {
            int buttonHeight = button->height();
            QFont buttonFont = button->font();
            buttonFont.setPointSize(buttonHeight * 0.3);
            button->setFont(buttonFont);
        }
    }
}

void MainWindow::updateTimeDisplay()
{
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;
    ui->timerLabel->setText(QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0')));
}

void MainWindow::updateWarningDisplay()
{
    ui->warningMinutesLabel->setText(QString("%1").arg(warningMinutes, 2, 10, QChar('0')));
    ui->warningSecondsLabel->setText(QString("%1").arg(warningSecondsOnly, 2, 10, QChar('0')));
    warningSeconds = warningMinutes * 60 + warningSecondsOnly;
             
    if (isRunning && remainingSeconds <= warningSeconds) {
        ui->timerLabel->setStyleSheet("color: red;");
    } else if (remainingSeconds > warningSeconds) {
        ui->timerLabel->setStyleSheet("");
    }
    
    updateWarningFontSizes();
}

void MainWindow::updateTimer()
{
    if (remainingSeconds > 0) {
        remainingSeconds--;
        updateTimeDisplay();
        checkWarningTime();
    } else {
        stopTimer();
        QMessageBox::warning(this, "Время истекло", "Отсчёт времени завершён!");
    }
}

void MainWindow::startTimer()
{
    if (!isRunning) {
        if (remainingSeconds > 0) {
            timer->start();
            isRunning = true;
            ui->startButton->setEnabled(false);
            ui->stopButton->setEnabled(true);
        } else {
            QMessageBox::information(this, "Таймер", "Установите время перед запуском");
        }
    }
}

void MainWindow::stopTimer()
{
    if (isRunning) {
        timer->stop();
        isRunning = false;
        ui->startButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
    }
}

void MainWindow::resetTimer()
{
    stopTimer();
    initialSeconds = 0;
    remainingSeconds = 0;
    updateTimeDisplay();
    ui->timerLabel->setStyleSheet("");
}

void MainWindow::addMinute()
{
    remainingSeconds += 60;
    initialSeconds = remainingSeconds;
    updateTimeDisplay();
    checkWarningTime();
}

void MainWindow::subtractMinute()
{
    if (remainingSeconds >= 60) {
        remainingSeconds -= 60;
        initialSeconds = remainingSeconds;
        updateTimeDisplay();
        checkWarningTime();
    }
}

void MainWindow::add10Seconds()
{
    remainingSeconds += 10;
    initialSeconds = remainingSeconds;
    updateTimeDisplay();
    checkWarningTime();
}

void MainWindow::subtract10Seconds()
{
    if (remainingSeconds >= 10) {
        remainingSeconds -= 10;
        initialSeconds = remainingSeconds;
        updateTimeDisplay();
        checkWarningTime();
    }
}

void MainWindow::resetToInitialTime()
{
    stopTimer();
    remainingSeconds = initialSeconds;
    updateTimeDisplay();
    ui->timerLabel->setStyleSheet("");
}

void MainWindow::checkWarningTime()
{
    if (remainingSeconds <= warningSeconds && remainingSeconds > 0) {
        ui->timerLabel->setStyleSheet("color: red;");
    }
}

void MainWindow::updateWarningTime()
{
    updateWarningDisplay();
}

void MainWindow::addWarningMinute()
{
    if (warningMinutes < 59) {
        warningMinutes++;
        updateWarningDisplay();
    }
}

void MainWindow::subtractWarningMinute()
{
    if (warningMinutes > 0) {
        warningMinutes--;
        updateWarningDisplay();
    }
}

void MainWindow::addWarningSeconds()
{
    if (warningSecondsOnly < 50) {
        warningSecondsOnly += 10;
    } else {
        warningSecondsOnly = 0;
        if (warningMinutes < 59) {
            warningMinutes++;
        }
    }
    updateWarningDisplay();
}

void MainWindow::subtractWarningSeconds()
{
    if (warningSecondsOnly >= 10) {
        warningSecondsOnly -= 10;
    } else if (warningMinutes > 0) {
        warningSecondsOnly = 50;
        warningMinutes--;
    }
    updateWarningDisplay();
}

void MainWindow::updateWarningFontSizes()
{
    QList<QLabel*> warningLabels = {
        ui->label_3,
        ui->label_4,
        ui->warningMinutesLabel,
        ui->warningSecondsLabel
    };
    
    int windowWidth = width();
    int windowHeight = height();
    
    int baseSize = qMin(windowWidth / 40, windowHeight / 25);
    
    baseSize = qBound(8, baseSize, 14);
    
    for (QLabel* label : warningLabels) {
        if (label) {
            QFont font = label->font();
            
            if (label == ui->label_3 || label == ui->label_4) {
                font.setPointSize(baseSize);
            } 
            else {
                font.setPointSize(baseSize + 2);
                font.setBold(true);
            }
            
            label->setFont(font);
            
            if (label == ui->warningMinutesLabel || label == ui->warningSecondsLabel) {
                QFontMetrics fm(font);
                int textWidth = fm.horizontalAdvance("00");
                label->setMinimumWidth(textWidth + 10);
            }
        }
    }
}