#ifndef SUBTABLE_H
#define SUBTABLE_H

#include "include.h"
class NotEditableDelegate : public QStyledItemDelegate{
Q_OBJECT
public:
    NotEditableDelegate(QWidget *parent = 0):QStyledItemDelegate(parent){}
    QWidget* createEditor ( QWidget*, const QStyleOptionViewItem&, const QModelIndex& ) const
    {
        return 0;
    }
};


class SubTable : public QWidget
{
    Q_OBJECT
    void makeGui();
    QMap<QString,QString> parameters;

    void conditionalAppearance();
public:
    QTableWidget *t;
    QTableWidgetItem* isNeedWrite;
    QString table_name;
    QList<QTableWidgetItem*>listUpdateTW;
    int _Version;
    explicit SubTable(QMap<QString,QString> parameters,QWidget *parent = 0);
signals:
    void sin_NewRow(QString table_name);
    void sig_refresh(QString table_name);
public slots:
    void action_addNewRow();
    void action_Refresh();
    void addNewRow(QString table,QString nomber);
    void refresh(QMap<QString, QString> attr, QList<QMap<QString, QString> > data);

    void closeEditor(QWidget *w, QAbstractItemDelegate::EndEditHint wtf);
};

#endif // SUBTABLE_H
