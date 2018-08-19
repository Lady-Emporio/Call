#ifndef ORDER_H
#define ORDER_H

#include "include.h"
#include "sql/s.h"
class AnyTable : public QWidget
{
    Q_OBJECT
    QMap<QString,QLineEdit*>edits;
    QTableWidget *dl;
    void makeList();
    void makeObject();
    void refreshObject();
    QMap<QString,QString>attr;
    QString table;
public:
    explicit AnyTable(QMap<QString,QString>parameters, QMap<QString, QString> attr, QWidget *parent = 0);

signals:
    sig_anyOpen(QMap<QString,QString>parameters);
public slots:
    void action_RefreshList();
    void action_Open();
    void action_Create();
    void action_UpdateExist();
    void action_CreateObject();
};

#endif // ORDER_H
