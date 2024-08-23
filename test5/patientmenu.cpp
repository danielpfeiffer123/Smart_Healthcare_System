#include "patientmenu.h"
#include "ui_patientmenu.h"

PatientMenu::PatientMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientMenu)
{
    ui->setupUi(this);
}

PatientMenu::~PatientMenu()
{
    delete ui;
}
