QT       += core gui widgets sql printsupport charts network serialport

CONFIG += c++17

QT += printsupport
QT += network
SOURCES += \
    arduino.cpp \
    client.cpp \
    emailcontrat.cpp \
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
    contratgenerator.cpp \
    produit.cpp \
    smtp.cpp \
    chatbot.cpp


HEADERS += \
    arduino.h \
    client.h \
    emailcontrat.h \
    equipement.h \
    equipementai.h \
    login.h \
    mainwindow.h \
    connection.h \
    employe.h \
    mapwidget.h \
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
