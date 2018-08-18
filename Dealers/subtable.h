#ifndef SUBTABLE_H
#define SUBTABLE_H

#include "include.h"

class SubTable : public QWidget
{
    Q_OBJECT
    QString table_name;
    QTableWidget *t;
public:
    explicit SubTable(QString table_name,QWidget *parent = 0);

signals:

public slots:
};

#endif // SUBTABLE_H
