#include "managers.h"

Managers::Managers(QMap<QString, QString> parameters, QWidget *parent) : QWidget(parent)
{
    if(parameters.contains("object")){
        makeFormObject(parameters.value("_code"));
    }else if(parameters.contains("list")){
        makeFormList();
        refreshList();
    }
}

void Managers::makeFormList()
{
    dl=new QTableWidget(this);
    dl->setColumnCount(2);
    QStringList headers;
    headers<<"Код"<<"Полное имя";
    dl->setHorizontalHeaderLabels(headers);
    dl->horizontalHeader()->setStretchLastSection(true);
    dl->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QVBoxLayout * mainLayout=new QVBoxLayout(this);
    this->setLayout(mainLayout);

    QPushButton * openButton=new QPushButton("Открыть",this);
    QPushButton * refreshButton=new QPushButton("Refresh",this);


    mainLayout->addWidget(openButton);
    mainLayout->addWidget(dl);
    mainLayout->addWidget(refreshButton);

    connect(openButton, SIGNAL(clicked()), this, SLOT(openObject()));
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(refreshList()));
}

void Managers::makeFormObject(QString _code)
{
    QVBoxLayout * mainLayout=new QVBoxLayout(this);
    this->setLayout(mainLayout);

    QMap<QString,QString>attr=S::s()->getManagers_ObjectForm(_code);
    for(QString key:attr.keys()){
        QHBoxLayout * row=new QHBoxLayout(this);
        QLabel *labelKey=new QLabel(key,this);
        labelKey->setAlignment(Qt::AlignLeft);
        row->addWidget(labelKey);
        QLabel *labelValue=new QLabel(attr[key],this);
        labelValue->setAlignment(Qt::AlignLeft);
        row->addWidget(labelValue);
        mainLayout->addLayout(row);
    }
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void Managers::refreshList()
{
    dl->clearContents();
    QList<QMap<QString,QString>> listManagets=S::s()->getManagers_listForm();
    int endRow=listManagets.length();
    dl->setRowCount(endRow);
    for(int i=0;i!=endRow;++i){
        QMap<QString,QString> managers=listManagets[i];
        dl->setItem(i,0,new QTableWidgetItem( managers.value("_code")));
        dl->setItem(i,1,new QTableWidgetItem( managers.value("_fullName")));
    }
}

void Managers::openObject()
{
    int row=dl->currentRow();
    if(row!=-1){
        emit OpenManagers( dl->item(row,0)->text() );
    }

}
