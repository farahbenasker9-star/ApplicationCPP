QT       += core gui widgets sql

CONFIG += c++17

SOURCES += \
    client.cpp \
    main.cpp \
    login.cpp \
    mainwindow.cpp \
    connection.cpp \
    employe.cpp \
    poubelle.cpp \
    contrat.cpp

HEADERS += \
    client.h \
    login.h \
    mainwindow.h \
    connection.h \
    employe.h \
    poubelle.h \
    contrat.h

FORMS += \
    login.ui \
    mainwindow.ui

RESOURCES += \
    ressources.qrc
