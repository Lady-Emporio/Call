#ifndef S_H
#define S_H

#include "include.h"
//function for work with sql.
//in begin create static class and all life app work with him.
//all work with db make this class
class S
{
    S();
    QSqlDatabase db;
    bool openDB();
    void createTables();
    void getError(QSqlQuery *query=nullptr,QString title="");
public:
    static S *s();

    QMap<QString,QString>getDealer_objectForm(QString code);
    QList<QMap<QString,QString>>getDealer_listForm();
    void createNewDealer(QMap<QString,QString> attributes);
    void updateDealer(QMap<QString,QString> attributes);

    QList<QMap<QString,QString>>getFullCall_listForm(QString code);
    void createFullCall(QMap<QString,QString> attributes);
    void updateFullCall(QMap<QString,QString> attributes);

    QList<QMap<QString,QString>>getEmail_listForm(QString code);
    void createEmail(QMap<QString,QString> attributes);
    void updateEmail(QMap<QString,QString> attributes);
};

#endif // S_H
