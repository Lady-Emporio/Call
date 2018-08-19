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
    mainMenu->addAction("Managers list",this,SLOT(action_OpenManagersList()));
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
    parameters.insert("title",title);
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
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(QMdiSubWindow *x:allSub){
        if(x->windowTitle()==attr.value("title")){
            x->close();
        };
    };
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
    attr.insert("title",title);
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(QMdiSubWindow *x:allSub){
        if(x->windowTitle()==title){
            mdiArea->setActiveSubWindow(x);
            Dealer * dealer=(Dealer * )x->widget();
            for(auto subWin:dealer->mdiArea->subWindowList()){
                if(subWin->windowTitle()=="Full call"){
                    dealer->mdiArea->setActiveSubWindow(subWin);
                    break;
                }
            }
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
    connect(widget, SIGNAL(sig_refreshSubTable(QMap<QString,QString>)), this, SLOT(sql_refreshDealer(QMap<QString,QString>)));
    connect(this, SIGNAL(sig_refreshSubTable(QMap<QString,QString>,QList<QMap<QString,QString>>)),
            widget, SLOT(refreshSubTable(QMap<QString,QString>,QList<QMap<QString,QString>>)));
    connect(widget, SIGNAL(sig_newRowSubTable(QMap<QString,QString>)), this, SLOT(sql_NewRow(QMap<QString,QString>)));
    connect(this, SIGNAL(sig_newRow(QMap<QString,QString>,QString)), widget, SLOT(setNewRowInSubTable(QMap<QString,QString>,QString)));
    connect(widget, SIGNAL(sig_createRowSubTable(QMap<QString,QString>)), this, SLOT(sql_createRowSubTable(QMap<QString,QString>)));
    connect(widget, SIGNAL(sig_UpdateST(QMap<QString,QString>)), this, SLOT(sql_updateSubT(QMap<QString,QString>)));

    widget->refreshSubTable("full_call");
    widget->refreshSubTable("email");

    for(auto subWin:widget->mdiArea->subWindowList()){
        if(subWin->windowTitle()=="Full call"){
            widget->mdiArea->setActiveSubWindow(subWin);
            break;
        }
    }
}

void MainWindow::sql_updateDealer(QMap<QString, QString> attr)
{
    S::s()->updateDealer(attr);
    sql_dealerList();
}

void MainWindow::sql_refreshDealer(QMap<QString, QString> attr)
{
    QString table=attr.value("table");
    QString code=attr.value("_code");
    QString title=attr.value("title");
    if(table=="full_call"){
        QList<QMap<QString,QString>>list_fc=S::s()->getFullCall_listForm(code);
        emit sig_refreshSubTable(attr,list_fc);
    }else if(table=="email"){
        QList<QMap<QString,QString>>list_e=S::s()->getEmail_listForm(code);
        emit sig_refreshSubTable(attr,list_e);
    }
}

void MainWindow::sql_NewRow(QMap<QString, QString> parameters)
{
    QString table=parameters.value("table");
    QString code=parameters.value("_code");
    QString title=parameters.value("title");
    QString newRow=S::s()->getNewRowNomber(table,code);
    emit sig_newRow(parameters,newRow);

}

void MainWindow::sql_createRowSubTable(QMap<QString, QString> parameters)
{
    QString table=parameters.value("table");
    if(table=="full_call"){
        S::s()->createFullCall(parameters);
    }else if(table=="email"){
        S::s()->createEmail(parameters);
    }
}

void MainWindow::sql_updateSubT(QMap<QString, QString> parameters)
{
    QString table=parameters.value("table");
    if(table=="full_call"){
        S::s()->updateFullCall(parameters);
    }else if(table=="email"){
        S::s()->updateEmail(parameters);
    }
}

void MainWindow::action_OpenManagersList()
{
    QString title="Managers list";
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(auto x:allSub){
        if(x->windowTitle()==title){
            mdiArea->setActiveSubWindow(x);
            return;
        };
    };
    QMdiSubWindow *subWindow = new QMdiSubWindow(this);
    subWindow->setWindowTitle(title);
    QMap<QString,QString>par;
    par.insert("list","");
    Managers * widget=new Managers(par,this);
    subWindow->setWidget(widget);
    mdiArea->addSubWindow(subWindow);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();

    connect(widget, SIGNAL(OpenManagers(QString)), this, SLOT(action_openManagersObject(QString)));

}

void MainWindow::action_openManagersObject(QString _code)
{
    QString title="Manager:"+_code;
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(auto x:allSub){
        if(x->windowTitle()==title){
            mdiArea->setActiveSubWindow(x);
            return;
        };
    };
    QMdiSubWindow *subWindow = new QMdiSubWindow(this);
    subWindow->setWindowTitle(title);
    QMap<QString,QString>par;
    par.insert("object","");
    par.insert("_code",_code);
    Managers * widget=new Managers(par,this);
    subWindow->setWidget(widget);
    mdiArea->addSubWindow(subWindow);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
}

