#ifndef _p347_HWMON_CLIENT_WRAPPER
#define _p347_HWMON_CLIENT_WRAPPER

#include <assert.h>
#include <stdarg.h>

#include "p347_pwr_user.h"
#include "hardware_monitor.pb.h"
#include "rcf_bind_hwmon_client.hpp"
#include "uki_log.hpp"

#include <boost/signals2.hpp>

//std lib
#include <atomic>
#include <mutex>

#define HWM_ERROR_OFFSET					-4100

#define HWM_SERVER_ALREADY_CONNECTED	HWM_ERROR_OFFSET
#define HWM_SERVER_CONNECTION_FAILED	(HWM_ERROR_OFFSET-1)
#define HWM_SERVER_NOT_CONNECTED		(HWM_ERROR_OFFSET-2)
#define HWM_UNKNOWN_FUNCTION_CODE		(HWM_ERROR_OFFSET-3)
#define HWM_SYNC_FUNCTION_FAILED		(HWM_ERROR_OFFSET-4)
#define HWM_ASYNC_FUNCTION_FAILED		(HWM_ERROR_OFFSET-5)
#define HWM_VOID_INITPARS				(HWM_ERROR_OFFSET-6)
#define HWM_EXCEPTION					(HWM_ERROR_OFFSET-7)

//internal functions codes
enum hwm_client_functions {
	HW_IF_GETSERVERVER = 1,
	HW_IF_CREATE_HWMON,
	HW_IF_DELETE_HWMON,
	HW_IF_ISHWMON_CREATED,
	//-------------------
	HW_IF_CHANGELOGPARAMS,
	HW_IF_CLIENT_SUBSCRIBE,
	HW_IF_CLIENT_UNSUBSCRIBE,
	HW_IF_GET_SUBMASK,			
	HW_IF_DOBEEP,		
	HW_IF_DOSUSPEND,		
	HW_IF_DOSHUTDOWN,		
	HW_IF_DOREBOOT,		
};

class pubHwSigHolder {
public:
	//boost::signals2::signal<void (int error_code)> sigAlarm;
	boost::signals2::signal<void (hardware_monitor::BatteryInformation* bi_ptr)> sigBatteryData;
	boost::signals2::signal<void (hardware_monitor::TemperatureInformation* ti_ptr)> sigTemperatureData;
	boost::signals2::signal<void (hardware_monitor::ButtonInformation* bi_ptr)> sigButtonData;
};

class HwDeviceSubscriber {
public:
	HwDeviceSubscriber() {
	};
	~HwDeviceSubscriber() {
	};
	
	pubHwSigHolder* backlink;

	void batteryData(hardware_monitor::BatteryInformation bi) {
		if (backlink != nullptr) {
			backlink->sigBatteryData(&bi);
		}
	};
	void cpuTemperatureData(hardware_monitor::TemperatureInformation ti) {
		if (backlink != nullptr) {
			backlink->sigTemperatureData(&ti);
		}
	};
	void buttonData(hardware_monitor::ButtonInformation bi) {
		if (backlink != nullptr) {
			backlink->sigButtonData(&bi);
		}
	};
};

class p347HwmonWrapper: public pubHwSigHolder, public UKI_LOG {
public:
	p347HwmonWrapper();
	~p347HwmonWrapper();
	//------------------------------------------------------connectionManaging
	bool getConnectionState();
	int getConnectionError();
	int connectToTcpServer(std::string ip,int port);
	void disconnectFromTcpServer();
	//-------------------------------------------------------
	void getErrorString(int error_code, std::string* dst_string);
	int getLastExceptionInfo(std::string* dst_string);
	int getLastConnectionError(std::string* dst_string);
	//-------------------------------------------------------
	int getServerVersion(bool async, hardware_monitor::ServerVersion* ret_value);
	//-----------------------------------------------------
	int createHardwareMonitor(bool async, const hardware_monitor::HardwareMonitoringClientInitParams & hwcip);
	int deleteHardwareMonitor(bool async);
	int isHwMonitorCreated(bool async, bool* ret_value);
	//-----------------------------------------------------
	int changeLoggingParams(bool async, const hardware_monitor::HwmonLoggingParams & hlp);
	int clientSubscribe(bool async, const hardware_monitor::SubscriptionMask & sm);
	int clientUnsubscribe(bool async);
	int getCurrentSubscriptionMask(bool async, hardware_monitor::SubscriptionMask* ret_value);
	//------------------------------------------------------actions
	int doBeep(bool async, int msec);
	int doSuspend(bool async);
	int doShutdown(bool async);
	int doReboot(bool async);
	//-------------------------------------------------------
	//-------------------------------------------------------
	void onAsyncFunctionCompleted(int if_code);
	
	//---------------------------------------------------------
	boost::signals2::signal<void (int if_code, int connection_error, int function_error)> sigAsyncFunctionComplete;
	boost::signals2::signal<void (int if_code, int connection_error, int function_error)> sigSyncFunctionComplete;
	
	RCF::Future<int> 								fRet_int;
	RCF::Future<hardware_monitor::SubscriptionMask>	fRet_smask;
	RCF::Future<bool> 								fRet_bool;
	RCF::Future<hardware_monitor::ServerVersion>	fRet_version;
	
	HwDeviceSubscriber								subDev;
private:
	//---------------------------------------------------------------
	void*						some_result_pointer;
	//---------------------------------------------------------------
	bool is_async_running;
	
	int _executeFunction(bool async, int if_code, ...);
	int _processResult(int if_code);
	
	std::mutex						cs;
	RcfClient<HWMON_connection>*	client;
	
	std::atomic_bool				is_connected_to_server;
	std::atomic_int					last_connection_error;
	std::string						last_connection_error_str;
	std::atomic_int					last_exception_code;
	std::string						last_exception_error_str;
	
	RCF::RcfServer*					subServer;
	RCF::SubscriptionParms 			subParms;
	RCF::SubscriptionPtr			subPtr;
};

#endif
