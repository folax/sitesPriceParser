QT += widgets
QT += xml

CONFIG += c++11

win32 {
QMAKE_CXXFLAGS = -Wall -Wextra -Werror
QMAKE_CFLAGS = -Wall -Wextra -Werror
}

HEADERS += \
    sitespriceparser.h

SOURCES += \
    sitespriceparser.cpp \
    main.cpp
