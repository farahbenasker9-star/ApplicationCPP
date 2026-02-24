#include "connection.h"

bool Connection::createconnect() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", CONNECTION_NAME);
    db.setDatabaseName("DRIVER={Oracle in XE};DBQ=XE;UID=ECOCYCLEUSER;PWD=esprit2025;");

    if (db.open()) return true;
    else {
        qDebug() << "Erreur de connexion Oracle:" << db.lastError().text();
        return false;
    }
}
