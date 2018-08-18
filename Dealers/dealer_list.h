#ifndef DEALER_LIST_H
#define DEALER_LIST_H

#include "include.h"
class Dealer_list : public QWidget
{
    Q_OBJECT
    void makeGui();
    QTableWidget *dl;
private slots:
    void signal_CreateNewDealer();
    void refresh();
    void open();
public:
    explicit Dealer_list(QWidget *parent = 0);
signals:
    void s_NewDealer();
    void sig_Refresh();
    void sin_open(QString code);
public slots:
    void dealerListRefresh(QList<QMap<QString,QString>> dealerList);
};

#endif // DEALER_LIST_H
