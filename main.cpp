#include <QtQml>
#include <QtQml/QQmlApplicationEngine>
#include <QtGui/QGuiApplication>
#include <QApplication>

#include "qt_client_wrapper.hpp"
#include "qt_hwmon_wrapper.hpp"
/*
class DeviceSubscriber {
public:
	DeviceSubscriber() {
	};
	~DeviceSubscriber() {
	};
	void alarmError(int error_code) {
		printf("client alarmError = %d\n",error_code);
	};
	void serverPing(int ping_code) {
		printf("client serverPing = %d\n",ping_code);
	};
};
*/
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
/*
	channel_manager::ChannelManagerInitParams	initpars;
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
    	initpars.add_chan_level_console(1);
    	initpars.add_chan_level_daemon(0);
    }

    channel_manager::DriverTimings* drt = initpars.mutable_driver_timings();
    drt->set_rot_run(1000);
    drt->set_adc_set_params1(500);
    drt->set_adc_set_params2(100);
    drt->set_adc_set_params3(100);
    drt->set_adc_run(100);
    drt->set_adc_run_sync(100);

	//initpars.mutable_driver_timings()->set_adc_run(100);
	//initpars.set_allocated_driver_timings(&drt);
	*/
	/*
	try {
		RCF::RcfServer subServer(RCF::TcpEndpoint(-1));
		subServer.start();
		
		DeviceSubscriber cm_client;
		RCF::SubscriptionParms subParms;
		subParms.setPublisherEndpoint(RCF::TcpEndpoint("192.168.1.197",50001));
		RCF::SubscriptionPtr subPtr = subServer.createSubscription<PUB_connection>(cm_client,subParms);


		int ret = client.initManager(initpars);
		printf("client.initManager ret %d\n",ret);
		Sleep(1);
		
		channel_manager::RotChannelInitParams rcip;
		rcip.set_rot_idx(1);
		rcip.set_av_num(10);
		rcip.set_pw_min_us(25);
		rcip.set_period_min_us(8333);
		rcip.set_period_max_us(1000000);
		client.doStartRotChannel(rcip);
		
		Sleep(1);
		
		client.stopRotChannel(1);
		
		Sleep(1);
		
		client.exitManager();
		
		for (int i=0; i<10; i++)
			Sleep(1);
	
		subPtr->close();
		subServer.stop();
	}
	catch (const RCF::Exception & e)
	{
		printf("Init Manager error: %s\n",e.getErrorString().c_str());
	}
	*/
	int ret = 0;
	try {
	
	
	int rc = RCF::getInitRefCount();
	printf("before constructor: init ref count = %d\n",rc);
	
	RCF::init();
	QClientWrapper* cw = new QClientWrapper();
	//cw->setConsoleLevel(LOG_LEVEL_FULL);
	cw->setConsoleLevel(LOG_LEVEL_MAIN);

	QHwmonWrapper* hw = new QHwmonWrapper();
	hw->setConsoleLevel(LOG_LEVEL_MAIN);
	
	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("clientWrapper",cw);
	engine.rootContext()->setContextProperty("hardwareMonitor",hw);
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

	ret = app.exec();

	delete cw;
	delete hw;
	rc = RCF::getInitRefCount();
	printf("after destructor: init ref count = %d\n",rc);
	if (rc > 0) {
		RCF::deinit();
	}
	}
	catch (...) {
		printf("exception!!!!!!!!!!!!!!\n");
	}
	printf("Application finished with %d\n",ret);
	return ret;
}
