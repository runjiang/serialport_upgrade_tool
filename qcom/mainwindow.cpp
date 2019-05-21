#include <QDebug>
#include <QDir>
#include <QFile>    
#include <QCryptographicHash>
#include <QtGlobal>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qglobal.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startInit();
    qDebug("startInit\r\n");
    myCom = NULL;

    //ui->qter->setText(
    //       tr("<a href=\"http://www.qter.org\">www.qter.org</a>"));
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

    ui->statusBar->showMessage(tr("欢迎使用Xlink MCU升级调试工具"));
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
    //实例QDir
    QDir *folder = new QDir;
    //判断创建文件夹是否存在
    bool exist = folder->exists("./McuBin");
    if(exist)
    {
        //QMessageBox::warning(this,tr("创建文件夹"),tr("文件夹已经存在！"));
    } 
    else //如果不存在，创建文件夹
    {
        //创建文件夹
        bool ok = folder->mkdir("./McuBin");
        //判断是否成功
        if(ok)
        {
            //QMessageBox::warning(this,tr("创建文件夹"),tr("文件夹创建成功！"));
        } 
        else
        {
            QMessageBox::warning(this,tr("创建文件夹"),tr("文件夹创建失败！"));
        }
    }

    setActionsEnabled(false);
    ui->delayspinBox->setEnabled(false);
    ui->sendmsgBtn->setEnabled(false);    
    ui->checkDeviceButton->setEnabled(false);
    ui->deviceRebootButton->setEnabled(false);
    ui->mcuUpgradeButton->setEnabled(false);

    ui->sendMsgLineEdit->setEnabled(false);
    ui->curVersionLineEdit->setEnabled(true);
    ui->identifyLineEdit->setEnabled(true);
    ui->PacketLenLineEdit->setEnabled(true);
    ui->obocheckBox->setEnabled(false);
    ui->actionAdd->setEnabled(true);

    ui->identifyLineEdit->setText("1");
    ui->curVersionLineEdit->setText("1");
    ui->PacketLenLineEdit->setText("64");

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
    showString("init xlink ptp\r\n");
    if (Xlink_PTP_Init() != 0) {
		qDebug("Xlink_PTP_Init error");
	}
    QString str;
    bool ok;
    str = ui->curVersionLineEdit->text();
    mcuCurVersion = str.toInt(&ok, 10);
    if(!ok){
        QMessageBox::information(this, tr("提示消息"), tr("输入的当前版本数据格式有错误！"), QMessageBox::Ok);
        return;
    }
    str = ui->identifyLineEdit->text();
    mcuOtaIdentify = str.toInt(&ok, 10);
    if(!ok){
        QMessageBox::information(this, tr("提示消息"), tr("输入的标识符数据格式有错误！"), QMessageBox::Ok);
        return;
    }
    str = ui->PacketLenLineEdit->text();
    mcuOtaPktLength = str.toInt(&ok, 10);
    if(!ok){
        QMessageBox::information(this, tr("提示消息"), tr("输入的数据包长度数据格式有错误！"), QMessageBox::Ok);
        return;
    }
    mcu_upgradeState = XLINK_MCU_UPGRADE_STATE_IDILE;
    ui->upgradeProgressBar->setValue(0);  // 当前进度 
   
    QString s;
    s.sprintf("mcuCurVersion:%d mcuOtaIdentify:%d mcuOtaPktLength:%d\r\n",mcuCurVersion,mcuOtaIdentify,mcuOtaPktLength);
    qDebug("mcuCurVersion:%d mcuOtaIdentify:%d mcuOtaPktLength:%d",mcuCurVersion,mcuOtaIdentify,mcuOtaPktLength);
    showString(s);

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
        ui->checkDeviceButton->setEnabled(true);
        ui->deviceRebootButton->setEnabled(true);
        ui->mcuUpgradeButton->setEnabled(true);
        setComboBoxEnabled(false);
        ui->sendMsgLineEdit->setEnabled(true);
        ui->sendMsgLineEdit->setFocus();
        ui->curVersionLineEdit->setEnabled(false);
        ui->identifyLineEdit->setEnabled(false);
        ui->PacketLenLineEdit->setEnabled(false);
        ui->actionOpen->setEnabled(false);
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
    ui->checkDeviceButton->setEnabled(false);
    ui->deviceRebootButton->setEnabled(false);
    ui->mcuUpgradeButton->setEnabled(false);

    setComboBoxEnabled(true);
    ui->curVersionLineEdit->setEnabled(true);
    ui->identifyLineEdit->setEnabled(true);
    ui->PacketLenLineEdit->setEnabled(true);
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

void MainWindow::on_checkDeviceButton_clicked()
{
    XLINK_PASSTHROUGHPROTOCOLPACKET respack;
    memset(&respack,'\0',sizeof(respack));
    respack.en = 1;
    respack.cmd = XLINK_PTR_CHECK_NET;
    respack.datalen = 0;
    respack.alldatalen = respack.datalen + 2;
    respack.data = NULL;
    qDebug("xlink mcu send check device state cmd 0x01");
    showString("xlink mcu send check device state cmd 0x01\r\n");
    Xlink_PassThroughProtolBuildSendData(&respack);
}

void MainWindow::on_deviceRebootButton_clicked()
{
    XLINK_PASSTHROUGHPROTOCOLPACKET respack;
	memset(&respack,'\0',sizeof(respack));
	respack.en = 1;
	respack.cmd = XLINK_PTR_REBOOT_WIFI;
	respack.alldatalen = 2;
	respack.datalen = 0;
	respack.data = NULL;
    qDebug("xlink mcu send reboot device cmd 0x05");
    showString("xlink mcu send reboot device cmd 0x05\r\n");
	Xlink_PassThroughProtolBuildSendData(&respack);
}

void MainWindow::on_getPidPkeyButton_clicked()
{
    XLINK_PASSTHROUGHPROTOCOLPACKET respack;
    memset(&respack,'\0',sizeof(respack));
    respack.en = 1;
    respack.cmd = XLINK_PTR_CHECK_PIDKEY;
    respack.datalen = 0;
    respack.alldatalen = respack.datalen + 2;
    respack.data = NULL;
    qDebug("xlink mcu send check device PidPkey cmd 0x02");
    showString("xlink mcu send check device PidPkey cmd 0x02\r\n");
    Xlink_PassThroughProtolBuildSendData(&respack);
}

void MainWindow::on_getWifiMACButton_clicked()
{
    XLINK_PASSTHROUGHPROTOCOLPACKET respack;
    memset(&respack,'\0',sizeof(respack));
    respack.en = 1;
    respack.cmd = XLINK_PTR_CHECK_MAC;
    respack.datalen = 0;
    respack.alldatalen = respack.datalen + 2;
    respack.data = NULL;
    qDebug("xlink mcu send check device MAC cmd 0x00");
    showString("xlink mcu send check device MAC cmd 0x00\r\n");
    Xlink_PassThroughProtolBuildSendData(&respack);
}


void MainWindow::on_mcuUpgradeButton_clicked()
{   
    if(mcu_upgradeState == XLINK_MCU_UPGRADE_STATE_IDILE){
        mcu_upgradeState = XLINK_MCU_SEND_CHECK_OTA_STATE;
        mcuBinSize = 0;
        ui->upgradeProgressBar->setValue(0);
        memset(mcuBinMd5,0,sizeof(mcuBinMd5));
        mcuBinOrderNumber = 0;
        curMcuBinFileName.clear();
        xlink_mcu_send_check_ota_cmd(mcuCurVersion,mcuOtaIdentify);
    }else{
       showString("Last upgrade is not finish!!!\r\n");
    }
}

void MainWindow::xlink_mcu_send_check_ota_cmd(unsigned short version, unsigned int identify)
{
    XLINK_PASSTHROUGHPROTOCOLPACKET respack;
    unsigned char Uart_SendBuf[7];
    memset(&respack,'\0',sizeof(respack));
    respack.en = 1;
    respack.cmd = XLINK_PTR_CHECK_OTA;
    Uart_SendBuf[0]=2;//任务类型 1 Wifi 2 MCU
    Uart_SendBuf[1]=(version>>8) & 0xff;
    Uart_SendBuf[2]=version & 0xff;
    Uart_SendBuf[3]=(identify>>24) & 0xff;
    Uart_SendBuf[4]=(identify>>16) & 0xff;
    Uart_SendBuf[5]=(identify>>8) & 0xff;
    Uart_SendBuf[6]=identify & 0xff;
    respack.datalen = 7;
    respack.data = Uart_SendBuf;
    respack.alldatalen = 2 + respack.datalen;
    qDebug("xlink mcu send check ota cmd 0x30");    
    showString("xlink mcu send check ota cmd 0x30\r\n");
    Xlink_PassThroughProtolBuildSendData(&respack);
}

void MainWindow::xlink_mcu_send_excute_ota_cmd(unsigned char packetLen)
{
    XLINK_PASSTHROUGHPROTOCOLPACKET respack;
    unsigned char Uart_SendBuf[7];
    memset(&respack,'\0',sizeof(respack));
    respack.en = 1;
    respack.cmd = XLINK_PTR_ASK_OTA;
    Uart_SendBuf[0]=packetLen;
    respack.datalen = 1;
    respack.data = Uart_SendBuf;
    respack.alldatalen = 2 + respack.datalen;
    qDebug("xlink mcu send excute ota cmd 0x30");    
    showString("xlink mcu send excute ota cmd 0x30\r\n");
    mcu_upgradeState = XLINK_MCU_ASK_OTA_STATE;
    Xlink_PassThroughProtolBuildSendData(&respack);
}

void MainWindow::xlink_mcu_send_finish_ota_cmd(unsigned short oldversion,unsigned short newversion, unsigned int identify)
{
    XLINK_PASSTHROUGHPROTOCOLPACKET respack;
    unsigned char Uart_SendBuf[9];
    memset(&respack,'\0',sizeof(respack));
    respack.en = 1;
    respack.cmd = XLINK_PTR_ANSWER_OTA;
    Uart_SendBuf[0]=2;//任务类型 1 Wifi 2 MCU
    Uart_SendBuf[1]=(identify>>24) & 0xff;
    Uart_SendBuf[2]=(identify>>16) & 0xff;
    Uart_SendBuf[3]=(identify>>8) & 0xff;
    Uart_SendBuf[4]=identify & 0xff;
    Uart_SendBuf[5]=(oldversion>>8) & 0xff;
    Uart_SendBuf[6]=oldversion & 0xff;
    Uart_SendBuf[7]=(newversion>>8) & 0xff;
    Uart_SendBuf[8]=newversion & 0xff;
    respack.datalen = 9;
    respack.data = Uart_SendBuf;
    respack.alldatalen = 2 + respack.datalen;
    qDebug("xlink mcu send finish ota cmd 0x34");    
    showString("xlink mcu send finish ota cmd 0x34\r\n");
    Xlink_PassThroughProtolBuildSendData(&respack);
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
    QByteArray temp;
    QString buf;
    forever{
        temp = myCom->readAll();
        if(!temp.isEmpty()){ 
            buf = "rev: ";
            for(int i = 0; i < temp.count(); i++){
                QString s;
                s.sprintf("%02x, ", (unsigned char)temp.at(i));
                buf += s;
            }
            buf = buf + "\r\n";
            qDebug("%s",buf.toStdString().data());
            //showString(buf);
            Xlink_PassThroughProtolPutData(&ptppkt, (unsigned char*)temp.data(), temp.length());
        }else{
            return;
        }
    }
}

//状态和数据显示
void MainWindow::showString(QString buf)
{
    QDateTime current_time = QDateTime::currentDateTime(); 
    //显示时间，格式为：年-月-日 时：分：秒 周几
    QString StrCurrentTime = current_time.toString("yyyy-MM-dd hh:mm:ss  "); 
    buf = StrCurrentTime + buf;
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

    ui->textBrowser->setTextColor(Qt::black);
    ui->textBrowser->setText(ui->textBrowser->document()->toPlainText() + buf);
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);

    ui->recvbyteslcdNumber->display(ui->recvbyteslcdNumber->value() + buf.length());
    ui->statusBar->showMessage(tr("成功读取%1字节数据").arg(buf.length()));
}

void MainWindow::witePacketToBinFile(unsigned char *data, unsigned char data_len)
{
     if(curMcuBinFileName.isEmpty()){
        showString("Error curMcuBinFileName is empty\r\n");
        return;
    }
    QFile writeFile(curMcuBinFileName);
    //存在打开，不存在创建
    writeFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QDataStream out(&writeFile);
    out.writeRawData((char *)data, data_len);
    writeFile.close();
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
            {
                 QString buf;
                 buf = "Device MAC: ";
                 for(int i = 0; i < pkt->datalen; i++){
                    QString s;
                    s.sprintf("%02X", pkt->data[i]);
                    buf += s;
                 }
                 buf = buf + "\r\n";
                 showString(buf);
				 break;
            }
			case XLINK_PTR_CHECK_NET:  //mcu check wifi net
            {
                 QString s;
                 s.sprintf("Network: %d , Server: %d\r\n",pkt->data[0],pkt->data[1]);
                 showString(s);
				 break;
            }
			case XLINK_PTR_CHECK_PIDKEY: //mcu check wifi pid and key
            {
                 QString buf;
                 buf = "PID: ";
                 for(int i = 0; i < 32; i++){
                    QString s;
                    s.sprintf("%c", pkt->data[i]);
                    buf += s;
                 }
                 buf = buf + " PKEY: ";
                 for(int i = 0; i < 32; i++){
                    QString s;
                    s.sprintf("%c", pkt->data[i+32]);
                    buf += s;
                 }
                 buf = buf + "\r\n\r\n";
                 showString(buf);
				 break;
            }
			case XLINK_PTR_SET_PIDKEY:  //mcu set wifi pid and key
                 //xlink_set_pidkey_resp(pkt);
				 break;
			case XLINK_PTR_ENTER_SMARTLINK: // mcu set wifi into smartlink
                 //xlink_enter_smartlink_resp();
				 break;

			case XLINK_PTR_REBOOT_WIFI:   //mcu reboot wifi
                showString("MCU get reboot cmd\r\n");
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
                unsigned char status = pkt->data[0];//状态： 0 表示无升级任务，后面无数据； 1 表示有新升级任务，后跟新固件版本
                mcuNewVersion = (pkt->data[1]<<8) + pkt->data[2];
			    qDebug("get wifi response check ota cmd 0x30\r\n");            
                QString s;
                s.sprintf("get wifi response check ota cmd 0x30, status:%d mcuNewVersion:%d\r\n",status,mcuNewVersion);
                showString(s);
                if(status==1){
                    showString("Has a mcu upgrade task\r\n");
                    xlink_mcu_send_excute_ota_cmd(mcuOtaPktLength);
                    QString s;
                    s.sprintf("version%d_",mcuNewVersion);
                    QDateTime current_time = QDateTime::currentDateTime(); 
                    s = s + current_time.toString("yyyyMMdd_hhmmss.bin"); 
                    curMcuBinFileName = "./McuBin/" + s;
                    showString("curMcuBinFileName = " + curMcuBinFileName + "\r\n");
                }else{
                    showString("No mcu upgrade task\r\n");
                    mcu_upgradeState = XLINK_MCU_UPGRADE_STATE_IDILE;
                    ui->upgradeProgressBar->setValue(0);  // 当前进度 
                }
                break;
			}
			case XLINK_PTR_ASK_OTA:  //mcu ask wifi for excuting an ota task
            {
                unsigned char status = pkt->data[0];//状态： 0 表示升级任务失败； 1 表示开始升级
                qDebug("get wifi response excute ota cmd 0x31\r\n");
                showString("get wifi response excute ota cmd 0x31\r\n");
                if(status==1){
                    showString("Upgrading ... ...\r\n");
                    mcu_upgradeState = XLINK_WIFI_SEND_OTA_STATE;
                }else{
                    showString("get wifi status upgrade failed\r\n");
                     mcu_upgradeState = XLINK_MCU_UPGRADE_STATE_IDILE;
                     ui->upgradeProgressBar->setValue(0);  // 当前进度 
                }
                break;
			}
			case XLINK_PTR_HF_SEND_OTA: //wifi send the ota bin to mcu
			{
                unsigned short orderNumber;// mcuBinOrderNumber
                unsigned char dataLen ;
                qDebug("get wifi send ota bin cmd 0x32\r\n");
              
                orderNumber = (pkt->data[0]<<8) + pkt->data[1];
                dataLen = pkt->data[2];
                if(pkt->datalen == dataLen+3){//包长度正常
                    //NULL
                }else{
                    QString s;
                    s.sprintf("upgrade failed, packet length error dataLen=%d pkt->datalen=%d\r\n",dataLen,pkt->datalen);
                    showString(s);
                    mcu_upgradeState = XLINK_MCU_UPGRADE_STATE_IDILE;
                    break;
                }
                if(orderNumber == mcuBinOrderNumber + 1 || orderNumber == mcuBinOrderNumber) {//包序号正常
                    if(orderNumber == mcuBinOrderNumber + 1){
                        //写文件
                        witePacketToBinFile(&pkt->data[3], dataLen);
                        ui->upgradeProgressBar->setValue(orderNumber);  // 当前进度 
                    }
                    mcuBinOrderNumber = orderNumber;
                }else{
                    QString s;
                    s.sprintf("upgrade failed, order number error orderNumber=%d mcuBinOrderNumber=%d\r\n",orderNumber,mcuBinOrderNumber);
                    showString(s);
                    mcu_upgradeState = XLINK_MCU_UPGRADE_STATE_IDILE;
                    break;
                }

               {//response
                    XLINK_PASSTHROUGHPROTOCOLPACKET respack;
                    unsigned char Uart_SendBuf[7];
                    memset(&respack,'\0',sizeof(respack));
                    respack.en = 1;
                    respack.cmd = XLINK_PTR_HF_SEND_OTA;
                    Uart_SendBuf[0]=pkt->data[0];
                    Uart_SendBuf[1]=pkt->data[1];
                    respack.datalen = 2;
                    respack.data = Uart_SendBuf;
                    respack.alldatalen = 2 + respack.datalen;
                    qDebug("xlink mcu response send ota bin cmd 0x32");    
                    //showString("xlink mcu response send ota bin cmd 0x32\r\n");
                    Xlink_PassThroughProtolBuildSendData(&respack);
                }
				break;
			 }
			case XLINK_PTR_Finish_OTA:	//wifi finish send the ota bin
			{
				qDebug("get wifi finish send ota bin cmd 0x33\r\n");
                mcu_upgradeState = XLINK_WIFI_SEND_FINISH_OTA_STATE;

                mcuBinSize = (pkt->data[0]<<24) + (pkt->data[1]<<16) + (pkt->data[2]<<8) + pkt->data[3];
                memcpy(mcuBinMd5, &pkt->data[4], 16);
                QString resultMd5 = fileMd5(curMcuBinFileName);
                QString binMd5;
                for(int i = 0; i < 16; i++){
                    QString s;
                    s.sprintf("%02x", mcuBinMd5[i]);
                    binMd5 += s;
                }
                showString("get mcu bin Md5:  "+ binMd5+"\r\n");  
                showString("download bin Md5: "+ resultMd5+"\r\n");  
                if(binMd5 == resultMd5){
                    xlink_mcu_send_finish_ota_cmd(mcuCurVersion,mcuNewVersion,mcuNewVersion);
                }else{
                    showString("MD5 error!!!\r\n");
                    mcu_upgradeState = XLINK_MCU_UPGRADE_STATE_IDILE;
                }
				break;
			}
			case XLINK_PTR_ANSWER_OTA: // MCU tell wifi the ota result
			{
                unsigned char status = pkt->data[0];//状态： 0 表示上报失败； 1 表示上报成功。
			    qDebug("get wifi response finish ota cmd 0x34\r\n");
                showString("get wifi response finish ota cmd 0x34\r\n");
                if(status == 1){
                    showString("Mcu ota sucessed\r\n");
                    ui->upgradeProgressBar->setValue(1000);  // 当前进度 
                }else{
                    showString("Mcu ota failed\r\n");
                }
                mcu_upgradeState = XLINK_MCU_UPGRADE_STATE_IDILE;
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
    ptppkt.PacketCallBack = 0;

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
							xlink_PacketCallBack(PTP_pck->pkt);
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

QString MainWindow::fileMd5(const QString &sourceFilePath)
{
    QFile sourceFile(sourceFilePath);
    qint64 fileSize = sourceFile.size();
    showString("Get bin file MD5\r\n");
    QString s;
    s.sprintf(" size = %d, get bin size = %d\r\n",fileSize, mcuBinSize);
    s = sourceFilePath + s;
    showString(s);
    fileSize = qMin((qint64)mcuBinSize,fileSize);
    const qint64 bufferSize = 10240;

    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }
    return QString();
}

void MainWindow::on_upgradeProgressBar_valueChanged(int value)
{

}
