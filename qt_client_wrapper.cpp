#include "qt_client_wrapper.hpp"
#include "DSPEmulDefines.h"

QClientWrapper::QClientWrapper(QObject* parent) : 
	QObject(parent), p347ClientWrapper()
{
	connection_async_complete = sigAsyncFunctionComplete.connect(boost::bind(&QClientWrapper::onAsyncCompleted,this,_1,_2,_3));
	connection_sync_complete = sigSyncFunctionComplete.connect(boost::bind(&QClientWrapper::onSyncCompleted,this,_1,_2,_3));
	connection_alarm = sigAlarm.connect(boost::bind(&QClientWrapper::onAlarm,this,_1));
	connection_ping = sigPing.connect(boost::bind(&QClientWrapper::onPing,this,_1));
}

QClientWrapper::~QClientWrapper() {
	connection_async_complete.disconnect();
	connection_sync_complete.disconnect();
	connection_alarm.disconnect();
	connection_ping.disconnect();
}

void QClientWrapper::onAlarm(int alarm_code) {
	last_alarm_code = alarm_code;
	emit isAlarmCodeChanged();
}

void QClientWrapper::onPing(int ping_code) {
	last_ping_code = ping_code;
	emit isPingCodeChanged();
}

inline void QClientWrapper::_gui_reaction(int if_code) {
	switch (if_code) {
		case CW_IF_READROT: {
			emit isFreqChanged();
		break; }
		case CW_IF_ISROTRUN: {
			emit isRRChanged();
		break; }
		case CW_IF_ISANYROTRUN: {
			emit isAnyRRChanged();
		break; }
		case CW_IF_ISADCRUN: {
			emit isARChanged();
		break; }
		case CW_IF_ISANYADCRUN: {
			emit isAnyARChanged();
		break; }
		case CW_IF_GETSERVERVER: {
			emit svChanged();
		break; }
		case CW_IF_GET_AVCHANNELS: {
			emit avChanged();
		break; }
		case CW_IF_ISCHANNELSCR: {
			emit isCHACChanged();
		break; }
		case CW_IF_ISCHANNELAV: {
			emit isCHAVChanged();
		break; }
	}
}
	
void QClientWrapper::onAsyncCompleted(int if_code, int connection_error, int function_error) {
	printf("onAsyncCompleted if_code=%d, connection_error=%d, function_error=%d\n",if_code,connection_error,function_error);
	_gui_reaction(if_code);
	
	is_async_running = false;
	emit isAsyncRunningChanged();
}

void QClientWrapper::onSyncCompleted(int if_code, int connection_error, int function_error) {
	printf("onSyncCompleted if_code=%d, connection_error=%d, function_error=%d\n",if_code,connection_error,function_error);
	_gui_reaction(if_code);
}

//---------------------------------------------------------------------------------------properties
//TODO: move to header

double QClientWrapper::lastFreq() {
	//printf("LF=%.5lf\n",rd.freq_avr());
	return rd.freq_avr();
}

QString QClientWrapper::serverVersion() {
	QString rets = tr("%1.%2.").arg(s_version.major()).arg(s_version.minor()) + QString::fromStdString(s_version.build());
	return rets;
}

QString QClientWrapper::avChannelsStr() {
	QString rets = "";//tr("%1.%2.").arg(s_version.major()).arg(s_version.minor()) + QString::fromStdString(s_version.build());
	int sz = av_channels.channel_idx_size();
	if (sz <= 0) {
		rets = "No channels";
	} else  for (int i=0; i<sz; i++) {
				rets.append(tr("%1,").arg(av_channels.channel_idx(i)));
			}
	return rets;
}

bool QClientWrapper::isCHAV() {
	return is_channel_available;
}

bool QClientWrapper::isCHAC() {
	return is_channels_created;
}

int QClientWrapper::alarmCode() {
	return last_alarm_code;
}

int QClientWrapper::pingCode() {
	return last_ping_code;
}

bool QClientWrapper::isRR() {
	return is_rr;
}

bool QClientWrapper::isAnyRR() {
	return is_anyrr;
}

bool QClientWrapper::isAR() {
	return is_ar;
}

bool QClientWrapper::isAnyAR() {
	return is_anyar;
}

bool QClientWrapper::isAsyncRunning() {
	return is_async_running;
}
/*
int QClientWrapper::totalTasksProgress() {
	return total_tasks_progress;
}
*/
QString QClientWrapper::getOffstring() {
	QString rets = "";
	int errc = adcto.error_code();
	
	if (errc != 0) {
		rets = tr("Error: %1").arg(errc);
	} else {
		if (adcto.offset_size() < 1) {
			rets = tr("No data for offsets");
		} else for (int i=0; i<adcto.offset_size(); i++) {
			QString addstr = tr("off[%1] = %2; ").arg(i).arg(adcto.offset(i));
			rets.append(addstr);
		}
	}
	
	return rets;
}

//---------------------------------------------------------------------------------------qt-wrapping
int QClientWrapper::qt_connectToTcpServer(QString ip,int port) {
	return connectToTcpServer(ip.toStdString(),port);
}

int QClientWrapper::qt_disconnectFromTcpServer() {
	disconnectFromTcpServer();
	return 0;
}



//---------------------------------------------------------------------------------------
#define UPDATE_BUSY \
	if (async) { is_async_running = true; emit isAsyncRunningChanged(); }

int QClientWrapper::qt_getAvailableChannels(bool async) {
	UPDATE_BUSY
	return getAvailableChannels(async,&av_channels);
}

int QClientWrapper::qt_isChannelAvailable(bool async, int ch_idx){
	UPDATE_BUSY
	return isChannelAvailable(async,ch_idx,&is_channel_available);
}

int QClientWrapper::qt_isChannelsCreated(bool async){
	UPDATE_BUSY
	return isChannelsCreated(async,&is_channels_created);
}

int QClientWrapper::qt_getServerVersion(bool async){
	UPDATE_BUSY
	return getServerVersion(async,&s_version);
}	
	
int QClientWrapper::qt_createChannelManager(bool async) {
	UPDATE_BUSY

	channel_manager::ChannelManagerInitParams initpars;
	
		//manager logging
	initpars.set_log_level_file(0);
	initpars.set_log_level_console(3);
	initpars.set_log_level_daemon(0);
    //dsp helper logging
	initpars.set_dsph_level_file(0);
	initpars.set_dsph_level_console(3);
	initpars.set_dsph_level_daemon(0);
    //initial parameters
	initpars.set_base_timing(1000);
	initpars.set_drv_buf_size(32768);
	initpars.set_usr_proc_len(32768);
	initpars.set_spi_speed_hz(93750);
	initpars.set_main_sleep_us(500);
	initpars.set_idle_sleep_us(1000);

	initpars.set_daemon_ipc_path("/mnt/share");
	initpars.set_daemon_ipc_key(45);
	initpars.set_reset_at_open(true);

    for (int i=0; i<p347_ADC_CHANNELS_CNT; i++) {
    	initpars.add_chan_level_file(0);
    	initpars.add_chan_level_console(3);
    	initpars.add_chan_level_daemon(0);
    }

    channel_manager::DriverTimings* drt = initpars.mutable_driver_timings();
    drt->set_rot_run(1000);
    drt->set_adc_set_params1(500);
    drt->set_adc_set_params2(100);
    drt->set_adc_set_params3(100);
    drt->set_adc_run(100);
    drt->set_adc_run_sync(100);
	
	task_manager::DSPEmulInitParams dip;
	dip.set_atsdtc(TSDTC_24DATA_8STATUS);
    dip.set_ainitsigbufferlength(8*20*65536);
    dip.set_asigbufferincrement(8*5*65536);
    dip.set_ainitrotbufferlength(1024);
    dip.set_arotbufferincrement(1024);
    //dip.set_achannelscount(p347_ADC_CHANNELS_CNT);
	
	return createChannelManager(async,initpars,dip);
}

int QClientWrapper::qt_deleteChannelManager(bool async) {
	UPDATE_BUSY
	return deleteChannelManager(async);
}

/*
int QClientWrapper::qt_createDSPEmul(bool async) {
	UPDATE_BUSY
	task_manager::DSPEmulInitParams dip;
	dip.set_atsdtc(TSDTC_24DATA_8STATUS);
    dip.set_ainitsigbufferlength(8*20*65536);
    dip.set_asigbufferincrement(8*5*65536);
    dip.set_ainitrotbufferlength(1024);
    dip.set_arotbufferincrement(1024);
    dip.set_achannelscount(p347_ADC_CHANNELS_CNT);
	
	return createDSPEmul(async,dip);
}

int QClientWrapper::qt_deleteDSPEmul(bool async) {
	UPDATE_BUSY
	return deleteDSPEmul(async);
}
*/
int	QClientWrapper::qt_sleepTest(bool async) {
	UPDATE_BUSY
	return callRemoteSleepTest(async);
}

//---------------------------------------------------------------------------------------

/*
int QClientWrapper::qt_initChannelManager(bool async) {
	UPDATE_BUSY
	channel_manager::ChannelManagerInitParams initpars;
	
		//manager logging
	initpars.set_log_level_file(0);
	initpars.set_log_level_console(3);
	initpars.set_log_level_daemon(0);
    //dsp helper logging
	initpars.set_dsph_level_file(0);
	initpars.set_dsph_level_console(3);
	initpars.set_dsph_level_daemon(0);
    //initial parameters
	initpars.set_base_timing(1000);
	initpars.set_drv_buf_size(32768);
	initpars.set_usr_proc_len(32768);
	initpars.set_spi_speed_hz(93750);
	initpars.set_main_sleep_us(500);
	initpars.set_idle_sleep_us(1000);

	initpars.set_daemon_ipc_path("/mnt/share");
	initpars.set_daemon_ipc_key(45);
	initpars.set_reset_at_open(true);

    for (int i=0; i<p347_ADC_CHANNELS_CNT; i++) {
    	initpars.add_chan_level_file(0);
    	initpars.add_chan_level_console(3);
    	initpars.add_chan_level_daemon(0);
    }

    channel_manager::DriverTimings* drt = initpars.mutable_driver_timings();
    drt->set_rot_run(1000);
    drt->set_adc_set_params1(500);
    drt->set_adc_set_params2(100);
    drt->set_adc_set_params3(100);
    drt->set_adc_run(100);
    drt->set_adc_run_sync(100);
	
	return initChannelManager(async, initpars);
}

int	QClientWrapper::qt_exitChannelManager(bool async) {
	UPDATE_BUSY
	exitChannelManager(async);
	return 0;
}
*/
//---------------------------------------------------------------------------------ROT

int	QClientWrapper::qt_doStartRotChannel(bool async, int ch_idx) {
	UPDATE_BUSY
	channel_manager::RotChannelInitParams rcip;
	rcip.set_rot_idx(ch_idx);
	rcip.set_av_num(5);
	rcip.set_pw_min_us(25);
	rcip.set_period_min_us(8333);
	rcip.set_period_max_us(1000000);
	
	return doStartRotChannel(async, rcip);
}

int	QClientWrapper::qt_doStopRotChannel(bool async, int ch_idx) {
	UPDATE_BUSY
	return stopRotChannel(async, ch_idx);
}

int	QClientWrapper::qt_doReadRotChannel(bool async, int ch_idx) {
	UPDATE_BUSY
	int ret = readRotData(async,ch_idx,&rd);
	emit isFreqChanged();
	return ret;
}

int QClientWrapper::qt_isRotRunning(bool async, int ch_idx) {
	UPDATE_BUSY
	int ret = isRotRunning(async, ch_idx, &is_rr);
	emit isRRChanged();
	return ret;
}

int	QClientWrapper::qt_isAnyRotRunning(bool async) {
	UPDATE_BUSY
	int ret = isAnyRotChannelRunning(async, &is_anyrr);
	emit isAnyRRChanged();
	return ret;
}

//---------------------------------------------------------------------------------ADC
int QClientWrapper::qt_isADCRunning(bool async, int ch_idx) {
	UPDATE_BUSY
	int ret = isADCRunning(async, ch_idx, &is_ar);
	emit isARChanged();
	return ret;
}

int QClientWrapper::qt_isAnyADCChannelRunning(bool async) {
	UPDATE_BUSY
	int ret = isAnyADCChannelRunning(async, &is_anyar);
	emit isAnyARChanged();
	return ret;
}

int QClientWrapper::qt_doSetupAdcChannel(bool async, int ch_idx) {
	UPDATE_BUSY
	channel_manager::ADCChannelInitParams adccip;
	
	adccip.set_ch_idx(ch_idx);
	adccip.set_rot_idx(0);
	adccip.set_drv_buf_size(32768);
	adccip.set_usr_proc_len(32768);
	adccip.set_usr_proc_cnt(0);
	adccip.set_sen_filter_id(0);
	
	channel_manager::ADCParams* ap = adccip.mutable_ap();
	ap->set_control1(0x001D);
	ap->set_control2(0x0002);
	ap->set_offset(0);
	ap->set_gain(0x8000);
	ap->set_overrange(0xCCCC);
	ap->set_ch_settings(0x001D);
	
	return doSetupAdcChannel(async, adccip);
}

int QClientWrapper::qt_startAdcChannel(bool async, int ch_idx) {
	UPDATE_BUSY
	return startAdcChannel(async, ch_idx);
}

int QClientWrapper::qt_stopAdcChannel(bool async, int ch_idx) {
	UPDATE_BUSY
	return stopAdcChannel(async, ch_idx, false, false);
}

int QClientWrapper::qt_warmChannelStart(bool async, int ch_idx) {
	UPDATE_BUSY
	return warmChannelStart(async, ch_idx, 0x0001);
}

int QClientWrapper::qt_warmChannelEnd(bool async, int ch_idx) {
	UPDATE_BUSY
	return warmChannelEnd(async, ch_idx);
}

int QClientWrapper::qt_doStartSyncChannels(bool async) {
	UPDATE_BUSY
	channel_manager::SynctaskChannels tsc;
	
	//start 3 and 4
	tsc.set_adc_ch_cnt(2);
	tsc.add_adc_ch_idx(3);
	tsc.add_adc_ch_idx(4);
	tsc.set_sync_reg(0x1501021D);

	return doStartSyncChannels(async, tsc);
}

int QClientWrapper::qt_doStopSyncChannels(bool async) {
	UPDATE_BUSY
	channel_manager::SynctaskChannels tsc;

	//stop 3 and 4
	tsc.set_adc_ch_cnt(2);
	tsc.add_adc_ch_idx(3);
	tsc.add_adc_ch_idx(4);
	tsc.set_sync_reg(0x1501021D);
	
	return doStopSyncChannels(async, tsc, false, false);
}

int QClientWrapper::qt_readADCTimeOffsets(bool async) {
	UPDATE_BUSY
	return readADCTimeOffsets(async, &adcto);
}

//------------------------------------------------------------------------------MUX

int QClientWrapper::qt_initMultiplexer(bool async) {
	UPDATE_BUSY
	channel_manager::MultiplexerInitParams mip;
	mip.set_dev_name("/dev/p347_ch_mux");
	mip.set_speed(115200);
	mip.set_params("8N1");
	
	return initMultiplexer(async,mip);
};

int QClientWrapper::qt_deinitMultiplexer(bool async) {
	UPDATE_BUSY
	return deinitMultiplexer(async);
};
	
int QClientWrapper::qt_switchCommutatorOn(bool async) {
	UPDATE_BUSY
	return switchCommutatorOn(async);
};

int QClientWrapper::qt_switchCommutatorOff(bool async) {
	UPDATE_BUSY
	return switchCommutatorOff(async);
};

int QClientWrapper::qt_unmuxAll(bool async) {
	UPDATE_BUSY
	return unmuxAll(async);
};

int QClientWrapper::qt_doMuxChannel(bool async) {
	UPDATE_BUSY
	return doMuxChannel(async,1,5);
};

//-------------------

int QClientWrapper::qt_getTotalTasksProgress(bool async, int ch_idx) {
	UPDATE_BUSY
	return getTotalTasksProgress(async,ch_idx,false);
};
