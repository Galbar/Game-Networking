CONFIG -= qt
QMAKE_CFLAGS += -std=c++11
QMAKE_CXXFLAGS += -std=c++11

TARGET=Client/client

LIBS += -lSDL2

SOURCES += \
        $$PWD/*.cpp \
        $$PWD/../Common/*.cpp
HEADERS += \
        $$PWD/*.h \
        $$PWD/../Common/*.h
