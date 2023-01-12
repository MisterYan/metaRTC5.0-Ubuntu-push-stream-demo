QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Copyright (c) 2019-2022 yanggaofeng

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#vc

HOME_BASE=../
unix{

    INCLUDEPATH += $$HOME_BASE/include
    CONFIG(debug, debug|release) {
        LIBS += -L$$HOME_BASE/bin/lib_debug
        DESTDIR += $$HOME_BASE/bin/app_debug
    }else{
        LIBS += -L$$HOME_BASE/bin/lib_release
        DESTDIR += $$HOME_BASE/bin/app_release
    }

 LIBS += -L$$HOME_BASE/thirdparty/lib -lmetartc5  -lmetartccore5  -lyuv -lopus -lopenh264 -lspeexdsp -lusrsctp -lpthread -lasound -ldl
#openssl
 #LIBS += -lssl2 -lcrypto2 -lsrtp2
#gmssl
 #LIBS += -lssl_gm -lcrypto_gm -lmetasrtp3

#mbtls
 LIBS += -lmbedtls -lmbedx509 -lmbedcrypto -lsrtp2_mbed
}

win32{
    INCLUDEPATH += $$HOME_BASE\include
    CONFIG(debug, debug|release) {
        LIBS += -L$$HOME_BASE/bin/lib_win_debug
        DESTDIR += $$HOME_BASE/bin/app_win_debug
    }else{
        LIBS += -L$$HOME_BASE/bin/lib_win_release
        DESTDIR += $$HOME_BASE/bin/app_win_release
    }
    LIBS += -lmetartc5 -lmetartccore5 -lyuv -lopus -lopenh264  -lspeexdsp -lavutil -lavcodec -lusrsctp -lksuser -lwinmm  -lole32 -lStrmiids
    #gpu encode and decode
    #LIBS +=  -lyangwincodec5  -ld3d11 -ld3d9 -ldxva2
    #using vr
    #LIBS += -lyangrtcext2


    YANG_LIB=  -L$$HOME_BASE/thirdparty/lib/win -lsrtp2  -lssl  -lcrypto
    msvc{
        #QMAKE_CFLAGS += /utf-8
        #QMAKE_CXXFLAGS += /utf-8
        QMAKE_LFLAGS    += /ignore:4099
        DEFINES +=HAVE_STRUCT_TIMESPEC
        DEFINES +=WIN32_LEAN_AND_MEAN
        INCLUDEPATH += $$HOME_BASE\thirdparty\include\win\include   #vc
        YANG_LIB=  -L$$HOME_BASE/thirdparty/lib/win/msvc -lpthreadVC2 -lavrt -luser32 -lAdvapi32
        #openssl
        YANG_LIB+= -lsrtp2  -llibcrypto -llibssl
        #gmssl
        #YANG_LIB+= -lmetasrtp3  -llibcrypto_gm -llibssl_gm
        QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
    }

    LIBS +=  $$YANG_LIB
    LIBS +=   -lCrypt32 -lws2_32
}

SOURCES += \
    main.cpp \
    recordmainwindow.cpp \
    video/YangPlayWidget.cpp \
    video/yangrecordthread.cpp \
    video/yangrecordvideowin.cpp


HEADERS += \
    recordmainwindow.h \
    video/YangPlayWidget.h \
    video/yangrecordthread.h \
    video/yangrecordvideowin.h \
    video/yangvideotype.h

FORMS += \
    recordmainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
