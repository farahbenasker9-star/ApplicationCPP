/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QFrame *frame_left;
    QLabel *label;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QFrame *frame_right;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *le_username;
    QLineEdit *le_password;
    QPushButton *btn_forgot;
    QPushButton *btn_login;
    QSpacerItem *verticalSpacer;
    QLabel *label_2;
    QWidget *page_2;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(1300, 800);
        Login->setStyleSheet(QString::fromUtf8("/* --- 1. LE FOND G\303\211N\303\211RAL --- */\n"
"QDialog {\n"
"    /* V\303\251rifie que ce chemin est bon dans tes ressources ! */\n"
"    border-image: url(:/new/prefix1/images/bg_left.jpg) 0 0 0 0 stretch stretch;\n"
"}\n"
"\n"
"/* --- 2. LE PANNEAU GAUCHE (Vert Fonc\303\251 Transparent) --- */\n"
"QFrame#frame_left {\n"
"    background-color: rgba(20, 60, 30, 210); \n"
"    border-top-left-radius: 15px;\n"
"    border-bottom-left-radius: 15px;\n"
"}\n"
"\n"
"/* Texte \303\240 gauche en BLANC */\n"
"QFrame#frame_left QLabel {\n"
"    color: #ffffff;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* Le titre EcoCycle Staff */\n"
"QFrame#frame_left QLabel:first-child {\n"
"    font-size: 26px;\n"
"    font-weight: bold;\n"
"    color: #4CAF50;\n"
"}\n"
"\n"
"/* --- 3. LE PANNEAU DROIT (Blanc Transparent) --- */\n"
"QFrame#frame_right {\n"
"  pour voir l'image */\n"
"    border-top-right-radius: 15px;\n"
"    border-bottom-right-radius: 15px;\n"
"}\n"
"\n"
"/* Texte \303"
                        "\240 droite */\n"
"QFrame#frame_right QLabel {\n"
"    color: #1B5E20;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 22px; \n"
"    font-weight: bold; \n"
"    qproperty-alignment: AlignCenter;\n"
"}\n"
"\n"
"/* --- 4. CHAMPS DE SAISIE --- */\n"
"QLineEdit {\n"
"    background-color: rgba(255, 255, 255, 180);\n"
"    border: 1px solid #cccccc;\n"
"    border-radius: 5px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"    color: #333;\n"
"    margin-bottom: 10px; /* Un peu d'espace entre les champs */\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    background-color: #ffffff;\n"
"    border: 2px solid #4CAF50;\n"
"}\n"
"\n"
"/* --- 5. BOUTON SIGN IN --- */\n"
"QPushButton#btn_login {\n"
"    background-color: #2E7D32; \n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 12px;\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"    margin-top: 10px;\n"
"}\n"
"\n"
"QPushButton#btn_login:hover {\n"
"    background-color: #1B5E20;\n"
"    cursor: pointer;\n"
""
                        "}\n"
"\n"
"QPushButton#btn_login:pressed {\n"
"    background-color: #003300;\n"
"}"));
        frame_left = new QFrame(Login);
        frame_left->setObjectName("frame_left");
        frame_left->setGeometry(QRect(1, 1, 451, 811));
        frame_left->setStyleSheet(QString::fromUtf8("background-color: rgb(180, 219, 199);"));
        frame_left->setFrameShape(QFrame::Shape::StyledPanel);
        frame_left->setFrameShadow(QFrame::Shadow::Raised);
        label = new QLabel(frame_left);
        label->setObjectName("label");
        label->setGeometry(QRect(130, 200, 221, 251));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/logo-removebg-preview (1).png")));
        stackedWidget = new QStackedWidget(Login);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(460, 10, 841, 701));
        stackedWidget->setStyleSheet(QString::fromUtf8("\n"
"\n"
"/* --- BOUTON \"MOT DE PASSE OUBLI\303\211\" (Style Lien) --- */\n"
"QPushButton#btn_forgot_nav {\n"
"    border: none;\n"
"    color: #2E7D32; /* Vert */\n"
"    text-decoration: underline;\n"
"    font-size: 12px;\n"
"}\n"
"QPushButton#btn_forgot_nav:hover {\n"
"    color: #1B5E20; /* Plus fonc\303\251 au survol */\n"
"}\n"
"\n"
"/* --- BOUTON RETOUR (Page Recovery) --- */\n"
"QPushButton#btn_back_login {\n"
"    border: 1px solid #aaa;\n"
"    color: #555;\n"
"    margin-top: 10px;\n"
"}\n"
"QPushButton#btn_back_login:hover {\n"
"    background-color: #eee;\n"
"}"));
        page = new QWidget();
        page->setObjectName("page");
        frame_right = new QFrame(page);
        frame_right->setObjectName("frame_right");
        frame_right->setGeometry(QRect(0, 0, 821, 701));
        frame_right->setStyleSheet(QString::fromUtf8(""));
        frame_right->setFrameShape(QFrame::Shape::StyledPanel);
        frame_right->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayoutWidget = new QWidget(frame_right);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(60, 300, 711, 351));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        le_username = new QLineEdit(verticalLayoutWidget);
        le_username->setObjectName("le_username");

        verticalLayout->addWidget(le_username);

        le_password = new QLineEdit(verticalLayoutWidget);
        le_password->setObjectName("le_password");
        le_password->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout->addWidget(le_password);

        btn_forgot = new QPushButton(verticalLayoutWidget);
        btn_forgot->setObjectName("btn_forgot");

        verticalLayout->addWidget(btn_forgot);

        btn_login = new QPushButton(verticalLayoutWidget);
        btn_login->setObjectName("btn_login");

        verticalLayout->addWidget(btn_login);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label_2 = new QLabel(frame_right);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(250, 150, 321, 71));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stackedWidget->addWidget(page_2);

        retranslateUi(Login);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Dialog", nullptr));
        label->setText(QString());
        le_username->setPlaceholderText(QCoreApplication::translate("Login", "Username", nullptr));
        le_password->setPlaceholderText(QCoreApplication::translate("Login", "Password", nullptr));
        btn_forgot->setText(QCoreApplication::translate("Login", "                                                                                                                                                 Reset Password", nullptr));
        btn_login->setText(QCoreApplication::translate("Login", "Sign in", nullptr));
        label_2->setText(QCoreApplication::translate("Login", "<html><head/><body><p><span style=\" font-size:24pt; color:#ffffff;\">Welcome Back</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
