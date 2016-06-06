QT += widgets
QT += xml
QT += network
QT += webkitwidgets
QT += core

CONFIG += c++11

win32 {
QMAKE_CXXFLAGS = -Wall -Wextra -Werror
QMAKE_CFLAGS = -Wall -Wextra -Werror
}

HEADERS += \
    sitespriceparser.h \
    singleton.h \
    htmlanalizer.h

SOURCES += \
    sitespriceparser.cpp \
    main.cpp \
    htmlanalizer.cpp

RESOURCES += \
    res.qrc

RC_ICONS = /images/app.ico
RC_FILE = app.rc

include(libs/xlsx/qtxlsx.pri)


