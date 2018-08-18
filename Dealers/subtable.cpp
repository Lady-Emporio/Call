#include "subtable.h"

SubTable::SubTable(QMap<QString,QString> parameters,QWidget *parent) : QWidget(parent),parameters(parameters)
{
    table_name=parameters.value("table_name");
    isNeedWrite=nullptr;
    _Version=0;
    this->setObjectName("SubTable");
    makeGui();
}


void SubTable::makeGui()
{
    t=new QTableWidget(this);
    QStringList headers;
    if(table_name=="full_call"){
        headers<<"№"<<"Дата"<<"Коммент"<<"Источник";
    }else if(table_name=="email"){
        headers<<"№"<<"Дата"<<"Почта"<<"Коммент";
    }
    t->setColumnCount(4);
    t->setHorizontalHeaderLabels(headers);
    t->setSortingEnabled(true);
    t->setItemDelegateForColumn(0,new NotEditableDelegate(this));

    connect(t->itemDelegate(), SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)), this,
            SLOT(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)));

    QVBoxLayout * mainLayout=new QVBoxLayout(this);
    this->setLayout(mainLayout);
    mainLayout->addWidget(t);
    QMenuBar * mainMenu=new QMenuBar(this);
    mainLayout->setMenuBar(mainMenu);

    mainMenu->addAction("Add",this,SLOT(action_addNewRow()));
    mainMenu->addAction("Refresh",this,SLOT(action_Refresh()));
}

void SubTable::conditionalAppearance()
{
    if(isNeedWrite!=nullptr){
        int row=t->row(isNeedWrite);
        for(int i=0;i!=t->columnCount();++i){
            QTableWidgetItem * item=t->item(row,i);
            //QBrush colorNewRow;
            //colorNewRow.setColor(QColor(255,0,0));
            //item->setBackground(Qt::red);
            //item->setText(QString("").setNum(i)+"|"+QString("").setNum(isNeedWrite));
            item->setBackgroundColor(Qt::red);
        }
    }
}

void SubTable::refresh(QMap<QString, QString> attr, QList<QMap<QString, QString> > data)
{
    if(attr.value("table")==table_name){
        ++_Version;
        t->clearContents();
        t->setRowCount(data.length());
        for(int i=0;i!=data.length();++i){
            QMap<QString,QString> row=data[i];
            if(table_name=="full_call"){
                QTableWidgetItem * c0=new QTableWidgetItem(row.value("_row"));
                QTableWidgetItem * c1=new QTableWidgetItem(row.value("_date"));
                QTableWidgetItem * c2=new QTableWidgetItem(row.value("_comment"));
                QTableWidgetItem * c3=new QTableWidgetItem(row.value("_source"));
                t->setItem(i,0,c0);
                t->setItem(i,1,c1);
                t->setItem(i,2,c2);
                t->setItem(i,3,c3);
            }else if(table_name=="email"){
                t->setItem(i,0,new QTableWidgetItem(row.value("_row")));
                t->setItem(i,1,new QTableWidgetItem(row.value("_date")));
                t->setItem(i,2,new QTableWidgetItem(row.value("_mail")));
                t->setItem(i,3,new QTableWidgetItem(row.value("_comment")));
            }
        }
    }
}

void SubTable::closeEditor(QWidget *w, QAbstractItemDelegate::EndEditHint wtf)
{
    static int localVersion=_Version;
    if(localVersion!=_Version){
        localVersion=_Version;
        listUpdateTW.clear();
    }
    QTableWidgetItem * edited=t->itemAt(w->pos());
    int row=t->row(edited);
    if(isNeedWrite==nullptr || row!=t->row(isNeedWrite)){
        if(!listUpdateTW.contains(edited)){
            qDebug()<<edited->text()<<edited;
            listUpdateTW.append(edited);
        }
    }


}

void SubTable::action_addNewRow()
{
    if(isNeedWrite==nullptr){
        emit sin_NewRow(table_name);
    }
}

void SubTable::action_Refresh()
{
    isNeedWrite=nullptr;
    emit sig_refresh(table_name);
}

void SubTable::addNewRow(QString table, QString nomber)
{
    if(table==table_name){
        int row=t->rowCount();
        t->insertRow( row );
        isNeedWrite=new QTableWidgetItem(nomber);
        t->setItem(row,0,isNeedWrite);
        for(int i=1;i<t->columnCount();++i){
            t->setItem(row,i,new QTableWidgetItem(""));
        }
        conditionalAppearance();
    }
}
