#include "dealer.h"



Dealer::Dealer(QMap<QString, QString> parameters, QWidget *parent) : QWidget(parent),parameters(parameters)
{
    makeGui();
    if(!parameters.contains("New")){
        isOpen();
    }
}

void Dealer::makeGui()
{
    mdiArea=new QMdiArea(this);
    mdiArea->setViewMode(QMdiArea::TabbedView);

    QVBoxLayout * mainLayout=new QVBoxLayout(this);
    this->setLayout(mainLayout);
    QHBoxLayout * rowCode=new QHBoxLayout(this);
    QHBoxLayout * rowName=new QHBoxLayout(this);
    QHBoxLayout * rowPhone=new QHBoxLayout(this);

    QLabel *codeLabel=new QLabel("код",this);
    codeEdit=new QLineEdit(this);
    rowCode->addWidget(codeLabel);
    rowCode->addWidget(codeEdit);

    QLabel *nameLabel=new QLabel("Наименование",this);
    nameEdit=new QLineEdit(this);
    rowName->addWidget(nameLabel);
    rowName->addWidget(nameEdit);

    QLabel *activePhoneLabel=new QLabel("Активный телефон",this);
    activePhoneEdit=new QLineEdit(this);
    rowPhone->addWidget(activePhoneLabel);
    rowPhone->addWidget(activePhoneEdit);

    QLabel *distryPhoneLabel=new QLabel("Дистри телефон",this);
    distryPhoneEdit=new QLineEdit(this);
    rowPhone->addWidget(distryPhoneLabel);
    rowPhone->addWidget(distryPhoneEdit);

    commentEdit=new QTextEdit(this);

    QString textSaveButton=parameters.contains("New")?"Create":"Update";
    QPushButton * saveButton=new QPushButton(textSaveButton,this);

    parameters.contains("New")?
        connect(saveButton, SIGNAL(clicked()), this, SLOT(sql_CreateNewDealer())):
        connect(saveButton, SIGNAL(clicked()), this, SLOT(sql_UpdateNewDealer()));

    mainLayout->addLayout(rowCode);
    mainLayout->addLayout(rowName);
    mainLayout->addWidget(commentEdit);
    mainLayout->addLayout(rowPhone);
    mainLayout->addWidget(mdiArea);
    mainLayout->addWidget(saveButton);
}

void Dealer::isOpen()
{
    codeEdit->setText(parameters.value("_code"));
    nameEdit->setText(parameters.value("_name"));
    commentEdit->setPlainText(parameters.value("_comment"));
    activePhoneEdit->setText(parameters.value("_activePhone"));
    distryPhoneEdit->setText(parameters.value("_distryPhone"));

    QMdiSubWindow *subWindow1 = new QMdiSubWindow(this);
    subWindow1->setWindowTitle("Full call");
    QMap<QString,QString> par1;
    par1.insert("table_name","full_call");
    SubTable * fullCall=new SubTable(par1,this);
    subWindow1->setWidget(fullCall);
    mdiArea->addSubWindow(subWindow1);
    subWindow1->setAttribute(Qt::WA_DeleteOnClose);
    subWindow1->show();

    QMdiSubWindow * subWindow2 = new QMdiSubWindow(this);
    subWindow2->setWindowTitle("Email");
    QMap<QString,QString> par2;
    par2.insert("table_name","email");
    SubTable * email=new SubTable(par2,this);
    subWindow2->setWidget(email);
    mdiArea->addSubWindow(subWindow2);
    subWindow2->setAttribute(Qt::WA_DeleteOnClose);
    subWindow2->show();

    subWindow1->setSystemMenu(nullptr);
    subWindow2->setSystemMenu(nullptr);

    mdiArea->setActiveSubWindow(subWindow1);
    connect(fullCall, SIGNAL(sig_refresh(QString)), this, SLOT(refreshSubTable(QString)));
    connect(email, SIGNAL(sig_refresh(QString)), this, SLOT(refreshSubTable(QString)));

    connect(fullCall, SIGNAL(sin_NewRow(QString)), this, SLOT(subTableNewRow(QString)));
    connect(email, SIGNAL(sin_NewRow(QString)), this, SLOT(subTableNewRow(QString)));

    connect(this, SIGNAL(sig_newRow(QString,QString)), fullCall, SLOT(addNewRow(QString,QString)));
    connect(this, SIGNAL(sig_newRow(QString,QString)), email, SLOT(addNewRow(QString,QString)));

    connect(this, SIGNAL(sin_SubTableRef(QMap<QString, QString>, QList<QMap<QString, QString> >)),
            fullCall, SLOT(refresh(QMap<QString, QString>, QList<QMap<QString, QString> >)));
    connect(this, SIGNAL(sin_SubTableRef(QMap<QString, QString>, QList<QMap<QString, QString> >)),
            email, SLOT(refresh(QMap<QString, QString>, QList<QMap<QString, QString> >)));
}

void Dealer::sql_CreateNewDealer()
{
    QMap<QString,QString> attributes;
    attributes.insert("_code",codeEdit->text());
    attributes.insert("_name",nameEdit->text());
    attributes.insert("_comment",commentEdit->toPlainText());
    attributes.insert("_activePhone",activePhoneEdit->text());
    attributes.insert("_distryPhone",distryPhoneEdit->text());
    emit sig_CreateNewDealer(attributes);
}

void Dealer::sql_UpdateNewDealer()
{
//    QMap<QString,QString> attributes;
//    attributes.insert("_code",codeEdit->text());
//    attributes.insert("_name",nameEdit->text());
//    attributes.insert("_comment",commentEdit->toPlainText());
//    attributes.insert("_activePhone",activePhoneEdit->text());
//    attributes.insert("_distryPhone",distryPhoneEdit->text());
//    emit sig_update(attributes);
    auto sublist= mdiArea->subWindowList();
    for(auto subWindow:sublist){
        if(subWindow->widget()->objectName()=="SubTable"){
            SubTable * widget=(SubTable*)subWindow->widget();
            if(widget->isNeedWrite!=nullptr){
                QString table=widget->table_name;
                if(table=="full_call"){
                    QMap<QString,QString>attr;
                }else if(table=="email"){
                    QMap<QString,QString>attr;
                }
                widget->isNeedWrite=nullptr;
            }
        }
    }
}

void Dealer::refreshSubTable(QString table)
{
    QMap<QString,QString>attr;
    attr.insert("title",parameters.value("title"));
    attr.insert("table",table);
    attr.insert("_code",parameters.value("_code"));
    emit sig_refreshSubTable(attr);
}

void Dealer::refreshSubTable(QMap<QString, QString> attr, QList<QMap<QString, QString> > data)
{
    if(attr.value("_code")==parameters.value("_code") and parameters.value("title")==attr.value("title")){
        emit sin_SubTableRef(attr, data);
    }
}

void Dealer::subTableNewRow(QString table)
{
    QMap<QString,QString>attr;
    attr.insert("title",parameters.value("title"));
    attr.insert("table",table);
    attr.insert("_code",parameters.value("_code"));
    emit sig_newRowSubTable(attr);
}

void Dealer::setNewRowInSubTable(QMap<QString, QString> attr, QString row)
{
    QString table=attr.value("table");
    emit sig_newRow(table,row);
}
