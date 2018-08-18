#ifndef DEALER_H
#define DEALER_H

#include "include.h"
#include "subtable.h"
class Dealer : public QWidget
{
    Q_OBJECT
    void makeGui();
    void isOpen();
    QMap<QString,QString> parameters;
    QLineEdit *nameEdit;
    QLineEdit *codeEdit;
    QLineEdit *activePhoneEdit;
    QLineEdit *distryPhoneEdit;
    QTextEdit *commentEdit;
    QMdiArea *mdiArea;
private slots:
    void sql_CreateNewDealer();
    void sql_UpdateNewDealer();
public:
    explicit Dealer(QMap<QString,QString> parameters, QWidget *parent = 0);
public slots:

signals:
    void sig_CreateNewDealer(QMap<QString,QString> attr);
    void sig_update(QMap<QString,QString> parameters);
};

#endif // DEALER_H
