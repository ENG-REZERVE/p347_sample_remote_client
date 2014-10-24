#include "qt_hwmon_wrapper.hpp"
#include <errno.h>

QHwmonWrapper::QHwmonWrapper(QObject* parent) : 
	QObject(parent), p347HwmonWrapper()
{
	bi.set_error_code(-ERANGE);
	bui.set_error_code(-ERANGE);
	ti.set_error_code(-ERANGE);

	connection_async_complete = sigAsyncFunctionComplete.connect(boost::bind(&QHwmonWrapper::onAsyncCompleted,this,_1,_2,_3));
	connection_sync_complete = sigSyncFunctionComplete.connect(boost::bind(&QHwmonWrapper::onSyncCompleted,this,_1,_2,_3));
	connection_battery_data = sigBatteryData.connect(boost::bind(&QHwmonWrapper::onBatteryData,this,_1));
	connection_temperature_data = sigTemperatureData.connect(boost::bind(&QHwmonWrapper::onTemperatureData,this,_1));
	connection_button_data = sigButtonData.connect(boost::bind(&QHwmonWrapper::onButtonData,this,_1));
}

QHwmonWrapper::~QHwmonWrapper() {
	connection_async_complete.disconnect();
	connection_sync_complete.disconnect();
	connection_battery_data.disconnect();
	connection_temperature_data.disconnect();
	connection_button_data.disconnect();
}

inline void QHwmonWrapper::_gui_reaction(int if_code) {
	switch (if_code) {
		case HW_IF_GETSERVERVER: {
			emit svChanged();
		break; }
		case HW_IF_CREATE_HWMON: {
			// emit ();
		break; }
		case HW_IF_DELETE_HWMON: {
			// emit ();
		break; }
		case HW_IF_ISHWMON_CREATED: {
			emit hwCreatedChanged();
		break; }
		case HW_IF_CHANGELOGPARAMS: {
			// emit ();
		break; }
		case HW_IF_CLIENT_SUBSCRIBE: {
			// emit ();
		break; }
		case HW_IF_CLIENT_UNSUBSCRIBE: {
			// emit ();
		break; }
		case HW_IF_GET_SUBMASK: {
			emit smChanged();
		break; }			
		case HW_IF_DOBEEP: {
			// emit ();
		break; }		
		case HW_IF_DOSUSPEND: {
			// emit ();
		break; }		
		case HW_IF_DOSHUTDOWN: {
			// emit ();
		break; }		
		case HW_IF_DOREBOOT: {
			// emit ();
		break; }		
	}
}
	
void QHwmonWrapper::onAsyncCompleted(int if_code, int connection_error, int function_error) {
	printf("onAsyncCompleted if_code=%d, connection_error=%d, function_error=%d\n",if_code,connection_error,function_error);
	_gui_reaction(if_code);
	
	is_async_running = false;
	emit isAsyncRunningChanged();
}

void QHwmonWrapper::onSyncCompleted(int if_code, int connection_error, int function_error) {
	printf("onSyncCompleted if_code=%d, connection_error=%d, function_error=%d\n",if_code,connection_error,function_error);
	_gui_reaction(if_code);
}

void QHwmonWrapper::onBatteryData(hardware_monitor::BatteryInformation* bi_ptr) {
	if (bi_ptr != nullptr) {
		bi.CopyFrom(*bi_ptr);
		emit batteryChanged();
	}
}

void QHwmonWrapper::onTemperatureData(hardware_monitor::TemperatureInformation* ti_ptr) {
	if (ti_ptr != nullptr) {
		ti.CopyFrom(*ti_ptr);
		emit CPUTempChanged();
	}
}

void QHwmonWrapper::onButtonData(hardware_monitor::ButtonInformation* bi_ptr) {
	if (bi_ptr != nullptr) {
		bui.CopyFrom(*bi_ptr);
		emit buttonChanged();
	}
}

//---------------------------------------------------------------------------------------properties
int QHwmonWrapper::batteryPercent() {
	int ret = -1;
	if (bi.has_charge_percent())
		ret = bi.charge_percent();
	return ret;
}

bool QHwmonWrapper::outerPower() {
	bool ret = -1;
	if (bi.has_outer_power_presense())
		ret = bi.outer_power_presense();
	return ret;
}

bool QHwmonWrapper::isCharging() {
	bool ret = -1;
	if (bi.has_is_charging())
		ret = bi.is_charging();
	return ret;
}

bool QHwmonWrapper::lowVoltageAlarm() {
	bool ret = -1;
	if (bi.has_alarm_low_voltage())
		ret = bi.alarm_low_voltage();
	return ret;
}

double QHwmonWrapper::midCPUTemp() {
	double ret = -273;
	if (ti.has_t_min() && ti.has_t_max()) {
		ret = 0.5*(ti.t_min() + ti.t_max());
	}
	return ret;
}

bool QHwmonWrapper::isHwCreated() {
	return is_hw_created;
}

bool QHwmonWrapper::isButtonPressed() {
	bool ret = false;
	if (bui.has_is_pressed())
		ret = bui.is_pressed();
	return ret;
}

QString QHwmonWrapper::serverVersion() {
	QString rets = tr("%1.%2.").arg(s_version.major()).arg(s_version.minor()) + QString::fromStdString(s_version.build());
	return rets;
}

QString QHwmonWrapper::subMask() {
	QString rets = "";
	if (sm.has_battery())
		if (sm.battery()) rets.append("CHG ");
	if (sm.has_cpu_temperature())
		if (sm.cpu_temperature()) rets.append("TEM ");
	if (sm.has_pbutton_info())
		if (sm.pbutton_info()) rets.append("BTI ");		
	if (sm.has_pbutton_action())
		if (sm.pbutton_action()) rets.append("BTA ");	
		
	return rets;
}

//---------------------------------------------------------------------------------------qt-wrapping
int QHwmonWrapper::qt_connectToTcpServer(QString ip,int port) {
	return connectToTcpServer(ip.toStdString(),port);
}

int QHwmonWrapper::qt_disconnectFromTcpServer() {
	disconnectFromTcpServer();
	return 0;
}

bool QHwmonWrapper::isAsyncRunning() {
	return is_async_running;
}

//---------------------------------------------------------------------------------------
#define UPDATE_BUSY \
	if (async) { is_async_running = true; emit isAsyncRunningChanged(); }

int QHwmonWrapper::qt_getServerVersion(bool async){
	UPDATE_BUSY
	return getServerVersion(async,&s_version);
}
	
int QHwmonWrapper::qt_createHardwareMonitor(bool async) {
	UPDATE_BUSY
	
	hardware_monitor::HardwareMonitoringClientInitParams hwcip;
	hwcip.set_srv_conn_wait_msec(500); 
	hwcip.set_srv_conn_retry_cnt(4);
	/* //let them use defaults
	hwcip.set_journal_ipc_path = 3   
	hwcip.set_journal_ipc_key = 4    
	hwcip.set_hwserver_ipc_path = 5  
	hwcip.set_hwserver_ipc_key = 6   
	hwcip.set_own_ipc_path = 7       
	hwcip.set_own_ipc_key = 8        
	*/
	return createHardwareMonitor(async, hwcip);
}

int QHwmonWrapper::qt_deleteHardwareMonitor(bool async) {
	UPDATE_BUSY
	return deleteHardwareMonitor(async);
}

int QHwmonWrapper::qt_isHwMonitorCreated(bool async) {
	UPDATE_BUSY
	return isHwMonitorCreated(async,&is_hw_created);
}

//-----------------------------------------------------

int QHwmonWrapper::qt_clientSubscribe(bool async, bool bat, bool temp, bool but_view, bool but_act) {
	UPDATE_BUSY
	
	hardware_monitor::SubscriptionMask smask;
	smask.set_error_code(0);
	smask.set_battery(bat);
	smask.set_cpu_temperature(temp);
	smask.set_pbutton_info(but_view);
	smask.set_pbutton_action(but_act);
	
	return clientSubscribe(async, smask);
}

int QHwmonWrapper::qt_clientUnsubscribe(bool async) {
	UPDATE_BUSY
	return clientUnsubscribe(async);
}

int QHwmonWrapper::qt_getCurrentSubscriptionMask(bool async) {
	UPDATE_BUSY
	return getCurrentSubscriptionMask(async,&sm);
}

//------------------------------------------------------actions
int QHwmonWrapper::qt_doBeep(bool async, int msec) {
	UPDATE_BUSY
	return doBeep(async,msec);
}

int QHwmonWrapper::qt_doSuspend(bool async) {
	UPDATE_BUSY
	return doSuspend(async);
}

int QHwmonWrapper::qt_doShutdown(bool async) {
	UPDATE_BUSY
	return doShutdown(async);
}

int QHwmonWrapper::qt_doReboot(bool async) {
	UPDATE_BUSY
	return doReboot(async);
}

	