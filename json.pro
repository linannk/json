TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    jsonobject.cpp \
    jsonvalue.cpp \
    jsonarray.cpp \
    jsoninputstream.cpp

HEADERS += \
    jsonobject.h \
    jsondef.h \
    jsonstringcharseq.h \
    jsonvalue.h \
    jsonarray.h \
    jsonfilecharseq.h \
    jsonstring.h \
    jsoninputstream.h
