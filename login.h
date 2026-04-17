#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_btn_login_clicked();
    void on_btn_forgot_clicked();
    void on_btn_reset_clicked();
    void on_btn_back_clicked();
private:
    Ui::Login *ui;
    QString hr_username = "HR";
    QString hr_password = "hr123";

    QString admin_username = "rej.rayan@gmail.com";
    QString admin_password = "admin123";


};

#endif // LOGIN_H
