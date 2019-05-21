#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QMessageBox>
#include <QFile>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextStream>

#include "qextserial/qextserialport.h"
#include "aboutdialog.h"


//延时，TIME_OUT是串口读写的延时
#define TIME_OUT 10

//连续发送定时器计时间隔,200ms
#define OBO_TIMER_INTERVAL 200

//载入文件时，如果文件大小超过TIP_FILE_SIZE字节则提示文件过大是否继续打开
#define TIP_FILE_SIZE 5000
//载入文件最大长度限制在MAX_FILE_SIZE字节内
#define MAX_FILE_SIZE 10000



#define PACKAGE_BUF_LEN     1024

#define XLINK_PTP_HEAD   0xFF
#define XLINK_PTP_CHANGE 0xFD
#define XLINK_PTP_TAIL   0xFE
#define XLINK_PTP_OR     0x80
typedef struct{
	unsigned char en;
	unsigned char cmd;
	unsigned short alldatalen;
	unsigned char *data;
	unsigned short datalen;
	unsigned char flag;
	int (*PacketSend)(QextSerialPort *myCom, unsigned char *data,unsigned short datalen);
}XLINK_PASSTHROUGHPROTOCOLPACKET;

typedef struct{
	unsigned char *pktbuf;
	unsigned short pktbuflen;
	unsigned short pktpos;

	unsigned short pktlentmp;
	XLINK_PASSTHROUGHPROTOCOLPACKET *pkt;
	void (*PacketCallBack)(XLINK_PASSTHROUGHPROTOCOLPACKET *PTP_pkt);

}XLINK_PASSTHROUGHPROTOCOL;

typedef enum{
		XLINK_PTR_CHECK_MAC = 0X00, //mcu check wifi mac
		XLINK_PTR_CHECK_NET = 0x01,      //mcu check wifi net
		XLINK_PTR_CHECK_PIDKEY = 0x02,   //mcu check wifi pid and key
		XLINK_PTR_SET_PIDKEY = 0x03,     //mcu set wifi pid and key
		XLINK_PTR_ENTER_SMARTLINK = 0x04, // mcu set wifi into smartlink
		XLINK_PTR_REBOOT_WIFI = 0x05,     //mcu reboot wifi
		XLINK_PTR_REPASS_WIFI = 0x06,     //mcu repass wifi
		XLINK_PTR_CHECK_WIFI_VER = 0x07,  //mcu check wifi version
		XLINK_PTR_GET_WIFI_TIME = 0x08,    //mcu get wifi time
		XLINK_PTR_SET_APSTA_MODE = 0x09, //mcu set wifi in STA or AP
		XLINK_PTR_AP_SMARTLINK_STATUS = 0x0a, //wifi output ap smartlink status
		XLINK_PTR_CHECK_SN = 0x0B,   //mcu check wifi sn       new add
		XLINK_PTR_SET_SN = 0x0C,	//mcu set wifi sn
		XLINK_PTR_CHECK_APSTA_MODE = 0x0D,  //mcu check wifi in STA or AP and wifi strength

		XLINK_PTR_PRODUCTION_TEST_SET = 0x0E, //mcu set  wifi  production test status
		XLINK_PTR_UPDATE_PRODUCTION_TEST_DATA = 0x0F, //mcu update wifi production test data
		XLINK_PTR_ENABLE_PRODUCTION_TEST = 0x10, //mcu  enable  production test
		XLINK_PTR_ENABLE_PAIRING = 0x11, //mcu  enable  pairing
		XLINK_PTR_ENABLE_SUBSCRIBE = 0x12, //mcu  enable  subscribe
		XLINK_PTR_CHECK_PINGCODE = 0x13,   //mcu check wifi pingcode       new add
		XLINK_PTR_SET_PINGCODE = 0x14,	//mcu set wifi pingcode
		XLINK_PTP_GET_MCU_VER = 0x15,	// get mcu version
		XLINK_PTP_SET_MCU_VER = 0x16,	// set mcu version

		XLINK_PTR_CHECK_OTA = 0X30, //mcu check whether new version ota bin or not.
		XLINK_PTR_ASK_OTA = 0x31,      //mcu ask wifi for excuting an ota task
		XLINK_PTR_HF_SEND_OTA = 0x32,   //wifi send the ota bin to mcu
		XLINK_PTR_Finish_OTA= 0x33,     //wifi finish the ota task
		XLINK_PTR_ANSWER_OTA = 0x34,   //MCU tell wifi mcu ota finish

		XLINK_PTR_WIFI_TO_MCU_TRAN_T = 0X80,  //wifi transfer passthrough data to mcu
		XLINK_PTR_MCU_TO_WIFI_TRAN_T = 0x81,        //mcu transfer passthrough data to wifi
		XLINK_PTR_WIFI_TO_MCU_TRAN_DP = 0x82,       //wifi transfer app or server datapionts data to mcu
		XLINK_PTR_MCU_TO_WIFI_TRAN_DP = 0x83,	     //mcu transfer datapoints data to wifi
		XLINK_PTR_MCU_TO_WIFI_TRAN_ALLDP = 0x84,     //mcu tranfer all datapoints data to wifi and for web refresh
		XLINK_PTR_MCU_TO_WIFI_TRAN_ALLDP_NO_ALARM = 0x85,

		XLINK_PTR_PRINTER_STARTUP = 0xD0,
		XLINK_PTR_PRINTER_ENABLE_ACCEPT_DATA = 0xD1,
		XLINK_PTR_PRINTER_DISABLE_ACCEPT_DATA = 0xD2,
		XLINK_PTR_PRINTER_STATUS = 0xD3,
		XLINK_PTR_PRINTER_ORDER = 0xE0,
		XLINK_PTR_PRINTER_ORDER_CONTENT = 0xE1,
		XLINK_PTR_PRINTER_ORDER_FINISH = 0xE2,
		XLINK_PTR_PRINTER_CHECK_STATUS = 0xE3,
		XLINK_PTR_PRINTER_COPIES = 0xE4,
		XLINK_PTR_PRINTER_COPIES_FINISH = 0xE5,
}XLINK_CMD_VALUE;


typedef enum{
        XLINK_MCU_UPGRADE_STATE_IDILE = 0,
		XLINK_MCU_SEND_CHECK_OTA_STATE = 0X30, //mcu check whether new version ota bin or not.
		XLINK_MCU_ASK_OTA_STATE = 0x31,      //mcu ask wifi for excuting an ota task
		XLINK_WIFI_SEND_OTA_STATE = 0x32,   //wifi send the ota bin to mcu
		XLINK_WIFI_SEND_FINISH_OTA_STATE= 0x33,     //wifi finish the ota task
		XLINK_MCU_SEND_OTA_FINISH_STATE = 0x34,   //MCU tell wifi mcu ota finish
}XLINK_MCU_UPGRADE_STATE;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void startInit();

protected:
    void changeEvent(QEvent *e);
    void setActionsEnabled(bool status);
    void setComboBoxEnabled(bool status);

private:
    Ui::MainWindow *ui;
    AboutDialog aboutdlg;
    QextSerialPort *myCom;
    QTimer *obotimer;
    unsigned int timerdly;
    unsigned int obotimerdly;
    QString write2fileName;    //写读取的串口数据到该文件

    //serial port struct and buffer
    XLINK_PASSTHROUGHPROTOCOLPACKET pkt;
    XLINK_PASSTHROUGHPROTOCOL ptppkt;
    unsigned char xlink_pktbuf[PACKAGE_BUF_LEN];
    XLINK_MCU_UPGRADE_STATE mcu_upgradeState;
    unsigned short mcuCurVersion;
    unsigned short mcuNewVersion;
    unsigned int mcuOtaIdentify;
    unsigned short mcuOtaPktLength;
	unsigned int   mcuBinSize;
	unsigned char  mcuBinMd5[16];
	unsigned short mcuBinOrderNumber;
	QString curMcuBinFileName;

private slots:

    void on_actionWriteToFile_triggered();
    void on_actionCleanPort_triggered();
    void on_actionLoadfile_triggered();
    void on_delayspinBox_valueChanged(int );
    void on_actionAdd_triggered();
    void on_actionExit_triggered();
    void on_actionSave_triggered();
    void on_obocheckBox_clicked();
    void on_actionClearBytes_triggered();
    void on_actionAbout_triggered();
    void on_actionClose_triggered();
    void on_actionOpen_triggered();
    void on_clearUpBtn_clicked();
    void on_sendmsgBtn_clicked();
    void on_checkDeviceButton_clicked();
    void on_deviceRebootButton_clicked();
    void on_mcuUpgradeButton_clicked();
    void on_getPidPkeyButton_clicked();
    void on_getWifiMACButton_clicked();
    void readMyCom();
    void sendMsg();
    //end by
    void showString(QString buf);
    static int XlinkUartSend(QextSerialPort *myCom, unsigned char * Buffer, unsigned short BufferLen);
    void xlink_PacketCallBack(XLINK_PASSTHROUGHPROTOCOLPACKET *pkt);
    int Xlink_PassThroughProtolInit(XLINK_PASSTHROUGHPROTOCOL *PTP_pck);
    int Xlink_PTP_Init(void);
    void Xlink_PassThroughProtolPutData(XLINK_PASSTHROUGHPROTOCOL *PTP_pck,unsigned char *data,unsigned short datalen);
    void Xlink_PassThroughProtolBuildSendData(XLINK_PASSTHROUGHPROTOCOLPACKET *pkt);
    unsigned char Xlink_PassThroughProtolBuildXor(unsigned char resold,unsigned char *data,unsigned int datalen);
    void xlink_mcu_send_check_ota_cmd(unsigned short version, unsigned int identify);
	void xlink_mcu_send_excute_ota_cmd(unsigned char packetLen);
	void xlink_mcu_send_finish_ota_cmd(unsigned short oldversion,unsigned short newversion, unsigned int identify);
	void witePacketToBinFile(unsigned char *data, unsigned char data_len);
    QString fileMd5(const QString &sourceFilePath);
    void on_upgradeProgressBar_valueChanged(int value);
};

#endif // MAINWINDOW_H
