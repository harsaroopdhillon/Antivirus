#-------------------------------------------------
#
# Project created by QtCreator 2017-12-28T15:32:00
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Antivirus
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    home.cpp \
    scan.cpp \
    scan_dialog.cpp \
    checksum.cpp \
    history.cpp \
    switch.cpp \
    setting.cpp \
    links.cpp \
    scan_setting.cpp \
    browserprotection.cpp \
    setting_page.cpp

HEADERS += \
        mainwindow.h \
    header.h \
    home.h \
    scan.h \
    scan_dialog.h \
    checksum.h \
    history.h \
    switch.h \
    setting.h \
    links.h \
    scan_setting.h \
    browserprotection.h \
    setting_page.h \
    mov.h

RESOURCES += \
    img.qrc

FORMS +=
