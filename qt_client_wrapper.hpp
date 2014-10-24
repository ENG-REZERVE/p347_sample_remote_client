#ifndef _p347_QTCLIENT_WRAPPER
#define _p347_QTCLIENT_WRAPPER

#include <QObject>
#include <QString>
#include "client_wrapper.hpp"

class QClientWrapper: public QObject, public p347ClientWrapper 
{
	Q_OBJECT
public:
	explicit QClientWrapper(QObject *parent = 0);
	~QClientWrapper();
	
	channel_manager::RotData rd;
	channel_manager::ADCTimeOffsets adcto;
	channel_manager::ServerVersion s_version;
	channel_manager::AvailableChannels av_channels;
	
	//bool last_bool;
	bool is_rr;
	bool is_anyrr;
	bool is_ar;
	bool is_anyar;
	bool is_async_running;
	bool is_channel_available;
	bool is_channels_created;
	int last_alarm_code;
	int last_ping_code;
	
	Q_PROPERTY(bool isAsyncRunning READ isAsyncRunning NOTIFY isAsyncRunningChanged)
	Q_PROPERTY(int alarmCode READ alarmCode NOTIFY isAlarmCodeChanged)
	Q_PROPERTY(int pingCode READ pingCode NOTIFY isPingCodeChanged)
	
	Q_PROPERTY(double lastFreq READ lastFreq NOTIFY isFreqChanged)
	Q_PROPERTY(QString offstring READ getOffstring)
	Q_PROPERTY(bool isRR READ isRR NOTIFY isRRChanged)
	Q_PROPERTY(bool isAnyRR READ isAnyRR NOTIFY isAnyRRChanged)
	Q_PROPERTY(bool isAR READ isAR NOTIFY isARChanged)
	Q_PROPERTY(bool isAnyAR READ isAnyAR NOTIFY isAnyARChanged)
	
	Q_PROPERTY(bool isCHAV READ isCHAV NOTIFY isCHAVChanged)
	Q_PROPERTY(bool isCHAC READ isCHAC NOTIFY isCHACChanged)
	Q_PROPERTY(QString serverVersion READ serverVersion NOTIFY svChanged)
	Q_PROPERTY(QString avChannelsStr READ avChannelsStr NOTIFY avChanged)
	
	double	lastFreq();
	int alarmCode();
	int pingCode();
	
	bool isCHAV();
	bool isCHAC();
	QString serverVersion();
	QString avChannelsStr();
	
	bool isRR();
	bool isAnyRR();
	bool isAR();
	bool isAnyAR();
	bool isAsyncRunning();
	
	QString getOffstring();
	
	void onAsyncCompleted(int if_code, int connection_error, int function_error);
	void onSyncCompleted(int if_code, int connection_error, int function_error);
	void onAlarm(int alarm_code);
	void onPing(int ping_code);
signals:
	void isRRChanged();
	void isAnyRRChanged();	
	void isARChanged();
	void isAnyARChanged();
	void isFreqChanged();
	void isAsyncRunningChanged();
	void isAlarmCodeChanged();
	void isPingCodeChanged();
	void svChanged();
	void avChanged();
	void isCHAVChanged();
	void isCHACChanged();
public slots:
	//----------------------------------------------------------
	int 	qt_connectToTcpServer(QString ip,int port);
	int 	qt_disconnectFromTcpServer();
	//----------------------------------------------------------
	int 	qt_createChannelManager(bool async);
	int 	qt_deleteChannelManager(bool async);
	//int 	qt_createDSPEmul(bool async);
	//int 	qt_deleteDSPEmul(bool async);
	int		qt_sleepTest(bool async);
	//----------------------------------------------------------
	int 	qt_getAvailableChannels(bool async);
	int 	qt_isChannelAvailable(bool async, int ch_idx);
	int 	qt_isChannelsCreated(bool async);
	int 	qt_getServerVersion(bool async);
	//int		qt_initChannelManager(bool async);
	//int		qt_exitChannelManager(bool async);
	//----------------------------------------------------------
	int		qt_doStartRotChannel(bool async, int ch_idx);
	int		qt_doStopRotChannel(bool async, int ch_idx);
	int		qt_doReadRotChannel(bool async, int ch_idx);
	int		qt_isRotRunning(bool async, int ch_idx);
	int		qt_isAnyRotRunning(bool async);
	//----------------------------------------------------------
	int 	qt_isADCRunning(bool async, int ch_idx);
	int 	qt_isAnyADCChannelRunning(bool async);
	int 	qt_doSetupAdcChannel(bool async, int ch_idx);
	int 	qt_startAdcChannel(bool async, int ch_idx);
	int 	qt_stopAdcChannel(bool async, int ch_idx);
	int 	qt_warmChannelStart(bool async, int ch_idx);
	int 	qt_warmChannelEnd(bool async, int ch_idx);
	int 	qt_doStartSyncChannels(bool async);
	int 	qt_doStopSyncChannels(bool async);
	int 	qt_readADCTimeOffsets(bool async);
	//-----------------------------------------------------------
	int 	qt_initMultiplexer(bool async);
	int  	qt_deinitMultiplexer(bool async);	
	int  	qt_switchCommutatorOn(bool async);
	int  	qt_switchCommutatorOff(bool async);
	int  	qt_unmuxAll(bool async);
	int  	qt_doMuxChannel(bool async);
private:
	boost::signals2::connection		connection_async_complete;
	boost::signals2::connection		connection_sync_complete;
	boost::signals2::connection		connection_alarm;
	boost::signals2::connection		connection_ping;
	
	void _gui_reaction(int if_code);
};

#endif
