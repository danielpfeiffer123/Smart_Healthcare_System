QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Session.cpp \
    addfrienddialog.cpp \
    appointment.cpp \
    chatinfomanage.cpp \
    custom/ToolBoxPow.cpp \
    custom/chatmessage.cpp \
    logindlg.cpp \
    main.cpp \
    patientinfo.cpp \
    pre.cpp \
    registdlg.cpp \
    search.cpp \
    widget.cpp

HEADERS += \
    Session.h \
    addfrienddialog.h \
    appointment.h \
    chatinfomanage.h \
    common.h \
    custom/ToolBoxPow.h \
    custom/chatmessage.h \
    logindlg.h \
    patientinfo.h \
    pre.h \
    registdlg.h \
    search.h \
    widget.h

FORMS += \
    addfrienddialog.ui \
    appointment.ui \
    logindlg.ui \
    patientinfo.ui \
    pre.ui \
    registdlg.ui \
    search.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc

RC_ICONS = wechat.ico
