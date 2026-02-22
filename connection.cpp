#include "connection.h"

Connection::Connection()
{
}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", CONNECTION_NAME);
    db.setDatabaseName("DRIVER={Oracle in XE};DBQ=XE;UID=ECOCYCLEUSER;PWD=esprit2025;");

    if (db.open())
    {
        test = true;
    }
    else
    {
        qDebug() << "Error:" << db.lastError().text();
    }

    return test;
}