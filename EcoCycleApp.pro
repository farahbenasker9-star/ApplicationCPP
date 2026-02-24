QT       += core gui widgets sql

CONFIG += c++17

SOURCES += \
    client.cpp \
    main.cpp \
    login.cpp \
    mainwindow.cpp \
    connection.cpp \
    employe.cpp

HEADERS += \
    client.h \
    login.h \
    mainwindow.h \
    connection.h \
    employe.h

FORMS += \
    login.ui \
    mainwindow.ui

RESOURCES += \
    ressources.qrc
