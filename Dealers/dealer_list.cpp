#include "dealer_list.h"

Dealer_list::Dealer_list(QWidget *parent) : QWidget(parent)
{
    makeGui();
}

void Dealer_list::dealerListRefresh(QList<QMap<QString, QString> > dealerList)
{
    dl->clearContents();
    int size=dealerList.count();
    dl->setRowCount(size);
    for(int i=0;i!=size;++i){
        QMap<QString, QString> nextRow=dealerList[i];
        dl->setItem(i,0,new QTableWidgetItem(nextRow.value("_code")));
        dl->setItem(i,1,new QTableWidgetItem(nextRow.value("_name")));
    };
    dl->resizeColumnsToContents();
}

void Dealer_list::makeGui()
{
    dl=new QTableWidget(this);
    dl->setColumnCount(2);
    QStringList headers;
    headers<<"Код"<<"Наименование";
    dl->setHorizontalHeaderLabels(headers);
    dl->horizontalHeader()->setStretchLastSection(true);
    dl->setSortingEnabled(true);
    dl->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QVBoxLayout * mainLayout=new QVBoxLayout(this);
    this->setLayout(mainLayout);
    QHBoxLayout * buttonRow=new QHBoxLayout(this);

    QPushButton * openButton=new QPushButton("Открыть",this);
    QPushButton * createButton=new QPushButton("Создать новый",this);
    QPushButton * refreshButton=new QPushButton("Refresh",this);

    buttonRow->addWidget(openButton);
    buttonRow->addWidget(createButton);
    mainLayout->addLayout(buttonRow);
    mainLayout->addWidget(dl);
    mainLayout->addWidget(refreshButton);

    connect(createButton, SIGNAL(clicked()), this, SLOT(signal_CreateNewDealer()));
    connect(openButton, SIGNAL(clicked()), this, SLOT(open()));
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));
}

void Dealer_list::refresh()
{
    emit sig_Refresh();
}

void Dealer_list::open()
{
    int row=dl->currentRow();
    if(row!=-1){
        QString code=dl->item(row,0)->text();
        emit sin_open(code);
    }
}

void Dealer_list::signal_CreateNewDealer()
{
    emit s_NewDealer();
}
