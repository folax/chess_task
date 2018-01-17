TEMPLATE = app

win32 {
QMAKE_CXXFLAGS = -Wall -Wextra -Werror
QMAKE_CFLAGS = -Wall -Wextra -Werror
}

CONFIG += c++11
CONFIG += CONSOLE

QT += qml quick widgets

SOURCES += src/main.cpp \
    src/logic.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

HEADERS += \
    src/logic.h

RC_FILE = app.rc
