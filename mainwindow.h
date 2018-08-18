#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include.h"
#include "Dealers/dealer.h"
#include "Dealers/dealer_list.h"
#include "sql/s.h"

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
    void sql_refreshDealer(QMap<QString,QString> attr);
    void sql_NewRow(QMap<QString,QString>parameters);
signals:
    void dealerListRefresh(QList<QMap<QString,QString>> dlist);
    void sig_refreshSubTable(QMap<QString,QString>attr,QList<QMap<QString,QString>>data);
    void sig_newRow(QMap<QString,QString> parameters,QString newRow);
};

#endif // MAINWINDOW_H
