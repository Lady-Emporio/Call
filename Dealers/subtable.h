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
class TableWidgetItem : public QTableWidgetItem
{
public:
    explicit TableWidgetItem(const QString &text, int type = Type):QTableWidgetItem(text,type){}

    bool operator< (const QTableWidgetItem&  other) const{
        return QDateTime::fromString(this->text(),"dd.MM.yyyy HH:mm:ss")>QDateTime::fromString(other.text(),"dd.MM.yyyy HH:mm:ss");
    }

};

class SubTable : public QWidget
{
    Q_OBJECT
    void makeGui();
    QMap<QString,QString> parameters;
public:
    void conditionalAppearance();
    QTableWidget *t;
    QTableWidgetItem* isNeedWrite;
    QString table_name;
    QList<QTableWidgetItem*>listUpdateTW;
    int _Version;
    QList<QTableWidgetItem*>conditionalAppearanceList;
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
