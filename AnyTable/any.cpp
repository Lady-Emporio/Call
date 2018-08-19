#include "any.h"


AnyTable::AnyTable(QMap<QString,QString>parameters,QMap<QString,QString>attr, QWidget *parent) : QWidget(parent),attr(attr)
{
    table=parameters.value("Table_Name");
if(parameters.value("typeForm")=="list"){
    makeList();
    action_RefreshList();
}else if(parameters.value("typeForm")=="object"){
    makeObject();
    refreshObject();
}
}

void AnyTable::makeList()
{
dl=new QTableWidget(this);
QStringList headers=attr.keys();
dl->setColumnCount(headers.length());
dl->setHorizontalHeaderLabels(headers);

QVBoxLayout * mainLayout=new QVBoxLayout(this);
this->setLayout(mainLayout);
mainLayout->addWidget(dl);
QMenuBar * mainMenu=new QMenuBar(this);
mainLayout->setMenuBar(mainMenu);

mainMenu->addAction("Open",this,SLOT(action_Open()));
mainMenu->addAction("Create",this,SLOT(action_Create()));
mainMenu->addAction("Refresh",this,SLOT(action_RefreshList()));
}

void AnyTable::makeObject()
{
QVBoxLayout * mainLayout=new QVBoxLayout(this);
this->setLayout(mainLayout);
QList<QString>keys=attr.keys();
for(auto key:keys){
    QHBoxLayout * rowLayout=new QHBoxLayout(this);
    rowLayout->addWidget(new QLabel(key,this));
    QLineEdit *rowEdit=new QLineEdit(this);
    edits.insert(key,rowEdit);
    rowLayout->addWidget(rowEdit);
    mainLayout->addLayout(rowLayout);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}
QMenuBar * mainMenu=new QMenuBar(this);
mainLayout->setMenuBar(mainMenu);
if(attr.value("_code")!=""){
    mainMenu->addAction("Update",this,SLOT(action_UpdateExist()));
}else{
    mainMenu->addAction("Create",this,SLOT(action_CreateObject()));
}
}

void AnyTable::refreshObject()
{
    if(attr.value("_code")!=""){
        QList<QString>keys=attr.keys();
        for(QString key:keys){
            edits.value(key)->setText(attr.value(key));
        }
    }
}

void AnyTable::action_RefreshList()
{
    QList<QString>fields=attr.keys();
    QList<QMap<QString,QString>>data=S::s()->getList(table,fields);
    dl->clearContents();
    int len=data.length();
    int cols=dl->columnCount();
    dl->setRowCount(len);
    for(int i=0;i!=len;i++){
        QMap<QString,QString> row=data[i];
        for(int col=0;col!=cols;++col){
            dl->setItem(i,col,new QTableWidgetItem(row[fields[col]]));
        }
    }
}

void AnyTable::action_Open()
{
int row=dl->currentRow();
if(row!=-1){
    QMap<QString,QString> attrOpen;
    attrOpen.insert("Table_Name",table);
    for(int i=0;i!=dl->columnCount();++i){
        if(dl->horizontalHeaderItem(i)->text()=="_code"){
            attrOpen.insert("_code",dl->item(row,i)->text());
            emit sig_anyOpen(attrOpen);
            return;
        }
    }
}
}

void AnyTable::action_Create()
{
QMap<QString,QString> attrOpen;
attrOpen.insert("Table_Name",table);
emit sig_anyOpen(attrOpen);
}

void AnyTable::action_UpdateExist()
{

}

void AnyTable::action_CreateObject()
{

}
