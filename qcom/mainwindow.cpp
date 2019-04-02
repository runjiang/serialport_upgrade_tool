#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startInit();
    qDebug("startInit\r\n");
    myCom = NULL;

    ui->qter->setText(
            tr("<a href=\"http://www.qter.org\">www.qter.org</a>"));
 #ifdef Q_OS_LINUX
    ui->portNameComboBox->addItem( "ttyUSB0");
    ui->portNameComboBox->addItem( "ttyUSB1");
    ui->portNameComboBox->addItem( "ttyUSB2");
    ui->portNameComboBox->addItem( "ttyUSB3");
    ui->portNameComboBox->addItem( "ttyS0");
    ui->portNameComboBox->addItem( "ttyS1");
    ui->portNameComboBox->addItem( "ttyS2");
    ui->portNameComboBox->addItem( "ttyS3");
    ui->portNameComboBox->addItem( "ttyS4");
    ui->portNameComboBox->addItem( "ttyS5");
    ui->portNameComboBox->addItem( "ttyS6");
#elif defined (Q_OS_WIN)
    ui->portNameComboBox->addItem("COM0");
    ui->portNameComboBox->addItem("COM1");
    ui->portNameComboBox->addItem("COM2");
    ui->portNameComboBox->addItem("COM3");
    ui->portNameComboBox->addItem("COM4");
    ui->portNameComboBox->addItem("COM5");
    ui->portNameComboBox->addItem("COM6");
    ui->portNameComboBox->addItem("COM7");
    ui->portNameComboBox->addItem("COM8");
    ui->portNameComboBox->addItem("COM9");
    ui->portNameComboBox->addItem("COM10");
    ui->portNameComboBox->addItem("COM11");
    ui->portNameComboBox->addItem("COM12");
    ui->portNameComboBox->addItem("COM13");
    ui->portNameComboBox->addItem("COM14");
    ui->portNameComboBox->addItem("COM15");

#endif

    ui->statusBar->showMessage(tr("欢迎使用QCom串口调试助手!"));
}

MainWindow::~MainWindow()
{
    if(myCom != NULL){
        if(myCom->isOpen()){
            myCom->close();
        }
        delete myCom;
    }
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::startInit(){
    setActionsEnabled(false);
    ui->delayspinBox->setEnabled(false);
    ui->sendmsgBtn->setEnabled(false);
    ui->sendMsgLineEdit->setEnabled(false);
    ui->obocheckBox->setEnabled(false);
    ui->actionAdd->setEnabled(true);

    //初始化连续发送计时器计时间隔
    obotimerdly = OBO_TIMER_INTERVAL;

    //设置连续发送计时器
    obotimer = new QTimer(this);
    connect(obotimer, SIGNAL(timeout()), this, SLOT(sendMsg()));

}

void MainWindow::setActionsEnabled(bool status)
{
    ui->actionSave->setEnabled(status);
    ui->actionClose->setEnabled(status);
    ui->actionLoadfile->setEnabled(status);
    ui->actionCleanPort->setEnabled(status);
    ui->actionWriteToFile->setEnabled(status);
}

void MainWindow::setComboBoxEnabled(bool status)
{
    ui->portNameComboBox->setEnabled(status);
    ui->baudRateComboBox->setEnabled(status);
    ui->dataBitsComboBox->setEnabled(status);
    ui->parityComboBox->setEnabled(status);
    ui->stopBitsComboBox->setEnabled(status);
}

//打开串口
void MainWindow::on_actionOpen_triggered()
{
    qDebug("init xlink ptp");
    if (Xlink_PTP_Init() != 0) {
		qDebug("Xlink_PTP_Init error");
	}

    QString portName = ui->portNameComboBox->currentText();   //获取串口名
#ifdef Q_OS_LINUX
    myCom = new QextSerialPort("/dev/" + portName);
#elif defined (Q_OS_WIN)
    myCom = new QextSerialPort(portName);
#endif
    connect(myCom, SIGNAL(readyRead()), this, SLOT(readMyCom()));

    //设置波特率
    myCom->setBaudRate((BaudRateType)ui->baudRateComboBox->currentText().toInt());

    //设置数据位
    myCom->setDataBits((DataBitsType)ui->dataBitsComboBox->currentText().toInt());

    //设置校验
    switch(ui->parityComboBox->currentIndex()){
    case 0:
         myCom->setParity(PAR_NONE);
         break;
    case 1:
        myCom->setParity(PAR_ODD);
        break;
    case 2:
        myCom->setParity(PAR_EVEN);
        break;
    default:
        myCom->setParity(PAR_NONE);
        qDebug("set to default : PAR_NONE");
        break;
    }

    //设置停止位
    switch(ui->stopBitsComboBox->currentIndex()){
    case 0:
        myCom->setStopBits(STOP_1);
        break;
    case 1:
 #ifdef Q_OS_WIN
        myCom->setStopBits(STOP_1_5);
 #endif
        break;
    case 2:
        myCom->setStopBits(STOP_2);
        break;
    default:
        myCom->setStopBits(STOP_1);
        qDebug("set to default : STOP_1");
        break;
    }    

    //设置数据流控制
    myCom->setFlowControl(FLOW_OFF);
    //设置延时
    myCom->setTimeout(TIME_OUT);

    if(myCom->open(QIODevice::ReadWrite)){
        QMessageBox::information(this, tr("打开成功"), tr("已成功打开串口") + portName, QMessageBox::Ok);
		//界面控制
		ui->sendmsgBtn->setEnabled(true);
        setComboBoxEnabled(false);

        ui->sendMsgLineEdit->setEnabled(true);

        ui->actionOpen->setEnabled(false);
        ui->sendMsgLineEdit->setFocus();
        ui->obocheckBox->setEnabled(true);
        ui->actionAdd->setEnabled(false);

        setActionsEnabled(true);
    }else{
        QMessageBox::critical(this, tr("打开失败"), tr("未能打开串口 ") + portName + tr("\n该串口设备不存在或已被占用"), QMessageBox::Ok);
        return;
    }

    ui->statusBar->showMessage(tr("打开串口成功"));
}

//关闭串口
void MainWindow::on_actionClose_triggered()
{

    myCom->close();
    delete myCom;
    myCom = NULL;

    ui->sendmsgBtn->setEnabled(false);

    setComboBoxEnabled(true);

    ui->actionOpen->setEnabled(true);
    ui->sendMsgLineEdit->setEnabled(false);
    ui->obocheckBox->setEnabled(false);
    ui->actionAdd->setEnabled(true);

    setActionsEnabled(false);

    ui->actionWriteToFile->setChecked(false);
    ui->statusBar->showMessage(tr("串口已经关闭"));
}

//关于
void MainWindow::on_actionAbout_triggered()
{
    aboutdlg.show();
    // 在主窗口中间显示
    int x =this->x() + (this->width() - aboutdlg.width()) / 2;
    int y =this->y() + (this->height() - aboutdlg.height()) / 2;
    aboutdlg.move(x, y);
    ui->statusBar->showMessage(tr("关于Wincom"));
}

//发送数据按钮
void MainWindow::on_sendmsgBtn_clicked()
{
    //如果当前正在连续发送数据，暂停发送
    if(ui->sendmsgBtn->text() == tr("暂停")){
        obotimer->stop();
        ui->sendmsgBtn->setText(tr("发送"));
        ui->sendmsgBtn->setIcon(QIcon(":/images/send.png"));
        return;
    }
    //如果发送数据为空，给出提示并返回
    if(ui->sendMsgLineEdit->text().isEmpty()){
        QMessageBox::information(this, tr("提示消息"), tr("没有需要发送的数据"), QMessageBox::Ok);
        return;
    }

    //如果不是连续发送
    if(!ui->obocheckBox->isChecked()){
        ui->sendMsgLineEdit->setFocus();
        //发送数据
        sendMsg();
    }else{ //连续发送
        obotimer->start(obotimerdly);
        ui->sendmsgBtn->setText(tr("暂停"));
        ui->sendmsgBtn->setIcon(QIcon(":/images/pause.png"));
    }
}
//清空记录
void MainWindow::on_clearUpBtn_clicked()
{
    ui->textBrowser->clear();
    ui->statusBar->showMessage(tr("记录已经清空"));
}

//计数器清零
void MainWindow::on_actionClearBytes_triggered()
{
    if(ui->recvbyteslcdNumber->value() == 0){
        QMessageBox::information(this, tr("提示消息"), tr("貌似已经清零了呀:)"), QMessageBox::Ok);
    }else{
        ui->recvbyteslcdNumber->display(0);
        ui->statusBar->showMessage(tr("计数器已经清零"));
    }
}

//单击连续发送checkBox
void MainWindow::on_obocheckBox_clicked()
{
    if(ui->obocheckBox->isChecked()){
        ui->delayspinBox->setEnabled(true);
        //检查是否有数据，如有则启动定时器
        ui->statusBar->showMessage(tr("启用连续发送"));
    }else{
        ui->delayspinBox->setEnabled(false);
        //若定时器已经启动则关闭它
        ui->statusBar->showMessage(tr("停止连续发送"));
    }
}

//保存textBrowser中的内容
void MainWindow::on_actionSave_triggered()
{

    if(ui->textBrowser->toPlainText().isEmpty()){
        QMessageBox::information(this, "提示消息", tr("貌似还没有数据! 您需要在发送编辑框中输入要发送的数据"), QMessageBox::Ok);
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("保存为"), tr("未命名.txt"));
    QFile file(filename);
    //如果用户取消了保存则直接退出函数
    if(file.fileName().isEmpty()){
        return;
    }
    //如果打开失败则给出提示并退出函数
    if(!file.open(QFile::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("保存文件"), tr("打开文件 %1 失败, 无法保存\n%2").arg(filename).arg(file.errorString()), QMessageBox::Ok);
        return;
    }
    //写数据到文件
    QTextStream out(&file);
    out<<ui->textBrowser->toPlainText();
    file.close();
    //修改窗口标题为保存文件路径
    setWindowTitle("saved: " + QFileInfo(filename).canonicalFilePath());

}

//退出程序
void MainWindow::on_actionExit_triggered()
{
    this->close();
}

//添加串口
void MainWindow::on_actionAdd_triggered()
{
    bool ok = false;
    QString portname;

    portname = QInputDialog::getText(this, tr("添加串口"), tr("设备文件名"), QLineEdit::Normal, 0, &ok);
    if(ok && !portname.isEmpty()){
        ui->portNameComboBox->addItem(portname);
        ui->statusBar->showMessage(tr("添加串口成功"));
    }
}

//调整连续发送时间间隔
void MainWindow::on_delayspinBox_valueChanged(int )
{
    obotimerdly = ui->delayspinBox->value();
}

//载入外部文件
void MainWindow::on_actionLoadfile_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("读取文件"), 0, tr("保存为 (*.txt *.log)"));  //添加更多的文件类型
    QFile file(filename);

    //如果取消打开则退出函数
    if(file.fileName().isEmpty()){
        return;
    }

    //如果打开失败则提示并退出函数
    if(!file.open(QFile::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("打开失败"),
                             tr("抱歉! Wincom未能打开此文件, 这可能是由于没有足够的权限造成的. 您可以尝试使用chmod命令修改文件权限."),
                             QMessageBox::Ok);
        return;
    }

    //文件大小超过限制
    if(file.size() > MAX_FILE_SIZE){
        QMessageBox::critical(this, tr("载入失败"), tr("文件大小为 %1 字节, 超过限制大小 10000 字节").arg(file.size()), QMessageBox::Ok);
        return;
    }
    //文件太大时提示是否继续打开
    if(file.size() > TIP_FILE_SIZE){
        if(QMessageBox::question(this, tr("提示消息"), tr("您要打开的文件过大, 这将消耗更多一些的时间,要继续打开么?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){

            return;
        }
    }

    //读取数据并加入到发送数据编辑框
    QByteArray line;
    while(!file.atEnd()){
        line = file.readLine();
        ui->sendMsgLineEdit->setText(ui->sendMsgLineEdit->text() + tr(line));

    }
    file.close();
    ui->statusBar->showMessage(tr("已经成功读取文件中的数据"));
}

//清空串口中的I/O数据
void MainWindow::on_actionCleanPort_triggered()
{
    myCom->flush();
}

//写入文件菜单
void MainWindow::on_actionWriteToFile_triggered()
{
    if(ui->actionWriteToFile->isChecked()){
        QString filename = QFileDialog::getSaveFileName(this, tr("写入文件"), 0, tr("保存为 (*.*)"));  //添加更多的文件类型
        if(filename.isEmpty()){
            ui->actionWriteToFile->setChecked(false);
        }else{
            write2fileName = filename;
            //ui->textBrowser->setEnabled(false);
            ui->actionWriteToFile->setToolTip(tr("停止写入到文件"));
        }
    }else{
        write2fileName.clear();
        //ui->textBrowser->setEnabled(true);
        ui->actionWriteToFile->setToolTip(tr("将读取数据写入到文件"));
    }

}


//发送数据
void MainWindow::sendMsg()
{
    QByteArray buf;
    if(ui->sendAsHexcheckBox->isChecked()){
        QString str;
        bool ok;
        char data;
        QStringList list;
        str = ui->sendMsgLineEdit->text();
        list = str.split(" ");
        for(int i = 0; i < list.count(); i++){
            if(list.at(i) == " ")
                continue;
            if(list.at(i).isEmpty())
                continue;
            data = (char)list.at(i).toInt(&ok, 16);
            if(!ok){
                QMessageBox::information(this, tr("提示消息"), tr("输入的数据格式有错误！"), QMessageBox::Ok);
                if(obotimer != NULL)
                    obotimer->stop();
                ui->sendmsgBtn->setText(tr("发送"));
                ui->sendmsgBtn->setIcon(QIcon(":/images/send.png"));
                return;
            }
            buf.append(data);
        }
    }else{
#if QT_VERSION < 0x050000
        buf = ui->sendMsgLineEdit->text().toAscii();
#else
        buf = ui->sendMsgLineEdit->text().toLocal8Bit();
#endif
    }
    //发送数据
    myCom->write(buf);
    ui->statusBar->showMessage(tr("发送数据成功"));
    //界面控制
    ui->textBrowser->setTextColor(Qt::lightGray);
}

//读取数据
void MainWindow::readMyCom()
{
    QByteArray temp = myCom->readAll();
    QString buf;

    if(!temp.isEmpty()){

        Xlink_PassThroughProtolPutData(&ptppkt, (unsigned char*)temp.data(), temp.length());

        ui->textBrowser->setTextColor(Qt::black);
        if(ui->ccradioButton->isChecked()){
            buf = temp;
        }else if(ui->chradioButton->isChecked()){
            QString str;
            for(int i = 0; i < temp.count(); i++){
                QString s;
                s.sprintf("0x%02x, ", (unsigned char)temp.at(i));
                buf += s;
            }
        }

        if(!write2fileName.isEmpty()){
            QFile file(write2fileName);
            //如果打开失败则给出提示并退出函数
            if(!file.open(QFile::WriteOnly | QIODevice::Text)){
                QMessageBox::warning(this, tr("写入文件"), tr("打开文件 %1 失败, 无法写入\n%2").arg(write2fileName).arg(file.errorString()), QMessageBox::Ok);
                return;
            }
            QTextStream out(&file);
            out<<buf;
            file.close();
        }

        ui->textBrowser->setText(ui->textBrowser->document()->toPlainText() + buf);
        QTextCursor cursor = ui->textBrowser->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->textBrowser->setTextCursor(cursor);

        ui->recvbyteslcdNumber->display(ui->recvbyteslcdNumber->value() + temp.size());
        ui->statusBar->showMessage(tr("成功读取%1字节数据").arg(temp.size()));
    }
}

int MainWindow::XlinkUartSend(QextSerialPort *myCom, unsigned char * Buffer, unsigned short BufferLen)
{
	int ret = -1;
    //QByteArray buf;
    //buf.append(data);
    //发送数据
    ret = myCom->write((const char*)Buffer, BufferLen);
	return ret;
}

void MainWindow::xlink_PacketCallBack(XLINK_PASSTHROUGHPROTOCOLPACKET *pkt)
{
	qDebug("rec one packet ........\r\n");
	XLINK_CMD_VALUE cmd_value;
	unsigned char temp;

	if(pkt == NULL)return;
	if(pkt->en == 0)return;

    cmd_value = (XLINK_CMD_VALUE)pkt->cmd;

    qDebug("get cmd value=%02x(HEX),datalen=%d\r\n",cmd_value,pkt->datalen);

	switch(cmd_value){
			case XLINK_PTR_CHECK_MAC:  //mcu check wifi mac
                 //xlink_getmac_resp();
				 break;
			case XLINK_PTR_CHECK_NET:  //mcu check wifi net
                 //xlink_getwifi_status_resp();
				 break;
			case XLINK_PTR_CHECK_PIDKEY: //mcu check wifi pid and key
                 //xlink_get_pidkey_resp();
				 break;
			case XLINK_PTR_SET_PIDKEY:  //mcu set wifi pid and key
                 //xlink_set_pidkey_resp(pkt);
				 break;
			case XLINK_PTR_ENTER_SMARTLINK: // mcu set wifi into smartlink
                 //xlink_enter_smartlink_resp();
				 break;

			case XLINK_PTR_REBOOT_WIFI:   //mcu reboot wifi
                //xlink_reboot_wifi_resp();
				break;

			case XLINK_PTR_REPASS_WIFI:  //mcu repass wifi
                //xlink_repass_wifi_resp();
				break;
			case XLINK_PTR_CHECK_WIFI_VER:   //mcu check wifi version
                //xlink_getwifi_ver_resp();
				break;
			case XLINK_PTR_GET_WIFI_TIME:   //mcu get wifi time
                //xlink_getwifi_time_resp();
				break;
			case XLINK_PTR_SET_APSTA_MODE:  //mcu set wifi in visiable
				break;
			case XLINK_PTR_AP_SMARTLINK_STATUS:  //mcu return data to app
				 break;

			case XLINK_PTR_CHECK_SN: //mcu check wifi sn
                 //xlink_get_sn_resp();
				 break;
			case XLINK_PTR_SET_SN:	//mcu set wifi sn
                // xlink_set_sn_resp(pkt);
				 break;

			case XLINK_PTR_CHECK_APSTA_MODE: //mcu check wifi apsta mode and wifi strength
                 //xlink_checkwifi_sta_or_ap_resp();
				 break;

			case XLINK_PTR_WIFI_TO_MCU_TRAN_T:
				break;
			case XLINK_PTR_MCU_TO_WIFI_TRAN_T:   //mcu transfer passthrough data to wifi
				//xlink_mcu_to_wifi_passthrough_resp(pkt);
				break;
			case XLINK_PTR_WIFI_TO_MCU_TRAN_DP:
				break;
			case XLINK_PTR_MCU_TO_WIFI_TRAN_DP:  //mcu transfer datapoints data to wifi
                //xlink_mcu_to_wifi_dp_process(pkt);
				break;
			case XLINK_PTR_MCU_TO_WIFI_TRAN_ALLDP:  //mcu tranfer all datapoints data to wifi and for web refresh
               //xlink_mcu_to_wifi_alldp_process(pkt);
				break;

			case XLINK_PTR_MCU_TO_WIFI_TRAN_ALLDP_NO_ALARM:  //mcu transfer datapoints data to wifi
                //xlink_mcu_to_wifi_no_alarm_dp_process(pkt);
				break;

			case XLINK_PTP_GET_MCU_VER:
                //xlink_get_mcu_ver();
				break;

			case XLINK_PTP_SET_MCU_VER:
                //xlink_set_mcu_ver(pkt);
				break;

			case XLINK_PTR_CHECK_OTA:  //mcu check whether new version ota bin or not.
			{
			    qDebug("get mcu uart cmd check ota 0x30\r\n");
                //xlink_check_ota(pkt);
                break;
			}
			case XLINK_PTR_ASK_OTA:  //mcu ask wifi for excuting an ota task
            {
                qDebug("get mcu uart cmd ask wifi ota 0x31\r\n");
                //xlink_ask_ota(pkt);
                break;
			}
			case XLINK_PTR_HF_SEND_OTA: //wifi send the ota bin to mcu
			{
                qDebug("get mcu uart cmd send the ota bin to mcu 0x32\r\n");
               // xlink_send_ota_bin(pkt);
				break;
			 }
			case XLINK_PTR_Finish_OTA:	//wifi finish send the ota bin
			{
				qDebug("get mcu uart cmd response wifi finish send ota bin 0x33\r\n");
                //xlink_finish_ota_bin();
				break;
			}
			case XLINK_PTR_ANSWER_OTA: // MCU tell wifi the ota result
			{
			    qDebug("get mcu uart cmd the ota task 0x34\r\n");
				break;
			}

			case XLINK_PTR_ENABLE_PAIRING:
			{
                //xlink_mcu_enable_pairing_pkt_process(pkt);
				break;
			}
			case XLINK_PTR_ENABLE_SUBSCRIBE:
			{
                //xlink_mcu_enable_subscribe_pkt_process(pkt);
				break;
			}
			case XLINK_PTR_CHECK_PINGCODE: //mcu check wifi pingcode
                 //xlink_get_pingcode_resp();
				 break;
			case XLINK_PTR_SET_PINGCODE:	//mcu set wifi pingcode
                 //xlink_set_pingcode_resp(pkt);
				 break;
			default:
				break;
	}
}

int MainWindow::Xlink_PassThroughProtolInit(XLINK_PASSTHROUGHPROTOCOL *PTP_pck)
{
	if(PTP_pck->PacketCallBack == NULL)return -1;
	if(PTP_pck->pktbuf == NULL)return -2;
	if(PTP_pck->pktbuflen < 7)return -3;
	PTP_pck->pktpos = 0;
	return 0;
}

int MainWindow::Xlink_PTP_Init(void)
{
    pkt.PacketSend		= XlinkUartSend;
	ptppkt.pkt			= &pkt;
	ptppkt.pktbuf		= xlink_pktbuf;
	ptppkt.pktbuflen	= PACKAGE_BUF_LEN;
	ptppkt.pktpos		= 0;
    ptppkt.PacketCallBack = xlink_PacketCallBack;

	return Xlink_PassThroughProtolInit(&ptppkt);
}

void MainWindow::Xlink_PassThroughProtolPutData(XLINK_PASSTHROUGHPROTOCOL *PTP_pck,unsigned char *data,unsigned short datalen)
{
	unsigned short i = 0;
	for (i = 0; i < datalen; i++) {
		if(PTP_pck->pktpos >= PTP_pck->pktbuflen) {
			PTP_pck->pktpos = 0;
			PTP_pck->pktbuf[0] = 0;
		}
		if((PTP_pck->pktpos >= 2) && (PTP_pck->pktbuf[0] == XLINK_PTP_HEAD)) {
			PTP_pck->pktlentmp = PTP_pck->pktbuf[1];
			PTP_pck->pktlentmp <<= 8;
			PTP_pck->pktlentmp += PTP_pck->pktbuf[2];
		}
		else {
			PTP_pck->pktlentmp = 0;
		}
		switch(data[i]) {
			case XLINK_PTP_HEAD:
				PTP_pck->pktpos = 0;
				PTP_pck->pktbuf[PTP_pck->pktpos++] = XLINK_PTP_HEAD;
				break;
			case XLINK_PTP_TAIL:
				if((PTP_pck->pktpos >= 5) && (PTP_pck->pktbuf[0] == XLINK_PTP_HEAD)) {
					//a packet
					PTP_pck->pkt->alldatalen = PTP_pck->pktbuf[1];
					PTP_pck->pkt->alldatalen <<= 8;
					PTP_pck->pkt->alldatalen += PTP_pck->pktbuf[2];
					if(PTP_pck->pkt->alldatalen == (PTP_pck->pktpos - 3)) {
						PTP_pck->pkt->cmd = PTP_pck->pktbuf[3];
						PTP_pck->pkt->datalen = PTP_pck->pkt->alldatalen - 2;
						if (PTP_pck->pkt->datalen > 0) {
							PTP_pck->pkt->data = &PTP_pck->pktbuf[4];
						}
						else {
							PTP_pck->pkt->data = NULL;
						}
						if(PTP_pck->pktbuf[PTP_pck->pktpos -1] == Xlink_PassThroughProtolBuildXor(0,&PTP_pck->pktbuf[1],PTP_pck->pktpos -2)) {
							PTP_pck->pkt->en = 1;
							PTP_pck->PacketCallBack(PTP_pck->pkt);
						}
					}
				}
				PTP_pck->pktpos = 0;
				PTP_pck->pktbuf[0] = 0;
				break;
			case XLINK_PTP_CHANGE:
				if((PTP_pck->pktpos >= 1) && (PTP_pck->pktbuf[0] == XLINK_PTP_HEAD)) {
					 PTP_pck->pktbuf[PTP_pck->pktpos - 1] |= XLINK_PTP_OR;
				}
				else {
					PTP_pck->pktpos = 0;
					PTP_pck->pktbuf[0] = 0;
				}
				break;
			default:
				if((PTP_pck->pktpos >= 0) && (PTP_pck->pktbuf[0] == XLINK_PTP_HEAD)) {
					 PTP_pck->pktbuf[PTP_pck->pktpos++] = data[i];
				}
				else {
					PTP_pck->pktpos = 0;
					PTP_pck->pktbuf[0] = 0;
				}
				break;
		}
	}
}

void MainWindow::Xlink_PassThroughProtolBuildSendData(XLINK_PASSTHROUGHPROTOCOLPACKET *pkt)
{
	unsigned char tmp = 0,res = 0;
	unsigned short i = 0;

	pkt->PacketSend = XlinkUartSend;
	if(pkt->en == 0)return;
	if(pkt->PacketSend == NULL)return;

	tmp = XLINK_PTP_HEAD;
    pkt->PacketSend(myCom,&tmp,1);

	tmp = pkt->alldatalen >> 8;
	res = Xlink_PassThroughProtolBuildXor(res,&tmp,1);
	if((tmp == XLINK_PTP_HEAD) || (tmp == XLINK_PTP_CHANGE) || (tmp == XLINK_PTP_TAIL)) {
		tmp = tmp & 0x7f;
        pkt->PacketSend(myCom,&tmp,1);
		tmp = XLINK_PTP_CHANGE;
        pkt->PacketSend(myCom,&tmp,1);
	}
	else {
        pkt->PacketSend(myCom,&tmp,1);
	}

	tmp = pkt->alldatalen >> 0;
	res = Xlink_PassThroughProtolBuildXor(res,&tmp,1);
	if((tmp == XLINK_PTP_HEAD) || (tmp == XLINK_PTP_CHANGE) || (tmp == XLINK_PTP_TAIL)) {
		tmp = tmp & 0x7f;
        pkt->PacketSend(myCom,&tmp,1);
		tmp = XLINK_PTP_CHANGE;
        pkt->PacketSend(myCom,&tmp,1);
	}
	else {
        pkt->PacketSend(myCom,&tmp,1);
	}

	tmp = pkt->cmd;
	res = Xlink_PassThroughProtolBuildXor(res,&tmp,1);
	if((tmp == XLINK_PTP_HEAD) || (tmp == XLINK_PTP_CHANGE) || (tmp == XLINK_PTP_TAIL)) {
		tmp = tmp & 0x7f;
        pkt->PacketSend(myCom,&tmp,1);
		tmp = XLINK_PTP_CHANGE;
        pkt->PacketSend(myCom,&tmp,1);
	}
	else {
        pkt->PacketSend(myCom,&tmp,1);
	}

	for(i = 0; i < pkt->datalen; i++) {
		res = Xlink_PassThroughProtolBuildXor(res,&pkt->data[i],1);
		switch(pkt->data[i]) {
			case XLINK_PTP_HEAD:
				tmp = XLINK_PTP_HEAD & 0x7f;
                pkt->PacketSend(myCom,&tmp,1);
				tmp = XLINK_PTP_CHANGE;
                pkt->PacketSend(myCom,&tmp,1);
				break;
			case XLINK_PTP_CHANGE:
				tmp = XLINK_PTP_CHANGE & 0x7f;
                pkt->PacketSend(myCom,&tmp,1);
				tmp = XLINK_PTP_CHANGE;
                pkt->PacketSend(myCom,&tmp,1);
				break;
			case XLINK_PTP_TAIL:
				tmp = XLINK_PTP_TAIL & 0x7f;
                pkt->PacketSend(myCom,&tmp,1);
				tmp = XLINK_PTP_CHANGE;
                pkt->PacketSend(myCom,&tmp,1);
				break;
			default:
                pkt->PacketSend(myCom,&pkt->data[i],1);
				break;
		}
	}

	tmp =res;
	if((tmp == XLINK_PTP_HEAD) || (tmp == XLINK_PTP_CHANGE) || (tmp == XLINK_PTP_TAIL)) {
		tmp = tmp & 0x7f;
        pkt->PacketSend(myCom,&tmp,1);
		tmp = XLINK_PTP_CHANGE;
        pkt->PacketSend(myCom,&tmp,1);
	}
	else {
        pkt->PacketSend(myCom,&tmp,1);
	}

	tmp = XLINK_PTP_TAIL;
    pkt->PacketSend(myCom,&tmp,1);
}



unsigned char MainWindow::Xlink_PassThroughProtolBuildXor(unsigned char resold,unsigned char *data,unsigned int datalen)
{
	unsigned char res = 0;
	unsigned int  i = 0;
	res = resold;
	for(i = 0; i < datalen; i++) {
		res ^=data[i];
	}
	return res;
}




