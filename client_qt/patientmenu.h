#ifndef PATIENTMENU_H
#define PATIENTMENU_H

#include <QDialog>
#include "patientinfo.h"
#include <QWidget>
#include <QMessageBox>
#include "common.h"
#include "logindlg.h"
#include "addfrienddialog.h"
#include "custom/chatmessage.h"
#include "patientinfo.h"
#include <map>
#include <list>
#include <QListWidget>
#include <QTimer>
#include "widget.h"
#include <QObject>
#include <QString>
#include <QDebug>





class Sender : public QObject {
    Q_OBJECT

public:
    explicit Sender(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void sendMessage(const QString &message);  // 定义信号
};


namespace Ui {
class PatientMenu;
}

class PatientMenu : public QDialog
{
    Q_OBJECT

public:
    explicit PatientMenu(QWidget *parent = nullptr);
    ~PatientMenu();

private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::PatientMenu *ui;
    QTcpSocket *socket; //socket通信
    bool        socketState;
    Session*    m_session;

private:
    void Init_menu();
    int  login();
    UserInfo    p_userInfo;
    bool        p_isLogin;
    int* status;

public:
    UserInfo* getUserInfo_p(){
        return &p_userInfo;
    };

    int* GetStatus(){
        return status;
    }

};

#endif // PATIENTMENU_H
