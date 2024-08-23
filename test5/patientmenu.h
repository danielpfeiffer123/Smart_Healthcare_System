#ifndef PATIENTMENU_H
#define PATIENTMENU_H

#include <QDialog>

namespace Ui {
class PatientMenu;
}

class PatientMenu : public QDialog
{
    Q_OBJECT

public:
    explicit PatientMenu(QWidget *parent = nullptr);
    ~PatientMenu();

private:
    Ui::PatientMenu *ui;
};

#endif // PATIENTMENU_H
