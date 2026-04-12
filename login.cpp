#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include "smtp.h"
#include <QRandomGenerator>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    // 1. Enlever la barre de titre Windows (Design moderne)
    setWindowFlags(Qt::FramelessWindowHint);


    // 3. Forcer l'affichage de la page de Connexion au démarrage (Index 0)
    ui->stackedWidget->setCurrentIndex(0);

    // Style the forgot password button to be transparent with gray text
    ui->btn_forgot->setStyleSheet("QPushButton { background-color: transparent; border: none; color: gray; }");
    ui->btn_forgot->setAutoDefault(false); // Prevents it from intercepting "Enter" key presses
    ui->btn_login->setDefault(true);       // Ensures "Enter" triggers the actual login button
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

    // ✅ CORRECTION : On utilise les variables (hr_username, hr_password, etc.)
    if (username == hr_username && password == hr_password) {
        this->accept();
    }
    else if(username == admin_username && password == admin_password) {
        this->accept();
    }
    else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}

void Login::on_btn_forgot_clicked()
{
    // Take exactly to page_2 of the stacked widget
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void Login::on_btn_reset_clicked()
{
    QString email = ui->le_reset_email->text().trimmed();

    if (email.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer votre adresse e-mail.");
        return;
    }

    // 1. Vérifier si l'e-mail saisi correspond à ton Admin
    if (email != admin_username) { // Ici, admin_username = "rej.rayan@gmail.com"
        QMessageBox::critical(this, "Erreur", "Cet utilisateur n'existe pas dans le système.");
        return;
    }

    // 2. Générer un nouveau mot de passe temporaire à 6 chiffres
    QString nouveauMdp = QString::number(QRandomGenerator::global()->bounded(100000, 999999));

    // 3. Préparer l'e-mail avec un joli rendu HTML
    QString sujet = "Réinitialisation de votre mot de passe - EcoCycle";
    QString corps = "<html><body>"
                    "<h2 style='color:#2E7D32;'>EcoCycle - Sécurité</h2>"
                    "<p>Bonjour,</p>"
                    "<p>Vous avez demandé la réinitialisation de votre mot de passe.</p>"
                    "<p>Voici votre mot de passe temporaire pour vous connecter :</p>"
                    "<h3 style='background-color:#f0f0f0; padding:10px; display:inline-block; border-radius:5px;'>" 
                    + nouveauMdp + "</h3>"
                    "<p>Nous vous conseillons de le conserver en lieu sûr.</p>"
                    "<p>Cordialement,<br><b>L'équipe EcoCycle</b></p>"
                    "</body></html>";

    // 4. Envoyer l'e-mail via notre classe SMTP
    QMessageBox::information(this, "Patientez", "Envoi de l'e-mail en cours..."); // Optionnel: pour faire patienter l'utilisateur
    
    if (Smtp::envoyerMail(email, sujet, corps)) {
        
        // 5. Si l'envoi a réussi, on met à jour la variable dans le code
        admin_password = nouveauMdp; 

        QMessageBox::information(this, "Succès", "Un nouveau mot de passe a été envoyé à votre adresse e-mail !");
        
        ui->le_reset_email->clear();
        ui->stackedWidget->setCurrentIndex(0); // Retour à la page de connexion
    } else {
        QMessageBox::critical(this, "Erreur Réseau", "Impossible d'envoyer l'e-mail.\nVérifiez votre connexion internet.");
    }
}

void Login::on_btn_back_clicked()
{
    // Nettoyer les champs pour faire propre
    ui->le_reset_email->clear();
    // Retourner à la page de connexion
    ui->stackedWidget->setCurrentIndex(0);
}