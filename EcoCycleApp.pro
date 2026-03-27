QT       += core gui widgets sql printsupport charts

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
    client.cpp \
    equipement.cpp \
    main.cpp \
    login.cpp \
    mainwindow.cpp \
    connection.cpp \
    employe.cpp \
    poubelle.cpp \
    contrat.cpp \
    produit.cpp


HEADERS += \
    client.h \
    equipement.h \
    login.h \
    mainwindow.h \
    connection.h \
    employe.h \
    poubelle.h \
    contrat.h \
    produit.h

FORMS += \
    login.ui \
    mainwindow.ui

RESOURCES += \
    ressources.qrc
