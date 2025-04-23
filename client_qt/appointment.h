#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <QDialog>
#include <QtNetwork>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include "common.h"

namespace Ui {
class appointment;
}

class appointment : public QDialog
{
    Q_OBJECT

public:
    explicit appointment(QWidget *parent = nullptr);
    ~appointment();

private slots:
    void readSocket();
    void discardSocket();
    void on_pushButton_fetch_clicked();

    void Send_File(QTcpSocket *socket, QString filename);


    void on_pushButton_app_clicked();

    void on_pushButton_fetch_2_clicked();

private:
    Ui::appointment *ui;
    QTcpSocket *TCP_socket;
};


#endif // APPOINTMENT_H
