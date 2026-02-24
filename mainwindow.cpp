#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Initialisation de l'affichage
    ui->stackedWidget->setCurrentIndex(0); // Dashboard par défaut
    ui->navbar_container->hide();

    // 2. INITIALISATION DES MANAGERS (Très important pour le CRUD)
    employe = new Employe(ui, this);
    client  = new Client(ui, this); // Initialise ta gestion client

    // ==================== DASHBOARD CONNECTIONS ====================
    connect(ui->btn_dash_employe, &QPushButton::clicked, this, [this]() {
        qDebug() << "Vers page Employe (6)";
        navigateToPage(6);
    });

    connect(ui->btn_dash_client, &QPushButton::clicked, this, [this]() {
        qDebug() << "Vers page Client (5)";
        navigateToPage(5);
    });

    connect(ui->btn_dash_contrat, &QPushButton::clicked, this, [this]() { navigateToPage(2); });
    connect(ui->btn_dash_poubelle, &QPushButton::clicked, this, [this]() { navigateToPage(7); });
    connect(ui->btn_dash_equipement, &QPushButton::clicked, this, [this]() { navigateToPage(3); });
    connect(ui->btn_dash_stock, &QPushButton::clicked, this, [this]() { navigateToPage(4); });


    // ==================== SIDEBAR VISIBILITY ====================
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [this](int index) {
        if (index == 0) {
            ui->navbar_container->hide();
        } else {
            ui->navbar_container->show();
        }
    });

    // ==================== NAVIGATION (SIDEBAR) ====================
    connect(ui->btn_nav_employes, &QPushButton::clicked, this, [this]() {
        navigateToPage(6);
    });

    connect(ui->btn_nav_client, &QPushButton::clicked, this, [this]() {
        navigateToPage(5);
    });

    connect(ui->btn_nav_poubelle, &QPushButton::clicked, this, [this]() {
        navigateToPage(7);
    });

    connect(ui->btn_nav_contrat, &QPushButton::clicked, this, [this]() {
        navigateToPage(2);
    });

    connect(ui->btn_nav_stock, &QPushButton::clicked, this, [this]() {
        navigateToPage(4);
    });

    connect(ui->btn_nav_equipements, &QPushButton::clicked, this, [this]() {
        navigateToPage(3);
    });

    connect(ui->btn_quit, &QPushButton::clicked, this, &QMainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::navigateToPage(int pageIndex)
{
    // Vérifie si l'index existe dans ton StackedWidget
    if(pageIndex < ui->stackedWidget->count()) {
        ui->stackedWidget->setCurrentIndex(pageIndex);
    } else {
        qDebug() << "Erreur : L'index de page" << pageIndex << "n'existe pas !";
    }
}

void MainWindow::on_btn_nav_employes_clicked() {}
