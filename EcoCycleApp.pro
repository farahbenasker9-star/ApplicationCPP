QT       += core gui widgets sql printsupport charts network

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17
QT += printsupport

SOURCES += \
    client.cpp \
    equipement.cpp \
    equipementai.cpp \
    main.cpp \
    login.cpp \
    mainwindow.cpp \
    connection.cpp \
    employe.cpp \
    mapwidget.cpp \
    poubelle.cpp \
    contrat.cpp \
    produit.cpp \
    smtp.cpp


HEADERS += \
    client.h \
    equipement.h \
    equipementai.h \
    login.h \
    mainwindow.h \
    connection.h \
    employe.h \
    mapwidget.h \
    poubelle.h \
    contrat.h \
    produit.h \
    smtp.h

FORMS += \
    login.ui \
    mainwindow.ui

RESOURCES += \
    ressources.qrc
