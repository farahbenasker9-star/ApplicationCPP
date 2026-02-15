QT       += core gui widgets

SOURCES += \
    connection.cpp \
    main.cpp \
    login.cpp \
    mainwindow.cpp

HEADERS += \
    connection.h \
    login.h \
    mainwindow.h

FORMS += \
    login.ui \
    mainwindow.ui

RESOURCES += \
    ressources.qrc

QT+= sql
