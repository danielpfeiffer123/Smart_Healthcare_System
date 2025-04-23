#ifndef PATIENTINFO_H
#define PATIENTINFO_H

#include <QDialog>
#include <QtNetwork>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include "common.h"

namespace Ui {
class PatientInfo;
}

class PatientInfo : public QDialog
{
    Q_OBJECT

public:
    explicit PatientInfo(QWidget *parent = nullptr);
    ~PatientInfo();

private slots:
    void readSocket();
    void discardSocket();

    void Send_File(QTcpSocket *socket, QString filename);

    void on_pushButton_Send_clicked();


    void on_pushButton_fetch_clicked();


    void on_comboBox_isPatient_activated(int index);

private:
    Ui::PatientInfo *ui;
    QTcpSocket *TCP_socket;
};

#endif // PATIENTINFO_H
