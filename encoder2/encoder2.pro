QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle
#LIBS += -L..\libmp3lame.dll.a

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += main.cpp \
    myencoder.cpp \
    worker.cpp

HEADERS += \
    myencoder.h \
    worker.h \
    include/lame.h

RESOURCES += \
    res.qrc

unix:!macx|win32: LIBS += -L$$PWD/../lame_3.100_x86/ -llibmp3lame.dll #-lpthread

INCLUDEPATH += $$PWD/../lame_3.100_x86
DEPENDPATH += $$PWD/../lame_3.100_x86

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lame_3.100_x86/libmp3lame.dll.lib
else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/../lame_3.100_x86/libmp3lame.dll.a

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../ -lwinpthread
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../ -lwinpthread
#else:unix:!macx: LIBS += -L$$PWD/../ -lwinpthread

#INCLUDEPATH += $$PWD/../
#DEPENDPATH += $$PWD/../

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../libwinpthread.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../libwinpthread.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../winpthread.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../winpthread.lib
#else:unix:!macx: PRE_TARGETDEPS += $$PWD/../libwinpthread.a
