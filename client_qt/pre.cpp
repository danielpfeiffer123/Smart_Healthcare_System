#include "pre.h"
#include "ui_pre.h"
#include "appointment.h"
#include "ui_appointment.h"
#include "patientinfo.h"
#include "ui_patientinfo.h"
#include <iostream>
#include <fstream>
#include "common.h"
#include <QString>
#include <QObject>
#include <QDebug>



Pre::Pre(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pre)
{
    ui->setupUi(this);
    TCP_socket = new QTcpSocket(); // 初始化服务器

    connect(TCP_socket, &QTcpSocket::readyRead, this, &Pre::readSocket);
    connect(TCP_socket, &QTcpSocket::disconnected, this, &Pre::discardSocket);

    QString IPAddress = SERVER_ADDR_DB;
    TCP_socket->connectToHost(QHostAddress(IPAddress), SERVER_PORT_DB);
    if (TCP_socket->waitForConnected(5000))
    {
        ui->label_connect->setText("用户（Socket）已连接");
    }
    else
    {
        ui->label_connect->setText("用户（Socket）无法连接" + TCP_socket->errorString());
    }
}

Pre::~Pre()
{
    delete ui;
}

void Pre::readSocket()
{
    //接受服务器的信息

    QByteArray DataBuffer;

    QDataStream socketstream(TCP_socket);
    socketstream.setVersion(QDataStream::Qt_5_12);

    socketstream.startTransaction();
    socketstream >> DataBuffer;//读取接收到的数据

    if (socketstream.commitTransaction() == false)
    {
        return;
    }
    //收取文件名以及文件大小
    //ui->statusbar->showMessage("用户（Socket）已连接");

    QString HeaderData = DataBuffer.mid(0, 128);

    QString Filename = HeaderData.split(",")[0].split(":")[1];
    QString FileExt = Filename.split(".")[1];
    QString FileSize = HeaderData.split(",")[1].split(":")[1];

    DataBuffer = DataBuffer.mid(128);//仅文件数据

    QString SaveFilePath = "/home/syx/Git_Project/receivedata/" + Filename;
    //ui->textEdit->append("保存文件地址：" + SaveFilePath);

    QFile File(SaveFilePath); //读取返回信息
    if (File.open(QIODevice::WriteOnly))
    {
        File.write(DataBuffer);
        File.close();
    }



}

void Pre::discardSocket()
{
    TCP_socket->deleteLater();
}

void Pre::Send_File(QTcpSocket *TCP_socket, QString filename)
{
    if (TCP_socket)
    {
        if (TCP_socket->isOpen())
        {
            QFile filedata(filename);
            if (filedata.open(QIODevice::ReadOnly))
            {
                QFileInfo fileinfo(filedata);
                QString FileNameWithExt(fileinfo.fileName());

                QDataStream socketstream(TCP_socket);
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
                QMessageBox::warning(this,
                       tr("发送成功"),
                       tr("病历提交成功"),
                       QMessageBox::Ok | QMessageBox::Cancel,
                       QMessageBox::Ok);

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
}

void Pre::on_pushButton_submit_clicked()
{
    std::string filepath = "/home/syx/Git_Project/senddata/pre_submit.txt";
    QString FilePath = "/home/syx/Git_Project/senddata/pre_submit.txt";
    std::ofstream outFile(filepath);

    // 检查文件是否成功打开
    if (outFile.is_open()) {
        QString str;
        std::string s;
        outFile << "4\n1\n" ;

        str=ui->lineEdit_patientname->text();
        s = str.toLocal8Bit().constData();
        outFile << s ;
        outFile << "\n" ;

        str=ui->lineEdit_symp->text();
        s = str.toLocal8Bit().constData();
        outFile << s ;
        outFile << "\n" ;

        str=ui->lineEdit_docname->text();
        s = str.toLocal8Bit().constData();
        outFile << s ;
        outFile << "\n" ;

        str=ui->lineEdit_advice->text();
        s = str.toLocal8Bit().constData();
        outFile << s ;
        outFile << "\n" ;

        str=ui->lineEdit_prescription->text();
        s = str.toLocal8Bit().constData();
        outFile << s ;
        outFile << "\n" ;

        outFile.close();
        Send_File(TCP_socket, FilePath);

    }
    else
        qDebug() << "文件无法打开" ;
    if (TCP_socket)
    {
        if (TCP_socket->isOpen())
        {
            Send_File(TCP_socket, FilePath);
        }
    }
}
