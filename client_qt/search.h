#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include <QtNetwork>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include "common.h"

namespace Ui {
class Search;
}

class Search : public QDialog
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();

private:
    Ui::Search *ui;
    QTcpSocket *TCP_socket;


private slots:
    void readSocket();
    void discardSocket();

    void Send_File(QTcpSocket *TCP_socket, QString filename);

    void on_pushButton_send_clicked();
};
#endif // SEARCH_H
