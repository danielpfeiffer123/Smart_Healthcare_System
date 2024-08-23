#ifndef DOCTORMENU_H
#define DOCTORMENU_H

#include <QDialog>

namespace Ui {
class DoctorMenu;
}

class DoctorMenu : public QDialog
{
    Q_OBJECT

public:
    explicit DoctorMenu(QWidget *parent = nullptr);
    ~DoctorMenu();

private:
    Ui::DoctorMenu *ui;
};

#endif // DOCTORMENU_H
