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
    mainLayout->addWidget(saveButton);
    mainLayout->addWidget(mdiArea);

}

void Dealer::isOpen()
{
    codeEdit->setText(parameters.value("_code"));
    nameEdit->setText(parameters.value("_name"));
    commentEdit->setPlainText(parameters.value("_comment"));
    activePhoneEdit->setText(parameters.value("_activePhone"));
    distryPhoneEdit->setText(parameters.value("_distryPhone"));

    QMdiSubWindow *subWindow1 = new QMdiSubWindow(this);
    subWindow1->setWindowTitle(title);
    SubTable * fullCall=new SubTable(this);
    subWindow1->setWidget(fullCall);
    mdiArea->addSubWindow(subWindow1);
    subWindow1->setAttribute(Qt::WA_DeleteOnClose);
    subWindow1->show();

    QMdiSubWindow *subWindow2 = new QMdiSubWindow(this);
    subWindow2->setWindowTitle(title);
    SubTable * email=new SubTable(this);
    subWindow2->setWidget(email);
    mdiArea->addSubWindow(subWindow2);
    subWindow2->setAttribute(Qt::WA_DeleteOnClose);
    subWindow2->show();

    mdiArea->setActiveSubWindow(fullCall);
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
    QMap<QString,QString> attributes;
    attributes.insert("_code",codeEdit->text());
    attributes.insert("_name",nameEdit->text());
    attributes.insert("_comment",commentEdit->toPlainText());
    attributes.insert("_activePhone",activePhoneEdit->text());
    attributes.insert("_distryPhone",distryPhoneEdit->text());
    emit sig_update(attributes);
}
