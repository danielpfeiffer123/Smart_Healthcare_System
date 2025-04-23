#include "registdlg.h"
#include "ui_registdlg.h"
#include <iostream>
#include <fstream>
#include <QGraphicsDropShadowEffect>
#include <QFile>
#include <QList>
#include <QString>
#include <QTextStream>
#include <QStringList>
#include <QFile>
#include <QMessageBox>


int is_Doctor=1;

RegistDlg::RegistDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistDlg)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground, false);

    Init();
}

RegistDlg::~RegistDlg()
{
    delete ui;
}

void RegistDlg::mousePressEvent(QMouseEvent *event)
{
    mouseWindowTopLeft = event->pos();
}

void RegistDlg::mouseMoveEvent(QMouseEvent *event)
{

    //窗口移动
    if (event->buttons() & Qt::LeftButton)
    {
        mouseDeskTopLeft = event->globalPos();
        windowDeskTopLeft = mouseDeskTopLeft - mouseWindowTopLeft;  //矢量计算
        this->move(windowDeskTopLeft);     //移动到目的地
    }
}

void RegistDlg::disconnectedSlot()
{
    ChatLogInfo()<<"close socket...";
}

void RegistDlg::readyReadSlot()
{
    DeMessageHead header;
    /*接收注册响应*/
    memset(&header,'\0',sizeof(DeMessageHead));
    int len = socket->read((char*)&header,sizeof(DeMessageHead));
    ChatLogInfo()<<"readLen:"<<len;
    ChatLogInfo()<<"length:"<<header.length;
    char *p = (char*)malloc(header.length);
    DeMessagePacket* pPacket = (DeMessagePacket *)p;
    socket->read((char*)pPacket,header.length);

    if(pPacket->error == 0){
        m_status = true;
    }

    RegistInfoResp* resp = (RegistInfoResp*)(p+sizeof(DeMessagePacket));
    ChatLogInfo()<<"account:"<<resp->m_account;

    userInfo.m_account = resp->m_account;
    strncpy(userInfo.m_userName,ui->lineEdit_username->text().toStdString().c_str(),ui->lineEdit_username->text().toStdString().size());
    strncpy(userInfo.m_password,ui->lineEdit_password->text().toStdString().c_str(),ui->lineEdit_password->text().size());

    free(p);
    //给数据库服务器发送数据
    socket = new QTcpSocket;
    QString ipAddressStr_DB = SERVER_ADDR_DB;
    quint16 port_DB = SERVER_PORT_DB;
    socket->connectToHost(ipAddressStr_DB, port_DB);
    if(socket->waitForConnected(3000))
    {
        ChatLogInfo() << "Connect Server success";
    }
    else
    {
        ChatLogInfo() << socket->errorString();
        QMessageBox::warning(this,
               tr("无法注册"),
               tr("数据库无法连接"),
               QMessageBox::Ok | QMessageBox::Cancel,
               QMessageBox::Ok);
        return;
    }

    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnectedSlot()));     //客户端断开连接
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyReadSlot()));           //接收消息

    std::string filePath = "/home/syx/Git_Project/senddata/clientinfo.txt"; // 请根据实际路径修改

    // 创建一个ofstream对象，并打开指定路径的txt文件
    std::ofstream outFile(filePath);

    // 检查文件是否成功打开
    if (outFile.is_open()) {
            // 向文件写入前两行
        QString str;
        std::string s;
        if (is_Doctor == 0)//患者信息
        {

            outFile <<"1\n1\n";

            QString str=ui->lineEdit_password->text();
            std::string s = str.toLocal8Bit().constData();
            outFile << s ;
            outFile << "\n" ;

            str=ui->lineEdit_Name->text();
            s = str.toLocal8Bit().constData();
            outFile << s ;
            outFile << "\n" ;

            str = ui->lineEdit_Age->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->comboBox_Sex->currentText();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_Address->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_ID->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_Tel->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_Allergies->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_Diseases->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;
            s.erase();
            str.clear();
        }
        else
        {//医生信息
            outFile <<"2\n1\n";


            str=ui->lineEdit_password->text();
            s = str.toLocal8Bit().constData();
            outFile << s ;
            outFile << "\n" ;

            str=ui->lineEdit_Name->text();
            s = str.toLocal8Bit().constData();
            outFile << s ;
            outFile << "\n" ;

            str = ui->lineEdit_Tel->text();//职称
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_Address->text();//部门
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_ID->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_year->text();//从医年份
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_Age->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->comboBox_Sex->currentText();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_time->text();//出诊时间
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_Allergies->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;//电话


        }
        s.erase();
        str.clear();


            // 关闭文件
        outFile.close();
        std::cout << "文件写入成功！" << std::endl;
        } else {
            std::cerr << "无法打开文件！" << std::endl;
        }
    if (socket)
    {
        if (socket->isOpen())
        {
            QString filename = "/home/syx/Git_Project/senddata/clientinfo.txt";
            QFile filedata(filename);
            if (filedata.open(QIODevice::ReadOnly))
            {
                QFileInfo fileinfo(filedata);
                QString FileNameWithExt(fileinfo.fileName());

                QDataStream socketstream(socket);
                socketstream.setVersion(QDataStream::Qt_5_12); //设置版本

                //在发送文件时附带发送文件名及其大小
                QByteArray header;
                header.prepend("filename:" + FileNameWithExt.toUtf8() + ",filesize:" + QString::number(filedata.size()).toUtf8());
                header.resize(128);

                //添加文件数据
                QByteArray ByteFileData = filedata.readAll();
                ByteFileData.prepend(header);

                //写入socket
                socketstream << ByteFileData;
                //ui->->append("发送文件");
                qDebug() << "文件发送"   ;
            }
            else
            {
                qDebug() << "文件无法打开"   ;
            }
        }
        else
        {
            qDebug() << "用户的Socket无法打开" ;

        }
    }
    else
    {
        qDebug() << "用户的Socket打开无效" ;
    }
    return accept();


}

void RegistDlg::Init()
{

    this->setWindowTitle("账号注册");
    memset(&userInfo,'\0',sizeof (userInfo));
    m_status = false;
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint); // 最小化按钮
    setWindowFlags(windowFlags() | Qt::WindowContextHelpButtonHint); // 帮助按钮


    this->setWindowFlags(Qt::FramelessWindowHint);          //去掉标题栏无边框
    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor(39,40,43,100));
    //设置阴影圆角
    shadow->setBlurRadius(10);

    ui->label_Diseases->clear();
    ui->lineEdit_Diseases->setDisabled(true);
    ui->lineEdit_Diseases->setPlaceholderText("");
    ui->label_Allergies->setText("电话号码");
    ui->lineEdit_Allergies->setPlaceholderText("请输入电话号码");

}

void RegistDlg::on_pushBtn_regist_clicked()
{
    socket = new QTcpSocket;
    QString ipAddressStr = SERVER_ADDR;

    quint16 port = SERVER_PORT;


    socket->connectToHost(ipAddressStr, port);
    if(socket->waitForConnected(3000))
    {
        ChatLogInfo() << "Connect Server success";
    }
    else
    {
        ChatLogInfo() << socket->errorString();
        return;
    }

    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnectedSlot()));     //客户端断开连接
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyReadSlot()));           //接收消息

    RegistInfoReq info;
    memset(&info,'\0',sizeof(RegistInfoReq));
    strncpy(info.m_userName,ui->lineEdit_username->text().toStdString().c_str(),ui->lineEdit_username->text().toStdString().size());
    strncpy(info.m_password,ui->lineEdit_password->text().toStdString().c_str(),ui->lineEdit_password->text().size());

    if (ui->comboBox_isPatient->currentIndex() == 0)
        doc=1;
    else
        doc=0;
    writeMsg(&info,sizeof(RegistInfoReq),CommandEnum_Registe);
    //return accept();    //Closes the dialog and emits the accepted() signal.


}
void RegistDlg::writeMsg(void *buf, int bufLen, int type)
{
    DeMessageHead header;
    memcpy(header.mark, "DE", sizeof(header.mark));
    header.encoded = '0';
    header.version = '0';
    header.length = sizeof(DeMessagePacket) + bufLen;

    char *p = (char *)malloc(header.length);
    DeMessagePacket *pPacket = (DeMessagePacket *)p;
    pPacket->mode = 2;
    pPacket->sequence = getSeqNum();
    pPacket->command = type;
    pPacket->error = 0;
    if(buf)
        memcpy(p + sizeof(DeMessagePacket), buf, bufLen);

    char *sendMsg = new char[sizeof(DeMessageHead) + header.length];
    memset(sendMsg, 0, sizeof(DeMessageHead) + header.length);
    memcpy(sendMsg, &header, sizeof(DeMessageHead));
    memcpy(sendMsg + sizeof(DeMessageHead), p, header.length);
    free(p);
    socket->write(sendMsg, sizeof(DeMessageHead) + header.length);
    delete[] sendMsg;



}

void RegistDlg::on_pushBtn_hide_clicked()
{
    QWidget* pWindow = this->window();
    if(pWindow->isTopLevel())
        pWindow->showMinimized();
}

void RegistDlg::on_pushBtn_close_clicked()
{
   this->close();
}

void RegistDlg::on_comboBox_isPatient_activated(int index)
{
    if (index == 0)
    {

        ui->label_Diseases->clear();
        ui->lineEdit_Diseases->setPlaceholderText(" ");
        ui->lineEdit_Diseases->setDisabled(true);
        ui->label_Allergies->setText("电话号码");
        ui->lineEdit_Allergies->setPlaceholderText("请输入电话号码");
        ui->label_level->setText("职称");
        ui->label_time->setText("出诊时间");
        ui->lineEdit_time->setPlaceholderText("请输入你的出诊时间");
        ui->lineEdit_time->setEnabled(true);
        ui->label_Addr_Dept->setText("部门");
        ui->label_year->setText("从医年份");
        ui->lineEdit_time->setPlaceholderText("请输入你的从医年份");
        ui->lineEdit_year->setEnabled(true);
        is_Doctor=1;
    }
    else
    {
        ui->label_Diseases->setText("过往病史");
        ui->lineEdit_Diseases->setEnabled(true);
        ui->lineEdit_Diseases->setPlaceholderText("请输入您的过往病史");
        ui->label_Allergies->setText("过敏原");
        ui->lineEdit_Allergies->setPlaceholderText("请输入过敏原");
        ui->label_level->setText("电话");
        ui->lineEdit_Tel->setPlaceholderText("请输入你的电话号码");
        //tel-level
        ui->label_Addr_Dept->setText("地址");
        ui->lineEdit_time->setDisabled(true);
        ui->label_time->clear();
        ui->lineEdit_time->setPlaceholderText(" ");
        ui->label_year->clear();
        ui->lineEdit_year->setDisabled(true);
        ui->lineEdit_year->setPlaceholderText(" ");
        is_Doctor=0;

    }
}
