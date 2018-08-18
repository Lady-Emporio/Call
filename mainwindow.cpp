#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    makeGui();
}

MainWindow::~MainWindow()
{

}



void MainWindow::makeGui()
{
    mdiArea = new QMdiArea(this);
    setCentralWidget(mdiArea);
    mdiArea->setViewMode(QMdiArea::TabbedView);

    QMenuBar * mainMenu=new QMenuBar(this);
    this->setMenuBar(mainMenu);

    mainMenu->addAction("Dealer list",this,SLOT(action_OpenDealerList()));
}

void MainWindow::action_OpenDealerList()
{
    QString title="Dealer list";
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(auto x:allSub){
        if(x->windowTitle()==title){
            x->close();
        };
    };
    QMdiSubWindow *subWindow = new QMdiSubWindow(this);
    subWindow->setWindowTitle(title);
    Dealer_list * widget=new Dealer_list(this);
    subWindow->setWidget(widget);
    mdiArea->addSubWindow(subWindow);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();

    connect(widget, SIGNAL(s_NewDealer()), this, SLOT(CreateNewDealer()));
    connect(widget, SIGNAL(sig_Refresh()), this, SLOT(sql_dealerList()));
    connect(this, SIGNAL(dealerListRefresh(QList<QMap<QString,QString>>)), widget, SLOT(dealerListRefresh(QList<QMap<QString,QString>>)));
    connect(widget, SIGNAL(sin_open(QString)), this, SLOT(sql_openDealer(QString)));
    sql_dealerList();
}

void MainWindow::CreateNewDealer()
{
    QString title="New dealer";
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(QMdiSubWindow *x:allSub){
        if(x->windowTitle()==title){
            mdiArea->setActiveSubWindow(x);
            return;
        };
    };
    QMdiSubWindow *subWindow = new QMdiSubWindow(this);
    subWindow->setWindowTitle(title);
    QMap<QString,QString>parameters;
    parameters.insert("New","CreateNew");
    Dealer * widget=new Dealer(parameters,this);
    subWindow->setWidget(widget);
    mdiArea->addSubWindow(subWindow);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();

    connect(widget, SIGNAL(sig_CreateNewDealer(QMap<QString,QString>)), this, SLOT(sql_CreateNewDealer(QMap<QString,QString>)));

}

void MainWindow::sql_CreateNewDealer(QMap<QString, QString> attr)
{
    S::s()->createNewDealer(attr);
    sql_dealerList();
}

void MainWindow::sql_dealerList()
{
    QList<QMap<QString,QString>> dealerList=S::s()->getDealer_listForm();
    emit dealerListRefresh(dealerList);
}

void MainWindow::sql_openDealer(QString code)
{
    QMap<QString,QString> attr=S::s()->getDealer_objectForm(code);

    QString title="Dealer:"+attr.value("_code");
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(QMdiSubWindow *x:allSub){
        if(x->windowTitle()==title){
            mdiArea->setActiveSubWindow(x);
            return;
        };
    };
    QMdiSubWindow *subWindow = new QMdiSubWindow(this);
    subWindow->setWindowTitle(title);

    Dealer * widget=new Dealer(attr,this);
    subWindow->setWidget(widget);
    mdiArea->addSubWindow(subWindow);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();

    connect(widget, SIGNAL(sig_update(QMap<QString,QString>)), this, SLOT(sql_updateDealer(QMap<QString,QString>)));
}

void MainWindow::sql_updateDealer(QMap<QString, QString> attr)
{
    S::s()->updateDealer(attr);
    sql_dealerList();
}
