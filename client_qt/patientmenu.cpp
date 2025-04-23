#include "patientmenu.h"
#include "ui_patientmenu.h"

class chatwindow;

int a=0;
PatientMenu::PatientMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientMenu)
{
    ui->setupUi(this);
    a=0;
}

PatientMenu::~PatientMenu()
{
    delete ui;
}

void PatientMenu::on_pushButton_7_clicked()
{

}

void PatientMenu::on_pushButton_4_clicked()
{
    PatientInfo *page = new PatientInfo;
    page->show();
}
