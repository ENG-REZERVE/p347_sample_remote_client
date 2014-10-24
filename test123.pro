#TEMPLATE = app
CONFIG += console

QT += qml quick widgets
TARGET = client_test

INCLUDEPATH += C:\protobuf-2.6.0\src \
    C:\RCF-2.0.1.101\include \
	C:\boost_1_55_0

HEADERS += channel_manager.pb.h \
	task_manager.pb.h \
	hardware_monitor.pb.h \
    rcf_bind_channel_manager.hpp \
	rcf_bind_hwmon_client.hpp \
	p347_fpga_user.h \
	client_wrapper.hpp \
	hwmon_client_wrapper.hpp \
	uki_log.hpp \
	os_define.h \
	syncobjs.hpp \
	thread.hpp \
	p347_ipc_global.h \
	p347_ipc_common.h \
	qt_client_wrapper.hpp \
	qt_hwmon_wrapper.hpp \
	DSPEmulDefines.h \
	list.h \
	FixedPoint.h \
	RobastAnalisis.h \
	p347_pwr_user.h
	
#	calib_data_structures.h \
#	calib_enums.h \
#	
#	
#	
#	
#	smartenums.h \
#	ini_local_conf.h

SOURCES += main.cpp \
	task_manager_pb.cpp \
    channel_manager_pb.cpp \
	hardware_monitor_pb.cpp \
	client_wrapper.cpp \
	hwmon_client_wrapper.cpp \
	uki_log.cpp \
	syncobjs.cpp \
	thread.cpp \
	qt_client_wrapper.cpp \
	qt_hwmon_wrapper.cpp \
	list.cpp

RESOURCES += qml.qrc

QMAKE_CC = C:\Qt\Qt5.3.1\Tools\mingw482_32\bin\gcc.exe
QMAKE_CXX = C:\Qt\Qt5.3.1\Tools\mingw482_32\bin\g++.exe

QMAKE_CFLAGS += -DRCF_USE_PROTOBUF -DWIN32_LEAN_AND_MEAN -Wno-unused-parameter -Wno-unused-variable -Wno-unused-local-typedefs -Wno-deprecated-declarations -std=c++11 -ggdb -O0
QMAKE_CXXFLAGS += -DRCF_USE_PROTOBUF -DWIN32_LEAN_AND_MEAN -Wno-unused-parameter -Wno-unused-variable -Wno-unused-local-typedefs -Wno-deprecated-declarations -std=c++11 -ggdb -O0

QMAKE_LIBS += -LC:\Qt\Qt5.3.1\Tools\QtCreator\bin\test123 -lrcf_win32 -lprotobuf-9

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
