QT += widgets
QT += xml

CONFIG += c++11

win32 {
QMAKE_CXXFLAGS = -Wall -Wextra -Werror
QMAKE_CFLAGS = -Wall -Wextra -Werror
}

HEADERS += \
    sitespriceparser.h \
    singleton.h

SOURCES += \
    sitespriceparser.cpp \
    main.cpp

RESOURCES += \
    res.qrc

RC_ICONS = /images/app.ico
RC_FILE = app.rc

