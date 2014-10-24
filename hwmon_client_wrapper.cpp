#include "hwmon_client_wrapper.hpp"

p347HwmonWrapper::p347HwmonWrapper() : UKI_LOG() {
	ssize_t rc = RCF::getInitRefCount();
	
	if (rc == 0)
		RCF::init();
	
	client = nullptr;
	subDev.backlink = (pubHwSigHolder*)this;
	
	is_connected_to_server.store(false);
	last_connection_error.store(0);
	
	subServer = new RCF::RcfServer(RCF::TcpEndpoint(-1));
	assert(subServer != nullptr);
	
	setPrefix("HwWrapper");
}

p347HwmonWrapper::~p347HwmonWrapper() {
	disconnectFromTcpServer();

	delete subServer;
}

//--------------------------------------------------------------------------------connectionManaging

int p347HwmonWrapper::connectToTcpServer(std::string ip, int port) {
	logMessage(LOG_LEVEL_FULL,"connectToTcpServer start\n");
	
	if (is_connected_to_server.load()) return HWM_SERVER_ALREADY_CONNECTED;
	
	client = new RcfClient<HWMON_connection>(RCF::TcpEndpoint(ip.c_str(),port));
	assert(client != nullptr);
	
	try {
		subServer->start();
		subParms.setPublisherEndpoint(RCF::TcpEndpoint(ip.c_str(),port));
		subPtr = subServer->createSubscription<PUB_hwmon>(subDev,subParms);
	
		is_connected_to_server.store(true);
		last_connection_error.store(0);
		logMessage(LOG_LEVEL_FULL,"connectToTcpServer end\n");
		return 0;
	}
	catch (const RCF::Exception & e) {
		int ret = e.getErrorId();
		is_connected_to_server.store(false);
		last_connection_error.store(ret);
		cs.lock();
		last_connection_error_str.assign(e.getErrorString());
		cs.unlock();
		logMessage(LOG_LEVEL_CRITICAL,"connectToTcpServer failed with %d\n",ret);
		logMessage(LOG_LEVEL_CRITICAL,"[%s]\n",e.getErrorString().c_str());
		return HWM_SERVER_CONNECTION_FAILED;	
	}
}

void p347HwmonWrapper::disconnectFromTcpServer() {
	logMessage(LOG_LEVEL_FULL,"disconnectFromTcpServer start\n");

	if (client != NULL) {
		subServer->stop();
		
		delete client;
		client = NULL;
	}

	is_connected_to_server = false;
	logMessage(LOG_LEVEL_FULL,"disconnectFromTcpServer end\n");
}

bool p347HwmonWrapper::getConnectionState() {
	return is_connected_to_server.load();
}

int p347HwmonWrapper::getConnectionError() {
	return last_connection_error.load();
}
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

int p347HwmonWrapper::getServerVersion(bool async, hardware_monitor::ServerVersion* ret_value) {
	return _executeFunction(async, HW_IF_GETSERVERVER, ret_value);
}

//-----------------------------------------------------
int p347HwmonWrapper::createHardwareMonitor(bool async, const hardware_monitor::HardwareMonitoringClientInitParams & hwcip) {
	return _executeFunction(async, HW_IF_CREATE_HWMON, &hwcip);
}

int p347HwmonWrapper::deleteHardwareMonitor(bool async) {
	return _executeFunction(async, HW_IF_DELETE_HWMON);
}

int p347HwmonWrapper::isHwMonitorCreated(bool async, bool* ret_value) {
	return _executeFunction(async, HW_IF_ISHWMON_CREATED, ret_value);
}

//-----------------------------------------------------
int p347HwmonWrapper::changeLoggingParams(bool async, const hardware_monitor::HwmonLoggingParams & hlp) {
	return _executeFunction(async, HW_IF_CHANGELOGPARAMS, &hlp);
}

int p347HwmonWrapper::clientSubscribe(bool async, const hardware_monitor::SubscriptionMask & sm) {
	return _executeFunction(async, HW_IF_CLIENT_SUBSCRIBE, &sm);
}

int p347HwmonWrapper::clientUnsubscribe(bool async) {
	return _executeFunction(async, HW_IF_CLIENT_UNSUBSCRIBE);
}

int p347HwmonWrapper::getCurrentSubscriptionMask(bool async, hardware_monitor::SubscriptionMask* ret_value) {
	return _executeFunction(async, HW_IF_GET_SUBMASK, ret_value);
}

//------------------------------------------------------actions
int p347HwmonWrapper::doBeep(bool async, int msec) {
	return _executeFunction(async, HW_IF_DOBEEP, msec);
}

int p347HwmonWrapper::doSuspend(bool async) {
	return _executeFunction(async, HW_IF_DOSUSPEND);
}

int p347HwmonWrapper::doShutdown(bool async) {
	return _executeFunction(async, HW_IF_DOSHUTDOWN);
}

int p347HwmonWrapper::doReboot(bool async) {
	return _executeFunction(async, HW_IF_DOREBOOT);
}

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

int p347HwmonWrapper::_processResult(int if_code) {
	int ret = 0;
	
		switch (if_code) {
			case HW_IF_ISHWMON_CREATED: {
				bool retb = *fRet_bool;
				if (some_result_pointer != nullptr) {
					*((bool*)some_result_pointer) = retb;
				} else {
					logMessage(LOG_LEVEL_CRITICAL,"result_ptr for bool suddenly is NULL!\n");
					ret = -EINVAL;
				}
			break; }
			case HW_IF_GETSERVERVER: {
				hardware_monitor::ServerVersion sv = *fRet_version;
				ret = sv.error_code();
				if (ret < 0) {
					logMessage(LOG_LEVEL_CRITICAL,"getServerVersion ret = %d\n",ret);
				} else {
					if (some_result_pointer != nullptr) {
						((hardware_monitor::ServerVersion*)(some_result_pointer))->CopyFrom(sv);
					} else {
						logMessage(LOG_LEVEL_CRITICAL,"result_ptr for getServerVersion suddenly is NULL!\n");
						ret = -EINVAL;
					}
				}					
			break; }
			case HW_IF_GET_SUBMASK: {
				hardware_monitor::SubscriptionMask sm = *fRet_smask;
				ret = sm.error_code();
				if (ret < 0) {
					logMessage(LOG_LEVEL_CRITICAL,"getCurrentSubscriptionMask ret = %d\n",ret);
				} else {
					if (some_result_pointer != nullptr) {
						((hardware_monitor::SubscriptionMask*)(some_result_pointer))->CopyFrom(sm);
					} else {
						logMessage(LOG_LEVEL_CRITICAL,"result_ptr for getCurrentSubscriptionMask suddenly is NULL!\n");
						ret = -EINVAL;
					}
				}					
			break; }
			default: { //supposed int return value, and nothing more
				ret = *fRet_int;
			break; }
		};		
	return ret;
}

void p347HwmonWrapper::onAsyncFunctionCompleted(int if_code) {
	int ret = 0;
	std::auto_ptr<RCF::Exception> ePtr = client->getClientStub().getAsyncException();
	if (ePtr.get()) {
		ret = ePtr->getErrorId();
		last_connection_error.store(ret);
		cs.lock();
		last_connection_error_str.assign(ePtr->getErrorString());
		cs.unlock();
		logMessage(LOG_LEVEL_CRITICAL,"Async function[%d] failed with %d\n",if_code,ret);
		logMessage(LOG_LEVEL_CRITICAL,"[%s]\n",last_connection_error_str.c_str());
		
		disconnectFromTcpServer();
	
		sigAsyncFunctionComplete(if_code,HWM_ASYNC_FUNCTION_FAILED,ret);
	} else {
		ret = _processResult(if_code);
		
		logMessage(LOG_LEVEL_FULL,"ASYNC RET of function[%d] ret = %d\n",if_code,ret);
		
		sigAsyncFunctionComplete(if_code,0,ret);
	}
};
#define CALLBACK_FUNC				RCF::AsyncTwoway( boost::bind(&p347HwmonWrapper::onAsyncFunctionCompleted, this, if_code) )

//--------------------------------------------------------------------------------Private

int p347HwmonWrapper::_executeFunction(bool async, int if_code, ...) {
	logMessage(LOG_LEVEL_FULL,"is_connected_to_server.load() = %d\n",is_connected_to_server.load());
	if (!is_connected_to_server.load()) return HWM_SERVER_NOT_CONNECTED;

	va_list ap;
	
	try {
	
		switch (if_code) {
			case HW_IF_GETSERVERVER: {
				va_start(ap,if_code);
				some_result_pointer = (void*)va_arg(ap,hardware_monitor::ServerVersion*);
				va_end(ap);
				if (async) {
					fRet_version = client->getServerVersion(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async getServerVersion called\n");
				} else {	
					fRet_version = client->getServerVersion();
					logMessage(LOG_LEVEL_FULL,"sync getServerVersion called\n");
					while (!fRet_version.ready()) RCF::sleepMs(100);				
				}					
			break; }
			case HW_IF_CREATE_HWMON: {
				va_start(ap,if_code);
				hardware_monitor::HardwareMonitoringClientInitParams* initpars = va_arg(ap,hardware_monitor::HardwareMonitoringClientInitParams*);
				va_end(ap);
				
				if (async) {
					fRet_int = client->createHardwareMonitor(CALLBACK_FUNC, *initpars);
					logMessage(LOG_LEVEL_FULL,"async createHardwareMonitor called\n");
				} else {	
					fRet_int = client->createHardwareMonitor(*initpars);
					logMessage(LOG_LEVEL_FULL,"sync createHardwareMonitor called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case HW_IF_DELETE_HWMON: {
				if (async) {
					fRet_int = client->deleteHardwareMonitor(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async deleteHardwareMonitor called\n");
				} else {	
					fRet_int = client->deleteHardwareMonitor();
					logMessage(LOG_LEVEL_FULL,"sync deleteHardwareMonitor called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case HW_IF_ISHWMON_CREATED: {
				va_start(ap,if_code);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->isHwMonitorCreated(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async isHwMonitorCreated called\n");
				} else {	
					fRet_bool = client->isHwMonitorCreated();
					logMessage(LOG_LEVEL_FULL,"sync isHwMonitorCreated called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);				
				}			
			break; }
			case HW_IF_CHANGELOGPARAMS: {
				va_start(ap,if_code);
				hardware_monitor::HwmonLoggingParams* hlp = va_arg(ap,hardware_monitor::HwmonLoggingParams*);
				va_end(ap);
				if (async) {
					fRet_int = client->changeLoggingParams(CALLBACK_FUNC,*hlp);
					logMessage(LOG_LEVEL_FULL,"async changeLoggingParams called\n");
				} else {	
					fRet_int = client->changeLoggingParams(*hlp);
					logMessage(LOG_LEVEL_FULL,"sync changeLoggingParams called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case HW_IF_CLIENT_SUBSCRIBE: {
				va_start(ap,if_code);
				hardware_monitor::SubscriptionMask* sm = va_arg(ap,hardware_monitor::SubscriptionMask*);
				va_end(ap);
				if (async) {
					fRet_int = client->clientSubscribe(CALLBACK_FUNC,*sm);
					logMessage(LOG_LEVEL_FULL,"async clientSubscribe called\n");
				} else {	
					fRet_int = client->clientSubscribe(*sm);
					logMessage(LOG_LEVEL_FULL,"sync clientSubscribe called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case HW_IF_CLIENT_UNSUBSCRIBE: {
				if (async) {
					fRet_int = client->clientUnsubscribe(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async clientUnsubscribe called\n");
				} else {	
					fRet_int = client->clientUnsubscribe();
					logMessage(LOG_LEVEL_FULL,"sync clientUnsubscribe called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case HW_IF_GET_SUBMASK: {
				va_start(ap,if_code);
				some_result_pointer = (void*)va_arg(ap,hardware_monitor::SubscriptionMask*);
				va_end(ap);
				
				if (async) {
					fRet_smask = client->getCurrentSubscriptionMask(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async getCurrentSubscriptionMask called\n");
				} else {	
					fRet_smask = client->getCurrentSubscriptionMask();
					logMessage(LOG_LEVEL_FULL,"sync getCurrentSubscriptionMask called\n");
					while (!fRet_smask.ready()) RCF::sleepMs(100);				
				}
			break; }
			case HW_IF_DOBEEP: {
				va_start(ap,if_code);
				int msec = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->doBeep(CALLBACK_FUNC,msec);
					logMessage(LOG_LEVEL_FULL,"async doBeep called\n");
				} else {	
					fRet_int = client->doBeep(msec);
					logMessage(LOG_LEVEL_FULL,"sync doBeep called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }		
			case HW_IF_DOSUSPEND: {
				if (async) {
					fRet_int = client->doSuspend(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async doSuspend called\n");
				} else {	
					fRet_int = client->doSuspend();
					logMessage(LOG_LEVEL_FULL,"sync doSuspend called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case HW_IF_DOSHUTDOWN: {
				if (async) {
					fRet_int = client->doShutdown(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async doShutdown called\n");
				} else {	
					fRet_int = client->doShutdown();
					logMessage(LOG_LEVEL_FULL,"sync doShutdown called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case HW_IF_DOREBOOT: {
				if (async) {
					fRet_int = client->doReboot(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async doReboot called\n");
				} else {	
					fRet_int = client->doReboot();
					logMessage(LOG_LEVEL_FULL,"sync doReboot called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }			

			default:
				return HWM_UNKNOWN_FUNCTION_CODE;
		}
	}
	catch (const RCF::Exception & e) {
		int ret = e.getErrorId();
		logMessage(LOG_LEVEL_CRITICAL,"CATCH EXCEPTION[%d] [%s]\n",ret,e.getErrorString().c_str());
		last_exception_code.store(ret);
		cs.lock();
		last_exception_error_str.assign(e.getErrorString());
		cs.unlock();
		return HWM_SERVER_CONNECTION_FAILED;	
	}
	
	if (!async) {
		std::auto_ptr<RCF::Exception> ePtr = client->getClientStub().getAsyncException();
		if (ePtr.get()) {
			int ret = ePtr->getErrorId();
			last_connection_error.store(ret);
			cs.lock();
			last_connection_error_str.assign(ePtr->getErrorString());
			cs.unlock();
			logMessage(LOG_LEVEL_CRITICAL,"_executeFunction[%d] failed with %d\n",if_code,ret);
			logMessage(LOG_LEVEL_CRITICAL,"[%s]\n",last_connection_error_str.c_str());
			
			//connection_fails_counter++;
			disconnectFromTcpServer();
			
			sigSyncFunctionComplete(if_code,HWM_SYNC_FUNCTION_FAILED,ret);
			return HWM_SYNC_FUNCTION_FAILED;	
		} else {
			//connection_fails_counter.store(0);
			int ret = _processResult(if_code);
			sigSyncFunctionComplete(if_code,0,ret);
			return ret;
		}
	} else {
		last_connection_error.store(0);
		return 0;
	}
}

//--------------------------------------------------------------------------------Errors Decoding

int p347HwmonWrapper::getLastExceptionInfo(std::string* dst_string) {
	if (dst_string == NULL) return -EINVAL;
	
	cs.lock();
	dst_string->assign(last_exception_error_str);
	cs.unlock();
	
	return last_exception_code.load();
}

int p347HwmonWrapper::getLastConnectionError(std::string* dst_string) {
	if (dst_string == NULL) return -EINVAL;
	
	cs.lock();
	dst_string->assign(last_connection_error_str);
	cs.unlock();
	
	return last_connection_error.load();
}

void p347HwmonWrapper::getErrorString(int error_code, std::string* dst_string) {
	if (dst_string == nullptr) return;
	
	switch (error_code) {
		case HWM_SERVER_ALREADY_CONNECTED: {
			dst_string->assign("Server is already connected, do not try to connect it again");
		break; }
		case HWM_SERVER_CONNECTION_FAILED: {
			dst_string->assign("Cannot establish connection with server! Call getLastConnectionError for details");
		break; }
		case HWM_SERVER_NOT_CONNECTED: {
			dst_string->assign("Server is not connected! You must call connectToTcpServer first.");
		break; }
		case HWM_UNKNOWN_FUNCTION_CODE: {
			dst_string->assign("Internal Wrapper error: unknown function code");
		break; }
		case HWM_SYNC_FUNCTION_FAILED: {
			dst_string->assign("Synchronous execution failed due to connection problems! Call getLastConnectionError for details");
		break; }
		case HWM_ASYNC_FUNCTION_FAILED: {
			dst_string->assign("Asynchronous execution failed due to connection problems! Call getLastConnectionError for details");
		break; }
		case HWM_VOID_INITPARS: {
			dst_string->assign("You have passed void instead of pointer to init params");
		break; }
		case HWM_EXCEPTION: {
			dst_string->assign("Exception catched! Call getLastExceptionInfo for details");
		break; }
		default: {
			dst_string->assign("This is not a Client Wrapper's error");
		break; }
	}
}
