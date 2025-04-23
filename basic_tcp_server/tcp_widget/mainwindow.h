#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtGui>
#include <QtCore>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readSocket();
    void discardSocket();

    void newConnection();
    void AddToSocketList(QTcpSocket *socket);

    void on_pushButton_Sendfile_clicked();

private:
    void Send_File(QTcpSocket *socket, QString filename);


private:
    Ui::MainWindow *ui;
    QTcpServer *TCP_server;
    QList<QTcpSocket *> Client_List;


};
#endif // MAINWINDOW_H
