#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    // 1. Enlever la barre de titre Windows (Design moderne)
    setWindowFlags(Qt::FramelessWindowHint);


    // 3. Forcer l'affichage de la page de Connexion au démarrage (Index 0)
    ui->stackedWidget->setCurrentIndex(0);
}

Login::~Login()
{
    delete ui;
}

// --- Action : Se Connecter ---
void Login::on_btn_login_clicked()
{
    QString username = ui->le_username->text();
    QString password = ui->le_password->text();

    if (username == "HR" && password == "hr123") {
        this->accept();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}
