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
#include <string>
#include <vector>
std::vector <QString> s ;


appointment::appointment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::appointment)
{
    ui->setupUi(this);

    TCP_socket = new QTcpSocket(); // 初始化服务器

    connect(TCP_socket, &QTcpSocket::readyRead, this, &appointment::readSocket);
    connect(TCP_socket, &QTcpSocket::disconnected, this, &appointment::discardSocket);

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
    ui->comboBox_docinfo->clear();
}

appointment::~appointment()
{
    delete ui;
}

void appointment::readSocket()
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
    if (File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&File);
        if (in.readLine() == "34")
        {
            while(1)
            {
                s.push_back(in.readLine());
                if(s.back().size() == 0)
                    break;
                ui->comboBox_docinfo->addItem(s.back() + " " +in.readLine());
            }
        }
        else
        {
            in.readLine();

            QString s1 = in.readLine(), s2 = in.readLine();

            QMessageBox::information(this,"挂号信息", "医生" + s1 + "，时间：" + s2);

        }
        File.close();
        }


}

void appointment::discardSocket()
{
    TCP_socket->deleteLater();
}

void appointment::Send_File(QTcpSocket *TCP_socket, QString filename)
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

void appointment::on_pushButton_fetch_clicked()
{
    //将数据转换为.txt文件
    std::string filePath = "/home/syx/Git_Project/senddata/fetchdoc.txt"; // 请根据实际路径修改

    // 创建一个ofstream对象，并打开指定路径的txt文件
    std::ofstream outFile(filePath);
    s.clear();
    // 检查文件是否成功打开
    if (outFile.is_open()) {
            // 向文件写入前两行
            outFile <<"3\n4\n";
        QString str;
        std::string s;

        str=ui->comboBox_dept->currentText();
        s = str.toLocal8Bit().constData();

        outFile << s;
        outFile << "\n" ;
        outFile.close();
        std::cout << "文件写入成功！" << std::endl;
        }
    else {
            std::cerr << "无法打开文件！" << std::endl;
        }


    if (TCP_socket)
    {
        if (TCP_socket->isOpen())
        {
            QString FilePath = "/home/syx/Git_Project/senddata/fetchdoc.txt";
            Send_File(TCP_socket, FilePath);
        }
    }

}




void appointment::on_pushButton_app_clicked()
{
    //将数据转换为.txt文件
    std::string filePath = "/home/syx/Git_Project/senddata/appoint.txt"; // 请根据实际路径修改

    // 创建一个ofstream对象，并打开指定路径的txt文件
    std::ofstream outFile(filePath);

    // 检查文件是否成功打开
    if (outFile.is_open()) {
        std::string s1;
        QString str, k;
            // 向文件写入前两行
            outFile <<"3\n1\n";

            str = ui->lineEdit_name->text();
            s1 = str.toLocal8Bit().constData();
            outFile << s1 ;
            outFile << "\n" ;

            str = ui->comboBox_docinfo->currentText();
            for(int i=0; i<100; i++){
                if (str[i] == " ")
                    break;
                k+=str[i];
            }
            outFile << k.toLocal8Bit().constData() ;
            outFile << "\n";

            str=ui->lineEdit_time->text();
            s1 = str.toLocal8Bit().constData();
            outFile << s1 ;
            outFile << "\n" ;
            outFile.close();
        s1.erase();
        s.clear();
        str.clear();
        std::cout << "文件写入成功！" << std::endl;
        }
    else {
            std::cerr << "无法打开文件！" << std::endl;
        }


    if (TCP_socket)
    {
        if (TCP_socket->isOpen())
        {
            QString FilePath = "/home/syx/Git_Project/senddata/appoint.txt";
            Send_File(TCP_socket, FilePath);
            this->close();
        }
    }
}

void appointment::on_pushButton_fetch_2_clicked()
{
    //将数据转换为.txt文件
    std::string filePath = "/home/syx/Git_Project/senddata/fetch_appoint.txt"; // 请根据实际路径修改

    // 创建一个ofstream对象，并打开指定路径的txt文件
    std::ofstream outFile(filePath);

    if (outFile.is_open()) {
        std::string s1;
        QString str, k;
            // 向文件写入前两行
            outFile <<"3\n2\n";

            str = ui->lineEdit_name->text();
            s1 = str.toLocal8Bit().constData();
            outFile << s1 ;
            outFile << "\n" ;
            outFile.close();
        s1.erase();
        str.clear();
        std::cout << "文件写入成功！" << std::endl;
        }
    else {
            std::cerr << "无法打开文件！" << std::endl;
        }


    if (TCP_socket)
    {
        if (TCP_socket->isOpen())
        {
            QString FilePath = "/home/syx/Git_Project/senddata/fetch_appoint.txt";
            Send_File(TCP_socket, FilePath);
            this->close();
        }
    }
}
