QT       += core gui widgets sql printsupport charts network

CONFIG += c++17

QT += printsupport

SOURCES += \
    client.cpp \
    emailcontrat.cpp \
    equipement.cpp \
    main.cpp \
    login.cpp \
    mainwindow.cpp \
    connection.cpp \
    employe.cpp \
    poubelle.cpp \
    contrat.cpp \
    contratgenerator.cpp \
    produit.cpp \
    smtp.cpp \
    chatbot.cpp


HEADERS += \
    client.h \
    emailcontrat.h \
    equipement.h \
    login.h \
    mainwindow.h \
    connection.h \
    employe.h \
    poubelle.h \
    contrat.h \
    contratgenerator.h \
    produit.h \
    smtp.h \
    chatbot.h

FORMS += \
    login.ui \
    mainwindow.ui

RESOURCES += \
    ressources.qrc
