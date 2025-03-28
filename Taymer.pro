QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# CONFIG += console

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "Taymer"
QMAKE_TARGET_PRODUCT = "Taymer"
QMAKE_TARGET_DESCRIPTION = "Приложение-таймер"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2025"

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# RC_ICONS = app.ico 