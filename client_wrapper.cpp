#include "client_wrapper.hpp"

//p347ClientWrapper::p347ClientWrapper() : TThread(true), UKI_LOG() {
p347ClientWrapper::p347ClientWrapper() : UKI_LOG() {
	ssize_t rc = RCF::getInitRefCount();
	//printf("constructor: init ref count = %d\n",rc);
	
	if (rc == 0)
		RCF::init();
	
	client = nullptr;
	subDev.backlink = (pubSigHolder*)this;
	
	is_connected_to_server.store(false);
	last_connection_error.store(0);
	//connection_fails_counter.store(0);
	
	subServer = new RCF::RcfServer(RCF::TcpEndpoint(-1));
	assert(subServer != nullptr);
	
	setPrefix("ClWrapper");
}

p347ClientWrapper::~p347ClientWrapper() {
	disconnectFromTcpServer();

	delete subServer;
}

//--------------------------------------------------------------------------------connectionManaging

int p347ClientWrapper::connectToTcpServer(std::string ip, int port) {
	logMessage(LOG_LEVEL_FULL,"connectToTcpServer start\n");
	
	if (is_connected_to_server.load()) return TCP_SERVER_ALREADY_CONNECTED;
	
	client = new RcfClient<CM_connection>(RCF::TcpEndpoint(ip.c_str(),port));
	assert(client != nullptr);
	
	try {
		subServer->start();
		subParms.setPublisherEndpoint(RCF::TcpEndpoint(ip.c_str(),port));
		subPtr = subServer->createSubscription<PUB_connection>(subDev,subParms);
		//subPtr = subServer->createSubscription<PUB_connection>(this,subParms);
	
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
		return TCP_SERVER_CONNECTION_FAILED;	
	}
}

void p347ClientWrapper::disconnectFromTcpServer() {
	logMessage(LOG_LEVEL_FULL,"disconnectFromTcpServer start\n");

	if (client != NULL) {
		//subPtr->close();
		subServer->stop();
		
		delete client;
		client = NULL;
	}

	is_connected_to_server = false;
	logMessage(LOG_LEVEL_FULL,"disconnectFromTcpServer end\n");
}

bool p347ClientWrapper::getConnectionState() {
	return is_connected_to_server.load();
	//return is_connected_to_server;
}

int p347ClientWrapper::getConnectionError() {
	return last_connection_error.load();
	//return last_connection_error;
}
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
/*
int p347ClientWrapper::callRemoteSleepTest(bool async) {
	int ret;
	for (int i=0; i<10; i++) {
		ret = _executeFunction(async,CW_IF_TEST_SLEEP,i);
		logMessage(LOG_LEVEL_FULL,"CW_IF_TEST_SLEEP start %d\n",i);
	}
	return ret;
}
*/
int p347ClientWrapper::getAvailableChannels(bool async, channel_manager::AvailableChannels* ret_value) {
	return _executeFunction(async, CW_IF_GET_AVCHANNELS,ret_value);
}

int p347ClientWrapper::isChannelAvailable(bool async, int ch_idx, bool* ret_value) {
	return _executeFunction(async, CW_IF_ISCHANNELAV,ch_idx,ret_value);
}

int p347ClientWrapper::isChannelsCreated(bool async, bool* ret_value) {
	return _executeFunction(async, CW_IF_ISCHANNELSCR, ret_value);
}

int p347ClientWrapper::getServerVersion(bool async, channel_manager::ServerVersion* ret_value) {
	return _executeFunction(async, CW_IF_GETSERVERVER,ret_value);
}	
	
//--------------------------------------------------------------------------------Create Basics

int p347ClientWrapper::initDevice(bool async, const p347_conf::DeviceInitParams & dip) {
	return _executeFunction(async, CW_IF_INIT_DEVICE,&dip);
}

int p347ClientWrapper::deInitDevice(bool async) {
	return _executeFunction(async, CW_IF_DEINIT_DEVICE);
}

/*
int p347ClientWrapper::createChannelManager(bool async, 
		const channel_manager::ChannelManagerInitParams & cmip, 
		const task_manager::DSPEmulInitParams & dspeip) {
	return _executeFunction(async, CW_IF_CREATE_CM,&cmip,&dspeip);
}

int p347ClientWrapper::deleteChannelManager(bool async) {
	return _executeFunction(async, CW_IF_DELETE_CM);
}
int p347ClientWrapper::initMultiplexer(bool async, const channel_manager::MultiplexerInitParams & mip) {
	return _executeFunction(async, CW_IF_INIT_MUX, &mip);
}

int p347ClientWrapper::deinitMultiplexer(bool async) {
	return _executeFunction(async, CW_IF_DEINIT_MUX);
}	
*/
int p347ClientWrapper::FPGADriverReload(bool async, const std::string & old_name,const std::string & new_path) {
	return _executeFunction(async, CW_IF_FPGA_DRELOAD, &old_name, &new_path);
}

int p347ClientWrapper::checkFPGAStatus(bool async) {
	return _executeFunction(async, CW_IF_CHECKFPGA);
}
//------------------------------------------------------------------------------------

int p347ClientWrapper::readRotData(bool async, unsigned char ch_idx, channel_manager::RotData* ret_value) {
	int ret = 0;
	ret = _executeFunction(async, CW_IF_READROT, ch_idx, ret_value);
	return ret;
}

int p347ClientWrapper::doStartRotChannel(bool async, const channel_manager::RotChannelInitParams & rcip) {
	return _executeFunction(async, CW_IF_STARTROT, &rcip);
}

int p347ClientWrapper::stopRotChannel(bool async, unsigned char ch_idx) {
	return _executeFunction(async, CW_IF_STOPROT, ch_idx);
}

int p347ClientWrapper::isRotRunning(bool async, unsigned char ch_idx, bool* ret_value) {
	return _executeFunction(async, CW_IF_ISROTRUN, ch_idx, ret_value);
	//TODO
}

int p347ClientWrapper::isAnyRotChannelRunning(bool async, bool* ret_value) {
	return _executeFunction(async, CW_IF_ISANYROTRUN, ret_value);
}


//------------------------------------------------------------------------------------
int p347ClientWrapper::isADCRunning(bool async, unsigned char ch_idx, bool* ret_value) {
	return _executeFunction(async, CW_IF_ISADCRUN, ch_idx, ret_value);
}

int p347ClientWrapper::isAnyADCChannelRunning(bool async, bool* ret_value) {
	return _executeFunction(async, CW_IF_ISANYADCRUN, ret_value);
}

int p347ClientWrapper::doSetupAdcChannel(bool async, const channel_manager::ADCChannelInitParams & adccip) {
	return _executeFunction(async, CW_IF_SETUP_ADC, &adccip);
}

int p347ClientWrapper::startAdcChannel(bool async, unsigned char ch_idx) {
	return _executeFunction(async, CW_IF_START_ADC,ch_idx);
}

int p347ClientWrapper::stopAdcChannel(bool async, unsigned char ch_idx, bool wfw, bool wfr) {
	printf("call stop ADC\n");
	return _executeFunction(async, CW_IF_STOP_ADC, ch_idx, wfw, wfr);
}

int p347ClientWrapper::warmChannelStart(bool async, unsigned char ch_idx, unsigned short reg_settings) {
	return _executeFunction(async, CW_IF_WARM_START,ch_idx,reg_settings);
}

int p347ClientWrapper::warmChannelEnd(bool async, unsigned char ch_idx) {
	return _executeFunction(async, CW_IF_WARM_STOP,ch_idx);
}

int p347ClientWrapper::doStartSyncChannels(bool async, const channel_manager::SynctaskChannels & tsc) {
	return _executeFunction(async, CW_IF_START_SYNC, &tsc);
}

int p347ClientWrapper::doStopSyncChannels(bool async, const channel_manager::SynctaskChannels & tsc, bool wfw, bool wfr) {
	return _executeFunction(async, CW_IF_STOP_SYNC, &tsc, wfw, wfr);
}

int p347ClientWrapper::readADCTimeOffsets(bool async, channel_manager::ADCTimeOffsets* ret_value) {
	return _executeFunction(async, CW_IF_READ_OFFSETS, ret_value);
	//TODO
}


//------------------------------------------------------------------------------------
int p347ClientWrapper::switchCommutatorOn(bool async) {
	return _executeFunction(async, CW_IF_MUX_ON);
}

int p347ClientWrapper::switchCommutatorOff(bool async) {
	return _executeFunction(async, CW_IF_MUX_OFF);
}

int p347ClientWrapper::unmuxAll(bool async) {
	return _executeFunction(async, CW_IF_UNMUX_ALL);
}

int p347ClientWrapper::doMuxChannel(bool async, int phys_idx, int mux_idx) {
	return _executeFunction(async, CW_IF_MUX_CHANNEL, phys_idx, mux_idx);
}

//------------------------------------------------------------------------------------
int p347ClientWrapper::addTask(bool async, int emu_idx, const task_manager::AnyTaskParams & task_params) {
	return _executeFunction(async, CW_IF_ADDTASK, emu_idx, &task_params);
}

int p347ClientWrapper::delTask(bool async, int emu_idx,int task_idx) {
	return _executeFunction(async, CW_IF_DELTASK, emu_idx, task_idx);
}

int p347ClientWrapper::clearTaskData(bool async, int emu_idx, int task_idx,bool AClearOffSet,bool AClearResults) {
	return _executeFunction(async, CW_IF_CLEARTASKDATA, emu_idx, task_idx, AClearOffSet, AClearResults);
}

int p347ClientWrapper::clearTaskList(bool async, int emu_idx) {
	return _executeFunction(async, CW_IF_CLEARTASKLIST, emu_idx);
}

int p347ClientWrapper::clearTaskListData(bool async, int emu_idx, bool AClearOffSet,bool AClearResults) {
	return _executeFunction(async, CW_IF_CLEARTASKLISTDATA, emu_idx, AClearOffSet, AClearResults);
}

int p347ClientWrapper::clearData(bool async, int emu_idx) {
	return _executeFunction(async, CW_IF_CLEARDATA, emu_idx);
}

int p347ClientWrapper::clear(bool async, int emu_idx) {
	return _executeFunction(async, CW_IF_CLEAR, emu_idx);
}

int p347ClientWrapper::getTaskResult(bool async, int emu_idx,int task_idx,int supposed_type,int res_code, task_manager::AnyTaskResult* ret_value) {
	return _executeFunction(async, CW_IF_GETTASKRESULT, emu_idx, task_idx, supposed_type, res_code, ret_value);
}

int p347ClientWrapper::getTaskStatus(bool async, int emu_idx,int task_idx) {
	return _executeFunction(async, CW_IF_GETTASKSTATUS, emu_idx, task_idx);
}

int p347ClientWrapper::getTaskProgress(bool async, int emu_idx,int task_idx) {
	return _executeFunction(async, CW_IF_GETTASKPROGRESS, emu_idx, task_idx);
}

int p347ClientWrapper::getTaskNewResult(bool async, int emu_idx,int task_idx) {
	return _executeFunction(async, CW_IF_GETTASKNEWRESULT, emu_idx, task_idx);
}

int p347ClientWrapper::getTaskState(bool async, int emu_idx,int task_idx) {
	return _executeFunction(async, CW_IF_GETTASKSTATE, emu_idx, task_idx);
}

int p347ClientWrapper::setTaskState(bool async, int emu_idx,int task_idx,int new_state) {
	return _executeFunction(async, CW_IF_SETTASKSTATE, emu_idx, task_idx, new_state);
}

int p347ClientWrapper::setTaskListState(bool async, int emu_idx, const task_manager::IntArray & task_idx_list, int new_state) {
	return _executeFunction(async, CW_IF_SETTASKLISTSTATE, emu_idx, &task_idx_list, new_state);
}

int p347ClientWrapper::getTotalTasksProgress(bool async, int emu_idx, bool AExceptMonitoring) {
	return _executeFunction(async, CW_IF_GETTOTALTASKPROGRESS, emu_idx, AExceptMonitoring);
}

int p347ClientWrapper::getAnyTasksStatusWaitingData(bool async, int emu_idx, bool* ret_value) {
	return _executeFunction(async, CW_IF_GETANYTASKSWD, emu_idx, ret_value);
}

int p347ClientWrapper::getAllTasksStatusFinished(bool async, int emu_idx, bool* ret_value) {
	return _executeFunction(async, CW_IF_GETTOTALTASKSSF, emu_idx, ret_value);
}

int p347ClientWrapper::getTotalTasksLastErrorCode(bool async, int emu_idx,bool AExceptMonitoring) {
	return _executeFunction(async, CW_IF_GETTOTALTASKSLEC, emu_idx, AExceptMonitoring);
}

int p347ClientWrapper::timesigOpen(bool async, int emu_idx) {
	return _executeFunction(async, CW_IF_TIMESIGOPEN, emu_idx);
}

int p347ClientWrapper::timesigClose(bool async, int emu_idx) {
	return _executeFunction(async, CW_IF_TIMESIGCLOSE, emu_idx);
}

int p347ClientWrapper::isTimesigOpened(bool async, int emu_idx, bool* ret_value) {
	return _executeFunction(async, CW_IF_ISTIMESIGOPENED, emu_idx, ret_value);
}

int p347ClientWrapper::getSigLength(bool async, int emu_idx) {
	return _executeFunction(async, CW_IF_GETSIGLEN, emu_idx);
}

int p347ClientWrapper::getRotLength(bool async, int emu_idx) {
	return _executeFunction(async, CW_IF_GETROTLEN, emu_idx);
}

int p347ClientWrapper::getSensitivity(bool async, int emu_idx, double* ret_value) {
	return _executeFunction(async, CW_IF_GETSENS, emu_idx, ret_value);
}

int p347ClientWrapper::getGain(bool async, int emu_idx, double* ret_value) {
	return _executeFunction(async, CW_IF_GETGAIN, emu_idx, ret_value);
}

int p347ClientWrapper::setSensitivity(bool async, int emu_idx,double ASensitivity) {
	return _executeFunction(async, CW_IF_SETSENS, emu_idx, ASensitivity);
}

int p347ClientWrapper::setGain(bool async, int emu_idx,double AGain) {
	return _executeFunction(async, CW_IF_SETGAIN, emu_idx, AGain);
}

int p347ClientWrapper::setRotLabelsCount(bool async, int emu_idx,int ARotLabelsCount) {
	return _executeFunction(async, CW_IF_SETROTLABELSCOUNT, emu_idx, ARotLabelsCount);
}

int p347ClientWrapper::getRotLabelsCount(bool async, int emu_idx) {
	return _executeFunction(async, CW_IF_GETROTLABELSCOUNT, emu_idx);
}

int p347ClientWrapper::getRotMetkasFromSig(bool async, int emu_idx, bool* ret_value) {
	return _executeFunction(async, CW_IF_GETRMFROMSIG, emu_idx, ret_value);
}

int p347ClientWrapper::setRotMetkasFromSig(bool async, int emu_idx, bool ARotMetkasFromSig) {
	return _executeFunction(async, CW_IF_SETRMFROMSIG, emu_idx, ARotMetkasFromSig);
}

int p347ClientWrapper::startSaveTimeSig(bool async, int emu_idx,const std::string & AFileNameForSave,int ATSDTCForSave, bool* ret_value) {
	return _executeFunction(async, CW_IF_STARTSAVETS, emu_idx, &AFileNameForSave, ATSDTCForSave, ret_value);
}

int p347ClientWrapper::stopSaveTimeSig(bool async, int emu_idx) {
	return _executeFunction(async, CW_IF_STOPSAVETS, emu_idx);
}

int p347ClientWrapper::startSaveTaskTimeSig(bool async, int emu_idx,int task_idx,const std::string & AFileNameForSave,int ATSDTCForSave, bool* ret_value) {
	return _executeFunction(async, CW_IF_STARTSAVERASKTS, emu_idx, task_idx, &AFileNameForSave, ATSDTCForSave, ret_value);
}

int p347ClientWrapper::stopSaveTaskTimeSig(bool async, int emu_idx,int task_idx) {
	return _executeFunction(async, CW_IF_STOPSAVETASKTS, emu_idx, task_idx);
}

int p347ClientWrapper::saveTimeSig(bool async, int emu_idx, const std::string & AFileName, bool* ret_value) {
	return _executeFunction(async, CW_IF_SAVETIMESIG, emu_idx, &AFileName, ret_value);
}

int p347ClientWrapper::getBegSigOffSet(bool async, int emu_idx, long int* ret_value) {
	return _executeFunction(async, CW_IF_GETBEGSIGOFFSET, emu_idx, ret_value);
}

int p347ClientWrapper::setBegSigOffSet(bool async, int emu_idx,long int ABegSigOffSet) {
	return _executeFunction(async, CW_IF_SETBEGSIGOFFSET, emu_idx, ABegSigOffSet);
}

int p347ClientWrapper::getBegRotOffSet(bool async, int emu_idx, long int* ret_value) {
	return _executeFunction(async, CW_IF_GETBEGROTOFFSET, emu_idx, ret_value);
}

int p347ClientWrapper::setBegRotOffSet(bool async, int emu_idx,long int ABegRotOffSet) {
	return _executeFunction(async, CW_IF_SETBEGROTOFFSET, emu_idx, ABegRotOffSet);
}

int p347ClientWrapper::getSrcSamplingFreq(bool async, int emu_idx) {
	return _executeFunction(async, CW_IF_GETSRCSF, emu_idx);
}

int p347ClientWrapper::setSrcSamplingFreq(bool async, int emu_idx, int ASrcSamplingFreq) {
	return _executeFunction(async, CW_IF_SETSRCSF, emu_idx, ASrcSamplingFreq);
}

int p347ClientWrapper::getDecim(bool async, int emu_idx) {
	return _executeFunction(async, CW_IF_GETDECIM, emu_idx);
}

int p347ClientWrapper::getInterp(bool async, int emu_idx) {
	return _executeFunction(async, CW_IF_GETINTERP, emu_idx);
}

int p347ClientWrapper::getVibeg(bool async, int emu_idx, bool* ret_value) {
	return _executeFunction(async, CW_IF_GETVIBEG, emu_idx, ret_value);
}

int p347ClientWrapper::setVibeg(bool async, int emu_idx, bool AVibeg) {
	return _executeFunction(async, CW_IF_SETVIBEG, emu_idx, AVibeg);
}

int p347ClientWrapper::getVibegTaskParams(bool async, int emu_idx, task_manager::AnyTaskParams* ret_value) {
	return _executeFunction(async, CW_IF_GETVIBEGTP, emu_idx, ret_value);
}

int p347ClientWrapper::setVibegTaskParams(bool async, int emu_idx, task_manager::VibegTaskParams & avtp) {
	return _executeFunction(async, CW_IF_SETVIBEGTP, emu_idx, &avtp);
}

int p347ClientWrapper::setDefaultDSPEmulParams(bool async, int emu_idx, int mode_gain) {
	return _executeFunction(async, CW_IF_SETDEFAULTDSPPARAMS, emu_idx, mode_gain);
}

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

int p347ClientWrapper::_processResult(int if_code) {
	int ret = 0;
	
		switch (if_code) {
			case CW_IF_READROT: {
				channel_manager::RotData rdat = *fRet_rot;
				ret = rdat.error_code();
				if (ret < 0) {
					logMessage(LOG_LEVEL_CRITICAL,"readRotData ret = %d\n",ret);
				} else {
					if (some_result_pointer != nullptr) {
						((channel_manager::RotData*)(some_result_pointer))->CopyFrom(rdat);
						if (rdat.has_freq_avr()) {
							printf("freq_avr = %.5lf\n",rdat.freq_avr());
						}
					} else {
						logMessage(LOG_LEVEL_CRITICAL,"result_ptr for RotData suddenly is NULL!\n");
						ret = -EINVAL;
					}
				}
			break; }
			case CW_IF_READ_OFFSETS: {
				channel_manager::ADCTimeOffsets rdat = *fRet_off;
				ret = rdat.error_code();
				if (ret < 0) {
					logMessage(LOG_LEVEL_CRITICAL,"readRotData ret = %d\n",ret);
				} else {
					if (some_result_pointer != nullptr) {
						((channel_manager::ADCTimeOffsets*)(some_result_pointer))->CopyFrom(rdat);
					} else {
						logMessage(LOG_LEVEL_CRITICAL,"result_ptr for ADCTimeOffsets suddenly is NULL!\n");
						ret = -EINVAL;
					}
				}
			break; }
			case CW_IF_ISROTRUN	:
			case CW_IF_ISANYROTRUN:
			case CW_IF_ISADCRUN:
			case CW_IF_ISANYADCRUN:
			case CW_IF_GETANYTASKSWD:
			case CW_IF_GETTOTALTASKSSF: 
			case CW_IF_ISTIMESIGOPENED: 
			case CW_IF_GETRMFROMSIG: 
			case CW_IF_STARTSAVETS: 
			case CW_IF_STARTSAVERASKTS: 
			case CW_IF_SAVETIMESIG: 
			case CW_IF_GETVIBEG:
			case CW_IF_ISCHANNELAV:
			case CW_IF_ISCHANNELSCR: {
				bool retb = *fRet_bool;
				if (some_result_pointer != nullptr) {
					*((bool*)some_result_pointer) = retb;
				} else {
					logMessage(LOG_LEVEL_CRITICAL,"result_ptr for bool suddenly is NULL!\n");
					ret = -EINVAL;
				}
			break; }
			case CW_IF_GETTASKRESULT: {
				task_manager::AnyTaskResult atp = *fRet_anyres;
				ret = atp.error_code();
				if (ret < 0) {
					logMessage(LOG_LEVEL_CRITICAL,"getTaskResult ret = %d\n",ret);
				} else {
					if (some_result_pointer != nullptr) {
						((task_manager::AnyTaskResult*)(some_result_pointer))->CopyFrom(atp);
					} else {
						logMessage(LOG_LEVEL_CRITICAL,"result_ptr for AnyTaskResult suddenly is NULL!\n");
						ret = -EINVAL;
					}
				}
			break; }
			case CW_IF_GETSENS:
			case CW_IF_GETGAIN: {
				double dval = *fRet_double;
				if (some_result_pointer != nullptr) {
					*((double*)some_result_pointer) = dval;
				} else {
					logMessage(LOG_LEVEL_CRITICAL,"result_ptr for double suddenly is NULL!\n");
					ret = -EINVAL;
				}
			break; }
			case CW_IF_GETBEGSIGOFFSET:
			case CW_IF_GETBEGROTOFFSET: {
				long int ival = *fRet_longint;
				if (some_result_pointer != nullptr) {
					*((long int*)some_result_pointer) = ival;
				} else {
					logMessage(LOG_LEVEL_CRITICAL,"result_ptr for long int suddenly is NULL!\n");
					ret = -EINVAL;
				}
			break; }
			case CW_IF_GETVIBEGTP: {
				task_manager::AnyTaskParams atp = *fRet_anypar;
				ret = atp.error_code();
				if (ret < 0) {
					logMessage(LOG_LEVEL_CRITICAL,"getVibegTaskParams ret = %d\n",ret);
				} else {
					if (some_result_pointer != nullptr) {
						((task_manager::AnyTaskParams*)(some_result_pointer))->CopyFrom(atp);
					} else {
						logMessage(LOG_LEVEL_CRITICAL,"result_ptr for AnyTaskParams suddenly is NULL!\n");
						ret = -EINVAL;
					}
				}			
			break; }
			case CW_IF_GET_AVCHANNELS: {
				channel_manager::AvailableChannels ac = *fRet_avchannels;
				ret = ac.error_code();
				if (ret < 0) {
					logMessage(LOG_LEVEL_CRITICAL,"getAvailableChannels ret = %d\n",ret);
				} else {
					if (some_result_pointer != nullptr) {
						((channel_manager::AvailableChannels*)(some_result_pointer))->CopyFrom(ac);
					} else {
						logMessage(LOG_LEVEL_CRITICAL,"result_ptr for getAvailableChannels suddenly is NULL!\n");
						ret = -EINVAL;
					}
				}	
			break; }
			case CW_IF_GETSERVERVER: {
				channel_manager::ServerVersion sv = *fRet_version;
				ret = sv.error_code();
				if (ret < 0) {
					logMessage(LOG_LEVEL_CRITICAL,"getServerVersion ret = %d\n",ret);
				} else {
					if (some_result_pointer != nullptr) {
						((channel_manager::ServerVersion*)(some_result_pointer))->CopyFrom(sv);
					} else {
						logMessage(LOG_LEVEL_CRITICAL,"result_ptr for getServerVersion suddenly is NULL!\n");
						ret = -EINVAL;
					}
				}					
			break; }
			default: { //supposed int return value, and nothing more
				
				ret = *fRet_int;
				//printf("Int result=%d for if_code=%d\n",ret,if_code);
			break; }
		};		
	return ret;
}

void p347ClientWrapper::onAsyncFunctionCompleted(int if_code) {
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
	
		sigAsyncFunctionComplete(if_code,TCP_ASYNC_FUNCTION_FAILED,ret);
	} else {
		ret = _processResult(if_code);
		
		//connection_fails_counter.store(0);
		logMessage(LOG_LEVEL_FULL,"ASYNC RET of function[%d] ret = %d\n",if_code,ret);
		
		sigAsyncFunctionComplete(if_code,0,ret);
	}
};
#define CALLBACK_FUNC				RCF::AsyncTwoway( boost::bind(&p347ClientWrapper::onAsyncFunctionCompleted, this, if_code) )

//--------------------------------------------------------------------------------Private

int p347ClientWrapper::_executeFunction(bool async, int if_code, ...) {
	logMessage(LOG_LEVEL_FULL,"is_connected_to_server.load() = %d\n",is_connected_to_server.load());
	if (!is_connected_to_server.load()) return TCP_SERVER_NOT_CONNECTED;

	va_list ap;
	
	try {
	
		switch (if_code) {
			case CW_IF_INIT_DEVICE: {
				va_start(ap,if_code);
				p347_conf::DeviceInitParams* initpars = va_arg(ap,p347_conf::DeviceInitParams*);
				va_end(ap);
				if (async) {
					fRet_int = client->initDevice(CALLBACK_FUNC, *initpars);
					logMessage(LOG_LEVEL_FULL,"async initDevice called\n");
				} else {	
					fRet_int = client->initDevice(*initpars);
					logMessage(LOG_LEVEL_FULL,"sync initDevice called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}				
			break; }
			case CW_IF_DEINIT_DEVICE: {
				if (async) {
					fRet_int = client->deinitDevice(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async deinitDevice called\n");
				} else {	
					fRet_int = client->deinitDevice();
					logMessage(LOG_LEVEL_FULL,"sync deinitDevice called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }			
/*		
			case CW_IF_CREATE_CM: {
				va_start(ap,if_code);
				channel_manager::ChannelManagerInitParams* initpars = va_arg(ap,channel_manager::ChannelManagerInitParams*);
				task_manager::DSPEmulInitParams* dspemuli = va_arg(ap,task_manager::DSPEmulInitParams*);
				//int reserved_parameter = va_arg(ap,int);
				va_end(ap);
				
				if (async) {
					fRet_int = client->createChannelManager(CALLBACK_FUNC, *initpars, *dspemuli);
					logMessage(LOG_LEVEL_FULL,"async createChannelManager called\n");
				} else {	
					fRet_int = client->createChannelManager(*initpars, *dspemuli);
					logMessage(LOG_LEVEL_FULL,"sync createChannelManager called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case CW_IF_DELETE_CM: {
				if (async) {
					fRet_int = client->deleteChannelManager(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async deleteChannelManager called\n");
				} else {	
					fRet_int = client->deleteChannelManager();
					logMessage(LOG_LEVEL_FULL,"sync deleteChannelManager called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case CW_IF_INIT_MUX: {
				va_start(ap,if_code);
				channel_manager::MultiplexerInitParams* mip = va_arg(ap,channel_manager::MultiplexerInitParams*);
				va_end(ap);
				if (async) {
					fRet_int = client->initMultiplexer(CALLBACK_FUNC, *mip);
					logMessage(LOG_LEVEL_FULL,"async initMultiplexer called\n");
				} else {	
					fRet_int = client->initMultiplexer(*mip);
					logMessage(LOG_LEVEL_FULL,"sync initMultiplexer called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case CW_IF_DEINIT_MUX: {
				if (async) {
					fRet_int = client->deinitMultiplexer(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async deinitMultiplexer called\n");
				} else {	
					fRet_int = client->deinitMultiplexer();
					logMessage(LOG_LEVEL_FULL,"sync deinitMultiplexer called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
*/
			case CW_IF_FPGA_DRELOAD: {
				va_start(ap,if_code);
				std::string* old_name = va_arg(ap,std::string*);
				std::string* new_path = va_arg(ap,std::string*);
				va_end(ap);
				if (async) {
					fRet_int = client->FPGADriverReload(CALLBACK_FUNC, *old_name, *new_path);
					logMessage(LOG_LEVEL_FULL,"async FPGADriverReload called\n");
				} else {	
					fRet_int = client->FPGADriverReload(*old_name, *new_path);
					logMessage(LOG_LEVEL_FULL,"sync FPGADriverReload called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case CW_IF_CHECKFPGA: {
				if (async) {
					fRet_int = client->checkFPGAStatus(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async checkFPGAStatus called\n");
				} else {	
					fRet_int = client->checkFPGAStatus();
					logMessage(LOG_LEVEL_FULL,"sync checkFPGAStatus called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case CW_IF_GET_AVCHANNELS: {
				va_start(ap,if_code);
				some_result_pointer = (void*)va_arg(ap,channel_manager::AvailableChannels*);
				va_end(ap);
				if (async) {
					fRet_avchannels = client->getAvailableChannels(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async getAvailableChannels called\n");
				} else {	
					fRet_avchannels = client->getAvailableChannels();
					logMessage(LOG_LEVEL_FULL,"sync getAvailableChannels called\n");
					while (!fRet_avchannels.ready()) RCF::sleepMs(100);				
				}			
			break; }
			case CW_IF_ISCHANNELAV: {
				va_start(ap,if_code);
				int ch_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->isChannelAvailable(CALLBACK_FUNC,ch_idx);
					logMessage(LOG_LEVEL_FULL,"async isChannelAvailable called\n");
				} else {	
					fRet_bool = client->isChannelAvailable(ch_idx);
					logMessage(LOG_LEVEL_FULL,"sync isChannelAvailable called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);				
				}			
			break; }
			case CW_IF_ISCHANNELSCR: {
				va_start(ap,if_code);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->isChannelsCreated(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async isChannelsCreated called\n");
				} else {	
					fRet_bool = client->isChannelsCreated();
					logMessage(LOG_LEVEL_FULL,"sync isChannelsCreated called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);				
				}				
			break; }
			case CW_IF_GETSERVERVER: {
				va_start(ap,if_code);
				some_result_pointer = (void*)va_arg(ap,channel_manager::ServerVersion*);
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
			/*
			case CW_IF_BINDCHEMUL: {
				va_start(ap,if_code);
				int ch_idx = va_arg(ap,int);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->bindChannelToEmul(CALLBACK_FUNC,ch_idx,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async bindChannelToEmul called\n");
				} else {	
					fRet_int = client->bindChannelToEmul(ch_idx,emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync bindChannelToEmul called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			*/
			//---------------------------------------------------------------------ROT
			case CW_IF_READROT: { 
				va_start(ap,if_code);
				int ch_idx = va_arg(ap,int);
				//rotdata_result_ptr = va_arg(ap,channel_manager::RotData*);
				some_result_pointer = (void*)va_arg(ap,channel_manager::RotData*);
				va_end(ap);
				
				if (async) {
					//fRet_rot = client->readRotData(CALLBACK_ROTDATA,ch_idx);
					fRet_rot = client->readRotData(CALLBACK_FUNC,ch_idx);
					logMessage(LOG_LEVEL_FULL,"async readRotData called\n");
				} else {	
					fRet_rot = client->readRotData(ch_idx);
					logMessage(LOG_LEVEL_FULL,"sync readRotData called\n");
					while (!fRet_rot.ready()) RCF::sleepMs(100);				
				}
			break; }
			case CW_IF_STARTROT: {
				va_start(ap,if_code);
				channel_manager::RotChannelInitParams* initpars = va_arg(ap,channel_manager::RotChannelInitParams*);
				va_end(ap);
				if (initpars != nullptr) {
					if (async) {
						fRet_int = client->doStartRotChannel(CALLBACK_FUNC, *initpars);
						//fRet = client->doStartRotChannel(CALLBACK_FUNC_SR, rcip);
						logMessage(LOG_LEVEL_FULL,"async doStartRotChannel called\n");
					} else {	
						fRet_int = client->doStartRotChannel(*initpars);
						logMessage(LOG_LEVEL_FULL,"sync doStartRotChannel called\n");
						while (!fRet_int.ready()) RCF::sleepMs(100);				
					}
				} else {
					logMessage(LOG_LEVEL_CRITICAL,"CW_IF_STARTROT - initpars is NULL\n");
					return TCP_VOID_INITPARS;
				}
			break; }
			case CW_IF_STOPROT: {
				va_start(ap,if_code);
				int ch_idx = va_arg(ap,int);	
				va_end(ap);
				if (async) {
					fRet_int = client->stopRotChannel(CALLBACK_FUNC, ch_idx);
					logMessage(LOG_LEVEL_FULL,"async stopRotChannel called\n");
				} else {	
					fRet_int = client->stopRotChannel(ch_idx);
					logMessage(LOG_LEVEL_FULL,"sync stopRotChannel called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }
			case CW_IF_ISROTRUN: {
				va_start(ap,if_code);
				int ch_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->isRotRunning(CALLBACK_FUNC, ch_idx);
					logMessage(LOG_LEVEL_FULL,"async isRotRunning called\n");
				} else {	
					fRet_bool = client->isRotRunning(ch_idx);
					logMessage(LOG_LEVEL_FULL,"sync isRotRunning called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);				
				}
			break; }
			case CW_IF_ISANYROTRUN: {
				va_start(ap,if_code);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->isAnyRotChannelRunning(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async isAnyRotRunning called\n");
				} else {	
					fRet_bool = client->isAnyRotChannelRunning();
					logMessage(LOG_LEVEL_FULL,"sync isAnyRotRunning called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);				
				}
			break; }
			//-----------------------------------------------------------------------------------ADC
			case CW_IF_ISADCRUN: {
				va_start(ap,if_code);
				int ch_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->isADCRunning(CALLBACK_FUNC, ch_idx);
					logMessage(LOG_LEVEL_FULL,"async isADCRunning called\n");
				} else {	
					fRet_bool = client->isADCRunning(ch_idx);
					logMessage(LOG_LEVEL_FULL,"sync isADCRunning called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);				
				}
			break; }
			case CW_IF_ISANYADCRUN: {
				va_start(ap,if_code);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->isAnyADCChannelRunning(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async isAnyADCChannelRunning called\n");
				} else {	
					fRet_bool = client->isAnyADCChannelRunning();
					logMessage(LOG_LEVEL_FULL,"sync isAnyADCChannelRunning called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);				
				}
			break; }
			case CW_IF_SETUP_ADC:	{
				va_start(ap,if_code);
				channel_manager::ADCChannelInitParams* initpars = va_arg(ap,channel_manager::ADCChannelInitParams*);
				va_end(ap);
				if (async) {
					fRet_int = client->doSetupAdcChannel(CALLBACK_FUNC,*initpars);
					logMessage(LOG_LEVEL_FULL,"async doSetupAdcChannel called\n");
				} else {	
					fRet_int = client->doSetupAdcChannel(*initpars);
					logMessage(LOG_LEVEL_FULL,"sync doSetupAdcChannel called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }	
			case CW_IF_START_ADC:	{
				va_start(ap,if_code);
				int ch_idx = va_arg(ap,int);	
				va_end(ap);
				if (async) {
					fRet_int = client->startAdcChannel(CALLBACK_FUNC, ch_idx);
					logMessage(LOG_LEVEL_FULL,"async startAdcChannel called\n");
				} else {	
					fRet_int = client->startAdcChannel(ch_idx);
					logMessage(LOG_LEVEL_FULL,"sync startAdcChannel called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }	
			case CW_IF_STOP_ADC: {
				va_start(ap,if_code);
				int ch_idx = va_arg(ap,int);
				bool wfw = static_cast<bool>(va_arg(ap,int));
				bool wfr = static_cast<bool>(va_arg(ap,int));
				va_end(ap);
				printf("stop step 3\n");
				if (async) {
					fRet_int = client->stopAdcChannel(CALLBACK_FUNC, ch_idx, wfw, wfr);
					logMessage(LOG_LEVEL_FULL,"async stopAdcChannel called\n");
				} else {	
					fRet_int = client->stopAdcChannel(ch_idx, wfw, wfr);
					logMessage(LOG_LEVEL_FULL,"sync stopAdcChannel called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }		
			case CW_IF_WARM_START: {
				va_start(ap,if_code);
				int ch_idx = va_arg(ap,int);
				unsigned short rs = static_cast<unsigned short>(va_arg(ap,int));
				va_end(ap);
				if (async) {
					fRet_int = client->warmChannelStart(CALLBACK_FUNC, ch_idx, rs);
					logMessage(LOG_LEVEL_FULL,"async warmChannelStart called\n");
				} else {	
					fRet_int = client->warmChannelStart(ch_idx, rs);
					logMessage(LOG_LEVEL_FULL,"sync warmChannelStart called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }	
			case CW_IF_WARM_STOP:	{
				va_start(ap,if_code);
				int ch_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->warmChannelEnd(CALLBACK_FUNC, ch_idx);
					logMessage(LOG_LEVEL_FULL,"async warmChannelEnd called\n");
				} else {	
					fRet_int = client->warmChannelEnd(ch_idx);
					logMessage(LOG_LEVEL_FULL,"sync warmChannelEnd called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}	
			break; }	
			case CW_IF_START_SYNC: {
				va_start(ap,if_code);
				channel_manager::SynctaskChannels* synctask = va_arg(ap,channel_manager::SynctaskChannels*);
				va_end(ap);
				if (async) {
					fRet_int = client->doStartSyncChannels(CALLBACK_FUNC, *synctask);
					logMessage(LOG_LEVEL_FULL,"async doStartSyncChannels called\n");
				} else {	
					fRet_int = client->doStartSyncChannels(*synctask);
					logMessage(LOG_LEVEL_FULL,"sync doStartSyncChannels called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }	
			case CW_IF_STOP_SYNC:	{
				va_start(ap,if_code);
				channel_manager::SynctaskChannels* synctask = va_arg(ap,channel_manager::SynctaskChannels*);
				bool wfw = static_cast<bool>(va_arg(ap,int));
				bool wfr = static_cast<bool>(va_arg(ap,int));
				va_end(ap);
				if (async) {
					fRet_int = client->doStopSyncChannels(CALLBACK_FUNC, *synctask, wfw, wfr);
					logMessage(LOG_LEVEL_FULL,"async doStopSyncChannels called\n");
				} else {	
					fRet_int = client->doStopSyncChannels(*synctask, wfw, wfr);
					logMessage(LOG_LEVEL_FULL,"sync doStopSyncChannels called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);				
				}
			break; }	
			case CW_IF_READ_OFFSETS: {
				va_start(ap,if_code);
				some_result_pointer = (void*)va_arg(ap,channel_manager::ADCTimeOffsets*);
				va_end(ap);
				if (async) {
					fRet_off = client->readADCTimeOffsets(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async readADCTimeOffsets called\n");
				} else {	
					fRet_off = client->readADCTimeOffsets();
					logMessage(LOG_LEVEL_FULL,"sync readADCTimeOffsets called\n");
					while (!fRet_off.ready()) RCF::sleepMs(100);				
				}
			break; }
			//----------------------------------------------------------------------------------------------------------MUX
			case CW_IF_MUX_ON: {
				if (async) {
					fRet_int = client->switchCommutatorOn(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async switchCommutatorOn called\n");
				} else {
					fRet_int = client->switchCommutatorOn();
					logMessage(LOG_LEVEL_FULL,"sync switchCommutatorOn called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_MUX_OFF: {
				if (async) {
					fRet_int = client->switchCommutatorOff(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async switchCommutatorOff called\n");
				} else {
					fRet_int = client->switchCommutatorOff();
					logMessage(LOG_LEVEL_FULL,"sync switchCommutatorOff called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }			
			case CW_IF_UNMUX_ALL: {
				if (async) {
					fRet_int = client->unmuxAll(CALLBACK_FUNC);
					logMessage(LOG_LEVEL_FULL,"async unmuxAll called\n");
				} else {
					fRet_int = client->unmuxAll();
					logMessage(LOG_LEVEL_FULL,"sync unmuxAll called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }			
			case CW_IF_MUX_CHANNEL: {
				va_start(ap,if_code);
				int phys_idx = va_arg(ap,int);
				int mux_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->doMuxChannel(CALLBACK_FUNC,phys_idx,mux_idx);
					logMessage(LOG_LEVEL_FULL,"async doMuxChannel called\n");
				} else {
					fRet_int = client->doMuxChannel(phys_idx,mux_idx);
					logMessage(LOG_LEVEL_FULL,"sync doMuxChannel called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }		
			//----------------------------------------------------------------------------------------------------------EMUL
			case CW_IF_ADDTASK: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				task_manager::AnyTaskParams* atp = va_arg(ap,task_manager::AnyTaskParams*);
				va_end(ap);
				if (async) {
					fRet_int = client->addTask(CALLBACK_FUNC,emu_idx,*atp);
					logMessage(LOG_LEVEL_FULL,"async addTask called\n");
				} else {
					fRet_int = client->addTask(emu_idx,*atp);
					logMessage(LOG_LEVEL_FULL,"sync addTask called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_DELTASK: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int task_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->delTask(CALLBACK_FUNC,emu_idx,task_idx);
					logMessage(LOG_LEVEL_FULL,"async delTask called\n");
				} else {
					fRet_int = client->delTask(emu_idx,task_idx);
					logMessage(LOG_LEVEL_FULL,"sync delTask called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_CLEARTASKDATA: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int task_idx = va_arg(ap,int);
				bool clr_off = static_cast<bool>(va_arg(ap,int));
				bool clr_res = static_cast<bool>(va_arg(ap,int));
				va_end(ap);
				if (async) {
					fRet_int = client->clearTaskData(CALLBACK_FUNC,emu_idx,task_idx,clr_off,clr_res);
					logMessage(LOG_LEVEL_FULL,"async clearTaskData called\n");
				} else {
					fRet_int = client->clearTaskData(emu_idx,task_idx,clr_off,clr_res);
					logMessage(LOG_LEVEL_FULL,"sync clearTaskData called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_CLEARTASKLIST: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->clearTaskList(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async clearTaskList called\n");
				} else {
					fRet_int = client->clearTaskList(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync clearTaskList called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_CLEARTASKLISTDATA: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				bool clr_off = static_cast<bool>(va_arg(ap,int));
				bool clr_res = static_cast<bool>(va_arg(ap,int));
				va_end(ap);
				if (async) {
					fRet_int = client->clearTaskListData(CALLBACK_FUNC,emu_idx,clr_off,clr_res);
					logMessage(LOG_LEVEL_FULL,"async clearTaskListData called\n");
				} else {
					fRet_int = client->clearTaskListData(emu_idx,clr_off,clr_res);
					logMessage(LOG_LEVEL_FULL,"sync clearTaskListData called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_CLEARDATA: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->clearData(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async clearData called\n");
				} else {
					fRet_int = client->clearData(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync clearData called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_CLEAR: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->clear(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async clear called\n");
				} else {
					fRet_int = client->clear(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync clear called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETTASKRESULT: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int task_idx = va_arg(ap,int);
				int supp_type = va_arg(ap,int);
				int res_code = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,task_manager::AnyTaskResult*);
				va_end(ap);
				if (async) {
					fRet_anyres = client->getTaskResult(CALLBACK_FUNC,emu_idx,task_idx,supp_type,res_code);
					logMessage(LOG_LEVEL_FULL,"async getTaskResult called\n");
				} else {
					fRet_anyres = client->getTaskResult(emu_idx,task_idx,supp_type,res_code);
					logMessage(LOG_LEVEL_FULL,"sync getTaskResult called\n");
					while (!fRet_anyres.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETTASKSTATUS: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int task_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->getTaskStatus(CALLBACK_FUNC,emu_idx,task_idx);
					logMessage(LOG_LEVEL_FULL,"async getTaskStatus called\n");
				} else {
					fRet_int = client->getTaskStatus(emu_idx,task_idx);
					logMessage(LOG_LEVEL_FULL,"sync getTaskStatus called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETTASKPROGRESS: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int task_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->getTaskProgress(CALLBACK_FUNC,emu_idx,task_idx);
					logMessage(LOG_LEVEL_FULL,"async getTaskProgress called\n");
				} else {
					fRet_int = client->getTaskProgress(emu_idx,task_idx);
					logMessage(LOG_LEVEL_FULL,"sync getTaskProgress called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETTASKNEWRESULT: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int task_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->getTaskNewResult(CALLBACK_FUNC,emu_idx,task_idx);
					logMessage(LOG_LEVEL_FULL,"async getTaskNewResult called\n");
				} else {
					fRet_int = client->getTaskNewResult(emu_idx,task_idx);
					logMessage(LOG_LEVEL_FULL,"sync getTaskNewResult called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETTASKSTATE: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int task_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->getTaskState(CALLBACK_FUNC,emu_idx,task_idx);
					logMessage(LOG_LEVEL_FULL,"async getTaskState called\n");
				} else {
					fRet_int = client->getTaskState(emu_idx,task_idx);
					logMessage(LOG_LEVEL_FULL,"sync getTaskState called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SETTASKSTATE: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int task_idx = va_arg(ap,int);
				int new_state = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->setTaskState(CALLBACK_FUNC,emu_idx,task_idx,new_state);
					logMessage(LOG_LEVEL_FULL,"async setTaskState called\n");
				} else {
					fRet_int = client->setTaskState(emu_idx,task_idx,new_state);
					logMessage(LOG_LEVEL_FULL,"sync setTaskState called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SETTASKLISTSTATE: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				task_manager::IntArray* iarr = va_arg(ap,task_manager::IntArray*);
				int new_state = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->setTaskListState(CALLBACK_FUNC,emu_idx,*iarr,new_state);
					logMessage(LOG_LEVEL_FULL,"async setTaskListState called\n");
				} else {
					fRet_int = client->setTaskListState(emu_idx,*iarr,new_state);
					logMessage(LOG_LEVEL_FULL,"syncsetTaskListState called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETTOTALTASKPROGRESS: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);		
				bool mon_flag = static_cast<bool>(va_arg(ap,int));
				va_end(ap);
				
				if (async) {
					fRet_int = client->getTotalTasksProgress(CALLBACK_FUNC,emu_idx,mon_flag);
					logMessage(LOG_LEVEL_FULL,"async getTotalTasksProgress called\n");
				} else {
					fRet_int = client->getTotalTasksProgress(emu_idx,mon_flag);
					logMessage(LOG_LEVEL_FULL,"sync getTotalTasksProgress called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETANYTASKSWD: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->getAnyTasksStatusWaitingData(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getAnyTasksStatusWaitingData called\n");
				} else {
					fRet_bool = client->getAnyTasksStatusWaitingData(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getAnyTasksStatusWaitingData called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETTOTALTASKSSF: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->getAllTasksStatusFinished(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getAllTasksStatusFinished called\n");
				} else {
					fRet_bool = client->getAllTasksStatusFinished(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getAllTasksStatusFinished called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETTOTALTASKSLEC: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				bool mon_flag = static_cast<bool>(va_arg(ap,int));
				va_end(ap);
				if (async) {
					fRet_int = client->getTotalTasksLastErrorCode(CALLBACK_FUNC,emu_idx,mon_flag);
					logMessage(LOG_LEVEL_FULL,"async getTotalTasksLastErrorCode called\n");
				} else {
					fRet_int = client->getTotalTasksLastErrorCode(emu_idx,mon_flag);
					logMessage(LOG_LEVEL_FULL,"sync getTotalTasksLastErrorCode called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_TIMESIGOPEN: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->timesigOpen(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async timesigOpen called\n");
				} else {
					fRet_int = client->timesigOpen(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync timesigOpen called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_TIMESIGCLOSE: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->timesigClose(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async timesigClose called\n");
				} else {
					fRet_int = client->timesigClose(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync timesigClose called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_ISTIMESIGOPENED: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->isTimesigOpened(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async isTimesigOpened called\n");
				} else {
					fRet_bool = client->isTimesigOpened(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync isTimesigOpened called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETSIGLEN: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->getSigLength(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getSigLength called\n");
				} else {
					fRet_int = client->getSigLength(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getSigLength called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETROTLEN: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->getRotLength(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getRotLength called\n");
				} else {
					fRet_int = client->getRotLength(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getRotLength called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETSENS: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,double*);
				va_end(ap);
				if (async) {
					fRet_double = client->getSensitivity(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getSensitivity called\n");
				} else {
					fRet_double = client->getSensitivity(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getSensitivity called\n");
					while (!fRet_double.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETGAIN: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,double*);
				va_end(ap);
				if (async) {
					fRet_double = client->getGain(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getGain called\n");
				} else {
					fRet_double = client->getGain(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getGain called\n");
					while (!fRet_double.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SETSENS: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				double value = va_arg(ap,double);
				va_end(ap);
				if (async) {
					fRet_int = client->setSensitivity(CALLBACK_FUNC,emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"async setSensitivity called\n");
				} else {
					fRet_int = client->setSensitivity(emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"sync setSensitivity called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SETGAIN: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				double value = va_arg(ap,double);
				va_end(ap);
				if (async) {
					fRet_int = client->setGain(CALLBACK_FUNC,emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"async setGain called\n");
				} else {
					fRet_int = client->setGain(emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"sync setGain called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SETROTLABELSCOUNT: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int value = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->setRotLabelsCount(CALLBACK_FUNC,emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"async setRotLabelsCount called\n");
				} else {
					fRet_int = client->setRotLabelsCount(emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"sync setRotLabelsCount called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETROTLABELSCOUNT: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->getRotLabelsCount(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getRotLabelsCount called\n");
				} else {
					fRet_int = client->getRotLabelsCount(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getRotLabelsCount called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETRMFROMSIG: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->getRotMetkasFromSig(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getRotMetkasFromSig called\n");
				} else {
					fRet_bool = client->getRotMetkasFromSig(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getRotMetkasFromSig called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SETRMFROMSIG: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				bool value = static_cast<bool>(va_arg(ap,int));
				va_end(ap);
				if (async) {
					fRet_int = client->setRotMetkasFromSig(CALLBACK_FUNC,emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"async setRotMetkasFromSig called\n");
				} else {
					fRet_int = client->setRotMetkasFromSig(emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"sync setRotMetkasFromSig called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_STARTSAVETS: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				std::string* name_ptr = va_arg(ap,std::string*);
				int ats = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->startSaveTimeSig(CALLBACK_FUNC,emu_idx,*name_ptr,ats);
					logMessage(LOG_LEVEL_FULL,"async startSaveTimeSig called\n");
				} else {
					fRet_bool = client->startSaveTimeSig(emu_idx,*name_ptr,ats);
					logMessage(LOG_LEVEL_FULL,"sync startSaveTimeSig called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_STOPSAVETS: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->stopSaveTimeSig(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async stopSaveTimeSig called\n");
				} else {
					fRet_int = client->stopSaveTimeSig(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync stopSaveTimeSig called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_STARTSAVERASKTS: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int task_idx = va_arg(ap,int);
				std::string* name_ptr = va_arg(ap,std::string*);
				int ats = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->startSaveTaskTimeSig(CALLBACK_FUNC,emu_idx,task_idx,*name_ptr,ats);
					logMessage(LOG_LEVEL_FULL,"async startSaveTaskTimeSig called\n");
				} else {
					fRet_bool = client->startSaveTaskTimeSig(emu_idx,task_idx,*name_ptr,ats);
					logMessage(LOG_LEVEL_FULL,"sync startSaveTaskTimeSig called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_STOPSAVETASKTS: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int task_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->stopSaveTaskTimeSig(CALLBACK_FUNC,emu_idx,task_idx);
					logMessage(LOG_LEVEL_FULL,"async stopSaveTaskTimeSig called\n");
				} else {
					fRet_int = client->stopSaveTaskTimeSig(emu_idx,task_idx);
					logMessage(LOG_LEVEL_FULL,"sync stopSaveTaskTimeSig called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SAVETIMESIG: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				std::string* name_ptr = va_arg(ap,std::string*);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->saveTimeSig(CALLBACK_FUNC,emu_idx,*name_ptr);
					logMessage(LOG_LEVEL_FULL,"async saveTimeSig called\n");
				} else {
					fRet_bool = client->saveTimeSig(emu_idx,*name_ptr);
					logMessage(LOG_LEVEL_FULL,"sync saveTimeSig called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETBEGSIGOFFSET: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,long int*);
				va_end(ap);
				if (async) {
					fRet_longint = client->getBegSigOffSet(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getBegSigOffSet called\n");
				} else {
					fRet_longint = client->getBegSigOffSet(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getBegSigOffSet called\n");
					while (!fRet_longint.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SETBEGSIGOFFSET: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				long int value = va_arg(ap,long int);
				va_end(ap);
				if (async) {
					fRet_int = client->setBegSigOffSet(CALLBACK_FUNC,emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"async setBegSigOffSet called\n");
				} else {
					fRet_int = client->setBegSigOffSet(emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"sync setBegSigOffSet called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETBEGROTOFFSET: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,long int*);
				va_end(ap);
				if (async) {
					fRet_longint = client->getBegRotOffSet(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getBegRotOffSet called\n");
				} else {
					fRet_longint = client->getBegRotOffSet(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getBegRotOffSet called\n");
					while (!fRet_longint.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SETBEGROTOFFSET: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				long int value = va_arg(ap,long int);
				va_end(ap);
				if (async) {
					fRet_int = client->setBegRotOffSet(CALLBACK_FUNC,emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"async setBegRotOffSet called\n");
				} else {
					fRet_int = client->setBegRotOffSet(emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"sync setBegRotOffSet called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETSRCSF: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->getSrcSamplingFreq(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getSrcSamplingFreq called\n");
				} else {
					fRet_int = client->getSrcSamplingFreq(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getSrcSamplingFreq called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SETSRCSF: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int value = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->setSrcSamplingFreq(CALLBACK_FUNC,emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"async setSrcSamplingFreq called\n");
				} else {
					fRet_int = client->setSrcSamplingFreq(emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"sync setSrcSamplingFreq called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETDECIM: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->getDecim(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getDecim called\n");
				} else {
					fRet_int = client->getDecim(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getDecim called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETINTERP: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->getInterp(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getInterp called\n");
				} else {
					fRet_int = client->getInterp(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getInterp called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETVIBEG: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,bool*);
				va_end(ap);
				if (async) {
					fRet_bool = client->getVibeg(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getVibeg called\n");
				} else {
					fRet_bool = client->getVibeg(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getVibeg called\n");
					while (!fRet_bool.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SETVIBEG: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				bool value = static_cast<bool>(va_arg(ap,int));
				va_end(ap);
				if (async) {
					fRet_int = client->setVibeg(CALLBACK_FUNC,emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"async setVibeg called\n");
				} else {
					fRet_int = client->setVibeg(emu_idx,value);
					logMessage(LOG_LEVEL_FULL,"sync setVibeg called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_GETVIBEGTP: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				some_result_pointer = (void*)va_arg(ap,task_manager::AnyTaskParams*);
				va_end(ap);
				if (async) {
					fRet_anypar = client->getVibegTaskParams(CALLBACK_FUNC,emu_idx);
					logMessage(LOG_LEVEL_FULL,"async getVibegTaskParams called\n");
				} else {
					fRet_anypar = client->getVibegTaskParams(emu_idx);
					logMessage(LOG_LEVEL_FULL,"sync getVibegTaskParams called\n");
					while (!fRet_anypar.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SETVIBEGTP: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				task_manager::VibegTaskParams* vtp = va_arg(ap,task_manager::VibegTaskParams*);
				va_end(ap);
				if (async) {
					fRet_int = client->setVibegTaskParams(CALLBACK_FUNC,emu_idx,*vtp);
					logMessage(LOG_LEVEL_FULL,"async setVibegTaskParams called\n");
				} else {
					fRet_int = client->setVibegTaskParams(emu_idx,*vtp);
					logMessage(LOG_LEVEL_FULL,"sync setVibegTaskParams called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			case CW_IF_SETDEFAULTDSPPARAMS: {
				va_start(ap,if_code);
				int emu_idx = va_arg(ap,int);
				int mode_gain = va_arg(ap,int);
				va_end(ap);
				if (async) {
					fRet_int = client->setDefaultDSPEmulParams(CALLBACK_FUNC,emu_idx,mode_gain);
					logMessage(LOG_LEVEL_FULL,"async setDefaultDSPEmulParams\n");
				} else {
					fRet_int = client->setDefaultDSPEmulParams(emu_idx,mode_gain);
					logMessage(LOG_LEVEL_FULL,"sync setDefaultDSPEmulParams called\n");
					while (!fRet_int.ready()) RCF::sleepMs(100);
				}
			break; }
			default:
				return TCP_UNKNOWN_FUNCTION_CODE;
		}
	}
	catch (const RCF::Exception & e) {
		int ret = e.getErrorId();
		logMessage(LOG_LEVEL_CRITICAL,"CATCH EXCEPTION[%d] [%s]\n",ret,e.getErrorString().c_str());
		last_exception_code.store(ret);
		cs.lock();
		last_exception_error_str.assign(e.getErrorString());
		cs.unlock();
		return TCP_SERVER_CONNECTION_FAILED;	
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
			
			sigSyncFunctionComplete(if_code,TCP_SYNC_FUNCTION_FAILED,ret);
			return TCP_SYNC_FUNCTION_FAILED;	
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

//--------------------------------------------------------------------------------Async Signals
/*	
void alarmError(int error_code) {
	sigAlarm(error_code);
}

void serverPing(int ping_code) {
	sigPing(ping_code);
}
*/
//--------------------------------------------------------------------------------Errors Decoding

int p347ClientWrapper::getLastExceptionInfo(std::string* dst_string) {
	if (dst_string == NULL) return -EINVAL;
	
	cs.lock();
	dst_string->assign(last_exception_error_str);
	cs.unlock();
	
	return last_exception_code.load();
}

int p347ClientWrapper::getLastConnectionError(std::string* dst_string) {
	if (dst_string == NULL) return -EINVAL;
	
	cs.lock();
	dst_string->assign(last_connection_error_str);
	cs.unlock();
	
	return last_connection_error.load();
}

void p347ClientWrapper::getErrorString(int error_code, std::string* dst_string) {
	if (dst_string == nullptr) return;
	
	switch (error_code) {
		case TCP_SERVER_ALREADY_CONNECTED: {
			dst_string->assign("Server is already connected, do not try to connect it again");
		break; }
		case TCP_SERVER_CONNECTION_FAILED: {
			dst_string->assign("Cannot establish connection with server! Call getLastConnectionError for details");
/*		
			cs.lock();
			char tmpbuf[255];
			memset(&tmpbuf[0],0,255);
			sprintf(&tmpbuf[0],"Cannot establish connection with server, last connection error is: [%d] %s",
								last_connection_error.load(),last_connection_error_str.c_str());
			dst_string->assign(tmpbuf);
			cs.unlock();
*/
		break; }
		case TCP_SERVER_NOT_CONNECTED: {
			dst_string->assign("Server is not connected! You must call connectToTcpServer first.");
		break; }
		case TCP_UNKNOWN_FUNCTION_CODE: {
			dst_string->assign("Internal Wrapper error: unknown function code");
		break; }
		case TCP_SYNC_FUNCTION_FAILED: {
			dst_string->assign("Synchronous execution failed due to connection problems! Call getLastConnectionError for details");
		/*
			cs.lock();
			char tmpbuf[255];
			memset(&tmpbuf[0],0,255);
			sprintf(&tmpbuf[0],"Function synchronous execution failed, last connection error is: [%d] %s",
								last_connection_error.load(),last_connection_error_str.c_str());
			dst_string->assign(tmpbuf);
			cs.unlock();
		*/
		break; }
		case TCP_ASYNC_FUNCTION_FAILED: {
			dst_string->assign("Asynchronous execution failed due to connection problems! Call getLastConnectionError for details");
		break; }
		case TCP_VOID_INITPARS: {
			dst_string->assign("You have passed void instead of pointer to init params");
		break; }
		case TCP_EXCEPTION: {
			dst_string->assign("Exception catched! Call getLastExceptionInfo for details");
		break; }
		default: {
			dst_string->assign("This is not a Client Wrapper's error");
		break; }
	}
}
