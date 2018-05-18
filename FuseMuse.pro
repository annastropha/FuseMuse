#-------------------------------------------------
#
# Project created by QtCreator 2018-04-05T22:16:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FuseMuse
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    fmzipinfo.cpp

HEADERS += \
        mainwindow.h \
    fmzipinfo.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    notes.txt

RESOURCES += \
    fusemuse.qrc

win32: LIBS += -lquazip

unix:!macx: LIBS += -lquazip5

copydata.commands = $(COPY_DIR) \"$$PWD/res\" \"$$OUT_PWD\"
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

INCLUDEPATH += $$PWD/libfm/deps
DEPENDPATH += $$PWD/libfm/deps

INCLUDEPATH += $$PWD/libfm/src
DEPENDPATH += $$PWD/libfm/src

macx {
    LIBS += -L$$PWD/../../../../../usr/local/Cellar/quazip/0.7.3_1/lib/ -lquazip.1.0.0

    INCLUDEPATH += $$PWD/../../../../../usr/local/Cellar/quazip/0.7.3_1/include
    DEPENDPATH += $$PWD/../../../../../usr/local/Cellar/quazip/0.7.3_1/include
}
