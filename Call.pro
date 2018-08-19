#-------------------------------------------------
#
# Project created by QtCreator 2018-08-16T20:54:36
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Call
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Dealers/dealer.cpp \
    sql/s.cpp \
    Dealers/dealer_list.cpp \
    Dealers/subtable.cpp \
    other/managers.cpp \
    AnyTable/any.cpp

HEADERS  += mainwindow.h \
    Dealers/dealer.h \
    include.h \
    sql/s.h \
    Dealers/dealer_list.h \
    Dealers/subtable.h \
    other/managers.h \
    AnyTable/any.h
