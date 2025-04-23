#include "patientinfo.h"
#include "ui_patientinfo.h"
#include <iostream>
#include <fstream>
#include "common.h"
#include <QString>
#include <widget.h>

int is_Doctor_p=1;

PatientInfo::PatientInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientInfo)
{
    ui->setupUi(this);

    ui->label_Diseases->clear();
    ui->Label_Allergies->clear();
    ui->label_Addr_Dept->setText("部门");
    ui->lineEdit_Diseases->setDisabled(true);
    ui->lineEdit_Allergies->setDisabled(true);

    TCP_socket = new QTcpSocket(); // 初始化服务器

    connect(TCP_socket, &QTcpSocket::readyRead, this, &PatientInfo::readSocket);
    connect(TCP_socket, &QTcpSocket::disconnected, this, &PatientInfo::discardSocket);

    QString IPAddress = SERVER_ADDR_DB;
    TCP_socket->connectToHost(QHostAddress(IPAddress), SERVER_PORT_DB);
    if (TCP_socket->waitForConnected(3000))
    {
        ui->label_connect->setText("用户（Socket）已连接");
    }
    else
    {
        ui->label_connect->setText("用户（Socket）无法连接" + TCP_socket->errorString());
    }
}

PatientInfo::~PatientInfo()
{
    delete ui;
}

void PatientInfo::readSocket()
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
        if (ui->comboBox_isPatient->currentIndex() == 1)
        {
            ui->lineEdit_Password->setText(in.readLine());
            ui->lineEdit_Name->setText(in.readLine());
            ui->lineEdit_Age->setText(in.readLine());
            ui->comboBox_Sex->setCurrentIndex(in.readLine().toInt());
            ui->lineEdit_Address->setText(in.readLine());
            ui->lineEdit_ID->setText(in.readLine());
            ui->lineEdit_Tel->setText(in.readLine());
            ui->lineEdit_Allergies->setText(in.readLine());
            ui->lineEdit_Diseases->setText(in.readLine());
        }
        else
        {
            ui->lineEdit_Password->setText(in.readLine());
            ui->lineEdit_Name->setText(in.readLine());
            ui->lineEdit_Age->setText(in.readLine());
            ui->comboBox_Sex->setCurrentIndex(in.readLine().toInt());
            ui->lineEdit_Address->setText(in.readLine());//部门信息
            ui->lineEdit_ID->setText(in.readLine());
            ui->lineEdit_level->setText(in.readLine());
            ui->lineEdit_year->setText(in.readLine());
            ui->lineEdit_Tel->setText(in.readLine());//电话
            ui->lineEdit_time->setText(in.readLine());

        }


    }
}

void PatientInfo::discardSocket()
{
    TCP_socket->deleteLater();
}

void PatientInfo::Send_File(QTcpSocket *TCP_socket, QString filename)
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
                       tr("已修改信息"),
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

void PatientInfo::on_pushButton_Send_clicked()
{
    //将数据转换为.txt文件
    std::string filePath = "/home/syx/Git_Project/senddata/clientinfo.txt"; // 请根据实际路径修改

    // 创建一个ofstream对象，并打开指定路径的txt文件
    std::ofstream outFile(filePath);

    // 检查文件是否成功打开
    if (outFile.is_open()) {
            // 向文件写入前两行

        QString str;
        std::string s;
        if (is_Doctor_p == 0)//患者信息
        {
            outFile <<"1\n2\n";

            str=ui->lineEdit_Password->text();
            s = str.toLocal8Bit().constData();
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
        {
            outFile <<"2\n2\n";

            str=ui->lineEdit_Password->text();
            s = str.toLocal8Bit().constData();
            outFile << s ;
            outFile << "\n" ;


            str=ui->lineEdit_Name->text();
            s = str.toLocal8Bit().constData();
            outFile << s ;
            outFile << "\n" ;

            str = ui->lineEdit_level->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;//职称

            str = ui->lineEdit_Address->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;//部门

            str = ui->lineEdit_ID->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_year->text();
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

            str = ui->lineEdit_time->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

            str = ui->lineEdit_Tel->text();
            s = str.toLocal8Bit().constData();
            outFile << s;
            outFile << "\n" ;

        }
        s.erase();
        str.clear();


            // 关闭文件
        outFile.close();
        std::cout << "文件写入成功！" << std::endl;
        } else {
            std::cerr << "无法打开文件！" << std::endl;
        }


    if (TCP_socket)
    {
        if (TCP_socket->isOpen())
        {
            QString FilePath = "/home/syx/Git_Project/senddata/clientinfo.txt";
            Send_File(TCP_socket, FilePath);
        }
    }
}

void PatientInfo::on_pushButton_fetch_clicked()
{

            std::string filepath = "/home/syx/Git_Project/senddata/fetchdata.txt";
            QString FilePath = "/home/syx/Git_Project/senddata/fetchdata.txt";
            std::ofstream outFile(filepath);

            // 检查文件是否成功打开
            if (outFile.is_open()) {
                if (ui->comboBox_isPatient->currentIndex() == 0)
                    outFile << "2";
                else
                    outFile << "1";
                    // 向文件写入前两行
                QString str;
                std::string s;
                outFile << "\n3\n" ;

                str=ui->lineEdit_Name->text();
                s = str.toLocal8Bit().constData();
                outFile << s ;
                outFile << "\n" ;

                Send_File(TCP_socket, FilePath);
                outFile.close();
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



void PatientInfo::on_comboBox_isPatient_activated(int index)
{
    if (index == 0)
    {
        ui->label_Diseases->clear();
        ui->Label_Allergies->clear();
        ui->label_Addr_Dept->setText("部门");
        ui->label_level->setText("职称");
        ui->label_year->setText("从医年份");
        ui->lineEdit_year->setEnabled(true);
        ui->label_time->setText("出诊时间");
        ui->lineEdit_Diseases->setDisabled(true);
        ui->lineEdit_Allergies->setDisabled(true);
        is_Doctor_p=1;
    }
    else
    {

        ui->label_level->clear();
        ui->lineEdit_level->setDisabled(true);
        ui->label_Diseases->setText("过往病史");
        ui->Label_Allergies->setText("过敏原");
        ui->lineEdit_Diseases->setEnabled(true);
        ui->lineEdit_Allergies->setEnabled(true);
        ui->label_Addr_Dept->setText("地址");
        ui->label_year->clear();
        ui->lineEdit_year->setDisabled(true);
        ui->label_time->clear();
        ui->lineEdit_time->setDisabled(true);
        is_Doctor_p=0;
    }
}
