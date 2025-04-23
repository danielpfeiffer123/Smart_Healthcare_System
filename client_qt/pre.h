#ifndef PRE_H
#define PRE_H

#include <QDialog>
#include <QtNetwork>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include "common.h"


namespace Ui {
class Pre;
}

class Pre : public QDialog
{
    Q_OBJECT

public:
    explicit Pre(QWidget *parent = nullptr);
    ~Pre();

private:
    Ui::Pre *ui;
    QTcpSocket *TCP_socket;

private slots:
    void readSocket();
    void discardSocket();

    void Send_File(QTcpSocket *socket, QString filename);

    void on_pushButton_submit_clicked();
};

#endif // PRE_H
