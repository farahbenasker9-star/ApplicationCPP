#include "connection.h"
#include <QMessageBox>  // Add this include

Connection::Connection()
{
}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Proje2A");
    db.setUserName("Amine");
    db.setPassword("esprit18");

    if (db.open())
    {
        test = true;
        QMessageBox::information(nullptr, "Connection Status",
                                 "Database connected successfully!");
    }
    else
    {
        QMessageBox::critical(nullptr, "Connection Error",
                              "Failed to connect to database!\n\nError: " + db.lastError().text());
    }

    return test;
}
