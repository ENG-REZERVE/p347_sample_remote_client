#ifndef _p347_CLIENT_WRAPPER
#define _p347_CLIENT_WRAPPER

#include <assert.h>
#include <stdarg.h>

#include "p347_fpga_user.h"
#include "channel_manager.pb.h"
#include "rcf_bind_channel_manager.hpp"
#include "thread.hpp"
#include "uki_log.hpp"

#include <boost/signals2.hpp>

//std lib
#include <atomic>
#include <mutex>

#define CW_ERROR_OFFSET					-4000

#define TCP_SERVER_ALREADY_CONNECTED	CW_ERROR_OFFSET
#define TCP_SERVER_CONNECTION_FAILED	(CW_ERROR_OFFSET-1)
#define TCP_SERVER_NOT_CONNECTED		(CW_ERROR_OFFSET-2)
#define TCP_UNKNOWN_FUNCTION_CODE		(CW_ERROR_OFFSET-3)
#define TCP_SYNC_FUNCTION_FAILED		(CW_ERROR_OFFSET-4)
#define TCP_ASYNC_FUNCTION_FAILED		(CW_ERROR_OFFSET-5)
#define TCP_VOID_INITPARS				(CW_ERROR_OFFSET-6)
#define TCP_EXCEPTION					(CW_ERROR_OFFSET-7)

//internal functions codes
enum client_functions {
	CW_IF_CREATE_CM = 1,
	CW_IF_DELETE_CM,			
	//CW_IF_CREATE_DE,			
	//CW_IF_DELETE_DE,			
	//-------------------
	CW_IF_GET_AVCHANNELS,
	CW_IF_ISCHANNELAV,
	CW_IF_ISCHANNELSCR,
	CW_IF_GETSERVERVER,
	//CW_IF_INIT_CM,		
	//CW_IF_EXIT_CM,			
	CW_IF_TEST_SLEEP,		
	CW_IF_INIT_MUX,		
	CW_IF_DEINIT_MUX,		
	CW_IF_FPGA_DRELOAD,		
	CW_IF_CHECKFPGA,	
	//CW_IF_BINDCHEMUL,
	//-------------------
	CW_IF_READROT,			
	CW_IF_STARTROT,			
	CW_IF_STOPROT,			
	CW_IF_ISROTRUN,			
	CW_IF_ISANYROTRUN,		
	//-------------------
	CW_IF_ISADCRUN,
	CW_IF_ISANYADCRUN,		
	CW_IF_SETUP_ADC,			
	CW_IF_START_ADC,			
	CW_IF_STOP_ADC,			
	CW_IF_WARM_START,		
	CW_IF_WARM_STOP,			
	CW_IF_START_SYNC,		
	CW_IF_STOP_SYNC,			
	CW_IF_READ_OFFSETS,		
	//-------------------
	CW_IF_MUX_ON,			
	CW_IF_MUX_OFF,			
	CW_IF_UNMUX_ALL,			
	CW_IF_MUX_CHANNEL,		
	//-------------------
	CW_IF_TESTSTRING,
	//-------------------
	CW_IF_ADDTASK,
	CW_IF_DELTASK,
	CW_IF_CLEARTASKDATA,
	CW_IF_CLEARTASKLIST,
	CW_IF_CLEARTASKLISTDATA,
	CW_IF_CLEARDATA,
	CW_IF_CLEAR,
	CW_IF_GETTASKRESULT,
	CW_IF_GETTASKSTATUS,
	CW_IF_GETTASKPROGRESS,
	CW_IF_GETTASKNEWRESULT,
	CW_IF_GETTASKSTATE,
	CW_IF_SETTASKSTATE,
	CW_IF_SETTASKLISTSTATE,
	CW_IF_GETTOTALTASKPROGRESS,
	CW_IF_GETANYTASKSWD,
	CW_IF_GETTOTALTASKSSF,
	CW_IF_GETTOTALTASKSLEC,
	CW_IF_TIMESIGOPEN,
	CW_IF_TIMESIGCLOSE,
	CW_IF_ISTIMESIGOPENED,
	CW_IF_GETSIGLEN,
	CW_IF_GETROTLEN,
	CW_IF_GETSENS,
	CW_IF_GETGAIN,
	CW_IF_SETSENS,
	CW_IF_SETGAIN,
	CW_IF_SETROTLABELSCOUNT,
	CW_IF_GETROTLABELSCOUNT,
	CW_IF_GETRMFROMSIG,
	CW_IF_SETRMFROMSIG,
	CW_IF_STARTSAVETS,
	CW_IF_STOPSAVETS,
	CW_IF_STARTSAVERASKTS,
	CW_IF_STOPSAVETASKTS,
	CW_IF_SAVETIMESIG,
	CW_IF_GETBEGSIGOFFSET,
	CW_IF_SETBEGSIGOFFSET,
	CW_IF_GETBEGROTOFFSET,
	CW_IF_SETBEGROTOFFSET,
	CW_IF_GETSRCSF,
	CW_IF_SETSRCSF,
	CW_IF_GETDECIM,
	CW_IF_GETINTERP,
	CW_IF_GETVIBEG,
	CW_IF_SETVIBEG,
	CW_IF_GETVIBEGTP,
	CW_IF_SETVIBEGTP,
};

class pubSigHolder {
public:
	boost::signals2::signal<void (int error_code)> sigAlarm;
	boost::signals2::signal<void (int ping_code)> sigPing;
};

class DeviceSubscriber {
public:
	DeviceSubscriber() {
	};
	~DeviceSubscriber() {
	};
	
	pubSigHolder* backlink;
	
	void alarmError(int error_code) {
		if (backlink != nullptr) {
			backlink->sigAlarm(error_code);
		}
	};
	void serverPing(int ping_code) {
		if (backlink != nullptr) {
			backlink->sigPing(ping_code);
		}
	};
};

class p347ClientWrapper: public pubSigHolder, public UKI_LOG {
public:
	p347ClientWrapper();
	~p347ClientWrapper();
	//------------------------------------------------------test
	int callRemoteSleepTest(bool async);
	//int callTestString(bool async);
	//------------------------------------------------------connectionManaging
	bool getConnectionState();
	int getConnectionError();
	int connectToTcpServer(std::string ip,int port);
	void disconnectFromTcpServer();
	//------------------------------------------------------stateCheckers
	int getAvailableChannels(bool async, channel_manager::AvailableChannels* ret_value);
	int isChannelAvailable(bool async, int ch_idx, bool* ret_value);
	int isChannelsCreated(bool async, bool* ret_value);
	int getServerVersion(bool async, channel_manager::ServerVersion* ret_value);
	//TODO:
	//getChannelManagerState();
	//getDSPEmulState();
	//getMUXManagerState();
	//------------------------------------------------------remoteDeConstructors
	//INFO: reserved parameter would be needed for different memory allocating
	//when we would create channel number-specific allocations
	int createChannelManager(bool async, const channel_manager::ChannelManagerInitParams & cmip, const task_manager::DSPEmulInitParams & dspeip);
	//int createDSPEmul(bool async, const task_manager::DSPEmulInitParams & dspeip);
	int deleteChannelManager(bool async);
	//int deleteDSPEmul(bool async);
	//-------------------------------------------------------init-deinit
	int initMultiplexer(bool async, const channel_manager::MultiplexerInitParams & mip);
	int deinitMultiplexer(bool async);
	//int initChannelManager(bool async, const channel_manager::ChannelManagerInitParams & cmip);
	//int exitChannelManager(bool async);
	int FPGADriverReload(bool async, const std::string & old_name,const std::string & new_path);
	int checkFPGAStatus(bool async);	
	//int bindChannelToEmul(bool async, int ch_idx, int emu_idx);
	//------------------------------------------------------rot
	int readRotData(bool async, unsigned char ch_idx, channel_manager::RotData* ret_value);
	int doStartRotChannel(bool async, const channel_manager::RotChannelInitParams & rcip);
	int stopRotChannel(bool async, unsigned char ch_idx);
	int isRotRunning(bool async, unsigned char ch_idx, bool* ret_value);
	int isAnyRotChannelRunning(bool async, bool* ret_value);
	//------------------------------------------------------adc
	int isADCRunning(bool async, unsigned char ch_idx, bool* ret_value);
	int isAnyADCChannelRunning(bool async, bool* ret_value);
	int doSetupAdcChannel(bool async, const channel_manager::ADCChannelInitParams & adccip);
	int startAdcChannel(bool async, unsigned char ch_idx);
	int stopAdcChannel(bool async, unsigned char ch_idx, bool wfw, bool wfr);
	int warmChannelStart(bool async, unsigned char ch_idx, unsigned short reg_settings);
	int warmChannelEnd(bool async, unsigned char ch_idx);
	int doStartSyncChannels(bool async, const channel_manager::SynctaskChannels & tsc);
	int doStopSyncChannels(bool async, const channel_manager::SynctaskChannels & tsc, bool wfw, bool wfr);
	int readADCTimeOffsets(bool async, channel_manager::ADCTimeOffsets* ret_value);
	//------------------------------------------------------mux
	int switchCommutatorOn(bool async);
	int switchCommutatorOff(bool async);
	int unmuxAll(bool async);
	int doMuxChannel(bool async, int phys_idx, int mux_idx);
	//------------------------------------------------------dspemul
	int addTask(bool async, int emu_idx, const task_manager::AnyTaskParams & task_params);
	int delTask(bool async, int emu_idx,int task_idx);
	int clearTaskData(bool async, int emu_idx, int task_idx,bool AClearOffSet,bool AClearResults);
	int clearTaskList(bool async, int emu_idx);
	int clearTaskListData(bool async, int emu_idx, bool AClearOffSet,bool AClearResults);
	int clearData(bool async, int emu_idx);
	int clear(bool async, int emu_idx);
	int getTaskResult(bool async, int emu_idx,int task_idx,int supposed_type,int res_code, task_manager::AnyTaskResult* ret_value);
	int getTaskStatus(bool async, int emu_idx,int task_idx);
	int getTaskProgress(bool async, int emu_idx,int task_idx);
	int getTaskNewResult(bool async, int emu_idx,int task_idx);
	int getTaskState(bool async, int emu_idx,int task_idx);
	int setTaskState(bool async, int emu_idx,int task_idx,int new_state);
	int setTaskListState(bool async, int emu_idx, const task_manager::IntArray & task_idx_list, int new_state);
	int getTotalTasksProgress(bool async, int emu_idx,bool AExceptMonitoring);
	int getAnyTasksStatusWaitingData(bool async, int emu_idx, bool* ret_value);
	int getAllTasksStatusFinished(bool async, int emu_idx, bool* ret_value);
	int getTotalTasksLastErrorCode(bool async, int emu_idx,bool AExceptMonitoring);
	int timesigOpen(bool async, int emu_idx);
	int timesigClose(bool async, int emu_idx);
	int isTimesigOpened(bool async, int emu_idx, bool* ret_value);
	int getSigLength(bool async, int emu_idx);
	int getRotLength(bool async, int emu_idx);
	int getSensitivity(bool async, int emu_idx, double* ret_value);
	int getGain(bool async, int emu_idx, double* ret_value);
	int setSensitivity(bool async, int emu_idx,double ASensitivity);
	int setGain(bool async, int emu_idx,double AGain);
	int setRotLabelsCount(bool async, int emu_idx,int ARotLabelsCount);
	int getRotLabelsCount(bool async, int emu_idx);
	int getRotMetkasFromSig(bool async, int emu_idx, bool* ret_value);
	int setRotMetkasFromSig(bool async, int emu_idx, bool ARotMetkasFromSig);
	int startSaveTimeSig(bool async, int emu_idx,const std::string & AFileNameForSave,int ATSDTCForSave, bool* ret_value);
	int stopSaveTimeSig(bool async, int emu_idx);
	int startSaveTaskTimeSig(bool async, int emu_idx,int task_idx,const std::string & AFileNameForSave,int ATSDTCForSave, bool* ret_value);
	int stopSaveTaskTimeSig(bool async, int emu_idx,int task_idx);
	int saveTimeSig(bool async, int emu_idx, const std::string & AFileName, bool* ret_value);
	int getBegSigOffSet(bool async, int emu_idx, long int* ret_value);
	int setBegSigOffSet(bool async, int emu_idx,long int ABegSigOffSet);
	int getBegRotOffSet(bool async, int emu_idx, long int* ret_value);
	int setBegRotOffSet(bool async, int emu_idx,long int ABegRotOffSet);
	int getSrcSamplingFreq(bool async, int emu_idx);
	int setSrcSamplingFreq(bool async, int emu_idx, int ASrcSamplingFreq);
	int getDecim(bool async, int emu_idx);
	int getInterp(bool async, int emu_idx);
	int getVibeg(bool async, int emu_idx, bool* ret_value);
	int setVibeg(bool async, int emu_idx, bool AVibeg);
	int getVibegTaskParams(bool async, int emu_idx, task_manager::AnyTaskParams* ret_value);
	int setVibegTaskParams(bool async, int emu_idx, task_manager::VibegTaskParams & avtp);
	//-------------------------------------------------------
	void getErrorString(int error_code, std::string* dst_string);
	int getLastExceptionInfo(std::string* dst_string);
	int getLastConnectionError(std::string* dst_string);
	//-------------------------------------------------------
	//-------------------------------------------------------
	//-------------------------------------------------------
	void onAsyncFunctionCompleted(int if_code);
	
	//void onAsyncFunctionCompleted(RCF::Future<int> fRet, int if_code);
	//void onReadRotDataCompleted(RCF::Future<channel_manager::RotData> fRet_rot, int if_code);
	//void onReadRotDataCompleted(RCF::Future<channel_manager::RotData>* fRet_rot, int if_code);
	
	//void onReadRotDataCompleted(channel_manager::RotData* result_ptr);
	void onReadRotDataCompleted(void);
	void onBoolDataCompleted(void);
	//---------------------------------------------------------
	boost::signals2::signal<void (int if_code, int connection_error, int function_error)> sigAsyncFunctionComplete;
	boost::signals2::signal<void (int if_code, int connection_error, int function_error)> sigSyncFunctionComplete;
	
	RCF::Future<int> 								fRet_int;
	RCF::Future<channel_manager::RotData> 			fRet_rot;
	RCF::Future<bool> 								fRet_bool;
	RCF::Future<channel_manager::ADCTimeOffsets> 	fRet_off;
	RCF::Future<task_manager::AnyTaskResult>		fRet_anyres;
	RCF::Future<double> 							fRet_double;
	RCF::Future<long int> 							fRet_longint;
	RCF::Future<task_manager::AnyTaskParams> 		fRet_anypar;
	RCF::Future<channel_manager::AvailableChannels>	fRet_avchannels;
	RCF::Future<channel_manager::ServerVersion>		fRet_version;
	
	DeviceSubscriber			subDev;
private:
	//---------------------------------------------------------------
	void*						some_result_pointer;
	//channel_manager::RotData* 	rotdata_result_ptr;
	//bool*						bool_result_ptr;
	//---------------------------------------------------------------
	bool is_async_running;
	
	int _executeFunction(bool async, int if_code, ...);
	int _processResult(int if_code);
	
	std::mutex					cs;
	RcfClient<CM_connection>*	client;
	
	std::atomic_bool			is_connected_to_server;
	std::atomic_int				last_connection_error;
	std::string					last_connection_error_str;
	std::atomic_int				last_exception_code;
	std::string					last_exception_error_str;
	//std::atomic_int				connection_fails_counter;
	
	RCF::RcfServer*				subServer;
	RCF::SubscriptionParms 		subParms;
	RCF::SubscriptionPtr		subPtr;
};

#endif
