QT       += core gui network widgets

CONFIG += c++17

VERSION=1.0.0

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_VERSION=$${VERSION}

SOURCES += \
    licensedialog.cpp \
    main.cpp \
    mainwindow.cpp \
    printer.cpp

HEADERS += \
    licensedialog.h \
    mainwindow.h \
    printer.h

FORMS += \
    licensedialog.ui \
    mainwindow.ui

RESOURCES += \
    res.qrc

RC_ICONS += printer.ico
QMAKE_TARGET_COMPANY = "Benjamin Lutz"
QMAKE_TARGET_COPYRIGHT = "© 2020 Benjamin Lutz"
