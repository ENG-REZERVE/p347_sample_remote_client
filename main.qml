import QtQuick 2.2
import QtQuick.Controls 1.2

ApplicationWindow {
    id: mainWindow
       title: "p347 Remote Client Tester GUI"
       width: 800
       height: 600
       visible: true


    Rectangle {
        id: serverRect
        anchors.margins: 2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 200

            Rectangle {
                id: rect_row_info
                color: "lightgray"
                height: 50
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top

                Row {
                    id: sr_row_info
                    spacing: 5

                    Text {
                        id: sr_text_ip
                        text: "Server IP:"
                    }
                    TextEdit {
                        id: sr_textedit_ip
                        text: "192.168.1.198"
                        width: 150
                        anchors.leftMargin: 5
                    }
                    Text {
                        id: sr_text_port
                        text: "Port"
                    }
                    SpinBox {
                        id: sr_spinbox_port
                        maximumValue: 65535
                        value: 50001
                        width: 80
                        anchors.leftMargin: 5
                    }
                    Text {
                        id: sr_ping_text
                        text: "Ping code"
                        anchors.leftMargin: 20
                    }
                    Text {
                        id: sr_ping_value
                        text: "---"
                        width: 50
                        anchors.leftMargin: 5
                    }
                    Text {
                        id: sr_error_text
                        text: "Error code"
                    }
                    Text {
                        id: sr_error_value
                        text: "0"
                        width: 50
                        anchors.leftMargin: 5
                    }
                    CheckBox {
                        id: sr_async_cb
                        text: "~SYNC/ASYNC"
                    }
                    Rectangle {
                        id: busy_indicator
                        anchors.leftMargin: 5
                        width: 30
                        height: 30
                        color: (clientWrapper.isAsyncRunning) ? "red" : "white"
                    }
                }
            }

            Rectangle {
                id: rect_row_config
                color: "lightgray"
                height: sr_row_config.height
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: rect_row_info.bottom
                anchors.topMargin: 3

                Row {
                    id: sr_row_config
                    spacing: 5
                    Text {
                        id: sr_text_config
                        text: "Client configurator: "
                    }
                    SpinBox {
                        id: conf_idx_val
                        maximumValue: 999
                        minimumValue: 1
                        value: 1
                        width: 60
                        anchors.leftMargin: 5
                    }
                    Button {
                        id: sr_button_loadconf
                        text: "Load Config"
                        onClicked: {
                            rett_loadconf.text = clientWrapper.qt_loadConfiguration(conf_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_loadconf
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }

                }
            }

            Rectangle {
                id: rect_row_command
                color: "lightgray"
                height: sr_row_command.height
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: rect_row_config.bottom
                anchors.topMargin: 3

                Row {
                    id: sr_row_command
                    //anchors.top: sr_row_info.bottom
                    spacing: 5
                    Text {
                        id: sr_text_conn
                        text: "Connect/Disconnect commands: "
                    }
                    Button {
                        id: sr_button_connect
                        text: "Try to connect"
                        onClicked: {
                            rett_connect.text = clientWrapper.qt_connectToTcpServer(sr_textedit_ip.text,sr_spinbox_port.value);
                        }
                    }
                    Text {
                        id: rett_connect
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_disconnect
                        text: "Try to disconnect"
                        onClicked: {
                            rett_disconnect.text = clientWrapper.qt_disconnectFromTcpServer();
                        }
                    }
                    Text {
                        id: rett_disconnect
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Text {
                        id: alarm_label
                        text: "Last Alarm: "+clientWrapper.alarmCode
                        anchors.leftMargin: 5
                    }
                    Text {
                        id: ping_label
                        text: "Server Ping: "+clientWrapper.pingCode
                        anchors.leftMargin: 5
                    }
                }
            }

            Rectangle {
                id: rect_row_deconstruct
                color: "lightgray"
                height: sr_row_deconstruct.height
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: rect_row_command.bottom
                anchors.topMargin: 3

                Row {
                    id: sr_row_deconstruct
                    //anchors.top: sr_row_info.bottom
                    spacing: 5

                    Button {
                        id: sr_button_create_cm
                        text: "Init Device"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_createcm.text = clientWrapper.qt_initDevice(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_createcm
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_delete_cm
                        text: "Deinit Device"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_deletecm.text = clientWrapper.qt_deInitDevice(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_deletecm
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }

                   /*
                    Button {
                        id: sr_button_create_dspemul
                        text: "Create DSPEmul base"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_createde.text = clientWrapper.qt_createDSPEmul(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_createde
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_delete_dspemul
                        text: "Delete DSPEmul base"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_deletede.text = clientWrapper.qt_deleteDSPEmul(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_deletede
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    */
                    /*
                    Button {
                        id: sr_button_sleeptest
                        text: "Remote multi sleep test"
                        onClicked: {
                            clientWrapper.qt_sleepTest(sr_async_cb.checked);
                        }
                    }
                    */
                }
            }

            //--------------------------------------------------------------------INIT-EXIT
            /*
            Rectangle {
                id: rect_row_initexit
                color: "lightgray"
                height: sr_row_initexit.height
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: rect_row_deconstruct.bottom
                anchors.topMargin: 3

                Row {
                    id: sr_row_initexit
                    //anchors.top: sr_row_info.bottom
                    spacing: 5

                    Button {
                        id: sr_button_init_cm
                        text: "Init channel manager"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_initcm.text = clientWrapper.qt_initChannelManager(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_initcm
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_exit_cm
                        text: "Exit channel manager"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_exitcm.text = clientWrapper.qt_exitChannelManager(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_exitcm
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                }
            }
            */
            Rectangle {
                id: rect_row_info2
                color: "lightgray"
                height: sr_row_info2.height
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: rect_row_deconstruct.bottom
                anchors.topMargin: 3

                Row {
                    id: sr_row_info2
                    //anchors.top: sr_row_info.bottom
                    spacing: 5

                    Button {
                        id: sr_button_avc
                        text: "getAvailableChannels"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_avc.text = clientWrapper.qt_getAvailableChannels(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_avc
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Text {
                        id: value_avc
                        text: clientWrapper.avChannelsStr
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_ischav
                        text: "isChannelAvailable"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_ischav.text = clientWrapper.qt_isChannelAvailable(sr_async_cb.checked,adc_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_ischav
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Text {
                        id: val_ischav
                        text: clientWrapper.isCHAV
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_ischac
                        text: "isChannelsCreated"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_ischac.text = clientWrapper.qt_isChannelsCreated(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_ischac
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Text {
                        id: val_ischac
                        text: clientWrapper.isCHAC
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_version
                        text: "getServerVersion"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_version.text = clientWrapper.qt_getServerVersion(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_version
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Text {
                        id: value_version
                        text: clientWrapper.serverVersion
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                }
            }
            //--------------------------------------------------------------------ROT

            Rectangle {
                id: rect_row_rot
                color: "lightgray"
                height: sr_row_rot.height
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: rect_row_info2.bottom //rect_row_initexit.bottom
                anchors.topMargin: 3

                Row {
                    id: sr_row_rot
                    //anchors.top: sr_row_info.bottom
                    spacing: 5

                    Text {
                        id: rot_idx_text
                        text: "Rot channel index: "
                    }
                    SpinBox {
                        id: rot_idx_val
                        maximumValue: 4
                        minimumValue: 1
                        value: 1
                        width: 60
                        anchors.leftMargin: 5
                    }
                    Button {
                        id: sr_button_start_rot
                        text: "Start rot"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_startrot.text = clientWrapper.qt_doStartRotChannel(sr_async_cb.checked, rot_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_startrot
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_stop_rot
                        text: "Stop rot"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_stoprot.text = clientWrapper.qt_doStopRotChannel(sr_async_cb.checked, rot_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_stoprot
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_read_rot
                        text: "Read rot"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_readrot.text = clientWrapper.qt_doReadRotChannel(sr_async_cb.checked, rot_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_readrot
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Text {
                        id: val_readrot
                        text: clientWrapper.lastFreq+"..."
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_isrr
                        text: "is Running"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_isrr.text = clientWrapper.qt_isRotRunning(sr_async_cb.checked, rot_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_isrr
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Text {
                        id: val_isrr
                        text: clientWrapper.isRR
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_isanyrr
                        text: "is any rot Running"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_isanyrr.text = clientWrapper.qt_isAnyRotRunning(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_isanyrr
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Text {
                        id: val_isanyrr
                        text: clientWrapper.isAnyRR
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                }
            }

            //--------------------------------------------------------------------ADC Single

            Rectangle {
                id: rect_row_adcs
                color: "lightgray"
                height: sr_row_adcs.height
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: rect_row_rot.bottom
                anchors.topMargin: 3

                Row {
                    id: sr_row_adcs
                    //anchors.top: sr_row_info.bottom
                    spacing: 5

                    Text {
                        id: adc_idx_text
                        text: "ADC channel index: "
                    }
                    SpinBox {
                        id: adc_idx_val
                        maximumValue: 4
                        minimumValue: 1
                        value: 1
                        width: 60
                        anchors.leftMargin: 5
                    }
                    Button {
                        id: sr_button_setup_adc
                        text: "Setup ADC"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_setup_adc.text = clientWrapper.qt_doSetupAdcChannel(sr_async_cb.checked, adc_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_setup_adc
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_start_adc
                        text: "Start ADC"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_start_adc.text = clientWrapper.qt_startAdcChannel(sr_async_cb.checked, adc_idx_val.value);
                            rett_isadcrun.text = clientWrapper.qt_isADCRunning(sr_async_cb.checked, adc_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_start_adc
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_stop_adc
                        text: "Stop ADC"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_stop_adc.text = clientWrapper.qt_stopAdcChannel(sr_async_cb.checked, adc_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_stop_adc
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_start_warm
                        text: "Start warm ADC"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_start_warm.text = clientWrapper.qt_warmChannelStart(sr_async_cb.checked, adc_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_start_warm
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_stop_warm
                        text: "Stop warm ADC"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_stop_warm.text = clientWrapper.qt_warmChannelEnd(sr_async_cb.checked, adc_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_stop_warm
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_isadcrun
                        text: "is ADC running"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_isadcrun.text = clientWrapper.qt_isADCRunning(sr_async_cb.checked, adc_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_isadcrun
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Text {
                        id: val_isadcrun
                        text: clientWrapper.isAR
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }

                }
            }

            //--------------------------------------------------------------------Tasks

            Rectangle {
                id: rect_row_task
                color: "lightgray"
                height: sr_row_task.height
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: rect_row_adcs.bottom
                anchors.topMargin: 3

                Row {
                    id: sr_row_task
                    //anchors.top: sr_row_info.bottom
                    spacing: 5

                    Text {
                        id: task_idx_text
                        text: "Task index: "
                    }
                    SpinBox {
                        id: task_idx_val
                        maximumValue: 4
                        minimumValue: 1
                        value: 1
                        width: 60
                        anchors.leftMargin: 5
                    }
                    Button {
                        id: sr_tsopen
                        text: "timesig open"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_tsopen.text = clientWrapper.qt_timesigOpen(sr_async_cb.checked, adc_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_tsopen
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_tsclose
                        text: "timesig close"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_tsclose.text = clientWrapper.qt_timesigClose(sr_async_cb.checked, adc_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_tsclose
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }

                    Button {
                        id: sr_addtask
                        text: "add quality task"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_addtask.text = clientWrapper.qt_addQualityTask(sr_async_cb.checked, adc_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_addtask
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }

                    Button {
                        id: sr_gttp
                        text: "getTotalTasksProgress"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_gttp.text = clientWrapper.qt_getTotalTasksProgress(sr_async_cb.checked, adc_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_gttp
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_gtr_quality
                        text: "getTaskResult - Quality"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_gtr_quality.text = clientWrapper.qt_getTaskResult(sr_async_cb.checked, adc_idx_val.value, task_idx_val.value);
                        }
                    }
                    Text {
                        id: rett_gtr_quality
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                }
            }

            //--------------------------------------------------------------------ADC Multi

            Rectangle {
                id: rect_row_adcm
                color: "lightgray"
                height: sr_row_adcm.height
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: rect_row_task.bottom
                anchors.topMargin: 3

                Row {
                    id: sr_row_adcm
                    //anchors.top: sr_row_info.bottom
                    spacing: 5


                    Button {
                        id: sr_button_start_sync
                        text: "Start sync"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_start_sync.text = clientWrapper.qt_doStartSyncChannels(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_start_sync
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_stop_sync
                        text: "Stop sync"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_stop_sync.text = clientWrapper.qt_doStopSyncChannels(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_stop_sync
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_isanyadcrun
                        text: "is any ADC running"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_isanyadcrun.text = clientWrapper.qt_isAnyADCChannelRunning(sr_async_cb.checked);
                        }
                    }
                    Text {
                        id: rett_isanyadcrun
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Text {
                        id: val_isanyadcrun
                        text: clientWrapper.isAnyAR
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Button {
                        id: sr_button_readoff
                        text: "Read offsets"
                        enabled: !clientWrapper.isAsyncRunning
                        onClicked: {
                            rett_readoff.text = clientWrapper.qt_readADCTimeOffsets(sr_async_cb.checked);
                            val_readoff.text = clientWrapper.offstring;
                        }
                    }
                    Text {
                        id: rett_readoff
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                    Text {
                        id: val_readoff
                        text: "-"
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                    }
                }

            }
    } //eof serverRect

    Rectangle {
        id: hwmonRect
        anchors.margins: 2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: serverRect.bottom
        anchors.topMargin: 5
        height: 200

        Rectangle {
            id: hrect_row_info
            color: "lightgray"
            height: 50
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            Row {
                id: hsr_row_info
                spacing: 5

                Text {
                    id: hsr_text_ip
                    text: "Server IP:"
                }
                TextEdit {
                    id: hsr_textedit_ip
                    text: "192.168.1.198"
                    width: 150
                    anchors.leftMargin: 5
                }
                Text {
                    id: hsr_text_port
                    text: "Port"
                }
                SpinBox {
                    id: hsr_spinbox_port
                    maximumValue: 65535
                    value: 50002
                    width: 80
                    anchors.leftMargin: 5
                }
                Text {
                    id: hsr_error_text
                    text: "Error code"
                }
                Text {
                    id: hsr_error_value
                    text: "0"
                    width: 50
                    anchors.leftMargin: 5
                }
                CheckBox {
                    id: hsr_async_cb
                    text: "~SYNC/ASYNC"
                }
                Rectangle {
                    id: hbusy_indicator
                    anchors.leftMargin: 5
                    width: 30
                    height: 30
                    color: (hardwareMonitor.isAsyncRunning) ? "red" : "white"
                }
            }
        }
        Rectangle {
            id: hrect_row_command
            color: "lightgray"
            height: hsr_row_command.height
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: hrect_row_info.bottom
            anchors.topMargin: 3

            Row {
                id: hsr_row_command
                spacing: 5
                Text {
                    id: hsr_text_conn
                    text: "Connect/Disconnect commands: "
                }
                Button {
                    id: hsr_button_connect
                    text: "Try to connect"
                    onClicked: {
                        hrett_connect.text = hardwareMonitor.qt_connectToTcpServer(hsr_textedit_ip.text,hsr_spinbox_port.value);
                    }
                }
                Text {
                    id: hrett_connect
                    text: "-"
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
                Button {
                    id: hsr_button_disconnect
                    text: "Try to disconnect"
                    onClicked: {
                        hrett_disconnect.text = hardwareMonitor.qt_disconnectFromTcpServer();
                    }
                }
                Text {
                    id: hrett_disconnect
                    text: "-"
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }

                Button {
                    id: hsr_button_create
                    text: "Create HWMON"
                    onClicked: {
                        hrett_create.text = hardwareMonitor.qt_createHardwareMonitor(hsr_async_cb.checked);
                    }
                }
                Text {
                    id: hrett_create
                    text: "-"
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
                Button {
                    id: hsr_button_delete
                    text: "Delete HWMON"
                    onClicked: {
                        hrett_delete.text = hardwareMonitor.qt_deleteHardwareMonitor(hsr_async_cb.checked);
                    }
                }
                Text {
                    id: hrett_delete
                    text: "-"
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
            }
        }

        Rectangle {
            id: hrect_row_subscribe
            color: "lightgray"
            height: hsr_row_subscribe.height
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: hrect_row_command.bottom
            anchors.topMargin: 3

            Row {
                id: hsr_row_subscribe
                spacing: 5
                Text {
                    id: hsr_subs
                    text: "Subscribe/unsubscribe commands: "
                }
                CheckBox {
                    id: hsr_battery_cb
                    text: "BATTERY"
                }
                CheckBox {
                    id: hsr_temperature_cb
                    text: "CPU T"
                }
                CheckBox {
                    id: hsr_button_cb
                    text: "BUTTON"
                }
                CheckBox {
                    id: hsr_baction_cb
                    text: "BAT ACT"
                }
                Button {
                    id: hsr_button_subscribe
                    text: "Subscribe"
                    onClicked: {
                        hrett_subscribe.text = hardwareMonitor.qt_clientSubscribe(hsr_async_cb.checked,
                                                    hsr_battery_cb.checked,hsr_temperature_cb.checked,
                                                    hsr_button_cb.checked, hsr_baction_cb.checked);
                    }
                }
                Text {
                    id: hrett_subscribe
                    text: "-"
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
                Button {
                    id: hsr_button_unsubscibe
                    text: "Unsubscribe"
                    onClicked: {
                        hrett_unsubscribe.text = hardwareMonitor.qt_clientUnsubscribe(hsr_async_cb.checked);
                    }
                }
                Text {
                    id: hrett_unsubscribe
                    text: "-"
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }

            }

        }

        Rectangle {
            id: hrect_row_indata
            color: "lightgray"
            height: hsr_row_indata.height
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: hrect_row_subscribe.bottom
            anchors.topMargin: 3

            Row {
                id: hsr_row_indata
                spacing: 5

                Text {
                    id: htext_battery_percent
                    text: "Battery %: "
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
                Text {
                    id: hval_battery_percent
                    text: hardwareMonitor.batteryPercent
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
                Text {
                    id: htext_op
                    text: "Outer power: "
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
                Text {
                    id: hval_op
                    text: hardwareMonitor.outerPower
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
                Text {
                    id: htext_ic
                    text: "Charging: "
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
                Text {
                    id: hval_ic
                    text: hardwareMonitor.isCharging
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
                Text {
                    id: htext_cput
                    text: "CPU T: "
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
                Text {
                    id: hval_cput
                    text: hardwareMonitor.midCPUTemp
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
                Text {
                    id: htext_butt
                    text: "Button: "
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
                Text {
                    id: hval_butt
                    text: hardwareMonitor.isButtonPressed
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                }
            }
        }

    } //eof hwmonRect
}
