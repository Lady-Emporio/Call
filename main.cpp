#include "mainwindow.h"
#include "sql/s.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    S*sql=S::s();
    MainWindow w;
    w.show();

    return a.exec();
}
