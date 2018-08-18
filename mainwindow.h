#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include.h"
#include "Dealers/dealer.h"
#include "Dealers/dealer_list.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    void makeGui();
    QMdiArea *mdiArea;
private slots:
    void action_OpenDealerList();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void CreateNewDealer();
    void sql_CreateNewDealer(QMap<QString,QString>attr);
    void sql_dealerList();
    void sql_openDealer(QString code);
    void sql_updateDealer(QMap<QString,QString> attr);
signals:
    void dealerListRefresh(QList<QMap<QString,QString>> dlist);
};

#endif // MAINWINDOW_H
