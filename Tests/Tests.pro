QT += testlib gui widgets
CONFIG += qt warn_on depend_includepath testcase c++17

TEMPLATE = app

SOURCES +=  tst_reactive.cpp

include(../QtReactive/QtReactive.pri)
