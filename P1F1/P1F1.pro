QT += core
QT -= gui

TARGET = P1F1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    nodoast.cpp \
    prueba.cpp \
    parser.cpp \
    scanner.cpp \
    mkdisk.cpp \
    fdisk.cpp \
    mount.cpp

DISTFILES += \
    generador.bat

HEADERS += \
    parser.yy \
    lexico.l \
    nodoast.h \
    prueba.h \
    parser.h \
    scanner.h \
    mkdisk.h \
    fdisk.h \
    mount.h

