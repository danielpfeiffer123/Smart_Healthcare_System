#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TCP_server = new QTcpServer(); // 初始化服务器
    if (TCP_server->listen(QHostAddress::AnyIPv4, 8090)) //在本机IP地址以及8090端口监听
    {
        connect(TCP_server, &QTcpServer::newConnection, this, &MainWindow::newConnection);

        ui->statusbar->showMessage("TCP 服务器启动");
    }
    else
    {
        QMessageBox::information(this, "TCP 服务器错误", TCP_server->errorString());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSocket()
{
    //接受客户端的信息
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray DataBuffer;

    QDataStream socketstream(socket);
    socketstream.setVersion(QDataStream::Qt_5_12);

    socketstream.startTransaction();
    socketstream >> DataBuffer;//读取接收到的数据

    if (socketstream.commitTransaction() == false)
    {
        return;
    }

    //收取文件名以及文件大小

    QString HeaderData = DataBuffer.mid(0, 128);

    QString Filename = HeaderData.split(",")[0].split(":")[1];
    QString FileExt = Filename.split(".")[1];
    QString FileSize = HeaderData.split(",")[1].split(":")[1];

    DataBuffer = DataBuffer.mid(128);//仅文件数据

    QString SaveFilePath = QCoreApplication::applicationDirPath() + "/" + Filename;

    QFile File(SaveFilePath);
    if (File.open(QIODevice::WriteOnly))
    {
        File.write(DataBuffer);
        File.close();
    }

}

void MainWindow::discardSocket()
{
    //若客户从服务器断开连接，则从Client_list中删除
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    int inx = Client_List.indexOf(socket);
    if (inx > -1)
    {
        Client_List.removeAt(inx);
    }
    //增加新连接的用户
    ui->comboBox_Clientlist->clear();

    foreach (QTcpSocket *sockettemp, Client_List)
    {
        ui->comboBox_Clientlist->addItem(QString::number(sockettemp->socketDescriptor()));
    }

    socket->deleteLater();
}

void MainWindow::newConnection()
{
    while (TCP_server->hasPendingConnections())
    {
        //在服务器添加新的TCP客户

    }
}

void MainWindow::AddToSocketList(QTcpSocket *socket)
{
    Client_List.append(socket);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::discardSocket);
    ui->textEdit_Message->append("客户与服务器成功连接 ： Socket ID : " + QString::number(socket->socketDescriptor()));
    ui->comboBox_Clientlist->addItem(QString::number(socket->socketDescriptor()));
}


void MainWindow::on_pushButton_Sendfile_clicked()
{
    //发送文件

    QString FilePath = QFileDialog::getOpenFileName(this, "Select File", QCoreApplication::applicationDirPath(), "文件（*.jpg *.txt  *.png  *.bmd）");
    if (ui->comboBox_Transfertype->currentText() == "全部发送")
    {
        foreach (QTcpSocket *sockettemp, Client_List)
        {
            Send_File(sockettemp, FilePath);
        }
    }
    else if (ui->comboBox_Transfertype->currentText() == "私发")
    {
        QString receiverid = ui->comboBox_Clientlist->currentText();
        foreach (QTcpSocket *sockettemp, Client_List)
        {
            if (sockettemp->socketDescriptor() == receiverid.toLongLong())
            {
                Send_File(sockettemp, FilePath);
            }
        }
    }
}

void MainWindow::Send_File(QTcpSocket *socket, QString filename)
{
    if (socket)
    {
        if (socket->isOpen())
        {
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
