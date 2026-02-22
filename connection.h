#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

// Use this name when calling QSqlDatabase::database(CONNECTION_NAME) elsewhere in the app
#define CONNECTION_NAME "conn_main"

class Connection
{
public:
    Connection();
    bool createconnect();
};

#endif // CONNECTION_H