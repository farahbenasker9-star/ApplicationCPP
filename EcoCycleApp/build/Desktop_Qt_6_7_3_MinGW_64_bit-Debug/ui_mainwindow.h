/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_4;
    QFrame *navbar_container;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_9;
    QPushButton *btn_nav_employes;
    QPushButton *btn_nav_client;
    QPushButton *btn_nav_contrat;
    QPushButton *btn_nav_poubelle;
    QPushButton *btn_nav_equipements;
    QPushButton *btn_nav_stock;
    QSpacerItem *verticalSpacer_5;
    QPushButton *btn_quit;
    QWidget *widget_2;
    QStackedWidget *stackedWidget;
    QWidget *page_dashboard;
    QLabel *label_logo_dash;
    QLabel *label_welcome;
    QLabel *label_user_icon;
    QPushButton *btn_dash_client;
    QPushButton *btn_dash_employe;
    QPushButton *btn_dash_equipement;
    QPushButton *btn_dash_contrat;
    QPushButton *btn_dash_poubelle;
    QPushButton *btn_dash_stock;
    QWidget *page_choix;
    QWidget *page_contrat;
    QLabel *lbl_page_title;
    QComboBox *cb_tri_option;
    QPushButton *btn_tri_2;
    QTableView *tab_contrat_2;
    QLineEdit *le_recherche_4;
    QTabWidget *tabWidget_right_contrat;
    QWidget *tab_stats_client_2;
    QGroupBox *gb_stats_contrat;
    QWidget *tab_2;
    QLabel *lbl_info_contrat_title;
    QGroupBox *group_informations;
    QFormLayout *formLayout_contrat;
    QLabel *label_id;
    QLineEdit *le_ID_Contrat;
    QLabel *label_type;
    QComboBox *cb_Type_Contrat;
    QLabel *label_matiere;
    QComboBox *cb_Type_Matiere;
    QLabel *label_qualite;
    QComboBox *cb_Qualite_Matiere;
    QLabel *label_date_deb;
    QDateEdit *date_debut;
    QLabel *label_date_fin;
    QDateEdit *date_fin;
    QLabel *label_duree;
    QDoubleSpinBox *dspin_prix_unitaire_2;
    QLabel *label_prix;
    QDoubleSpinBox *dspin_prix_unitaire;
    QLabel *label_statut;
    QComboBox *cb_statut_contrat;
    QLabel *label_details;
    QTextEdit *te_details;
    QFrame *frame_recherche;
    QHBoxLayout *horizontalLayout_recherche;
    QPushButton *btn_export_pdf;
    QPushButton *btn_modifier_3;
    QPushButton *btn_ajouter_3;
    QPushButton *btn_supprimer_4;
    QWidget *page_equipement;
    QWidget *headerWidget;
    QHBoxLayout *headerLayout;
    QLabel *headerLabel;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_9;
    QGroupBox *infoGroupBox_5;
    QVBoxLayout *infoGroupBoxLayout;
    QScrollArea *infoScrollArea;
    QWidget *infoScrollContent;
    QVBoxLayout *verticalLayout_48;
    QSpacerItem *verticalSpacer_35;
    QLabel *labelNomEquipement;
    QLineEdit *lineEditNomEquipement;
    QLabel *labelTypeEquipement;
    QComboBox *comboBoxTypeEquipement;
    QLabel *labelFabricant_7;
    QComboBox *comboBoxFabricant_7;
    QSpacerItem *verticalSpacer_32;
    QPushButton *pushButtonAjouter;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_8;
    QLineEdit *le_recherche_2;
    QComboBox *comboBox_2;
    QPushButton *btn_tri_3;
    QTableView *tab_employes_2;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *btn_ajouter_2;
    QPushButton *btn_modifier_2;
    QPushButton *btn_supprimer_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_pdf_2;
    QPushButton *btn_mailing_2;
    QTabWidget *tabWidget_right_contrat_2;
    QWidget *tab_stats_client_4;
    QGroupBox *gb_stats_contrat_3;
    QWidget *tab_4;
    QWidget *page_stock;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_31;
    QLineEdit *le_id;
    QLabel *label_34;
    QComboBox *cb_type;
    QLabel *label_33;
    QDoubleSpinBox *sb_poids;
    QLabel *label_35;
    QDoubleSpinBox *sb_prix;
    QLabel *label_30;
    QDateEdit *de_date;
    QLabel *label_27;
    QComboBox *cb_statut;
    QLabel *label_29;
    QComboBox *cb_etat;
    QTabWidget *tabWidget_Stock;
    QWidget *tab_stats_3;
    QWidget *tab_historique;
    QWidget *tab_finance;
    QTableView *tab_stock;
    QGroupBox *groupBox_3;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QComboBox *cb_chercher;
    QPushButton *btn_chercher;
    QLabel *label_3;
    QComboBox *cb_trier;
    QPushButton *btn_trier_2;
    QLabel *label_75;
    QPushButton *btn_ajouter_4;
    QPushButton *btn_modifier_4;
    QPushButton *btn_supprimer_5;
    QPushButton *btn_pdf_3;
    QWidget *page_client;
    QGroupBox *gb_form_client;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_8_client;
    QLineEdit *le_id_client;
    QLabel *label_10_client;
    QLineEdit *le_nom_client;
    QLabel *label_11_client;
    QLineEdit *le_adresse_client;
    QLabel *label_12_client;
    QComboBox *cb_ville_client;
    QLabel *label_13_client;
    QDoubleSpinBox *dsb_codepostal_client;
    QLabel *label_14_client;
    QDateEdit *de_inscription_client;
    QLabel *label_15_client;
    QLineEdit *le_responsable_client;
    QTabWidget *tabWidget_right_client;
    QWidget *tab_stats_client;
    QGroupBox *gb_stats_client;
    QWidget *tab_contrat_client;
    QTableView *tab_clients_4;
    QTableWidget *tableWidget_5;
    QTableView *tab_clients;
    QTableWidget *tableWidget_3;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_client;
    QPushButton *btn_ajouter_client;
    QPushButton *btn_modifier_client;
    QPushButton *btn_supprimer_client;
    QSpacerItem *horizontalSpacer_client;
    QPushButton *btn_pdf_client;
    QPushButton *btn_mailing_client;
    QLabel *label_4_client;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_7_client;
    QLineEdit *le_recherche_client;
    QComboBox *comboBox_client;
    QPushButton *btn_tri_client;
    QWidget *page_employes;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *gb_form;
    QFormLayout *formLayout;
    QLabel *label_8;
    QLineEdit *le_cin;
    QLabel *label_10;
    QLineEdit *le_nom;
    QLabel *label_11;
    QLineEdit *le_prenom;
    QLabel *label_12;
    QComboBox *cb_poste;
    QLabel *label_13;
    QDoubleSpinBox *dsb_salaire;
    QLabel *label_14;
    QDateEdit *de_embauche;
    QLabel *label_15;
    QLineEdit *le_badge;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_7;
    QLineEdit *le_recherche;
    QComboBox *comboBox;
    QPushButton *btn_tri;
    QTableView *tab_employes;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_ajouter;
    QPushButton *btn_modifier;
    QPushButton *btn_supprimer;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_pdf;
    QPushButton *btn_contrat;
    QPushButton *btn_mailing;
    QTabWidget *tabWidget_right;
    QWidget *tab_stats;
    QGroupBox *gb_stats;
    QWidget *tab_contrat;
    QWidget *page_poubelle;
    QGroupBox *gb_form_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_24;
    QLineEdit *le_cin_2;
    QLabel *label_25;
    QLineEdit *le_nom_2;
    QLabel *label_26;
    QLineEdit *le_prenom_2;
    QLabel *label_28;
    QDoubleSpinBox *dsb_salaire_2;
    QLabel *lbl_page_title_3;
    QTableWidget *tableWidget_2;
    QWidget *layoutWidget_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboBox_client_3;
    QPushButton *btn_tri_client_3;
    QTabWidget *tabWidget_right_2;
    QWidget *tab_stats_5;
    QGroupBox *gb_stats_3;
    QWidget *tab_contrat_3;
    QTableView *tab_poubelle_2;
    QWidget *tab;
    QTableView *tab_poubelle_3;
    QTableView *tab_poubelle;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_7_client_2;
    QLineEdit *le_recherche_client_2;
    QComboBox *comboBox_client_2;
    QPushButton *btn_tri_client_2;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_client_2;
    QPushButton *btn_ajouter_client_2;
    QPushButton *btn_modifier_client_2;
    QPushButton *btn_supprimer_client_2;
    QSpacerItem *horizontalSpacer_client_2;
    QPushButton *btn_pdf_client_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1587, 795);
        MainWindow->setStyleSheet(QString::fromUtf8("/* --- G\303\211N\303\211RAL --- */\n"
"QMainWindow {\n"
"    background-color: white; /* Fond g\303\251n\303\251ral */\n"
"}\n"
"\n"
"QLabel {\n"
"    font-family: \"Segoe UI\", \"Verdana\", sans-serif;\n"
"}\n"
"\n"
"/* --- PANNEAU GAUCHE (VERT) --- */\n"
"#frame_left {\n"
"    /* Un d\303\251grad\303\251 subtil pour faire moins \"Paint\" */\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #1b5e20, stop:1 #2e7d32);\n"
"    border-top-left-radius: 15px;\n"
"    border-bottom-left-radius: 15px;\n"
"}\n"
"\n"
"#frame_left QLabel {\n"
"    color: white;\n"
"}\n"
"\n"
"/* --- PANNEAU DROIT (BLANC) --- */\n"
"#frame_right {\n"
"    background-color: white;\n"
"    border-top-right-radius: 15px;\n"
"    border-bottom-right-radius: 15px;\n"
"}\n"
"\n"
"/* --- CHAMPS DE TEXTE (MODERNES) --- */\n"
"QLineEdit {\n"
"    background-color: #F5F7F9; /* Gris tr\303\250s tr\303\250s clair */\n"
"    border: 1px solid #E0E0E0; /* Bordure fine grise */\n"
"    border-radius: 8px;        /* Coins a"
                        "rrondis */\n"
"    padding-left: 15px;        /* Espace entre le bord gauche et le texte */\n"
"    font-size: 14px;\n"
"    color: #333;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    background-color: white;\n"
"    border: 2px solid #1b5e20; /* Bordure verte quand on \303\251crit */\n"
"}\n"
"\n"
"/* --- BOUTON (SIGN IN) --- */\n"
"QPushButton#btn_login {\n"
"    background-color: #1b5e20;\n"
"    color: white;\n"
"    border-radius: 8px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#btn_login:hover {\n"
"    background-color: #2e7d32; /* Un peu plus clair au survol */\n"
"    cursor: pointer;\n"
"}\n"
"\n"
"QPushButton#btn_login:pressed {\n"
"    background-color: #003300;\n"
"}\n"
"\n"
"/* --- TITRES --- */\n"
"/* Pour cibler sp\303\251cifiquement le titre \"Welcome Back\" si tu l'as nomm\303\251 lbl_titre */\n"
"#lbl_titre {\n"
"    color: #2c3e50;\n"
"    font-weight: bold;\n"
"    font-size: 26px;\n"
"}\n"
"\n"
"/* --- CHECKBOX --- */\n"
"QCheckBox {\n"
""
                        "    color: #666;\n"
"    font-size: 13px;\n"
"}\n"
"QCheckBox::indicator {\n"
"    width: 18px;\n"
"    height: 18px;\n"
"    border-radius: 3px;\n"
"    border: 1px solid #ccc;\n"
"}\n"
"QCheckBox::indicator:checked {\n"
"    background-color: #1b5e20;\n"
"}\n"
"/* --- NAVBAR (Cadre Gauche) --- */\n"
"QFrame#navbar_container {\n"
"    /* Blanc transparent pour voir le fond global derri\303\250re */\n"
"    background-color: rgba(255, 255, 255, 220);\n"
"    border-right: 1px solid rgba(0,0,0, 20);\n"
"    min-width: 200px; /* Largeur fixe */\n"
"    max-width: 220px;\n"
"}\n"
"\n"
"/* --- BOUTONS DE NAVIGATION --- */\n"
"QFrame#navbar_container QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    text-align: left; /* Texte \303\240 gauche */\n"
"    padding: 15px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    color: #555555;\n"
"    border-left: 5px solid transparent; /* Barre verte invisible par d\303\251faut */\n"
"}\n"
"\n"
"/* Survol des boutons */\n"
"QFra"
                        "me#navbar_container QPushButton:hover, QFrame#navbar_container QPushButton:checked {\n"
"    background-color: rgba(46, 125, 50, 30); /* Vert tr\303\250s p\303\242le */\n"
"    color: #2E7D32; /* Texte Vert */\n"
"    border-left: 5px solid #2E7D32; /* La barre verte appara\303\256t */\n"
"}\n"
"\n"
"/* BOUTON D\303\211CONNEXION (Sp\303\251cial) */\n"
"QPushButton#btn_logout {\n"
"    text-align: center;\n"
"    background-color: rgba(211, 47, 47, 30);\n"
"    color: #D32F2F;\n"
"    border-radius: 5px;\n"
"    margin: 20px;\n"
"    padding: 10px;\n"
"}\n"
"QPushButton#btn_logout:hover {\n"
"    background-color: #D32F2F;\n"
"    color: white;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("/* Applique l'image sur le fond global de la fen\303\252tre */\n"
"QWidget#centralwidget {\n"
"    border-image: url(:/new/prefix1/images/bg_left.jpg) 0 0 0 0 stretch stretch;\n"
"}"));
        horizontalLayout_4 = new QHBoxLayout(centralwidget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        navbar_container = new QFrame(centralwidget);
        navbar_container->setObjectName("navbar_container");
        navbar_container->setMinimumSize(QSize(202, 0));
        navbar_container->setMaximumSize(QSize(222, 16777215));
        navbar_container->setStyleSheet(QString::fromUtf8("background-color: rgb(180, 219, 199);"));
        navbar_container->setFrameShape(QFrame::Shape::StyledPanel);
        navbar_container->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_4 = new QVBoxLayout(navbar_container);
        verticalLayout_4->setSpacing(10);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(0, 5, 0, 20);
        label_9 = new QLabel(navbar_container);
        label_9->setObjectName("label_9");
        label_9->setPixmap(QPixmap(QString::fromUtf8(":/images/logo-removebg-preview (1) (1).png")));

        verticalLayout_4->addWidget(label_9, 0, Qt::AlignmentFlag::AlignHCenter);

        btn_nav_employes = new QPushButton(navbar_container);
        btn_nav_employes->setObjectName("btn_nav_employes");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/employer.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_nav_employes->setIcon(icon);
        btn_nav_employes->setIconSize(QSize(24, 24));
        btn_nav_employes->setCheckable(true);
        btn_nav_employes->setAutoExclusive(true);

        verticalLayout_4->addWidget(btn_nav_employes);

        btn_nav_client = new QPushButton(navbar_container);
        btn_nav_client->setObjectName("btn_nav_client");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/client0.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_nav_client->setIcon(icon1);
        btn_nav_client->setIconSize(QSize(24, 24));
        btn_nav_client->setCheckable(true);
        btn_nav_client->setAutoExclusive(true);

        verticalLayout_4->addWidget(btn_nav_client);

        btn_nav_contrat = new QPushButton(navbar_container);
        btn_nav_contrat->setObjectName("btn_nav_contrat");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/contrat0.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_nav_contrat->setIcon(icon2);
        btn_nav_contrat->setIconSize(QSize(24, 24));
        btn_nav_contrat->setCheckable(true);
        btn_nav_contrat->setAutoExclusive(true);

        verticalLayout_4->addWidget(btn_nav_contrat);

        btn_nav_poubelle = new QPushButton(navbar_container);
        btn_nav_poubelle->setObjectName("btn_nav_poubelle");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/pubelle0.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_nav_poubelle->setIcon(icon3);
        btn_nav_poubelle->setIconSize(QSize(24, 24));
        btn_nav_poubelle->setCheckable(true);
        btn_nav_poubelle->setAutoExclusive(true);

        verticalLayout_4->addWidget(btn_nav_poubelle);

        btn_nav_equipements = new QPushButton(navbar_container);
        btn_nav_equipements->setObjectName("btn_nav_equipements");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/equipement0.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_nav_equipements->setIcon(icon4);
        btn_nav_equipements->setIconSize(QSize(24, 24));
        btn_nav_equipements->setCheckable(true);
        btn_nav_equipements->setAutoExclusive(true);

        verticalLayout_4->addWidget(btn_nav_equipements);

        btn_nav_stock = new QPushButton(navbar_container);
        btn_nav_stock->setObjectName("btn_nav_stock");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/stock0.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_nav_stock->setIcon(icon5);
        btn_nav_stock->setIconSize(QSize(24, 24));
        btn_nav_stock->setCheckable(true);
        btn_nav_stock->setAutoExclusive(true);

        verticalLayout_4->addWidget(btn_nav_stock);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_5);

        btn_quit = new QPushButton(navbar_container);
        btn_quit->setObjectName("btn_quit");
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/porte.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_quit->setIcon(icon6);
        btn_quit->setIconSize(QSize(24, 24));

        verticalLayout_4->addWidget(btn_quit);


        horizontalLayout_4->addWidget(navbar_container);

        widget_2 = new QWidget(centralwidget);
        widget_2->setObjectName("widget_2");
        stackedWidget = new QStackedWidget(widget_2);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 0, 1341, 731));
        page_dashboard = new QWidget();
        page_dashboard->setObjectName("page_dashboard");
        label_logo_dash = new QLabel(page_dashboard);
        label_logo_dash->setObjectName("label_logo_dash");
        label_logo_dash->setGeometry(QRect(30, 10, 200, 100));
        label_logo_dash->setMinimumSize(QSize(200, 100));
        label_logo_dash->setMaximumSize(QSize(200, 100));
        label_logo_dash->setPixmap(QPixmap(QString::fromUtf8(":/images/logo2.png")));
        label_logo_dash->setScaledContents(true);
        label_welcome = new QLabel(page_dashboard);
        label_welcome->setObjectName("label_welcome");
        label_welcome->setGeometry(QRect(1120, 40, 135, 24));
        label_welcome->setStyleSheet(QString::fromUtf8("font-size: 18px; color: white;"));
        label_user_icon = new QLabel(page_dashboard);
        label_user_icon->setObjectName("label_user_icon");
        label_user_icon->setGeometry(QRect(1260, 20, 60, 60));
        label_user_icon->setMinimumSize(QSize(60, 60));
        label_user_icon->setMaximumSize(QSize(60, 60));
        label_user_icon->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_profile.png")));
        label_user_icon->setScaledContents(true);
        label_user_icon->setAlignment(Qt::AlignmentFlag::AlignCenter);
        btn_dash_client = new QPushButton(page_dashboard);
        btn_dash_client->setObjectName("btn_dash_client");
        btn_dash_client->setGeometry(QRect(530, 130, 380, 240));
        btn_dash_client->setMinimumSize(QSize(380, 240));
        btn_dash_client->setMaximumSize(QSize(380, 240));
        btn_dash_client->setStyleSheet(QString::fromUtf8("QPushButton { border: none; background-color: transparent; }\n"
"QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    border-bottom: 3px solid transparent;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border-bottom: 3px solid rgb(0, 255, 127);\n"
"    background-color: rgba(0, 255, 127, 20);\n"
"}"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/btn2.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_dash_client->setIcon(icon7);
        btn_dash_client->setIconSize(QSize(380, 240));
        btn_dash_employe = new QPushButton(page_dashboard);
        btn_dash_employe->setObjectName("btn_dash_employe");
        btn_dash_employe->setGeometry(QRect(170, 130, 380, 240));
        btn_dash_employe->setMinimumSize(QSize(380, 240));
        btn_dash_employe->setMaximumSize(QSize(380, 240));
        btn_dash_employe->setStyleSheet(QString::fromUtf8("QPushButton { border: none; background-color: transparent; }\n"
"QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    border-bottom: 3px solid transparent;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border-bottom: 3px solid rgb(0, 255, 127);\n"
"    background-color: rgba(0, 255, 127, 20);\n"
"}"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/btn1.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_dash_employe->setIcon(icon8);
        btn_dash_employe->setIconSize(QSize(380, 240));
        btn_dash_equipement = new QPushButton(page_dashboard);
        btn_dash_equipement->setObjectName("btn_dash_equipement");
        btn_dash_equipement->setGeometry(QRect(540, 400, 380, 240));
        btn_dash_equipement->setMinimumSize(QSize(380, 240));
        btn_dash_equipement->setMaximumSize(QSize(380, 240));
        btn_dash_equipement->setStyleSheet(QString::fromUtf8("QPushButton { border: none; background-color: transparent; }\n"
"QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    border-bottom: 3px solid transparent;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border-bottom: 3px solid rgb(0, 255, 127);\n"
"    background-color: rgba(0, 255, 127, 20);\n"
"}"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/btn5.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_dash_equipement->setIcon(icon9);
        btn_dash_equipement->setIconSize(QSize(380, 240));
        btn_dash_contrat = new QPushButton(page_dashboard);
        btn_dash_contrat->setObjectName("btn_dash_contrat");
        btn_dash_contrat->setGeometry(QRect(880, 130, 380, 240));
        btn_dash_contrat->setMinimumSize(QSize(380, 240));
        btn_dash_contrat->setMaximumSize(QSize(380, 240));
        btn_dash_contrat->setStyleSheet(QString::fromUtf8("QPushButton { border: none; background-color: transparent; }\n"
"QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    border-bottom: 3px solid transparent;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border-bottom: 3px solid rgb(0, 255, 127);\n"
"    background-color: rgba(0, 255, 127, 20);\n"
"}"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/btn3.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_dash_contrat->setIcon(icon10);
        btn_dash_contrat->setIconSize(QSize(380, 240));
        btn_dash_poubelle = new QPushButton(page_dashboard);
        btn_dash_poubelle->setObjectName("btn_dash_poubelle");
        btn_dash_poubelle->setGeometry(QRect(180, 400, 380, 240));
        btn_dash_poubelle->setMinimumSize(QSize(380, 240));
        btn_dash_poubelle->setMaximumSize(QSize(380, 240));
        btn_dash_poubelle->setStyleSheet(QString::fromUtf8("QPushButton { border: none; background-color: transparent; }\n"
"QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    border-bottom: 3px solid transparent;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border-bottom: 3px solid rgb(0, 255, 127);\n"
"    background-color: rgba(0, 255, 127, 20);\n"
"}"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/images/btn4.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_dash_poubelle->setIcon(icon11);
        btn_dash_poubelle->setIconSize(QSize(380, 240));
        btn_dash_stock = new QPushButton(page_dashboard);
        btn_dash_stock->setObjectName("btn_dash_stock");
        btn_dash_stock->setGeometry(QRect(880, 400, 380, 240));
        btn_dash_stock->setMinimumSize(QSize(380, 240));
        btn_dash_stock->setMaximumSize(QSize(380, 240));
        btn_dash_stock->setStyleSheet(QString::fromUtf8("QPushButton { border: none; background-color: transparent; }\n"
"QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    border-bottom: 3px solid transparent;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border-bottom: 3px solid rgb(0, 255, 127);\n"
"    background-color: rgba(0, 255, 127, 20);\n"
"}"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/images/btn6.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_dash_stock->setIcon(icon12);
        btn_dash_stock->setIconSize(QSize(380, 240));
        stackedWidget->addWidget(page_dashboard);
        page_choix = new QWidget();
        page_choix->setObjectName("page_choix");
        stackedWidget->addWidget(page_choix);
        page_contrat = new QWidget();
        page_contrat->setObjectName("page_contrat");
        page_contrat->setStyleSheet(QString::fromUtf8("QWidget#page_contrat {\n"
"    /* Assure-toi que le chemin de ton image est correct ici ! */\n"
"    border-image: url(:/new/prefix1/images/bg_left.jpg) 0 0 0 0 stretch stretch ;\n"
"}\n"
"\n"
"/* On rend la zone de contenu INVISIBLE pour voir l'image derri\303\250re */\n"
"#stackedWidget {\n"
"    background: transparent;\n"
"}\n"
"#page_contrat {\n"
"    background: transparent;\n"
"}\n"
"\n"
"/* =======================================================\n"
"   2. LA BARRE DE NAVIGATION (GAUCHE)\n"
"   ======================================================= */\n"
"\n"
"/* On donne une couleur \"Verre blanc\" \303\240 ton menu pour qu'il soit assorti */\n"
"#frame_navbar {\n"
"    background-color: rgba(255, 255, 255, 210); /* Blanc semi-transparent */\n"
"    border-right: 2px solid #2E7D32; /* Petite ligne verte \303\240 droite pour s\303\251parer */\n"
"}\n"
"\n"
"/* Style des boutons du menu (Navigation) */\n"
"#frame_navbar QPushButton {\n"
"    background-color: transparent;\n"
"    color: #333;\n"
"    te"
                        "xt-align: left;\n"
"    padding-left: 20px;\n"
"    border: none;\n"
"    font-weight: bold;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"#frame_navbar QPushButton:hover {\n"
"    background-color: #2E7D32; /* Devient vert au survol */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"/* =======================================================\n"
"   3. ADAPTATION DU STYLE DE TON CO\303\211QUIPIER (FORMULAIRES)\n"
"   ======================================================= */\n"
"\n"
"/* Le fond des zones de texte (Inputs) */\n"
"QLineEdit, QComboBox, QDateEdit, QDoubleSpinBox {\n"
"    background-color: rgba(245, 245, 245, 200); \n"
"    border: 1px solid #BDBDBD;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    color: #000000;\n"
"    font-weight: bold;\n"
"    font-size: 12px;\n"
"}\n"
"\n"
"/* Quand on clique dans une case */\n"
"QLineEdit:focus, QComboBox:focus, QDateEdit:focus {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #4CAF50; /* Bordure VERTE */\n"
"}\n"
"\n"
"/* A"
                        "m\303\251lioration des fl\303\250ches pour QDateEdit / QSpinBox */\n"
"QDateEdit, QSpinBox {\n"
"    padding-right: 6px;\n"
"}\n"
"QDateEdit::up-button, QDateEdit::down-button,\n"
"QSpinBox::up-button, QSpinBox::down-button {\n"
"    width: 18px;\n"
"    height: 18px;\n"
"}\n"
"QDateEdit::up-arrow, QDateEdit::down-arrow,\n"
"QSpinBox::up-arrow, QSpinBox::down-arrow {\n"
"    width: 8px;\n"
"    height: 8px;\n"
"}\n"
"\n"
"/* Les Labels (Textes : \"Nom\", \"Pr\303\251nom\"...) */\n"
"QLabel {\n"
"    color: #333333; /* Noir/Gris fonc\303\251 pour bien lire sur le fond */\n"
"    font-weight: bold;\n"
"    font-size: 12px;\n"
"}\n"
"\n"
"/* La zone de droite (Liste/Stats) si c'est un GroupBox ou Frame */\n"
"QGroupBox, QFrame#frame_droite {\n"
"    background-color: rgba(255, 255, 255, 210); \n"
"    border-radius: 15px;\n"
"}\n"
"\n"
"/* =======================================================\n"
"   4. LES BOUTONS D'ACTION (BAS)\n"
"   ======================================================= */\n"
"\n"
"QPushBut"
                        "ton {\n"
"    border-radius: 8px;\n"
"    padding: 8px;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* Bouton Ajouter (Vert) */\n"
"QPushButton#btn_ajouter { background-color: #43A047; }\n"
"QPushButton#btn_ajouter:hover { background-color: #2E7D32; }\n"
"\n"
"/* Bouton Modifier (Bleu) */\n"
"QPushButton#btn_modifier { background-color: #1E88E5; }\n"
"QPushButton#btn_modifier:hover { background-color: #1565C0; }\n"
"\n"
"/* Bouton Supprimer (Rouge) */\n"
"QPushButton#btn_supprimer { background-color: #E53935; }\n"
"QPushButton#btn_supprimer:hover { background-color: #C62828; }\n"
""));
        lbl_page_title = new QLabel(page_contrat);
        lbl_page_title->setObjectName("lbl_page_title");
        lbl_page_title->setGeometry(QRect(510, 0, 213, 52));
        lbl_page_title->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: white; padding: 10px;"));
        lbl_page_title->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        cb_tri_option = new QComboBox(page_contrat);
        cb_tri_option->addItem(QString());
        cb_tri_option->addItem(QString());
        cb_tri_option->addItem(QString());
        cb_tri_option->setObjectName("cb_tri_option");
        cb_tri_option->setGeometry(QRect(710, 70, 101, 30));
        btn_tri_2 = new QPushButton(page_contrat);
        btn_tri_2->setObjectName("btn_tri_2");
        btn_tri_2->setGeometry(QRect(820, 70, 71, 36));
        btn_tri_2->setStyleSheet(QString::fromUtf8("background-color: rgb(251, 192, 45);\n"
"color: rgb(0, 0, 0);"));
        tab_contrat_2 = new QTableView(page_contrat);
        tab_contrat_2->setObjectName("tab_contrat_2");
        tab_contrat_2->setGeometry(QRect(310, 110, 591, 561));
        tab_contrat_2->setStyleSheet(QString::fromUtf8("QTableView {\n"
"    background-color: rgba(255, 255, 255, 200); \n"
"    border: 1px solid rgba(0, 0, 0, 20);\n"
"    border-radius: 8px;\n"
"    gridline-color: rgba(0, 0, 0, 10); /* Lignes grises tr\303\250s fines */\n"
"    color: #000000; /* Texte Noir */\n"
"    selection-background-color: rgba(76, 175, 80, 180); /* Vert s\303\251lection */\n"
"    selection-color: white;\n"
"    outline: 0;\n"
"}"));
        tab_contrat_2->setAlternatingRowColors(true);
        tab_contrat_2->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        le_recherche_4 = new QLineEdit(page_contrat);
        le_recherche_4->setObjectName("le_recherche_4");
        le_recherche_4->setGeometry(QRect(310, 70, 391, 30));
        tabWidget_right_contrat = new QTabWidget(page_contrat);
        tabWidget_right_contrat->setObjectName("tabWidget_right_contrat");
        tabWidget_right_contrat->setGeometry(QRect(910, 30, 611, 701));
        tabWidget_right_contrat->setStyleSheet(QString::fromUtf8("QTabWidget::pane {\n"
"    border: 1px solid #ccc;\n"
"    background-color: transparent;\n"
"    border: transparent;\n"
"    border-radius: 8px;\n"
"    gridline-color: rgba(0, 0, 0, 10); /* Lignes grises tr\303\250s fines /\n"
"    color: #000000; / Texte Noir /\n"
"    selection-background-color: rgba(76, 175, 80, 180); / Vert s\303\251lection */\n"
"    selection-color: white;\n"
"    outline: 0;\n"
"    top: -1px;\n"
"}\n"
"\n"
"QTabWidget::tab-bar {\n"
"    left: 0px;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background: #2d7c4d;\n"
"    color: white;\n"
"    padding: 8px 20px;\n"
"    margin-right: 2px;\n"
"    border: 1px solid #2d7c4d;\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 4px;\n"
"    border-top-right-radius: 4px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background: #a8d5ba;\n"
"    color: #2d7c4d;\n"
"    border: 1px solid #ccc;\n"
"    border-bottom: 1px solid white;\n"
"}\n"
"\n"
"QTabBar::tab:hover:!selected {\n"
"    background: #3d8c5d;\n"
"}"
                        "\n"
"\n"
"QTabBar::tab:first {\n"
"    margin-left: 0;\n"
"}"));
        tab_stats_client_2 = new QWidget();
        tab_stats_client_2->setObjectName("tab_stats_client_2");
        gb_stats_contrat = new QGroupBox(tab_stats_client_2);
        gb_stats_contrat->setObjectName("gb_stats_contrat");
        gb_stats_contrat->setGeometry(QRect(11, 11, 581, 646));
        QFont font;
        font.setBold(true);
        gb_stats_contrat->setFont(font);
        gb_stats_contrat->setStyleSheet(QString::fromUtf8(" /* --- 2. LE CADRE FORMULAIRE & STATISTIQUES (Verre Blanc) --- */\n"
"QGroupBox {\n"
"    /* Fond BLANC avec transparence (Alpha = 210 sur 255 pour lisibilit\303\251) */\n"
"    background-color: rgba(255, 255, 255, 210); \n"
"    border: 1px solid rgba(0, 0, 0, 20); /* Bordure grise subtile */\n"
"    border-radius: 15px;\n"
"    margin-top: 25px; \n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top left;\n"
"    left: 20px;\n"
"    color: #2E7D32; /* Vert Fonc\303\251 EcoCycle pour le contraste sur blanc */\n"
"    font-weight: bold;\n"
"    font-size: 16px;\n"
"    background-color: transparent;\n"
"}"));
        tabWidget_right_contrat->addTab(tab_stats_client_2, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget_right_contrat->addTab(tab_2, QString());
        lbl_info_contrat_title = new QLabel(page_contrat);
        lbl_info_contrat_title->setObjectName("lbl_info_contrat_title");
        lbl_info_contrat_title->setGeometry(QRect(0, 40, 149, 16));
        lbl_info_contrat_title->setStyleSheet(QString::fromUtf8("font-weight: bold; font-size: 12px; color: #2e7d32; padding-left: 5px;"));
        group_informations = new QGroupBox(page_contrat);
        group_informations->setObjectName("group_informations");
        group_informations->setGeometry(QRect(0, 70, 301, 661));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(group_informations->sizePolicy().hasHeightForWidth());
        group_informations->setSizePolicy(sizePolicy);
        group_informations->setMaximumSize(QSize(400, 16777215));
        group_informations->setStyleSheet(QString::fromUtf8("QGroupBox { padding-top: 15px; }"));
        formLayout_contrat = new QFormLayout(group_informations);
        formLayout_contrat->setObjectName("formLayout_contrat");
        formLayout_contrat->setHorizontalSpacing(10);
        formLayout_contrat->setVerticalSpacing(6);
        formLayout_contrat->setContentsMargins(15, 5, 15, 5);
        label_id = new QLabel(group_informations);
        label_id->setObjectName("label_id");

        formLayout_contrat->setWidget(0, QFormLayout::LabelRole, label_id);

        le_ID_Contrat = new QLineEdit(group_informations);
        le_ID_Contrat->setObjectName("le_ID_Contrat");
        le_ID_Contrat->setReadOnly(true);

        formLayout_contrat->setWidget(0, QFormLayout::FieldRole, le_ID_Contrat);

        label_type = new QLabel(group_informations);
        label_type->setObjectName("label_type");

        formLayout_contrat->setWidget(1, QFormLayout::LabelRole, label_type);

        cb_Type_Contrat = new QComboBox(group_informations);
        cb_Type_Contrat->addItem(QString());
        cb_Type_Contrat->addItem(QString());
        cb_Type_Contrat->setObjectName("cb_Type_Contrat");

        formLayout_contrat->setWidget(1, QFormLayout::FieldRole, cb_Type_Contrat);

        label_matiere = new QLabel(group_informations);
        label_matiere->setObjectName("label_matiere");

        formLayout_contrat->setWidget(2, QFormLayout::LabelRole, label_matiere);

        cb_Type_Matiere = new QComboBox(group_informations);
        cb_Type_Matiere->addItem(QString());
        cb_Type_Matiere->addItem(QString());
        cb_Type_Matiere->addItem(QString());
        cb_Type_Matiere->addItem(QString());
        cb_Type_Matiere->addItem(QString());
        cb_Type_Matiere->setObjectName("cb_Type_Matiere");

        formLayout_contrat->setWidget(2, QFormLayout::FieldRole, cb_Type_Matiere);

        label_qualite = new QLabel(group_informations);
        label_qualite->setObjectName("label_qualite");

        formLayout_contrat->setWidget(3, QFormLayout::LabelRole, label_qualite);

        cb_Qualite_Matiere = new QComboBox(group_informations);
        cb_Qualite_Matiere->addItem(QString());
        cb_Qualite_Matiere->addItem(QString());
        cb_Qualite_Matiere->addItem(QString());
        cb_Qualite_Matiere->setObjectName("cb_Qualite_Matiere");

        formLayout_contrat->setWidget(3, QFormLayout::FieldRole, cb_Qualite_Matiere);

        label_date_deb = new QLabel(group_informations);
        label_date_deb->setObjectName("label_date_deb");

        formLayout_contrat->setWidget(4, QFormLayout::LabelRole, label_date_deb);

        date_debut = new QDateEdit(group_informations);
        date_debut->setObjectName("date_debut");

        formLayout_contrat->setWidget(4, QFormLayout::FieldRole, date_debut);

        label_date_fin = new QLabel(group_informations);
        label_date_fin->setObjectName("label_date_fin");

        formLayout_contrat->setWidget(5, QFormLayout::LabelRole, label_date_fin);

        date_fin = new QDateEdit(group_informations);
        date_fin->setObjectName("date_fin");

        formLayout_contrat->setWidget(5, QFormLayout::FieldRole, date_fin);

        label_duree = new QLabel(group_informations);
        label_duree->setObjectName("label_duree");

        formLayout_contrat->setWidget(6, QFormLayout::LabelRole, label_duree);

        dspin_prix_unitaire_2 = new QDoubleSpinBox(group_informations);
        dspin_prix_unitaire_2->setObjectName("dspin_prix_unitaire_2");
        dspin_prix_unitaire_2->setDecimals(0);
        dspin_prix_unitaire_2->setValue(0.000000000000000);

        formLayout_contrat->setWidget(6, QFormLayout::FieldRole, dspin_prix_unitaire_2);

        label_prix = new QLabel(group_informations);
        label_prix->setObjectName("label_prix");

        formLayout_contrat->setWidget(7, QFormLayout::LabelRole, label_prix);

        dspin_prix_unitaire = new QDoubleSpinBox(group_informations);
        dspin_prix_unitaire->setObjectName("dspin_prix_unitaire");
        dspin_prix_unitaire->setValue(0.000000000000000);

        formLayout_contrat->setWidget(7, QFormLayout::FieldRole, dspin_prix_unitaire);

        label_statut = new QLabel(group_informations);
        label_statut->setObjectName("label_statut");

        formLayout_contrat->setWidget(8, QFormLayout::LabelRole, label_statut);

        cb_statut_contrat = new QComboBox(group_informations);
        cb_statut_contrat->addItem(QString());
        cb_statut_contrat->addItem(QString());
        cb_statut_contrat->addItem(QString());
        cb_statut_contrat->addItem(QString());
        cb_statut_contrat->setObjectName("cb_statut_contrat");

        formLayout_contrat->setWidget(8, QFormLayout::FieldRole, cb_statut_contrat);

        label_details = new QLabel(group_informations);
        label_details->setObjectName("label_details");

        formLayout_contrat->setWidget(9, QFormLayout::LabelRole, label_details);

        te_details = new QTextEdit(group_informations);
        te_details->setObjectName("te_details");
        te_details->setMinimumSize(QSize(0, 50));
        te_details->setMaximumSize(QSize(16777215, 50));
        te_details->setStyleSheet(QString::fromUtf8("background-color: white; color: black;"));

        formLayout_contrat->setWidget(9, QFormLayout::FieldRole, te_details);

        frame_recherche = new QFrame(page_contrat);
        frame_recherche->setObjectName("frame_recherche");
        frame_recherche->setGeometry(QRect(70, 0, 381, 40));
        frame_recherche->setMinimumSize(QSize(0, 35));
        frame_recherche->setFrameShape(QFrame::Shape::NoFrame);
        horizontalLayout_recherche = new QHBoxLayout(frame_recherche);
        horizontalLayout_recherche->setSpacing(4);
        horizontalLayout_recherche->setObjectName("horizontalLayout_recherche");
        horizontalLayout_recherche->setContentsMargins(4, 2, 4, 2);
        btn_export_pdf = new QPushButton(page_contrat);
        btn_export_pdf->setObjectName("btn_export_pdf");
        btn_export_pdf->setGeometry(QRect(740, 680, 133, 40));
        btn_export_pdf->setMinimumSize(QSize(120, 40));
        btn_export_pdf->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(255, 235, 59);"));
        btn_modifier_3 = new QPushButton(page_contrat);
        btn_modifier_3->setObjectName("btn_modifier_3");
        btn_modifier_3->setGeometry(QRect(460, 680, 120, 40));
        btn_modifier_3->setMinimumSize(QSize(120, 40));
        btn_modifier_3->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 136, 229)"));
        btn_ajouter_3 = new QPushButton(page_contrat);
        btn_ajouter_3->setObjectName("btn_ajouter_3");
        btn_ajouter_3->setGeometry(QRect(320, 680, 120, 40));
        btn_ajouter_3->setMinimumSize(QSize(120, 40));
        btn_ajouter_3->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(18, 172, 43);\n"
""));
        btn_supprimer_4 = new QPushButton(page_contrat);
        btn_supprimer_4->setObjectName("btn_supprimer_4");
        btn_supprimer_4->setGeometry(QRect(600, 680, 120, 40));
        btn_supprimer_4->setMinimumSize(QSize(120, 40));
        btn_supprimer_4->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
        stackedWidget->addWidget(page_contrat);
        page_equipement = new QWidget();
        page_equipement->setObjectName("page_equipement");
        QFont font1;
        font1.setPointSize(14);
        page_equipement->setFont(font1);
        page_equipement->setStyleSheet(QString::fromUtf8("/* --- 1. L'IMAGE DE FOND --- */\n"
"QWidget#page_employes {\n"
"    /* Assure-toi que le chemin de ton image est correct ici ! */\n"
"    border-image: url(:/new/prefix1/images/bg_left.jpg) 0 0 0 0 stretch stretch ;\n"
"}\n"
"\n"
"/* --- 2. LE CADRE FORMULAIRE & STATISTIQUES (Verre Blanc) --- */\n"
"QGroupBox {\n"
"    /* Fond BLANC avec transparence (Alpha = 210 sur 255 pour lisibilit\303\251) */\n"
"    background-color: rgba(255, 255, 255, 210); \n"
"    border: 1px solid rgba(0, 0, 0, 20); /* Bordure grise subtile */\n"
"    border-radius: 15px;\n"
"    margin-top: 25px; \n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top left;\n"
"    left: 20px;\n"
"    color: #2E7D32; /* Vert Fonc\303\251 EcoCycle pour le contraste sur blanc */\n"
"    font-weight: bold;\n"
"    font-size: 16px;\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"/* --- 3. LES LABELS --- */\n"
"QLabel {\n"
"    color: #333333; /* Texte GRIS FONC\303\211/NOIR */\n"
"    font-weight: b"
                        "old;\n"
"    font-size: 13px;\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"/* --- 4. CHAMPS DE SAISIE (Inputs Clairs) --- */\n"
"QLineEdit, QComboBox, QDateEdit, QDoubleSpinBox {\n"
"    /* Fond blanc cass\303\251 semi-transparent */\n"
"    background-color: rgba(245, 245, 245, 200); \n"
"    border: 1px solid #BDBDBD; /* Bordure grise */\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    color: #000000; /* Texte NOIR quand on \303\251crit */\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QLineEdit:focus, QComboBox:focus, QDateEdit:focus {\n"
"    background-color: #FFFFFF; /* Devient blanc pur au clic */\n"
"    border: 2px solid #4CAF50; /* Bordure VERTE */\n"
"}\n"
"\n"
"/* Sp\303\251cifique : La liste de choix de Tri */\n"
"QComboBox#cb_tri_option {\n"
"    min-width: 80px;\n"
"    background-color: rgba(255, 255, 255, 200);\n"
"    color: #333;\n"
"}\n"
"\n"
"/* --- 5. TABLEAU (Liste Blanche) --- */\n"
"QTableView {\n"
"    background-color: rgba(255, 255, 255, 200); \n"
"    border: 1px solid"
                        " rgba(0, 0, 0, 20);\n"
"    border-radius: 8px;\n"
"    gridline-color: rgba(0, 0, 0, 10); /* Lignes grises tr\303\250s fines */\n"
"    color: #000000; /* Texte Noir */\n"
"    selection-background-color: rgba(76, 175, 80, 180); /* Vert s\303\251lection */\n"
"    selection-color: white;\n"
"    outline: 0;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #2E7D32; /* Ent\303\252te Vert Fonc\303\251 */\n"
"    color: white;\n"
"    padding: 8px;\n"
"    border: none;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QTableCornerButton::section {\n"
"    background-color: rgba(255, 255, 255, 200);\n"
"    border: none;\n"
"}\n"
"\n"
"/* --- 6. BOUTONS (Couleurs Vives) --- */\n"
"QPushButton {\n"
"    border-radius: 8px;\n"
"    padding: 10px 15px;\n"
"    color: white; /* Texte Blanc sur bouton color\303\251 */\n"
"    font-weight: bold;\n"
"    font-size: 13px;\n"
"    border: 1px solid rgba(0,0,0, 10); /* L\303\251g\303\250re ombre */\n"
"}\n"
"\n"
"/* --- COULEURS DES BOUTONS --- */\n"
"\n"
"/* Ajoute"
                        "r : Vert */\n"
"QPushButton#btn_ajouter {\n"
"    background-color: #43A047; \n"
"}\n"
"QPushButton#btn_ajouter:hover { background-color: #2E7D32; }\n"
"\n"
"/* Modifier : Bleu */\n"
"QPushButton#btn_modifier {\n"
"    background-color: #1E88E5; \n"
"}\n"
"QPushButton#btn_modifier:hover { background-color: #1565C0; }\n"
"\n"
"/* Supprimer : Rouge */\n"
"QPushButton#btn_supprimer {\n"
"    background-color: #E53935; \n"
"}\n"
"QPushButton#btn_supprimer:hover { background-color: #C62828; }\n"
"\n"
"/* PDF : Gris */\n"
"QPushButton#btn_pdf {\n"
"    background-color: #757575; \n"
"}\n"
"QPushButton#btn_pdf:hover { background-color: #424242; }\n"
"\n"
"/* Contrat : Marron */\n"
"QPushButton#btn_contrat {\n"
"    background-color: #795548;\n"
"}\n"
"QPushButton#btn_contrat:hover { background-color: #5D4037; }\n"
"\n"
"/* Mailing : Violet */\n"
"QPushButton#btn_mailing {\n"
"    background-color: #8E24AA;\n"
"}\n"
"QPushButton#btn_mailing:hover { background-color: #7B1FA2; }\n"
"\n"
"/* TRIER : Jaune/Ambre */\n"
"QP"
                        "ushButton#btn_tri {\n"
"    background-color: #FFC107;\n"
"    color: #333; /* Texte fonc\303\251 sur jaune */\n"
"}\n"
"QPushButton#btn_tri:hover { background-color: #FFB300; }\n"
"\n"
"/* --- SCROLLBAR --- */\n"
"QScrollBar:vertical {\n"
"    border: none;\n"
"    background: rgba(0,0,0,10);\n"
"    width: 10px;\n"
"    border-radius: 5px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background: rgba(0,0,0,30); /* Poign\303\251e grise */\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"/* --- TAB WIDGET (ONGLETS) --- */\n"
"QTabWidget::pane {\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    background-color: transparent; /* Fond transparent pour voir l'image principale */\n"
"    top: -1px; \n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background-color: rgba(255, 255, 255, 150); /* Onglet inactif */\n"
"    border: 1px solid rgba(0,0,0, 10);\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 8px;\n"
"    border-top-right-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    margin-right: 2px;\n"
"    fon"
                        "t-weight: bold;\n"
"    color: #555;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background-color: rgba(255, 255, 255, 220); /* Onglet actif plus blanc */\n"
"    color: #2E7D32; /* Texte Vert EcoCycle */\n"
"    border-top: 3px solid #2E7D32; /* Petite barre verte en haut */\n"
"}\n"
"\n"
"QTabBar::tab:hover {\n"
"    background-color: rgba(255, 255, 255, 180);\n"
"}\n"
"QGroupBox {\n"
"    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #d4e7e1, stop:1 #c8d9e8);\n"
"    border: 2px solid #2d5016;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"    font-weight: bold;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 5px;\n"
"    color: #2d5016;\n"
"}\n"
"QLineEdit {\n"
"    background-color: white;\n"
"    border: 1px solid #8a9a7a;\n"
"    border-radius: 3px;\n"
"    padding: 5px;\n"
"    min-height: 25px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid #2d5016;\n"
"}\n"
"QDateEdit {\n"
""
                        "    background-color: white;\n"
"    border: 1px solid #8a9a7a;\n"
"    border-radius: 3px;\n"
"    padding: 5px;\n"
"    min-height: 25px;\n"
"}\n"
"\n"
"QDateEdit:focus {\n"
"    border: 2px solid #2d5016;\n"
"}\n"
"QPushButton {\n"
"    background-color: #ffa500;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 5px;\n"
"    padding: 8px 15px;\n"
"    font-weight: bold;\n"
"    min-height: 30px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #ff8c00;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #ff7700;\n"
"}"));
        headerWidget = new QWidget(page_equipement);
        headerWidget->setObjectName("headerWidget");
        headerWidget->setGeometry(QRect(0, 10, 1305, 60));
        headerWidget->setMinimumSize(QSize(0, 60));
        headerWidget->setMaximumSize(QSize(16777215, 60));
        headerWidget->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        headerLayout = new QHBoxLayout(headerWidget);
        headerLayout->setObjectName("headerLayout");
        headerLabel = new QLabel(headerWidget);
        headerLabel->setObjectName("headerLabel");
        headerLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: rgb(255, 255, 255);\n"
"    font: 700 24pt \"Segoe UI\";\n"
"    background-color: transparent;\n"
"}"));
        headerLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        headerLayout->addWidget(headerLabel);

        layoutWidget_2 = new QWidget(page_equipement);
        layoutWidget_2->setObjectName("layoutWidget_2");
        layoutWidget_2->setGeometry(QRect(30, 70, 1234, 581));
        horizontalLayout_9 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_9->setSpacing(10);
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        horizontalLayout_9->setContentsMargins(10, 10, 10, 10);
        infoGroupBox_5 = new QGroupBox(layoutWidget_2);
        infoGroupBox_5->setObjectName("infoGroupBox_5");
        infoGroupBox_5->setMinimumSize(QSize(401, 561));
        infoGroupBox_5->setMaximumSize(QSize(401, 561));
        infoGroupBox_5->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    /* Fond BLANC avec transparence (Alpha = 210 sur 255 pour lisibilit\303\251) */\n"
"    background-color: rgba(255, 255, 255, 210); \n"
"    border: 1px solid rgba(0, 0, 0, 20); /* Bordure grise subtile */\n"
"    border-radius: 15px;\n"
"    margin-top: 25px; \n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 5px;\n"
"    color: qlineargradient(spread:repeat, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(8, 102, 0, 255), stop:1 rgba(0, 255, 127, 255));\n"
"}"));
        infoGroupBoxLayout = new QVBoxLayout(infoGroupBox_5);
        infoGroupBoxLayout->setObjectName("infoGroupBoxLayout");
        infoScrollArea = new QScrollArea(infoGroupBox_5);
        infoScrollArea->setObjectName("infoScrollArea");
        infoScrollArea->setStyleSheet(QString::fromUtf8("QScrollArea { background-color: transparent; border: none; }"));
        infoScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        infoScrollArea->setWidgetResizable(true);
        infoScrollContent = new QWidget();
        infoScrollContent->setObjectName("infoScrollContent");
        infoScrollContent->setGeometry(QRect(0, 0, 357, 492));
        infoScrollContent->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        verticalLayout_48 = new QVBoxLayout(infoScrollContent);
        verticalLayout_48->setObjectName("verticalLayout_48");
        verticalSpacer_35 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_48->addItem(verticalSpacer_35);

        labelNomEquipement = new QLabel(infoScrollContent);
        labelNomEquipement->setObjectName("labelNomEquipement");

        verticalLayout_48->addWidget(labelNomEquipement);

        lineEditNomEquipement = new QLineEdit(infoScrollContent);
        lineEditNomEquipement->setObjectName("lineEditNomEquipement");

        verticalLayout_48->addWidget(lineEditNomEquipement);

        labelTypeEquipement = new QLabel(infoScrollContent);
        labelTypeEquipement->setObjectName("labelTypeEquipement");

        verticalLayout_48->addWidget(labelTypeEquipement);

        comboBoxTypeEquipement = new QComboBox(infoScrollContent);
        comboBoxTypeEquipement->addItem(QString());
        comboBoxTypeEquipement->addItem(QString());
        comboBoxTypeEquipement->addItem(QString());
        comboBoxTypeEquipement->addItem(QString());
        comboBoxTypeEquipement->addItem(QString());
        comboBoxTypeEquipement->setObjectName("comboBoxTypeEquipement");

        verticalLayout_48->addWidget(comboBoxTypeEquipement);

        labelFabricant_7 = new QLabel(infoScrollContent);
        labelFabricant_7->setObjectName("labelFabricant_7");

        verticalLayout_48->addWidget(labelFabricant_7);

        comboBoxFabricant_7 = new QComboBox(infoScrollContent);
        comboBoxFabricant_7->addItem(QString());
        comboBoxFabricant_7->addItem(QString());
        comboBoxFabricant_7->addItem(QString());
        comboBoxFabricant_7->addItem(QString());
        comboBoxFabricant_7->addItem(QString());
        comboBoxFabricant_7->addItem(QString());
        comboBoxFabricant_7->addItem(QString());
        comboBoxFabricant_7->setObjectName("comboBoxFabricant_7");

        verticalLayout_48->addWidget(comboBoxFabricant_7);

        verticalSpacer_32 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_48->addItem(verticalSpacer_32);

        pushButtonAjouter = new QPushButton(infoScrollContent);
        pushButtonAjouter->setObjectName("pushButtonAjouter");
        pushButtonAjouter->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ffa500;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 5px;\n"
"    padding: 8px 15px;\n"
"    font-weight: bold;\n"
"    min-height: 30px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #ff8c00;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #ff7700;\n"
"}"));

        verticalLayout_48->addWidget(pushButtonAjouter);

        infoScrollArea->setWidget(infoScrollContent);

        infoGroupBoxLayout->addWidget(infoScrollArea);


        horizontalLayout_9->addWidget(infoGroupBox_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        le_recherche_2 = new QLineEdit(layoutWidget_2);
        le_recherche_2->setObjectName("le_recherche_2");

        horizontalLayout_8->addWidget(le_recherche_2);

        comboBox_2 = new QComboBox(layoutWidget_2);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");

        horizontalLayout_8->addWidget(comboBox_2);

        btn_tri_3 = new QPushButton(layoutWidget_2);
        btn_tri_3->setObjectName("btn_tri_3");
        btn_tri_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 193, 7);"));

        horizontalLayout_8->addWidget(btn_tri_3);


        verticalLayout_6->addLayout(horizontalLayout_8);

        tab_employes_2 = new QTableView(layoutWidget_2);
        tab_employes_2->setObjectName("tab_employes_2");
        tab_employes_2->setAlternatingRowColors(true);
        tab_employes_2->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout_6->addWidget(tab_employes_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        btn_ajouter_2 = new QPushButton(layoutWidget_2);
        btn_ajouter_2->setObjectName("btn_ajouter_2");
        btn_ajouter_2->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_5->addWidget(btn_ajouter_2);

        btn_modifier_2 = new QPushButton(layoutWidget_2);
        btn_modifier_2->setObjectName("btn_modifier_2");

        horizontalLayout_5->addWidget(btn_modifier_2);

        btn_supprimer_2 = new QPushButton(layoutWidget_2);
        btn_supprimer_2->setObjectName("btn_supprimer_2");

        horizontalLayout_5->addWidget(btn_supprimer_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        btn_pdf_2 = new QPushButton(layoutWidget_2);
        btn_pdf_2->setObjectName("btn_pdf_2");
        btn_pdf_2->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_5->addWidget(btn_pdf_2);

        btn_mailing_2 = new QPushButton(layoutWidget_2);
        btn_mailing_2->setObjectName("btn_mailing_2");
        btn_mailing_2->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 0, 255);"));

        horizontalLayout_5->addWidget(btn_mailing_2);


        verticalLayout_6->addLayout(horizontalLayout_5);


        horizontalLayout_9->addLayout(verticalLayout_6);

        tabWidget_right_contrat_2 = new QTabWidget(layoutWidget_2);
        tabWidget_right_contrat_2->setObjectName("tabWidget_right_contrat_2");
        tabWidget_right_contrat_2->setStyleSheet(QString::fromUtf8("QTabWidget::pane {\n"
"    border: 1px solid #ccc;\n"
"    background-color: transparent;\n"
"    border: transparent;\n"
"    border-radius: 8px;\n"
"    gridline-color: rgba(0, 0, 0, 10); /* Lignes grises tr\303\250s fines /\n"
"    color: #000000; / Texte Noir /\n"
"    selection-background-color: rgba(76, 175, 80, 180); / Vert s\303\251lection */\n"
"    selection-color: white;\n"
"    outline: 0;\n"
"    top: -1px;\n"
"}\n"
"\n"
"QTabWidget::tab-bar {\n"
"    left: 0px;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background: #2d7c4d;\n"
"    color: white;\n"
"    padding: 8px 20px;\n"
"    margin-right: 2px;\n"
"    border: 1px solid #2d7c4d;\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 4px;\n"
"    border-top-right-radius: 4px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background: #a8d5ba;\n"
"    color: #2d7c4d;\n"
"    border: 1px solid #ccc;\n"
"    border-bottom: 1px solid white;\n"
"}\n"
"\n"
"QTabBar::tab:hover:!selected {\n"
"    background: #3d8c5d;\n"
"}"
                        "\n"
"\n"
"QTabBar::tab:first {\n"
"    margin-left: 0;\n"
"}"));
        tab_stats_client_4 = new QWidget();
        tab_stats_client_4->setObjectName("tab_stats_client_4");
        gb_stats_contrat_3 = new QGroupBox(tab_stats_client_4);
        gb_stats_contrat_3->setObjectName("gb_stats_contrat_3");
        gb_stats_contrat_3->setGeometry(QRect(0, 0, 271, 521));
        gb_stats_contrat_3->setFont(font);
        gb_stats_contrat_3->setStyleSheet(QString::fromUtf8(" /* --- 2. LE CADRE FORMULAIRE & STATISTIQUES (Verre Blanc) --- */\n"
"QGroupBox {\n"
"    /* Fond BLANC avec transparence (Alpha = 210 sur 255 pour lisibilit\303\251) */\n"
"    background-color: rgba(255, 255, 255, 210); \n"
"    border: 1px solid rgba(0, 0, 0, 20); /* Bordure grise subtile */\n"
"    border-radius: 15px;\n"
"    margin-top: 25px; \n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top left;\n"
"    left: 20px;\n"
"    color: #2E7D32; /* Vert Fonc\303\251 EcoCycle pour le contraste sur blanc */\n"
"    font-weight: bold;\n"
"    font-size: 16px;\n"
"    background-color: transparent;\n"
"}"));
        tabWidget_right_contrat_2->addTab(tab_stats_client_4, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        tabWidget_right_contrat_2->addTab(tab_4, QString());

        horizontalLayout_9->addWidget(tabWidget_right_contrat_2);

        stackedWidget->addWidget(page_equipement);
        page_stock = new QWidget();
        page_stock->setObjectName("page_stock");
        page_stock->setStyleSheet(QString::fromUtf8("/* --- 1. L'IMAGE DE FOND --- */\n"
"QWidget#page_employes {\n"
"    /* Assure-toi que le chemin de ton image est correct ici ! */\n"
"    border-image: url(:/new/prefix1/images/bg_left.jpg) 0 0 0 0 stretch stretch;\n"
"}\n"
"\n"
"/* --- 2. LE CADRE FORMULAIRE & STATISTIQUES (Verre Blanc) --- */\n"
"QGroupBox {\n"
"    /* Fond BLANC avec transparence (Alpha = 210 sur 255 pour lisibilit\303\251) */\n"
"    background-color: rgba(255, 255, 255, 210); \n"
"    border: 1px solid rgba(0, 0, 0, 20); /* Bordure grise subtile */\n"
"    border-radius: 15px;\n"
"    margin-top: 25px; \n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top left;\n"
"    left: 20px;\n"
"    color: #2E7D32; /* Vert Fonc\303\251 EcoCycle pour le contraste sur blanc */\n"
"    font-weight: bold;\n"
"    font-size: 16px;\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"/* --- 3. LES LABELS --- */\n"
"QLabel {\n"
"    color: #333333; /* Texte GRIS FONC\303\211/NOIR */\n"
"    font-weight: bo"
                        "ld;\n"
"    font-size: 13px;\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"/* --- 4. CHAMPS DE SAISIE (Inputs Clairs) --- */\n"
"QLineEdit, QComboBox, QDateEdit, QDoubleSpinBox {\n"
"    /* Fond blanc cass\303\251 semi-transparent */\n"
"    background-color: rgba(245, 245, 245, 200); \n"
"    border: 1px solid #BDBDBD; /* Bordure grise */\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    color: #000000; /* Texte NOIR quand on \303\251crit */\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QLineEdit:focus, QComboBox:focus, QDateEdit:focus {\n"
"    background-color: #FFFFFF; /* Devient blanc pur au clic */\n"
"    border: 2px solid #4CAF50; /* Bordure VERTE */\n"
"}\n"
"\n"
"/* Sp\303\251cifique : La liste de choix de Tri */\n"
"QComboBox#cb_tri_option {\n"
"    min-width: 80px;\n"
"    background-color: rgba(255, 255, 255, 200);\n"
"    color: #333;\n"
"}\n"
"\n"
"/* --- 5. TABLEAU (Liste Blanche) --- */\n"
"QTableView {\n"
"    background-color: rgba(255, 255, 255, 200); \n"
"    border: 1px solid "
                        "rgba(0, 0, 0, 20);\n"
"    border-radius: 8px;\n"
"    gridline-color: rgba(0, 0, 0, 10); /* Lignes grises tr\303\250s fines */\n"
"    color: #000000; /* Texte Noir */\n"
"    selection-background-color: rgba(76, 175, 80, 180); /* Vert s\303\251lection */\n"
"    selection-color: white;\n"
"    outline: 0;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #2E7D32; /* Ent\303\252te Vert Fonc\303\251 */\n"
"    color: white;\n"
"    padding: 8px;\n"
"    border: none;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QTableCornerButton::section {\n"
"    background-color: rgba(255, 255, 255, 200);\n"
"    border: none;\n"
"}\n"
"\n"
"/* --- 6. BOUTONS (Couleurs Vives) --- */\n"
"QPushButton {\n"
"    border-radius: 8px;\n"
"    padding: 10px 15px;\n"
"    color: white; /* Texte Blanc sur bouton color\303\251 */\n"
"    font-weight: bold;\n"
"    font-size: 13px;\n"
"    border: 1px solid rgba(0,0,0, 10); /* L\303\251g\303\250re ombre */\n"
"}\n"
"\n"
"/* --- COULEURS DES BOUTONS --- */\n"
"\n"
"/* Ajouter"
                        " : Vert */\n"
"QPushButton#btn_ajouter {\n"
"    background-color: #43A047; \n"
"}\n"
"QPushButton#btn_ajouter:hover { background-color: #2E7D32; }\n"
"\n"
"/* Modifier : Bleu */\n"
"QPushButton#btn_modifier {\n"
"    background-color: #1E88E5; \n"
"}\n"
"QPushButton#btn_modifier:hover { background-color: #1565C0; }\n"
"\n"
"/* Supprimer : Rouge */\n"
"QPushButton#btn_supprimer {\n"
"    background-color: #E53935; \n"
"}\n"
"QPushButton#btn_supprimer:hover { background-color: #C62828; }\n"
"\n"
"/* PDF : Gris */\n"
"QPushButton#btn_pdf {\n"
"    background-color: #757575; \n"
"}\n"
"QPushButton#btn_pdf:hover { background-color: #424242; }\n"
"\n"
"/* Contrat : Marron */\n"
"QPushButton#btn_contrat {\n"
"    background-color: #795548;\n"
"}\n"
"QPushButton#btn_contrat:hover { background-color: #5D4037; }\n"
"\n"
"/* Mailing : Violet */\n"
"QPushButton#btn_mailing {\n"
"    background-color: #8E24AA;\n"
"}\n"
"QPushButton#btn_mailing:hover { background-color: #7B1FA2; }\n"
"\n"
"/* TRIER : Jaune/Ambre */\n"
"QPu"
                        "shButton#btn_tri {\n"
"    background-color: #FFC107;\n"
"    color: #333; /* Texte fonc\303\251 sur jaune */\n"
"}\n"
"QPushButton#btn_tri:hover { background-color: #FFB300; }\n"
"\n"
"/* --- SCROLLBAR --- */\n"
"QScrollBar:vertical {\n"
"    border: none;\n"
"    background: rgba(0,0,0,10);\n"
"    width: 10px;\n"
"    border-radius: 5px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background: rgba(0,0,0,30); /* Poign\303\251e grise */\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"/* --- TAB WIDGET (ONGLETS) --- */\n"
"QTabWidget::pane {\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    background-color: transparent; /* Fond transparent pour voir l'image principale */\n"
"    top: -1px; \n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background-color: rgba(255, 255, 255, 150); /* Onglet inactif */\n"
"    border: 1px solid rgba(0,0,0, 10);\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 8px;\n"
"    border-top-right-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    margin-right: 2px;\n"
"    font"
                        "-weight: bold;\n"
"    color: #555;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background-color: rgba(255, 255, 255, 220); /* Onglet actif plus blanc */\n"
"    color: #2E7D32; /* Texte Vert EcoCycle */\n"
"    border-top: 3px solid #2E7D32; /* Petite barre verte en haut */\n"
"}\n"
"\n"
"QTabBar::tab:hover {\n"
"    background-color: rgba(255, 255, 255, 180);\n"
"}\n"
""));
        groupBox_2 = new QGroupBox(page_stock);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(0, 50, 261, 651));
        groupBox_2->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";\n"
"font: 8pt \"Segoe UI\";\n"
"font: 700 9pt \"Segoe UI\";"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_31 = new QLabel(groupBox_2);
        label_31->setObjectName("label_31");

        verticalLayout_2->addWidget(label_31);

        le_id = new QLineEdit(groupBox_2);
        le_id->setObjectName("le_id");
        le_id->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe UI\";"));

        verticalLayout_2->addWidget(le_id);

        label_34 = new QLabel(groupBox_2);
        label_34->setObjectName("label_34");

        verticalLayout_2->addWidget(label_34);

        cb_type = new QComboBox(groupBox_2);
        cb_type->addItem(QString());
        cb_type->addItem(QString());
        cb_type->addItem(QString());
        cb_type->addItem(QString());
        cb_type->setObjectName("cb_type");
        cb_type->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe UI\";"));

        verticalLayout_2->addWidget(cb_type);

        label_33 = new QLabel(groupBox_2);
        label_33->setObjectName("label_33");

        verticalLayout_2->addWidget(label_33);

        sb_poids = new QDoubleSpinBox(groupBox_2);
        sb_poids->setObjectName("sb_poids");
        sb_poids->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe UI\";"));

        verticalLayout_2->addWidget(sb_poids);

        label_35 = new QLabel(groupBox_2);
        label_35->setObjectName("label_35");

        verticalLayout_2->addWidget(label_35);

        sb_prix = new QDoubleSpinBox(groupBox_2);
        sb_prix->setObjectName("sb_prix");
        sb_prix->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe UI\";"));

        verticalLayout_2->addWidget(sb_prix);

        label_30 = new QLabel(groupBox_2);
        label_30->setObjectName("label_30");

        verticalLayout_2->addWidget(label_30);

        de_date = new QDateEdit(groupBox_2);
        de_date->setObjectName("de_date");
        de_date->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe UI\";"));

        verticalLayout_2->addWidget(de_date);

        label_27 = new QLabel(groupBox_2);
        label_27->setObjectName("label_27");

        verticalLayout_2->addWidget(label_27);

        cb_statut = new QComboBox(groupBox_2);
        cb_statut->addItem(QString());
        cb_statut->addItem(QString());
        cb_statut->addItem(QString());
        cb_statut->setObjectName("cb_statut");
        cb_statut->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe UI\";"));

        verticalLayout_2->addWidget(cb_statut);

        label_29 = new QLabel(groupBox_2);
        label_29->setObjectName("label_29");

        verticalLayout_2->addWidget(label_29);

        cb_etat = new QComboBox(groupBox_2);
        cb_etat->addItem(QString());
        cb_etat->addItem(QString());
        cb_etat->addItem(QString());
        cb_etat->setObjectName("cb_etat");
        cb_etat->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe UI\";"));

        verticalLayout_2->addWidget(cb_etat);

        tabWidget_Stock = new QTabWidget(page_stock);
        tabWidget_Stock->setObjectName("tabWidget_Stock");
        tabWidget_Stock->setGeometry(QRect(860, 40, 471, 661));
        tabWidget_Stock->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";\n"
"font: 700 9pt \"Segoe UI\";\n"
"background-color: rgb(200, 221, 211); border-radius: 15px;"));
        tab_stats_3 = new QWidget();
        tab_stats_3->setObjectName("tab_stats_3");
        tabWidget_Stock->addTab(tab_stats_3, QString());
        tab_historique = new QWidget();
        tab_historique->setObjectName("tab_historique");
        tabWidget_Stock->addTab(tab_historique, QString());
        tab_finance = new QWidget();
        tab_finance->setObjectName("tab_finance");
        tabWidget_Stock->addTab(tab_finance, QString());
        tab_stock = new QTableView(page_stock);
        tab_stock->setObjectName("tab_stock");
        tab_stock->setGeometry(QRect(270, 190, 581, 461));
        groupBox_3 = new QGroupBox(page_stock);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(270, 50, 581, 131));
        groupBox_3->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";\n"
"font: 700 9pt \"Segoe UI\";"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 40, 151, 20));
        lineEdit = new QLineEdit(groupBox_3);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(122, 30, 131, 41));
        cb_chercher = new QComboBox(groupBox_3);
        cb_chercher->addItem(QString());
        cb_chercher->addItem(QString());
        cb_chercher->addItem(QString());
        cb_chercher->setObjectName("cb_chercher");
        cb_chercher->setGeometry(QRect(270, 30, 131, 41));
        cb_chercher->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe UI\";"));
        btn_chercher = new QPushButton(groupBox_3);
        btn_chercher->setObjectName("btn_chercher");
        btn_chercher->setGeometry(QRect(432, 30, 141, 41));
        btn_chercher->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 121, 58);\n"
"background-color: rgb(254, 164, 0);"));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 80, 121, 31));
        cb_trier = new QComboBox(groupBox_3);
        cb_trier->addItem(QString());
        cb_trier->addItem(QString());
        cb_trier->addItem(QString());
        cb_trier->setObjectName("cb_trier");
        cb_trier->setGeometry(QRect(270, 80, 131, 41));
        cb_trier->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe UI\";"));
        btn_trier_2 = new QPushButton(groupBox_3);
        btn_trier_2->setObjectName("btn_trier_2");
        btn_trier_2->setGeometry(QRect(432, 80, 141, 41));
        btn_trier_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 165, 0);"));
        label_75 = new QLabel(page_stock);
        label_75->setObjectName("label_75");
        label_75->setGeometry(QRect(260, 10, 671, 51));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Segoe UI")});
        font2.setBold(true);
        label_75->setFont(font2);
        label_75->setStyleSheet(QString::fromUtf8(""));
        btn_ajouter_4 = new QPushButton(page_stock);
        btn_ajouter_4->setObjectName("btn_ajouter_4");
        btn_ajouter_4->setGeometry(QRect(270, 660, 121, 41));
        btn_ajouter_4->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 96, 0);"));
        btn_modifier_4 = new QPushButton(page_stock);
        btn_modifier_4->setObjectName("btn_modifier_4");
        btn_modifier_4->setGeometry(QRect(400, 660, 121, 41));
        btn_modifier_4->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 127);\n"
"background-color: rgb(30, 136, 229);"));
        btn_supprimer_5 = new QPushButton(page_stock);
        btn_supprimer_5->setObjectName("btn_supprimer_5");
        btn_supprimer_5->setGeometry(QRect(530, 660, 121, 39));
        btn_supprimer_5->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
        btn_pdf_3 = new QPushButton(page_stock);
        btn_pdf_3->setObjectName("btn_pdf_3");
        btn_pdf_3->setGeometry(QRect(700, 660, 151, 41));
        btn_pdf_3->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 127);\n"
"background-color: rgb(50, 119, 53);"));
        stackedWidget->addWidget(page_stock);
        page_client = new QWidget();
        page_client->setObjectName("page_client");
        page_client->setStyleSheet(QString::fromUtf8("/* --- 1. L'IMAGE DE FOND --- */\n"
"QWidget#page_client {\n"
"    /* Assure-toi que le chemin de ton image est correct ici ! */\n"
"    border-image: url(:/new/prefix1/images/bg_left.jpg) 0 0 0 0 stretch stretch ;\n"
"}\n"
"\n"
"/* --- 2. LE CADRE FORMULAIRE & STATISTIQUES (Verre Blanc) --- */\n"
"QGroupBox {\n"
"    /* Fond BLANC avec transparence (Alpha = 210 sur 255 pour lisibilit\303\251) */\n"
"    background-color: rgba(255, 255, 255, 210); \n"
"    border: 1px solid rgba(0, 0, 0, 20); /* Bordure grise subtile */\n"
"    border-radius: 15px;\n"
"    margin-top: 25px; \n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top left;\n"
"    left: 20px;\n"
"    color: #2E7D32; /* Vert Fonc\303\251 EcoCycle pour le contraste sur blanc */\n"
"    font-weight: bold;\n"
"    font-size: 16px;\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"/* --- 3. LES LABELS --- */\n"
"QLabel {\n"
"    color: #333333; /* Texte GRIS FONC\303\211/NOIR */\n"
"    font-weight: bol"
                        "d;\n"
"    font-size: 13px;\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"/* --- 4. CHAMPS DE SAISIE (Inputs Clairs) --- */\n"
"QLineEdit, QComboBox, QDateEdit, QDoubleSpinBox {\n"
"    /* Fond blanc cass\303\251 semi-transparent */\n"
"    background-color: rgba(245, 245, 245, 200); \n"
"    border: 1px solid #BDBDBD; /* Bordure grise */\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    color: #000000; /* Texte NOIR quand on \303\251crit */\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QLineEdit:focus, QComboBox:focus, QDateEdit:focus {\n"
"    background-color: #FFFFFF; /* Devient blanc pur au clic */\n"
"    border: 2px solid #4CAF50; /* Bordure VERTE */\n"
"}\n"
"\n"
"/* Sp\303\251cifique : La liste de choix de Tri */\n"
"QComboBox#cb_tri_option_client {\n"
"    min-width: 80px;\n"
"    background-color: rgba(255, 255, 255, 200);\n"
"    color: #333;\n"
"}\n"
"\n"
"/* --- 5. TABLEAU (Liste Blanche) --- */\n"
"QTableView {\n"
"    background-color: rgba(255, 255, 255, 200); \n"
"    border: 1px "
                        "solid rgba(0, 0, 0, 20);\n"
"    border-radius: 8px;\n"
"    gridline-color: rgba(0, 0, 0, 10); /* Lignes grises tr\303\250s fines */\n"
"    color: #000000; /* Texte Noir */\n"
"    selection-background-color: rgba(76, 175, 80, 180); /* Vert s\303\251lection */\n"
"    selection-color: white;\n"
"    outline: 0;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #2E7D32; /* Ent\303\252te Vert Fonc\303\251 */\n"
"    color: white;\n"
"    padding: 8px;\n"
"    border: none;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QTableCornerButton::section {\n"
"    background-color: rgba(255, 255, 255, 200);\n"
"    border: none;\n"
"}\n"
"\n"
"/* --- 6. BOUTONS (Couleurs Vives) --- */\n"
"QPushButton {\n"
"    border-radius: 8px;\n"
"    padding: 10px 15px;\n"
"    color: white; /* Texte Blanc sur bouton color\303\251 */\n"
"    font-weight: bold;\n"
"    font-size: 13px;\n"
"    border: 1px solid rgba(0,0,0, 10); /* L\303\251g\303\250re ombre */\n"
"}\n"
"\n"
"/* --- COULEURS DES BOUTONS --- */\n"
"\n"
"/* A"
                        "jouter : Vert */\n"
"QPushButton#btn_ajouter_client {\n"
"    background-color: #43A047; \n"
"}\n"
"QPushButton#btn_ajouter_client:hover { background-color: #2E7D32; }\n"
"\n"
"/* Modifier : Bleu */\n"
"QPushButton#btn_modifier_client {\n"
"    background-color: #1E88E5; \n"
"}\n"
"QPushButton#btn_modifier_client:hover { background-color: #1565C0; }\n"
"\n"
"/* Supprimer : Rouge */\n"
"QPushButton#btn_supprimer_client {\n"
"    background-color: #E53935; \n"
"}\n"
"QPushButton#btn_supprimer_client:hover { background-color: #C62828; }\n"
"\n"
"/* PDF : Gris */\n"
"QPushButton#btn_pdf_client {\n"
"    background-color: #757575; \n"
"}\n"
"QPushButton#btn_pdf_client:hover { background-color: #424242; }\n"
"\n"
"/* Contrat : Marron */\n"
"QPushButton#btn_contrat_client {\n"
"    background-color: #795548;\n"
"}\n"
"QPushButton#btn_contrat_client:hover { background-color: #5D4037; }\n"
"\n"
"/* Mailing : Violet */\n"
"QPushButton#btn_mailing_client {\n"
"    background-color: #8E24AA;\n"
"}\n"
"QPushButton#btn_mail"
                        "ing_client:hover { background-color: #7B1FA2; }\n"
"\n"
"/* TRIER : Jaune/Ambre */\n"
"QPushButton#btn_tri_client {\n"
"    background-color: #FFC107;\n"
"    color: #333; /* Texte fonc\303\251 sur jaune */\n"
"}\n"
"QPushButton#btn_tri_client:hover { background-color: #FFB300; }\n"
"\n"
"/* --- SCROLLBAR --- */\n"
"QScrollBar:vertical {\n"
"    border: none;\n"
"    background: rgba(0,0,0,10);\n"
"    width: 10px;\n"
"    border-radius: 5px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background: rgba(0,0,0,30); /* Poign\303\251e grise */\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"/* --- TAB WIDGET (ONGLETS) --- */\n"
"QTabWidget::pane {\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    background-color: transparent; /* Fond transparent pour voir l'image principale */\n"
"    top: -1px; \n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background-color: rgba(255, 255, 255, 150); /* Onglet inactif */\n"
"    border: 1px solid rgba(0,0,0, 10);\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 8px;\n"
""
                        "    border-top-right-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    margin-right: 2px;\n"
"    font-weight: bold;\n"
"    color: #555;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background-color: rgba(255, 255, 255, 220); /* Onglet actif plus blanc */\n"
"    color: #2E7D32; /* Texte Vert EcoCycle */\n"
"    border-top: 3px solid #2E7D32; /* Petite barre verte en haut */\n"
"}\n"
"\n"
"QTabBar::tab:hover {\n"
"    background-color: rgba(255, 255, 255, 180);\n"
"}\n"
""));
        gb_form_client = new QGroupBox(page_client);
        gb_form_client->setObjectName("gb_form_client");
        gb_form_client->setGeometry(QRect(0, 10, 231, 721));
        gb_form_client->setFont(font);
        verticalLayout_5 = new QVBoxLayout(gb_form_client);
        verticalLayout_5->setObjectName("verticalLayout_5");
        label_8_client = new QLabel(gb_form_client);
        label_8_client->setObjectName("label_8_client");

        verticalLayout_5->addWidget(label_8_client);

        le_id_client = new QLineEdit(gb_form_client);
        le_id_client->setObjectName("le_id_client");
        le_id_client->setMaxLength(8);

        verticalLayout_5->addWidget(le_id_client);

        label_10_client = new QLabel(gb_form_client);
        label_10_client->setObjectName("label_10_client");

        verticalLayout_5->addWidget(label_10_client);

        le_nom_client = new QLineEdit(gb_form_client);
        le_nom_client->setObjectName("le_nom_client");

        verticalLayout_5->addWidget(le_nom_client);

        label_11_client = new QLabel(gb_form_client);
        label_11_client->setObjectName("label_11_client");

        verticalLayout_5->addWidget(label_11_client);

        le_adresse_client = new QLineEdit(gb_form_client);
        le_adresse_client->setObjectName("le_adresse_client");

        verticalLayout_5->addWidget(le_adresse_client);

        label_12_client = new QLabel(gb_form_client);
        label_12_client->setObjectName("label_12_client");

        verticalLayout_5->addWidget(label_12_client);

        cb_ville_client = new QComboBox(gb_form_client);
        cb_ville_client->addItem(QString());
        cb_ville_client->addItem(QString());
        cb_ville_client->addItem(QString());
        cb_ville_client->addItem(QString());
        cb_ville_client->setObjectName("cb_ville_client");

        verticalLayout_5->addWidget(cb_ville_client);

        label_13_client = new QLabel(gb_form_client);
        label_13_client->setObjectName("label_13_client");

        verticalLayout_5->addWidget(label_13_client);

        dsb_codepostal_client = new QDoubleSpinBox(gb_form_client);
        dsb_codepostal_client->setObjectName("dsb_codepostal_client");

        verticalLayout_5->addWidget(dsb_codepostal_client);

        label_14_client = new QLabel(gb_form_client);
        label_14_client->setObjectName("label_14_client");

        verticalLayout_5->addWidget(label_14_client);

        de_inscription_client = new QDateEdit(gb_form_client);
        de_inscription_client->setObjectName("de_inscription_client");
        de_inscription_client->setCalendarPopup(true);

        verticalLayout_5->addWidget(de_inscription_client);

        label_15_client = new QLabel(gb_form_client);
        label_15_client->setObjectName("label_15_client");

        verticalLayout_5->addWidget(label_15_client);

        le_responsable_client = new QLineEdit(gb_form_client);
        le_responsable_client->setObjectName("le_responsable_client");

        verticalLayout_5->addWidget(le_responsable_client);

        tabWidget_right_client = new QTabWidget(page_client);
        tabWidget_right_client->setObjectName("tabWidget_right_client");
        tabWidget_right_client->setGeometry(QRect(880, 40, 611, 701));
        tab_stats_client = new QWidget();
        tab_stats_client->setObjectName("tab_stats_client");
        gb_stats_client = new QGroupBox(tab_stats_client);
        gb_stats_client->setObjectName("gb_stats_client");
        gb_stats_client->setGeometry(QRect(11, 11, 581, 646));
        gb_stats_client->setFont(font);
        tabWidget_right_client->addTab(tab_stats_client, QString());
        tab_contrat_client = new QWidget();
        tab_contrat_client->setObjectName("tab_contrat_client");
        tab_clients_4 = new QTableView(tab_contrat_client);
        tab_clients_4->setObjectName("tab_clients_4");
        tab_clients_4->setGeometry(QRect(30, 40, 561, 591));
        tab_clients_4->setAlternatingRowColors(true);
        tab_clients_4->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableWidget_5 = new QTableWidget(tab_contrat_client);
        if (tableWidget_5->columnCount() < 7)
            tableWidget_5->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableWidget_5->setObjectName("tableWidget_5");
        tableWidget_5->setGeometry(QRect(120, 100, 401, 411));
        tableWidget_5->setStyleSheet(QString::fromUtf8("border-color: rgb(0, 0, 0);"));
        tabWidget_right_client->addTab(tab_contrat_client, QString());
        tab_clients = new QTableView(page_client);
        tab_clients->setObjectName("tab_clients");
        tab_clients->setGeometry(QRect(240, 90, 621, 591));
        tab_clients->setAlternatingRowColors(true);
        tab_clients->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableWidget_3 = new QTableWidget(page_client);
        if (tableWidget_3->columnCount() < 7)
            tableWidget_3->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(2, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(3, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(4, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(5, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(6, __qtablewidgetitem13);
        tableWidget_3->setObjectName("tableWidget_3");
        tableWidget_3->setGeometry(QRect(280, 200, 551, 411));
        tableWidget_3->setStyleSheet(QString::fromUtf8("border-color: rgb(0, 0, 0);"));
        layoutWidget = new QWidget(page_client);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(240, 690, 621, 41));
        horizontalLayout_client = new QHBoxLayout(layoutWidget);
        horizontalLayout_client->setObjectName("horizontalLayout_client");
        horizontalLayout_client->setContentsMargins(0, 0, 0, 0);
        btn_ajouter_client = new QPushButton(layoutWidget);
        btn_ajouter_client->setObjectName("btn_ajouter_client");
        btn_ajouter_client->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 96, 0);"));

        horizontalLayout_client->addWidget(btn_ajouter_client);

        btn_modifier_client = new QPushButton(layoutWidget);
        btn_modifier_client->setObjectName("btn_modifier_client");

        horizontalLayout_client->addWidget(btn_modifier_client);

        btn_supprimer_client = new QPushButton(layoutWidget);
        btn_supprimer_client->setObjectName("btn_supprimer_client");

        horizontalLayout_client->addWidget(btn_supprimer_client);

        horizontalSpacer_client = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_client->addItem(horizontalSpacer_client);

        btn_pdf_client = new QPushButton(layoutWidget);
        btn_pdf_client->setObjectName("btn_pdf_client");
        btn_pdf_client->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(67, 160, 71);"));

        horizontalLayout_client->addWidget(btn_pdf_client);

        btn_mailing_client = new QPushButton(layoutWidget);
        btn_mailing_client->setObjectName("btn_mailing_client");
        btn_mailing_client->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 136, 229);"));

        horizontalLayout_client->addWidget(btn_mailing_client);

        label_4_client = new QLabel(page_client);
        label_4_client->setObjectName("label_4_client");
        label_4_client->setGeometry(QRect(430, -10, 211, 41));
        label_4_client->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        layoutWidget1 = new QWidget(page_client);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(240, 40, 621, 42));
        horizontalLayout_7_client = new QHBoxLayout(layoutWidget1);
        horizontalLayout_7_client->setObjectName("horizontalLayout_7_client");
        horizontalLayout_7_client->setContentsMargins(0, 0, 0, 0);
        le_recherche_client = new QLineEdit(layoutWidget1);
        le_recherche_client->setObjectName("le_recherche_client");

        horizontalLayout_7_client->addWidget(le_recherche_client);

        comboBox_client = new QComboBox(layoutWidget1);
        comboBox_client->addItem(QString());
        comboBox_client->addItem(QString());
        comboBox_client->addItem(QString());
        comboBox_client->setObjectName("comboBox_client");

        horizontalLayout_7_client->addWidget(comboBox_client);

        btn_tri_client = new QPushButton(layoutWidget1);
        btn_tri_client->setObjectName("btn_tri_client");

        horizontalLayout_7_client->addWidget(btn_tri_client);

        stackedWidget->addWidget(page_client);
        page_employes = new QWidget();
        page_employes->setObjectName("page_employes");
        page_employes->setStyleSheet(QString::fromUtf8("/* --- 1. L'IMAGE DE FOND --- */\n"
"QWidget#page_employes {\n"
"    /* Assure-toi que le chemin de ton image est correct ici ! */\n"
"    border-image: url(:/new/prefix1/images/bg_left.jpg) 0 0 0 0 stretch stretch ;\n"
"}\n"
"\n"
"/* --- 2. LE CADRE FORMULAIRE & STATISTIQUES (Verre Blanc) --- */\n"
"QGroupBox {\n"
"    /* Fond BLANC avec transparence (Alpha = 210 sur 255 pour lisibilit\303\251) */\n"
"    background-color: rgba(255, 255, 255, 210); \n"
"    border: 1px solid rgba(0, 0, 0, 20); /* Bordure grise subtile */\n"
"    border-radius: 15px;\n"
"    margin-top: 25px; \n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top left;\n"
"    left: 20px;\n"
"    color: #2E7D32; /* Vert Fonc\303\251 EcoCycle pour le contraste sur blanc */\n"
"    font-weight: bold;\n"
"    font-size: 16px;\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"/* --- 3. LES LABELS --- */\n"
"QLabel {\n"
"    color: #333333; /* Texte GRIS FONC\303\211/NOIR */\n"
"    font-weight: b"
                        "old;\n"
"    font-size: 13px;\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"/* --- 4. CHAMPS DE SAISIE (Inputs Clairs) --- */\n"
"QLineEdit, QComboBox, QDateEdit, QDoubleSpinBox {\n"
"    /* Fond blanc cass\303\251 semi-transparent */\n"
"    background-color: rgba(245, 245, 245, 200); \n"
"    border: 1px solid #BDBDBD; /* Bordure grise */\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    color: #000000; /* Texte NOIR quand on \303\251crit */\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QLineEdit:focus, QComboBox:focus, QDateEdit:focus {\n"
"    background-color: #FFFFFF; /* Devient blanc pur au clic */\n"
"    border: 2px solid #4CAF50; /* Bordure VERTE */\n"
"}\n"
"\n"
"/* Sp\303\251cifique : La liste de choix de Tri */\n"
"QComboBox#cb_tri_option {\n"
"    min-width: 80px;\n"
"    background-color: rgba(255, 255, 255, 200);\n"
"    color: #333;\n"
"}\n"
"\n"
"/* --- 5. TABLEAU (Liste Blanche) --- */\n"
"QTableView {\n"
"    background-color: rgba(255, 255, 255, 200); \n"
"    border: 1px solid"
                        " rgba(0, 0, 0, 20);\n"
"    border-radius: 8px;\n"
"    gridline-color: rgba(0, 0, 0, 10); /* Lignes grises tr\303\250s fines */\n"
"    color: #000000; /* Texte Noir */\n"
"    selection-background-color: rgba(76, 175, 80, 180); /* Vert s\303\251lection */\n"
"    selection-color: white;\n"
"    outline: 0;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #2E7D32; /* Ent\303\252te Vert Fonc\303\251 */\n"
"    color: white;\n"
"    padding: 8px;\n"
"    border: none;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QTableCornerButton::section {\n"
"    background-color: rgba(255, 255, 255, 200);\n"
"    border: none;\n"
"}\n"
"\n"
"/* --- 6. BOUTONS (Couleurs Vives) --- */\n"
"QPushButton {\n"
"    border-radius: 8px;\n"
"    padding: 10px 15px;\n"
"    color: white; /* Texte Blanc sur bouton color\303\251 */\n"
"    font-weight: bold;\n"
"    font-size: 13px;\n"
"    border: 1px solid rgba(0,0,0, 10); /* L\303\251g\303\250re ombre */\n"
"}\n"
"\n"
"/* --- COULEURS DES BOUTONS --- */\n"
"\n"
"/* Ajoute"
                        "r : Vert */\n"
"QPushButton#btn_ajouter {\n"
"    background-color: #43A047; \n"
"}\n"
"QPushButton#btn_ajouter:hover { background-color: #2E7D32; }\n"
"\n"
"/* Modifier : Bleu */\n"
"QPushButton#btn_modifier {\n"
"    background-color: #1E88E5; \n"
"}\n"
"QPushButton#btn_modifier:hover { background-color: #1565C0; }\n"
"\n"
"/* Supprimer : Rouge */\n"
"QPushButton#btn_supprimer {\n"
"    background-color: #E53935; \n"
"}\n"
"QPushButton#btn_supprimer:hover { background-color: #C62828; }\n"
"\n"
"/* PDF : Gris */\n"
"QPushButton#btn_pdf {\n"
"    background-color: #757575; \n"
"}\n"
"QPushButton#btn_pdf:hover { background-color: #424242; }\n"
"\n"
"/* Contrat : Marron */\n"
"QPushButton#btn_contrat {\n"
"    background-color: #795548;\n"
"}\n"
"QPushButton#btn_contrat:hover { background-color: #5D4037; }\n"
"\n"
"/* Mailing : Violet */\n"
"QPushButton#btn_mailing {\n"
"    background-color: #8E24AA;\n"
"}\n"
"QPushButton#btn_mailing:hover { background-color: #7B1FA2; }\n"
"\n"
"/* TRIER : Jaune/Ambre */\n"
"QP"
                        "ushButton#btn_tri {\n"
"    background-color: #FFC107;\n"
"    color: #333; /* Texte fonc\303\251 sur jaune */\n"
"}\n"
"QPushButton#btn_tri:hover { background-color: #FFB300; }\n"
"\n"
"/* --- SCROLLBAR --- */\n"
"QScrollBar:vertical {\n"
"    border: none;\n"
"    background: rgba(0,0,0,10);\n"
"    width: 10px;\n"
"    border-radius: 5px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background: rgba(0,0,0,30); /* Poign\303\251e grise */\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"/* --- TAB WIDGET (ONGLETS) --- */\n"
"QTabWidget::pane {\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    background-color: transparent; /* Fond transparent pour voir l'image principale */\n"
"    top: -1px; \n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background-color: rgba(255, 255, 255, 150); /* Onglet inactif */\n"
"    border: 1px solid rgba(0,0,0, 10);\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 8px;\n"
"    border-top-right-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    margin-right: 2px;\n"
"    fon"
                        "t-weight: bold;\n"
"    color: #555;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background-color: rgba(255, 255, 255, 220); /* Onglet actif plus blanc */\n"
"    color: #2E7D32; /* Texte Vert EcoCycle */\n"
"    border-top: 3px solid #2E7D32; /* Petite barre verte en haut */\n"
"}\n"
"\n"
"QTabBar::tab:hover {\n"
"    background-color: rgba(255, 255, 255, 180);\n"
"}\n"
""));
        horizontalLayout_3 = new QHBoxLayout(page_employes);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        gb_form = new QGroupBox(page_employes);
        gb_form->setObjectName("gb_form");
        gb_form->setFont(font);
        formLayout = new QFormLayout(gb_form);
        formLayout->setObjectName("formLayout");
        formLayout->setHorizontalSpacing(15);
        formLayout->setVerticalSpacing(15);
        formLayout->setContentsMargins(15, 15, 15, 15);
        label_8 = new QLabel(gb_form);
        label_8->setObjectName("label_8");

        formLayout->setWidget(0, QFormLayout::LabelRole, label_8);

        le_cin = new QLineEdit(gb_form);
        le_cin->setObjectName("le_cin");
        le_cin->setMaxLength(8);

        formLayout->setWidget(0, QFormLayout::FieldRole, le_cin);

        label_10 = new QLabel(gb_form);
        label_10->setObjectName("label_10");

        formLayout->setWidget(1, QFormLayout::LabelRole, label_10);

        le_nom = new QLineEdit(gb_form);
        le_nom->setObjectName("le_nom");

        formLayout->setWidget(1, QFormLayout::FieldRole, le_nom);

        label_11 = new QLabel(gb_form);
        label_11->setObjectName("label_11");

        formLayout->setWidget(2, QFormLayout::LabelRole, label_11);

        le_prenom = new QLineEdit(gb_form);
        le_prenom->setObjectName("le_prenom");

        formLayout->setWidget(2, QFormLayout::FieldRole, le_prenom);

        label_12 = new QLabel(gb_form);
        label_12->setObjectName("label_12");

        formLayout->setWidget(3, QFormLayout::LabelRole, label_12);

        cb_poste = new QComboBox(gb_form);
        cb_poste->addItem(QString());
        cb_poste->addItem(QString());
        cb_poste->addItem(QString());
        cb_poste->addItem(QString());
        cb_poste->setObjectName("cb_poste");

        formLayout->setWidget(3, QFormLayout::FieldRole, cb_poste);

        label_13 = new QLabel(gb_form);
        label_13->setObjectName("label_13");

        formLayout->setWidget(4, QFormLayout::LabelRole, label_13);

        dsb_salaire = new QDoubleSpinBox(gb_form);
        dsb_salaire->setObjectName("dsb_salaire");

        formLayout->setWidget(4, QFormLayout::FieldRole, dsb_salaire);

        label_14 = new QLabel(gb_form);
        label_14->setObjectName("label_14");

        formLayout->setWidget(5, QFormLayout::LabelRole, label_14);

        de_embauche = new QDateEdit(gb_form);
        de_embauche->setObjectName("de_embauche");
        de_embauche->setCalendarPopup(true);

        formLayout->setWidget(5, QFormLayout::FieldRole, de_embauche);

        label_15 = new QLabel(gb_form);
        label_15->setObjectName("label_15");

        formLayout->setWidget(6, QFormLayout::LabelRole, label_15);

        le_badge = new QLineEdit(gb_form);
        le_badge->setObjectName("le_badge");

        formLayout->setWidget(6, QFormLayout::FieldRole, le_badge);


        horizontalLayout_3->addWidget(gb_form);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_4 = new QLabel(page_employes);
        label_4->setObjectName("label_4");
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        verticalLayout_3->addWidget(label_4, 0, Qt::AlignmentFlag::AlignHCenter);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        le_recherche = new QLineEdit(page_employes);
        le_recherche->setObjectName("le_recherche");

        horizontalLayout_7->addWidget(le_recherche);

        comboBox = new QComboBox(page_employes);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        horizontalLayout_7->addWidget(comboBox);

        btn_tri = new QPushButton(page_employes);
        btn_tri->setObjectName("btn_tri");
        btn_tri->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 193, 7);"));

        horizontalLayout_7->addWidget(btn_tri);


        verticalLayout_3->addLayout(horizontalLayout_7);

        tab_employes = new QTableView(page_employes);
        tab_employes->setObjectName("tab_employes");
        tab_employes->setAlternatingRowColors(true);
        tab_employes->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout_3->addWidget(tab_employes);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btn_ajouter = new QPushButton(page_employes);
        btn_ajouter->setObjectName("btn_ajouter");
        btn_ajouter->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 96, 0);"));

        horizontalLayout->addWidget(btn_ajouter);

        btn_modifier = new QPushButton(page_employes);
        btn_modifier->setObjectName("btn_modifier");

        horizontalLayout->addWidget(btn_modifier);

        btn_supprimer = new QPushButton(page_employes);
        btn_supprimer->setObjectName("btn_supprimer");

        horizontalLayout->addWidget(btn_supprimer);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_pdf = new QPushButton(page_employes);
        btn_pdf->setObjectName("btn_pdf");
        btn_pdf->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(67, 160, 71);"));

        horizontalLayout->addWidget(btn_pdf);

        btn_contrat = new QPushButton(page_employes);
        btn_contrat->setObjectName("btn_contrat");
        btn_contrat->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(30, 136, 229);"));

        horizontalLayout->addWidget(btn_contrat);

        btn_mailing = new QPushButton(page_employes);
        btn_mailing->setObjectName("btn_mailing");
        btn_mailing->setStyleSheet(QString::fromUtf8("background-color: rgb(229, 57, 53);"));

        horizontalLayout->addWidget(btn_mailing);


        verticalLayout_3->addLayout(horizontalLayout);


        horizontalLayout_3->addLayout(verticalLayout_3);

        tabWidget_right = new QTabWidget(page_employes);
        tabWidget_right->setObjectName("tabWidget_right");
        tab_stats = new QWidget();
        tab_stats->setObjectName("tab_stats");
        gb_stats = new QGroupBox(tab_stats);
        gb_stats->setObjectName("gb_stats");
        gb_stats->setGeometry(QRect(11, 11, 641, 646));
        gb_stats->setFont(font);
        tabWidget_right->addTab(tab_stats, QString());
        tab_contrat = new QWidget();
        tab_contrat->setObjectName("tab_contrat");
        tabWidget_right->addTab(tab_contrat, QString());

        horizontalLayout_3->addWidget(tabWidget_right);

        horizontalLayout_3->setStretch(2, 1);
        stackedWidget->addWidget(page_employes);
        page_poubelle = new QWidget();
        page_poubelle->setObjectName("page_poubelle");
        page_poubelle->setStyleSheet(QString::fromUtf8("/* --- 1. L'IMAGE DE FOND --- */\n"
"QWidget#page_employes {\n"
"    /* Assure-toi que le chemin de ton image est correct ici ! */\n"
"    border-image: url(:/new/prefix1/images/bg_left.jpg) 0 0 0 0 stretch stretch;\n"
"}\n"
"\n"
"/* --- 2. LE CADRE FORMULAIRE & STATISTIQUES (Verre Blanc) --- */\n"
"QGroupBox {\n"
"    /* Fond BLANC avec transparence (Alpha = 210 sur 255 pour lisibilit\303\251) */\n"
"    background-color: rgba(255, 255, 255, 210); \n"
"    border: 1px solid rgba(0, 0, 0, 20); /* Bordure grise subtile */\n"
"    border-radius: 15px;\n"
"    margin-top: 25px; \n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top left;\n"
"    left: 20px;\n"
"    color: #2E7D32; /* Vert Fonc\303\251 EcoCycle pour le contraste sur blanc */\n"
"    font-weight: bold;\n"
"    font-size: 16px;\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"/* --- 3. LES LABELS --- */\n"
"QLabel {\n"
"    color: #333333; /* Texte GRIS FONC\303\211/NOIR */\n"
"    font-weight: bo"
                        "ld;\n"
"    font-size: 13px;\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"/* --- 4. CHAMPS DE SAISIE (Inputs Clairs) --- */\n"
"QLineEdit, QComboBox, QDateEdit, QDoubleSpinBox {\n"
"    /* Fond blanc cass\303\251 semi-transparent */\n"
"    background-color: rgba(245, 245, 245, 200); \n"
"    border: 1px solid #BDBDBD; /* Bordure grise */\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    color: #000000; /* Texte NOIR quand on \303\251crit */\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QLineEdit:focus, QComboBox:focus, QDateEdit:focus {\n"
"    background-color: #FFFFFF; /* Devient blanc pur au clic */\n"
"    border: 2px solid #4CAF50; /* Bordure VERTE */\n"
"}\n"
"\n"
"/* Sp\303\251cifique : La liste de choix de Tri */\n"
"QComboBox#cb_tri_option {\n"
"    min-width: 80px;\n"
"    background-color: rgba(255, 255, 255, 200);\n"
"    color: #333;\n"
"}\n"
"\n"
"/* --- 5. TABLEAU (Liste Blanche) --- */\n"
"QTableView {\n"
"    background-color: rgba(255, 255, 255, 200); \n"
"    border: 1px solid "
                        "rgba(0, 0, 0, 20);\n"
"    border-radius: 8px;\n"
"    gridline-color: rgba(0, 0, 0, 10); /* Lignes grises tr\303\250s fines */\n"
"    color: #000000; /* Texte Noir */\n"
"    selection-background-color: rgba(76, 175, 80, 180); /* Vert s\303\251lection */\n"
"    selection-color: white;\n"
"    outline: 0;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #2E7D32; /* Ent\303\252te Vert Fonc\303\251 */\n"
"    color: white;\n"
"    padding: 8px;\n"
"    border: none;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QTableCornerButton::section {\n"
"    background-color: rgba(255, 255, 255, 200);\n"
"    border: none;\n"
"}\n"
"\n"
"/* --- 6. BOUTONS (Couleurs Vives) --- */\n"
"QPushButton {\n"
"    border-radius: 8px;\n"
"    padding: 10px 15px;\n"
"    color: white; /* Texte Blanc sur bouton color\303\251 */\n"
"    font-weight: bold;\n"
"    font-size: 13px;\n"
"    border: 1px solid rgba(0,0,0, 10); /* L\303\251g\303\250re ombre */\n"
"}\n"
"\n"
"/* --- COULEURS DES BOUTONS --- */\n"
"\n"
"/* Ajouter"
                        " : Vert */\n"
"QPushButton#btn_ajouter {\n"
"    background-color: #43A047; \n"
"}\n"
"QPushButton#btn_ajouter:hover { background-color: #2E7D32; }\n"
"\n"
"/* Modifier : Bleu */\n"
"QPushButton#btn_modifier {\n"
"    background-color: #1E88E5; \n"
"}\n"
"QPushButton#btn_modifier:hover { background-color: #1565C0; }\n"
"\n"
"/* Supprimer : Rouge */\n"
"QPushButton#btn_supprimer {\n"
"    background-color: #E53935; \n"
"}\n"
"QPushButton#btn_supprimer:hover { background-color: #C62828; }\n"
"\n"
"/* PDF : Gris */\n"
"QPushButton#btn_pdf {\n"
"    background-color: #757575; \n"
"}\n"
"QPushButton#btn_pdf:hover { background-color: #424242; }\n"
"\n"
"/* Contrat : Marron */\n"
"QPushButton#btn_contrat {\n"
"    background-color: #795548;\n"
"}\n"
"QPushButton#btn_contrat:hover { background-color: #5D4037; }\n"
"\n"
"/* Mailing : Violet */\n"
"QPushButton#btn_mailing {\n"
"    background-color: #8E24AA;\n"
"}\n"
"QPushButton#btn_mailing:hover { background-color: #7B1FA2; }\n"
"\n"
"/* TRIER : Jaune/Ambre */\n"
"QPu"
                        "shButton#btn_tri {\n"
"    background-color: #FFC107;\n"
"    color: #333; /* Texte fonc\303\251 sur jaune */\n"
"}\n"
"QPushButton#btn_tri:hover { background-color: #FFB300; }\n"
"\n"
"/* --- SCROLLBAR --- */\n"
"QScrollBar:vertical {\n"
"    border: none;\n"
"    background: rgba(0,0,0,10);\n"
"    width: 10px;\n"
"    border-radius: 5px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background: rgba(0,0,0,30); /* Poign\303\251e grise */\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"/* --- TAB WIDGET (ONGLETS) --- */\n"
"QTabWidget::pane {\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    background-color: transparent; /* Fond transparent pour voir l'image principale */\n"
"    top: -1px; \n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background-color: rgba(255, 255, 255, 150); /* Onglet inactif */\n"
"    border: 1px solid rgba(0,0,0, 10);\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 8px;\n"
"    border-top-right-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    margin-right: 2px;\n"
"    font"
                        "-weight: bold;\n"
"    color: #555;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background-color: rgba(255, 255, 255, 220); /* Onglet actif plus blanc */\n"
"    color: #2E7D32; /* Texte Vert EcoCycle */\n"
"    border-top: 3px solid #2E7D32; /* Petite barre verte en haut */\n"
"}\n"
"\n"
"QTabBar::tab:hover {\n"
"    background-color: rgba(255, 255, 255, 180);\n"
"}\n"
""));
        gb_form_2 = new QGroupBox(page_poubelle);
        gb_form_2->setObjectName("gb_form_2");
        gb_form_2->setGeometry(QRect(10, 50, 321, 661));
        gb_form_2->setFont(font);
        verticalLayout = new QVBoxLayout(gb_form_2);
        verticalLayout->setObjectName("verticalLayout");
        label_24 = new QLabel(gb_form_2);
        label_24->setObjectName("label_24");

        verticalLayout->addWidget(label_24);

        le_cin_2 = new QLineEdit(gb_form_2);
        le_cin_2->setObjectName("le_cin_2");
        le_cin_2->setMaxLength(8);

        verticalLayout->addWidget(le_cin_2);

        label_25 = new QLabel(gb_form_2);
        label_25->setObjectName("label_25");

        verticalLayout->addWidget(label_25);

        le_nom_2 = new QLineEdit(gb_form_2);
        le_nom_2->setObjectName("le_nom_2");

        verticalLayout->addWidget(le_nom_2);

        label_26 = new QLabel(gb_form_2);
        label_26->setObjectName("label_26");

        verticalLayout->addWidget(label_26);

        le_prenom_2 = new QLineEdit(gb_form_2);
        le_prenom_2->setObjectName("le_prenom_2");

        verticalLayout->addWidget(le_prenom_2);

        label_28 = new QLabel(gb_form_2);
        label_28->setObjectName("label_28");

        verticalLayout->addWidget(label_28);

        dsb_salaire_2 = new QDoubleSpinBox(gb_form_2);
        dsb_salaire_2->setObjectName("dsb_salaire_2");

        verticalLayout->addWidget(dsb_salaire_2);

        lbl_page_title_3 = new QLabel(page_poubelle);
        lbl_page_title_3->setObjectName("lbl_page_title_3");
        lbl_page_title_3->setGeometry(QRect(460, 11, 441, 51));
        lbl_page_title_3->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: white; padding: 10px;"));
        lbl_page_title_3->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        tableWidget_2 = new QTableWidget(page_poubelle);
        if (tableWidget_2->columnCount() < 9)
            tableWidget_2->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(3, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(4, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(5, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(6, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(7, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(8, __qtablewidgetitem22);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(390, 230, 591, 371));
        tableWidget_2->setStyleSheet(QString::fromUtf8("border-color: rgb(0, 0, 0);"));
        layoutWidget_3 = new QWidget(page_poubelle);
        layoutWidget_3->setObjectName("layoutWidget_3");
        layoutWidget_3->setGeometry(QRect(360, 120, 631, 42));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget_3);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget_3);
        label->setObjectName("label");
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(197, 197, 197);"));

        horizontalLayout_2->addWidget(label);

        comboBox_client_3 = new QComboBox(layoutWidget_3);
        comboBox_client_3->addItem(QString());
        comboBox_client_3->addItem(QString());
        comboBox_client_3->addItem(QString());
        comboBox_client_3->setObjectName("comboBox_client_3");

        horizontalLayout_2->addWidget(comboBox_client_3);

        btn_tri_client_3 = new QPushButton(layoutWidget_3);
        btn_tri_client_3->setObjectName("btn_tri_client_3");
        btn_tri_client_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 193, 7);"));

        horizontalLayout_2->addWidget(btn_tri_client_3);

        tabWidget_right_2 = new QTabWidget(page_poubelle);
        tabWidget_right_2->setObjectName("tabWidget_right_2");
        tabWidget_right_2->setGeometry(QRect(1000, 60, 491, 851));
        tab_stats_5 = new QWidget();
        tab_stats_5->setObjectName("tab_stats_5");
        gb_stats_3 = new QGroupBox(tab_stats_5);
        gb_stats_3->setObjectName("gb_stats_3");
        gb_stats_3->setGeometry(QRect(11, 11, 471, 601));
        gb_stats_3->setFont(font);
        tabWidget_right_2->addTab(tab_stats_5, QString());
        tab_contrat_3 = new QWidget();
        tab_contrat_3->setObjectName("tab_contrat_3");
        tab_poubelle_2 = new QTableView(tab_contrat_3);
        tab_poubelle_2->setObjectName("tab_poubelle_2");
        tab_poubelle_2->setGeometry(QRect(10, 20, 631, 591));
        tab_poubelle_2->setAlternatingRowColors(true);
        tab_poubelle_2->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tabWidget_right_2->addTab(tab_contrat_3, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        tab_poubelle_3 = new QTableView(tab);
        tab_poubelle_3->setObjectName("tab_poubelle_3");
        tab_poubelle_3->setGeometry(QRect(10, 30, 631, 591));
        tab_poubelle_3->setAlternatingRowColors(true);
        tab_poubelle_3->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tabWidget_right_2->addTab(tab, QString());
        tab_poubelle = new QTableView(page_poubelle);
        tab_poubelle->setObjectName("tab_poubelle");
        tab_poubelle->setGeometry(QRect(360, 170, 631, 491));
        tab_poubelle->setAlternatingRowColors(true);
        tab_poubelle->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        layoutWidget2 = new QWidget(page_poubelle);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(360, 60, 631, 54));
        horizontalLayout_7_client_2 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_7_client_2->setObjectName("horizontalLayout_7_client_2");
        horizontalLayout_7_client_2->setContentsMargins(0, 0, 0, 0);
        le_recherche_client_2 = new QLineEdit(layoutWidget2);
        le_recherche_client_2->setObjectName("le_recherche_client_2");

        horizontalLayout_7_client_2->addWidget(le_recherche_client_2);

        comboBox_client_2 = new QComboBox(layoutWidget2);
        comboBox_client_2->addItem(QString());
        comboBox_client_2->addItem(QString());
        comboBox_client_2->addItem(QString());
        comboBox_client_2->setObjectName("comboBox_client_2");

        horizontalLayout_7_client_2->addWidget(comboBox_client_2);

        btn_tri_client_2 = new QPushButton(layoutWidget2);
        btn_tri_client_2->setObjectName("btn_tri_client_2");
        btn_tri_client_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 193, 7);"));

        horizontalLayout_7_client_2->addWidget(btn_tri_client_2);

        layoutWidget3 = new QWidget(page_poubelle);
        layoutWidget3->setObjectName("layoutWidget3");
        layoutWidget3->setGeometry(QRect(361, 660, 631, 51));
        horizontalLayout_client_2 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_client_2->setObjectName("horizontalLayout_client_2");
        horizontalLayout_client_2->setContentsMargins(0, 0, 0, 0);
        btn_ajouter_client_2 = new QPushButton(layoutWidget3);
        btn_ajouter_client_2->setObjectName("btn_ajouter_client_2");
        btn_ajouter_client_2->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 96, 0);"));

        horizontalLayout_client_2->addWidget(btn_ajouter_client_2);

        btn_modifier_client_2 = new QPushButton(layoutWidget3);
        btn_modifier_client_2->setObjectName("btn_modifier_client_2");
        btn_modifier_client_2->setStyleSheet(QString::fromUtf8("background-color: rgb(26, 120, 202);"));

        horizontalLayout_client_2->addWidget(btn_modifier_client_2);

        btn_supprimer_client_2 = new QPushButton(layoutWidget3);
        btn_supprimer_client_2->setObjectName("btn_supprimer_client_2");
        btn_supprimer_client_2->setStyleSheet(QString::fromUtf8("background-color: rgb(226, 56, 52);"));

        horizontalLayout_client_2->addWidget(btn_supprimer_client_2);

        horizontalSpacer_client_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_client_2->addItem(horizontalSpacer_client_2);

        btn_pdf_client_2 = new QPushButton(layoutWidget3);
        btn_pdf_client_2->setObjectName("btn_pdf_client_2");
        btn_pdf_client_2->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(67, 160, 71);"));

        horizontalLayout_client_2->addWidget(btn_pdf_client_2);

        stackedWidget->addWidget(page_poubelle);
        layoutWidget2->raise();
        layoutWidget2->raise();
        gb_form_2->raise();
        lbl_page_title_3->raise();
        tab_poubelle->raise();
        tableWidget_2->raise();
        layoutWidget_3->raise();
        tabWidget_right_2->raise();

        horizontalLayout_4->addWidget(widget_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1587, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(3);
        tabWidget_right_contrat->setCurrentIndex(0);
        tabWidget_right_contrat_2->setCurrentIndex(0);
        tabWidget_Stock->setCurrentIndex(2);
        tabWidget_right_client->setCurrentIndex(0);
        tabWidget_right->setCurrentIndex(0);
        tabWidget_right_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_9->setText(QString());
        btn_nav_employes->setText(QCoreApplication::translate("MainWindow", "Gestion employ\303\251", nullptr));
        btn_nav_client->setText(QCoreApplication::translate("MainWindow", "Gestion client", nullptr));
        btn_nav_contrat->setText(QCoreApplication::translate("MainWindow", "Gestion contrat", nullptr));
        btn_nav_poubelle->setText(QCoreApplication::translate("MainWindow", "Gestion de poubelles", nullptr));
        btn_nav_equipements->setText(QCoreApplication::translate("MainWindow", "Gestion d'equipement", nullptr));
        btn_nav_stock->setText(QCoreApplication::translate("MainWindow", "Gestion de stock", nullptr));
        btn_quit->setText(QCoreApplication::translate("MainWindow", "      Quitter", nullptr));
        label_logo_dash->setText(QString());
        label_welcome->setText(QCoreApplication::translate("MainWindow", "Welcome, Admin", nullptr));
        lbl_page_title->setText(QCoreApplication::translate("MainWindow", "Gestion Contrat", nullptr));
        cb_tri_option->setItemText(0, QCoreApplication::translate("MainWindow", "ID", nullptr));
        cb_tri_option->setItemText(1, QCoreApplication::translate("MainWindow", "Type contrat", nullptr));
        cb_tri_option->setItemText(2, QCoreApplication::translate("MainWindow", "Type mati\303\250re", nullptr));

        btn_tri_2->setText(QCoreApplication::translate("MainWindow", "\360\237\224\203 Trier", nullptr));
        le_recherche_4->setText(QString());
        le_recherche_4->setPlaceholderText(QCoreApplication::translate("MainWindow", "\360\237\224\215 Rechercher par ID, Type, Mati\303\250re...", nullptr));
        gb_stats_contrat->setTitle(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        tabWidget_right_contrat->setTabText(tabWidget_right_contrat->indexOf(tab_stats_client_2), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        tabWidget_right_contrat->setTabText(tabWidget_right_contrat->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Page", nullptr));
        lbl_info_contrat_title->setText(QCoreApplication::translate("MainWindow", "\360\237\223\235 Informations Contrat", nullptr));
        group_informations->setTitle(QString());
        label_id->setText(QCoreApplication::translate("MainWindow", "ID Contrat", nullptr));
        le_ID_Contrat->setText(QString());
        label_type->setText(QCoreApplication::translate("MainWindow", "Type Contrat", nullptr));
        cb_Type_Contrat->setItemText(0, QCoreApplication::translate("MainWindow", "Vente", nullptr));
        cb_Type_Contrat->setItemText(1, QCoreApplication::translate("MainWindow", "Achat", nullptr));

        label_matiere->setText(QCoreApplication::translate("MainWindow", "Type Mati\303\250re", nullptr));
        cb_Type_Matiere->setItemText(0, QCoreApplication::translate("MainWindow", "Plastique", nullptr));
        cb_Type_Matiere->setItemText(1, QCoreApplication::translate("MainWindow", "Verre", nullptr));
        cb_Type_Matiere->setItemText(2, QCoreApplication::translate("MainWindow", "Carton", nullptr));
        cb_Type_Matiere->setItemText(3, QCoreApplication::translate("MainWindow", "Aluminium", nullptr));
        cb_Type_Matiere->setItemText(4, QCoreApplication::translate("MainWindow", "Papier", nullptr));

        label_qualite->setText(QCoreApplication::translate("MainWindow", "Qualit\303\251 Mati\303\250re", nullptr));
        cb_Qualite_Matiere->setItemText(0, QCoreApplication::translate("MainWindow", "Grade A", nullptr));
        cb_Qualite_Matiere->setItemText(1, QCoreApplication::translate("MainWindow", "Grade B", nullptr));
        cb_Qualite_Matiere->setItemText(2, QCoreApplication::translate("MainWindow", "Grade C", nullptr));

        label_date_deb->setText(QCoreApplication::translate("MainWindow", "Date D\303\251but", nullptr));
        label_date_fin->setText(QCoreApplication::translate("MainWindow", "Date Fin", nullptr));
        label_duree->setText(QCoreApplication::translate("MainWindow", "Dur\303\251e (mois)", nullptr));
        label_prix->setText(QCoreApplication::translate("MainWindow", "Prix Unitaire (DT / tonne)", nullptr));
        label_statut->setText(QCoreApplication::translate("MainWindow", "Statut Contrat", nullptr));
        cb_statut_contrat->setItemText(0, QCoreApplication::translate("MainWindow", "Actif", nullptr));
        cb_statut_contrat->setItemText(1, QCoreApplication::translate("MainWindow", "Suspendu", nullptr));
        cb_statut_contrat->setItemText(2, QCoreApplication::translate("MainWindow", "R\303\251sili\303\251", nullptr));
        cb_statut_contrat->setItemText(3, QCoreApplication::translate("MainWindow", "Expir\303\251", nullptr));

        label_details->setText(QCoreApplication::translate("MainWindow", "D\303\251tails du contrat", nullptr));
        btn_export_pdf->setText(QCoreApplication::translate("MainWindow", "\360\237\223\204 Exporter PDF", nullptr));
        btn_modifier_3->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217 Modifier", nullptr));
        btn_ajouter_3->setText(QCoreApplication::translate("MainWindow", "\342\236\225 Ajouter", nullptr));
        btn_supprimer_4->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        headerLabel->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt;\">Gestion d'\303\251quipements</span></p></body></html>", nullptr));
        infoGroupBox_5->setTitle(QCoreApplication::translate("MainWindow", "Informations \303\211quipement", nullptr));
        labelNomEquipement->setText(QCoreApplication::translate("MainWindow", "Nom d'\303\251quipement", nullptr));
        labelTypeEquipement->setText(QCoreApplication::translate("MainWindow", "Type d'\303\251quipement", nullptr));
        comboBoxTypeEquipement->setItemText(0, QCoreApplication::translate("MainWindow", "Ordinateur", nullptr));
        comboBoxTypeEquipement->setItemText(1, QCoreApplication::translate("MainWindow", "Imprimante", nullptr));
        comboBoxTypeEquipement->setItemText(2, QCoreApplication::translate("MainWindow", "Scanner", nullptr));
        comboBoxTypeEquipement->setItemText(3, QCoreApplication::translate("MainWindow", "Projecteur", nullptr));
        comboBoxTypeEquipement->setItemText(4, QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone", nullptr));

        labelFabricant_7->setText(QCoreApplication::translate("MainWindow", "Fabricant", nullptr));
        comboBoxFabricant_7->setItemText(0, QCoreApplication::translate("MainWindow", "HP", nullptr));
        comboBoxFabricant_7->setItemText(1, QCoreApplication::translate("MainWindow", "Dell", nullptr));
        comboBoxFabricant_7->setItemText(2, QCoreApplication::translate("MainWindow", "Lenovo", nullptr));
        comboBoxFabricant_7->setItemText(3, QCoreApplication::translate("MainWindow", "Apple", nullptr));
        comboBoxFabricant_7->setItemText(4, QCoreApplication::translate("MainWindow", "Canon", nullptr));
        comboBoxFabricant_7->setItemText(5, QCoreApplication::translate("MainWindow", "Epson", nullptr));
        comboBoxFabricant_7->setItemText(6, QCoreApplication::translate("MainWindow", "Samsung", nullptr));

        pushButtonAjouter->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        le_recherche_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "Rechercher par Fabricant, Type...", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("MainWindow", "Fabricant", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("MainWindow", "Status", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("MainWindow", "Type", nullptr));

        btn_tri_3->setText(QCoreApplication::translate("MainWindow", "Trier", nullptr));
        btn_ajouter_2->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        btn_modifier_2->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        btn_supprimer_2->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        btn_pdf_2->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        btn_mailing_2->setText(QCoreApplication::translate("MainWindow", "Analyse Notes AI", nullptr));
        gb_stats_contrat_3->setTitle(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        tabWidget_right_contrat_2->setTabText(tabWidget_right_contrat_2->indexOf(tab_stats_client_4), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        tabWidget_right_contrat_2->setTabText(tabWidget_right_contrat_2->indexOf(tab_4), QCoreApplication::translate("MainWindow", "Assignation Optimale (AI)", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Op\303\251rations", nullptr));
        label_31->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">ID Lot :</span></p></body></html>", nullptr));
        label_34->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">Type Mati\303\250re :</span></p></body></html>", nullptr));
        cb_type->setItemText(0, QCoreApplication::translate("MainWindow", "Plastique", nullptr));
        cb_type->setItemText(1, QCoreApplication::translate("MainWindow", "Verre", nullptr));
        cb_type->setItemText(2, QCoreApplication::translate("MainWindow", "Aluminium", nullptr));
        cb_type->setItemText(3, QCoreApplication::translate("MainWindow", "Papier", nullptr));

        label_33->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">Poids (kg) :</span></p></body></html>", nullptr));
        label_35->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">Prix Vente :</span></p></body></html>", nullptr));
        label_30->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">Date Entr\303\251e :</span></p></body></html>", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">Statut :</span></p></body></html>", nullptr));
        cb_statut->setItemText(0, QCoreApplication::translate("MainWindow", "Disponible", nullptr));
        cb_statut->setItemText(1, QCoreApplication::translate("MainWindow", "R\303\251serv\303\251", nullptr));
        cb_statut->setItemText(2, QCoreApplication::translate("MainWindow", "Vendu", nullptr));

        label_29->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">Etat :</span></p></body></html>", nullptr));
        cb_etat->setItemText(0, QCoreApplication::translate("MainWindow", "Brut", nullptr));
        cb_etat->setItemText(1, QCoreApplication::translate("MainWindow", "Compact\303\251", nullptr));
        cb_etat->setItemText(2, QCoreApplication::translate("MainWindow", "Tri\303\251", nullptr));

        tabWidget_Stock->setTabText(tabWidget_Stock->indexOf(tab_stats_3), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        tabWidget_Stock->setTabText(tabWidget_Stock->indexOf(tab_historique), QCoreApplication::translate("MainWindow", "Historique", nullptr));
        tabWidget_Stock->setTabText(tabWidget_Stock->indexOf(tab_finance), QCoreApplication::translate("MainWindow", "Valorisation Financi\303\251re", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Recherche et Tri", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">Rechercher par :</span></p></body></html>", nullptr));
        cb_chercher->setItemText(0, QCoreApplication::translate("MainWindow", "ID", nullptr));
        cb_chercher->setItemText(1, QCoreApplication::translate("MainWindow", "Type", nullptr));
        cb_chercher->setItemText(2, QCoreApplication::translate("MainWindow", "Statut", nullptr));

        btn_chercher->setText(QCoreApplication::translate("MainWindow", "Chercher", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">Trier par :</span></p></body></html>", nullptr));
        cb_trier->setItemText(0, QCoreApplication::translate("MainWindow", "Date", nullptr));
        cb_trier->setItemText(1, QCoreApplication::translate("MainWindow", "Poids", nullptr));
        cb_trier->setItemText(2, QCoreApplication::translate("MainWindow", "Prix", nullptr));

        btn_trier_2->setText(QCoreApplication::translate("MainWindow", "Trier", nullptr));
        label_75->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt; color:#ffffff;\">Gestion Du Stock Collect\303\251</span></p></body></html>", nullptr));
        btn_ajouter_4->setText(QCoreApplication::translate("MainWindow", "AJOUTER", nullptr));
        btn_modifier_4->setText(QCoreApplication::translate("MainWindow", "MODIFIER", nullptr));
        btn_supprimer_5->setText(QCoreApplication::translate("MainWindow", "SUPPRIMER", nullptr));
        btn_pdf_3->setText(QCoreApplication::translate("MainWindow", "Exporter PDF", nullptr));
        gb_form_client->setTitle(QCoreApplication::translate("MainWindow", "Informations Client", nullptr));
        label_8_client->setText(QCoreApplication::translate("MainWindow", "ID Client", nullptr));
        le_id_client->setPlaceholderText(QCoreApplication::translate("MainWindow", "12345678", nullptr));
        label_10_client->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        le_nom_client->setText(QString());
        le_nom_client->setPlaceholderText(QCoreApplication::translate("MainWindow", "Soci\303\251t\303\251 ABC", nullptr));
        label_11_client->setText(QCoreApplication::translate("MainWindow", "Adresse", nullptr));
        le_adresse_client->setText(QString());
        le_adresse_client->setPlaceholderText(QCoreApplication::translate("MainWindow", "123 Rue de la R\303\251publique", nullptr));
        label_12_client->setText(QCoreApplication::translate("MainWindow", "Ville", nullptr));
        cb_ville_client->setItemText(0, QCoreApplication::translate("MainWindow", "Tunis", nullptr));
        cb_ville_client->setItemText(1, QCoreApplication::translate("MainWindow", "Sfax", nullptr));
        cb_ville_client->setItemText(2, QCoreApplication::translate("MainWindow", "Sousse", nullptr));
        cb_ville_client->setItemText(3, QCoreApplication::translate("MainWindow", "Bizerte", nullptr));

        cb_ville_client->setPlaceholderText(QCoreApplication::translate("MainWindow", "Tunis", nullptr));
        label_13_client->setText(QCoreApplication::translate("MainWindow", "Code Postal", nullptr));
        label_14_client->setText(QCoreApplication::translate("MainWindow", "Date Inscription", nullptr));
        label_15_client->setText(QCoreApplication::translate("MainWindow", "Responsable", nullptr));
        gb_stats_client->setTitle(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        tabWidget_right_client->setTabText(tabWidget_right_client->indexOf(tab_stats_client), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_5->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID_client", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_5->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Nom_client", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_5->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Code postale", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_5->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Nature_du_Flux", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_5->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Points_de_fid\303\251lit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_5->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "PDG", nullptr));
        tabWidget_right_client->setTabText(tabWidget_right_client->indexOf(tab_contrat_client), QCoreApplication::translate("MainWindow", "Rendement des clients ", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_3->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "ID_client", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_3->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Nom_client", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_3->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_3->horizontalHeaderItem(3);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Code postale", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_3->horizontalHeaderItem(4);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Nature_du_Flux", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_3->horizontalHeaderItem(5);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "Points_de_fid\303\251lit\303\251", nullptr));
        btn_ajouter_client->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        btn_modifier_client->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        btn_supprimer_client->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        btn_pdf_client->setText(QCoreApplication::translate("MainWindow", "Pdf", nullptr));
        btn_mailing_client->setText(QCoreApplication::translate("MainWindow", "sms", nullptr));
        label_4_client->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt;\">Gestion des clients</span></p></body></html>", nullptr));
        le_recherche_client->setPlaceholderText(QCoreApplication::translate("MainWindow", "Rechercher par Nom, ID...", nullptr));
        comboBox_client->setItemText(0, QCoreApplication::translate("MainWindow", "ID Client", nullptr));
        comboBox_client->setItemText(1, QCoreApplication::translate("MainWindow", "Responsable", nullptr));
        comboBox_client->setItemText(2, QCoreApplication::translate("MainWindow", "Nom", nullptr));

        btn_tri_client->setText(QCoreApplication::translate("MainWindow", "Trier", nullptr));
        gb_form->setTitle(QCoreApplication::translate("MainWindow", "Informations Employ\303\251", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        le_cin->setPlaceholderText(QCoreApplication::translate("MainWindow", "12345678", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        le_nom->setText(QString());
        le_nom->setPlaceholderText(QCoreApplication::translate("MainWindow", "Ben Ali", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Prenom", nullptr));
        le_prenom->setText(QString());
        le_prenom->setPlaceholderText(QCoreApplication::translate("MainWindow", "Ahmed", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Poste", nullptr));
        cb_poste->setItemText(0, QCoreApplication::translate("MainWindow", "Responsable RH", nullptr));
        cb_poste->setItemText(1, QCoreApplication::translate("MainWindow", "Administratif", nullptr));
        cb_poste->setItemText(2, QCoreApplication::translate("MainWindow", "Trieur", nullptr));
        cb_poste->setItemText(3, QCoreApplication::translate("MainWindow", "Chauffeur", nullptr));

        cb_poste->setPlaceholderText(QCoreApplication::translate("MainWindow", "Reponsable RH", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Salaire", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Date ", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "ID Badge", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt;\">Gestion des employ\303\251s</span></p></body></html>", nullptr));
        le_recherche->setPlaceholderText(QCoreApplication::translate("MainWindow", "Rechercher par Nom, CIN...", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "CIN", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "ID Badge", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Nom", nullptr));

        btn_tri->setText(QCoreApplication::translate("MainWindow", "Trier", nullptr));
        btn_ajouter->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        btn_modifier->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        btn_supprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        btn_pdf->setText(QCoreApplication::translate("MainWindow", "Pdf", nullptr));
        btn_contrat->setText(QCoreApplication::translate("MainWindow", "Contrat", nullptr));
        btn_mailing->setText(QCoreApplication::translate("MainWindow", "Mailing", nullptr));
        gb_stats->setTitle(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        tabWidget_right->setTabText(tabWidget_right->indexOf(tab_stats), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        tabWidget_right->setTabText(tabWidget_right->indexOf(tab_contrat), QCoreApplication::translate("MainWindow", "Contrat", nullptr));
        gb_form_2->setTitle(QCoreApplication::translate("MainWindow", "Informations Poubelle", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "ID_poubelle", nullptr));
        le_cin_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "12345678", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "Adresse", nullptr));
        le_nom_2->setText(QString());
        le_nom_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "ariana, ghazela", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "Type-dechet", nullptr));
        le_prenom_2->setText(QString());
        le_prenom_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "plastiques...", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "Capacit\303\251(L)", nullptr));
        lbl_page_title_3->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt;\">Gestion des poubelles intelligentes</span></p></body></html>", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "ID_poubelles", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "Type_Dechet", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget_2->horizontalHeaderItem(3);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "Capacit\303\251(L)", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget_2->horizontalHeaderItem(4);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "Niveau_Remp", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = tableWidget_2->horizontalHeaderItem(5);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("MainWindow", "etat_batterie", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = tableWidget_2->horizontalHeaderItem(6);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("MainWindow", "Statut_capteur", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = tableWidget_2->horizontalHeaderItem(7);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("MainWindow", "date_installation", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tableWidget_2->horizontalHeaderItem(8);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("MainWindow", "date_derni\303\250re_collecte", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:10pt;\">Trier par </span></p></body></html>", nullptr));
        comboBox_client_3->setItemText(0, QCoreApplication::translate("MainWindow", "Niveau de remplissage ", nullptr));
        comboBox_client_3->setItemText(1, QCoreApplication::translate("MainWindow", "Etat batterie", nullptr));
        comboBox_client_3->setItemText(2, QCoreApplication::translate("MainWindow", "Adresse", nullptr));

        btn_tri_client_3->setText(QCoreApplication::translate("MainWindow", "Trier", nullptr));
        gb_stats_3->setTitle(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        tabWidget_right_2->setTabText(tabWidget_right_2->indexOf(tab_stats_5), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        tabWidget_right_2->setTabText(tabWidget_right_2->indexOf(tab_contrat_3), QCoreApplication::translate("MainWindow", "Carte g\303\251olocalisation", nullptr));
        tabWidget_right_2->setTabText(tabWidget_right_2->indexOf(tab), QCoreApplication::translate("MainWindow", "Pr\303\251diction AI", nullptr));
        le_recherche_client_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "Chercher une poubelle", nullptr));
        comboBox_client_2->setItemText(0, QCoreApplication::translate("MainWindow", "ID poubelle", nullptr));
        comboBox_client_2->setItemText(1, QCoreApplication::translate("MainWindow", "Type de d\303\251chets", nullptr));
        comboBox_client_2->setItemText(2, QCoreApplication::translate("MainWindow", "Adresse", nullptr));

        btn_tri_client_2->setText(QCoreApplication::translate("MainWindow", "chercher", nullptr));
        btn_ajouter_client_2->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        btn_modifier_client_2->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        btn_supprimer_client_2->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        btn_pdf_client_2->setText(QCoreApplication::translate("MainWindow", "Pdf", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
