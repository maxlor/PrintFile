QT       += core gui network widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    printer.cpp

HEADERS += \
    mainwindow.h \
    printer.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    res.qrc
