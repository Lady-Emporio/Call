#include "subtable.h"

SubTable::SubTable(QString table_name,QWidget *parent) : QWidget(parent),table_name(table_name)
{
t=new QTableWidget(this);
}
