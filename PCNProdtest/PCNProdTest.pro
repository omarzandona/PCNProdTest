#-------------------------------------------------
#
# Project created by QtCreator 2013-11-13T12:59:11
#
#-------------------------------------------------

QT += core gui
QT += widgets

TARGET = PCNProdTest
TEMPLATE = app

RC_FILE = PCNProdTest.rc


SOCKET_API = Ws2_32.lib
LIBS += -L$$SOCKET_API -lWs2_32

SNP_LIB = C:/SNP_Lib
LIBS += -L$$SNP_LIB -lSNP

SOURCES += main.cpp\
        mainwindow.cpp \
    socket_utils.cpp \
    utils.cpp \
    acq_thread.cpp \
    CheckPCN/GraphDisparity.cpp \
    CheckPCN/check_pcn.cpp \
    parameters_panel.cpp

HEADERS  += mainwindow.h \
    SOCKET/apc_defines.h \
    define_file.h \
    socket_utils.h \
    utils.h \
    connected.h \
    structures.h \
    acq_thread.h \
    CheckPCN/GraphDisparity.h \
    CheckPCN/connected.h \
    CheckPCN/check_pcn.h \
    parameters_panel.h \
    SNP/SNP.h

FORMS  += mainwindow.ui \
    parameters_panel.ui

OTHER_FILES += \
    PCNProdTest.rc

RESOURCES += \
    main.qrc
