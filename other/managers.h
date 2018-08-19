#ifndef MANAGERS_H
#define MANAGERS_H

#include "include.h"
#include "sql/s.h"
class Managers : public QWidget
{
    Q_OBJECT
    QTableWidget *dl;
    void makeFormList();
    void makeFormObject(QString _code);
public:
    explicit Managers(QMap<QString,QString> parameters,QWidget *parent = 0);

signals:
    void OpenManagers(QString _code);
public slots:
    void refreshList();
    void openObject();
};

#endif // MANAGERS_H
