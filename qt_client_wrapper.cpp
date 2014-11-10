#include "qt_client_wrapper.hpp"
#include "dfilter.h"
//#include "DSPEmulDefines.h"

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
	
//int QClientWrapper::qt_createChannelManager(bool async) {
int QClientWrapper::qt_initDevice(bool async) {
	UPDATE_BUSY

	p347_conf::DeviceInitParams dip;
	/*
	channel_manager::ChannelManagerInitParams* initpars = dip.mutable_cm_init();
	
	//manager logging
	initpars->set_log_level_file(0);
	initpars->set_log_level_console(3);
	initpars->set_log_level_daemon(0);
    //dsp helper logging
	initpars->set_dsph_level_file(0);
	initpars->set_dsph_level_console(3);
	initpars->set_dsph_level_daemon(0);
    //initial parameters
	initpars->set_base_timing(1000);
	initpars->set_drv_buf_size(32768);
	initpars->set_usr_proc_len(32768);
	initpars->set_spi_speed_hz(93750);
	initpars->set_main_sleep_us(500);
	initpars->set_idle_sleep_us(1000);

	initpars->set_daemon_ipc_path("/mnt/share");
	initpars->set_daemon_ipc_key(45);
	initpars->set_reset_at_open(true);

    for (int i=0; i<p347_ADC_CHANNELS_CNT; i++) {
    	initpars->add_chan_level_file(0);
    	initpars->add_chan_level_console(3);
    	initpars->add_chan_level_daemon(0);
    }

    channel_manager::DriverTimings* drt = initpars->mutable_driver_timings();
    drt->set_rot_run(1000);
    drt->set_adc_set_params1(500);
    drt->set_adc_set_params2(100);
    drt->set_adc_set_params3(100);
    drt->set_adc_run(100);
    drt->set_adc_run_sync(100);
	
	task_manager::DSPEmulInitParams* dspip = dip.mutable_dspemul_init();
	dspip->set_atsdtc(TSDTC_24DATA_8STATUS);
    dspip->set_ainitsigbufferlength(8*20*65536);
    dspip->set_asigbufferincrement(8*5*65536);
    dspip->set_ainitrotbufferlength(1024);
    dspip->set_arotbufferincrement(1024);

	channel_manager::MultiplexerInitParams* muxip = dip.mutable_mux_init();
	muxip->set_dev_name("/dev/ttyO2");
	muxip->set_speed(115200);
	muxip->set_params("8N1");
	*/
	//return createChannelManager(async,initpars,dip);
	return initDevice(async,dip);
}


int QClientWrapper::qt_deInitDevice(bool async) {
	UPDATE_BUSY
	return deInitDevice(async);
}

/*
int QClientWrapper::qt_deleteChannelManager(bool async) {
	UPDATE_BUSY
	return deleteChannelManager(async);
}
*/

//---------------------------------------------------------------------------------ROT

int	QClientWrapper::qt_doStartRotChannel(bool async, int ch_idx) {
	UPDATE_BUSY
	channel_manager::RotChannelInitParams rcip;
	rcip.set_rot_idx(ch_idx);
	/*
	rcip.set_av_num(5);
	rcip.set_pw_min_us(25);
	rcip.set_period_min_us(8333);
	rcip.set_period_max_us(1000000);
	*/
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
	/*
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
	*/
	
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
	//tsc.set_sync_reg(0x1501021D);

	return doStartSyncChannels(async, tsc);
}

int QClientWrapper::qt_doStopSyncChannels(bool async) {
	UPDATE_BUSY
	channel_manager::SynctaskChannels tsc;

	//stop 3 and 4
	tsc.set_adc_ch_cnt(2);
	tsc.add_adc_ch_idx(3);
	tsc.add_adc_ch_idx(4);
	//tsc.set_sync_reg(0x1501021D);
	
	return doStopSyncChannels(async, tsc, false, false);
}

int QClientWrapper::qt_readADCTimeOffsets(bool async) {
	UPDATE_BUSY
	return readADCTimeOffsets(async, &adcto);
}

//------------------------------------------------------------------------------MUX
/*
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
*/	
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

int QClientWrapper::qt_getTaskResult(bool async, int ch_idx, int task_idx) {
	UPDATE_BUSY
	int ret = getTaskResult(async,ch_idx,task_idx,EMUL_TASK_TYPE_QUALITY,0,&anyres);

	printf("async=%d, error_code =%d\n",async,anyres.error_code());
	
	return ret;
};

int QClientWrapper::qt_addQualityTask(bool async, int ch_idx) {
	UPDATE_BUSY
	
	setGain(async, ch_idx,5);
	setSensitivity(async, ch_idx,70);
	setRotLabelsCount(async, ch_idx,1);
	setBegSigOffSet(async, ch_idx,0);
	setBegRotOffSet(async, ch_idx,0);
	setSrcSamplingFreq(async, ch_idx, 72000);
	
	//-------------------------------------------------------------------------
	task_manager::AnyTaskParams	any_task_params;
	any_task_params.set_error_code(0);
	
	task_manager::StatTimeSigTaskParams* stattimesig_task_params = any_task_params.mutable_stattimesig_par();
	stattimesig_task_params->set_onemeastimesiglength(65536); //(s_freq*1125/1024)*realization_time/1000;
	stattimesig_task_params->set_ampunit(TU_AMP_A_M_S2);
	stattimesig_task_params->set_integrateintimedomain(false);
	//common task params for stat
	task_manager::CommonTaskParams* stat_CMN_task_params = stattimesig_task_params->mutable_cmn();
	stat_CMN_task_params->set_monitoring(false);
	stat_CMN_task_params->set_timesigtype(TS_DIRECT);      // = SpectrType
	stat_CMN_task_params->set_lpfilterindex(8);	//25600+¡ - ò ýþòv¿ øýôõú¸ð¿
	stat_CMN_task_params->set_lpfilterdecim(0);	//ø¸ÿþû¹÷ºõª¸  LPFilterIndex
	stat_CMN_task_params->set_lpfiltertype(TF_SLIDING_LOWPASS_FIR);
	stat_CMN_task_params->set_lpfilterusualw(false);   // = LPFilterGenW
	stat_CMN_task_params->set_bpfilterindex(0);
	stat_CMN_task_params->set_bpfiltertype(TF_NONE);
	stat_CMN_task_params->set_bpfilterusualw(false);
	stat_CMN_task_params->set_rsmpfilterindex(5);
	stat_CMN_task_params->set_rsmpfiltertype(TF_SLIDING_RESAMPLE_FIR);
	stat_CMN_task_params->set_rsmpfilterusualw(false); // = RSMPFilterGenW
	stat_CMN_task_params->set_hpfilterindex(0);	//HPFT_01_1_40_002_4096
	stat_CMN_task_params->set_hpfiltertype(0);	//
	double stat_CMN_Alfa = 0.9985;
	stat_CMN_task_params->set_hpfilterparam1(stat_CMN_Alfa);
	stat_CMN_task_params->set_fparamscount(0);
	stat_CMN_task_params->set_avgcount(1);			// = SpectAvgCount
	stat_CMN_task_params->set_frotlimitsactive(false);
	stat_CMN_task_params->set_stabcontrol(false);
	stat_CMN_task_params->set_rotcontrol(false);
	stat_CMN_task_params->set_waitperiodforrotmetka(0);
	stat_CMN_task_params->set_timesigsrctype(TSST_ORIGINAL);
	stat_CMN_task_params->set_timesigid(TSST_ORIGINAL);
	stat_CMN_task_params->set_inittaskstate(TTS_PLAY);
	stat_CMN_task_params->set_finaltaskstate(TTS_STOP);
	stat_CMN_task_params->set_tsdtc(TSDTC_LONGDOUBLE);
	stat_CMN_task_params->set_calctypetsdtc(TSDTC_LONGDOUBLE);
	stat_CMN_task_params->set_tde(TDE_P347ADC24);
	//offset
	task_manager::TOffset* stat_offset = stat_CMN_task_params->mutable_offset();
	stat_offset->set_type(OFFST_SAMPLES);
	stat_offset->set_value(TOSA_FROM_END_TS);
	//delta offset
	task_manager::TOffset* stat_delta_offset = stat_CMN_task_params->mutable_deltaoffset();
	stat_delta_offset->set_type(OFFST_SAMPLES);
	stat_delta_offset->set_value(0);
	//task_manager::FParams* stat_filter_params = stat_CMN_task_params->add_fparamslist();
	//filter params
	//stat_filter_params->set_type(FPARAMS_NONE);
	//frot limits
	task_manager::TFrotLimits* stat_frot_limits = stat_CMN_task_params->mutable_frotlimits();
	stat_frot_limits->set_frot_min(0);
	stat_frot_limits->set_frot_max(1000000);
	stat_frot_limits->set_deltafrotonemeas(0);
	stat_frot_limits->set_deltafrotallmeas(0);
	//add task stat
	int ret = addTask(async, ch_idx, any_task_params);
	printf("addtask stattimesig ret %d\n",ret);

	//-----------------------------------------------------------------quality
	task_manager::AnyTaskParams	atp;
	atp.set_error_code(0);
	
	task_manager::QualityTimeSigTaskParams* quality_task_params = atp.mutable_quality_par();
	quality_task_params->set_onemeastimesiglength(65536); //(s_freq*1125/1024)*realization_time/1000;
	quality_task_params->set_amppeaktopeakmin(0);
	quality_task_params->set_amppeaktopeakmax(100);
	quality_task_params->set_ampstdmin(0.015);
	quality_task_params->set_ampstdmax(2);
	//common task params for vharm
	task_manager::CommonTaskParams* quality_CMN_task_params = quality_task_params->mutable_cmn();
	quality_CMN_task_params->set_monitoring(false);
	quality_CMN_task_params->set_timesigtype(TS_DIRECT);      // = SpectrType
	quality_CMN_task_params->set_lpfilterindex(8);	//25600+¡ - ò ýþòv¿ øýôõú¸ð¿
	quality_CMN_task_params->set_lpfilterdecim(0);	//ø¸ÿþû¹÷ºõª¸  LPFilterIndex
	quality_CMN_task_params->set_lpfiltertype(TF_SLIDING_LOWPASS_FIR);
	quality_CMN_task_params->set_lpfilterusualw(false);   // = LPFilterGenW
	quality_CMN_task_params->set_bpfilterindex(0);
	quality_CMN_task_params->set_bpfiltertype(TF_NONE);
	quality_CMN_task_params->set_bpfilterusualw(false);
	quality_CMN_task_params->set_rsmpfilterindex(5);
	quality_CMN_task_params->set_rsmpfiltertype(TF_SLIDING_RESAMPLE_FIR);
	quality_CMN_task_params->set_rsmpfilterusualw(false); // = RSMPFilterGenW
	quality_CMN_task_params->set_hpfilterindex(0);	//HPFT_01_1_40_002_4096
	quality_CMN_task_params->set_hpfiltertype(0);	//
	double quality_CMN_Alfa = 0.9985;
	quality_CMN_task_params->set_hpfilterparam1(quality_CMN_Alfa);
	quality_CMN_task_params->set_fparamscount(0);
	quality_CMN_task_params->set_avgcount(1);			// = SpectAvgCount
	quality_CMN_task_params->set_frotlimitsactive(false);
	quality_CMN_task_params->set_stabcontrol(false);
	quality_CMN_task_params->set_rotcontrol(false);
	quality_CMN_task_params->set_waitperiodforrotmetka(0);
	quality_CMN_task_params->set_timesigsrctype(TSST_ORIGINAL);
	quality_CMN_task_params->set_timesigid(TSST_ORIGINAL);
	quality_CMN_task_params->set_inittaskstate(TTS_PLAY);
	quality_CMN_task_params->set_finaltaskstate(TTS_STOP);
	quality_CMN_task_params->set_tsdtc(TSDTC_LONGDOUBLE);
	quality_CMN_task_params->set_calctypetsdtc(TSDTC_LONGDOUBLE);
	quality_CMN_task_params->set_tde(TDE_P347ADC24);
	//offset
	task_manager::TOffset* quality_offset = quality_CMN_task_params->mutable_offset();
	quality_offset->set_type(OFFST_SAMPLES);
	quality_offset->set_value(TOSA_FROM_END_TS);
	//delta offset
	task_manager::TOffset* quality_delta_offset = quality_CMN_task_params->mutable_deltaoffset();
	quality_delta_offset->set_type(OFFST_SAMPLES);
	quality_delta_offset->set_value(0);
	//task_manager::FParams* filter_params = quality_CMN_task_params->add_fparamslist();
	//filter params
	//quality_filter_params->set_type(FPARAMS_NONE);
	//frot limits
	task_manager::TFrotLimits* quality_frot_limits = quality_CMN_task_params->mutable_frotlimits();
	quality_frot_limits->set_frot_min(0);
	quality_frot_limits->set_frot_max(1000000);
	quality_frot_limits->set_deltafrotonemeas(0);
	quality_frot_limits->set_deltafrotallmeas(0);	
	
	return addTask(async,ch_idx,atp);
};

int QClientWrapper::qt_timesigOpen(bool async, int ch_idx) {
	return timesigOpen(async, ch_idx);
}

int QClientWrapper::qt_timesigClose(bool async, int ch_idx) {
	return timesigClose(async, ch_idx);
}

int	QClientWrapper::qt_loadConfiguration(int conf_id) {
	if (configurator == NULL) return -EINVAL;
	
	return configurator->loadConfig(conf_id);
}
