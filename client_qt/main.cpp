#include "widget.h"
#include "common.h"
#include <QApplication>
#include "patientmenu.h"
#include "appointment.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);//防止文本框过小

    QApplication a(argc, argv);
    Widget w;
    if (w.getLoginStatus() == false)
        return 0;
    w.show();
    //Widget::w.show();


    return a.exec();
}
