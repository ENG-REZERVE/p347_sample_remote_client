#ifndef _p347_HWMON_QTCLIENT_WRAPPER
#define _p347_HWMON_QTCLIENT_WRAPPER

#include <QObject>
#include <QString>
#include "hwmon_client_wrapper.hpp"

class QHwmonWrapper: public QObject, public p347HwmonWrapper 
{
	Q_OBJECT
public:
	explicit QHwmonWrapper(QObject *parent = 0);
	~QHwmonWrapper();
	
	hardware_monitor::BatteryInformation 		bi;
	hardware_monitor::TemperatureInformation 	ti;
	hardware_monitor::ButtonInformation 		bui;
	hardware_monitor::ServerVersion 			s_version;
	hardware_monitor::SubscriptionMask			sm;
	
	bool is_hw_created;
	bool is_async_running;
	
	Q_PROPERTY(bool isAsyncRunning READ isAsyncRunning NOTIFY isAsyncRunningChanged)
	
	Q_PROPERTY(int batteryPercent READ batteryPercent NOTIFY batteryChanged)
	Q_PROPERTY(bool outerPower READ outerPower NOTIFY batteryChanged)
	Q_PROPERTY(bool isCharging READ isCharging NOTIFY batteryChanged)
	Q_PROPERTY(bool lowVoltageAlarm READ lowVoltageAlarm NOTIFY batteryChanged)
	
	Q_PROPERTY(double midCPUTemp READ midCPUTemp NOTIFY CPUTempChanged)
	Q_PROPERTY(bool isButtonPressed READ isButtonPressed NOTIFY buttonChanged)
	
	Q_PROPERTY(bool isHwCreated READ isHwCreated NOTIFY hwCreatedChanged)
	
	Q_PROPERTY(QString serverVersion READ serverVersion NOTIFY svChanged)
	Q_PROPERTY(QString subMask READ subMask NOTIFY smChanged)

	int batteryPercent();
	bool outerPower();
	bool isCharging();
	bool lowVoltageAlarm();
	double midCPUTemp();
	bool isButtonPressed();
	bool isHwCreated();
	bool isAsyncRunning();
	QString serverVersion();
	QString subMask();
	
	void onAsyncCompleted(int if_code, int connection_error, int function_error);
	void onSyncCompleted(int if_code, int connection_error, int function_error);
	void onBatteryData(hardware_monitor::BatteryInformation* bi_ptr);
	void onTemperatureData(hardware_monitor::TemperatureInformation* ti_ptr);
	void onButtonData(hardware_monitor::ButtonInformation* bi_ptr);
	
	
signals:
	void batteryChanged();
	void CPUTempChanged();
	void buttonChanged();
	void svChanged();
	void smChanged();
	void hwCreatedChanged();
	void isAsyncRunningChanged();
public slots:
	//----------------------------------------------------------
	//bool qt_getConnectionState();
	//int qt_getConnectionError();
	int qt_connectToTcpServer(QString ip,int port);
	int qt_disconnectFromTcpServer();
	//-------------------------------------------------------
	//void qt_getErrorString(int error_code, std::string* dst_string);
	//int qt_getLastExceptionInfo(std::string* dst_string);
	//int qt_getLastConnectionError(std::string* dst_string);
	//-------------------------------------------------------
	int qt_getServerVersion(bool async);
	//-----------------------------------------------------
	int qt_createHardwareMonitor(bool async);
	int qt_deleteHardwareMonitor(bool async);
	int qt_isHwMonitorCreated(bool async);
	//-----------------------------------------------------
	//int qt_changeLoggingParams(bool async);
	int qt_clientSubscribe(bool async, bool bat, bool temp, bool but_view, bool but_act);
	int qt_clientUnsubscribe(bool async);
	int qt_getCurrentSubscriptionMask(bool async);
	//------------------------------------------------------actions
	int qt_doBeep(bool async, int msec);
	int qt_doSuspend(bool async);
	int qt_doShutdown(bool async);
	int qt_doReboot(bool async);
private:
	boost::signals2::connection		connection_async_complete;
	boost::signals2::connection		connection_sync_complete;
	boost::signals2::connection		connection_battery_data;
	boost::signals2::connection		connection_temperature_data;
	boost::signals2::connection		connection_button_data;
	
	void _gui_reaction(int if_code);
};

#endif
