#include "s.h"

S::S()
{
    if(!openDB()){return;}
    createTables();
}

bool S::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("CallDB.sqlite");//Имя базы.
    if (!db.open()){
        getError();
        return false;
    };
    db.exec("PRAGMA foreign_keys = ON;");
    return true;
}

void S::createTables()
{
    QSqlQuery query(db);
    QStringList sqls;
    sqls<<"CREATE TABLE IF NOT EXISTS dealers("
          "_code TEXT PRIMARY KEY,"
          "_name TEXT,"
          "_comment TEXT,"
          "_activePhone TEXT,"
          "_distryPhone TEXT);"
         <<"CREATE TABLE IF NOT EXISTS full_call("
           "_row INTEGER,"
           "_date TEXT,"
           "_comment TEXT,"
           "_source TEXT,"
           "_parent text REFERENCES dealers(_code) ON UPDATE CASCADE);"
        <<"CREATE TABLE IF NOT EXISTS email("
          "_row INTEGER,"
          "_date TEXT,"
          "_mail TEXT,"
          "_comment TEXT,"
          "_parent text REFERENCES dealers(_code) ON UPDATE CASCADE);"
        <<"CREATE INDEX IF NOT EXISTS fc_index ON full_call (_row, _parent);"
        <<"CREATE INDEX IF NOT EXISTS e_index ON email (_row, _parent);"
        <<"CREATE TABLE IF NOT EXISTS managers("
          "_code TEXT PRIMARY KEY, "
          "_fullName text, "
          "_nameForOrder text);"
        <<"CREATE TABLE IF NOT EXISTS orders("
          "_code INTEGER PRIMARY KEY AUTOINCREMENT, "
          "_date TEXT, "
          "_manager text REFERENCES managers(_code) ON UPDATE CASCADE, "
          " _workList  text, "
          " _client text, "
          " _model text, "
          " _version text, "
          " _colors text, "
          " _mark text DEFAULT '1', "
          " _options text);";
    for(QString next_sql:sqls){
        query.finish();
        if(!query.exec(next_sql)){
            getError(&query);
            continue;
        };
    };
    db.commit();
}

void S::getError(QSqlQuery *query, QString title)
{
    QMessageBox msgBox;
    if(title!=""){
        msgBox.setWindowTitle(title);
    }
    QString error=(query==nullptr)?
              db.lastError().text():
              db.lastError().text()+" | "+query->lastError().text()+" | "+query->lastQuery()  ;
    msgBox.setText(error);
    msgBox.exec();
}

S *S::s()
{
    static S * sql=new S();
    return sql;
}

QMap<QString, QString> S::getDealer_objectForm(QString code)
{
    QMap<QString, QString> attributes;
    QSqlQuery query(db);
    query.prepare("SELECT _code,_name,_comment,_activePhone,_distryPhone FROM "
                      " dealers WHERE _code=:code ;");
    query.bindValue(":code", code);
    if(!query.exec()){
        getError(&query);
        return attributes;
    }

    query.next();
    attributes.insert("_code",query.value("_code").toString());
    attributes.insert("_name",query.value("_name").toString());
    attributes.insert("_comment",query.value("_comment").toString());
    attributes.insert("_activePhone",query.value("_activePhone").toString());
    attributes.insert("_distryPhone",query.value("_distryPhone").toString());
    return attributes;
}

QList<QMap<QString, QString> > S::getDealer_listForm()
{
    QList<QMap<QString, QString> > dynamical_list;
    QSqlQuery query(db);
    QString sql="SELECT _code,_name from dealers";
    if(!query.exec(sql)){
        getError(&query);
        return dynamical_list;
    }
    while(query.next()){
        QMap<QString, QString> nextDealer;
        nextDealer.insert("_code",query.value("_code").toString());
        nextDealer.insert("_name",query.value("_name").toString());
        dynamical_list.append(nextDealer);
    }
    return dynamical_list;
}

void S::createNewDealer(QMap<QString, QString> attributes)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO dealers(_code,_name,_comment,_activePhone,_distryPhone) VALUES"
                      "(:_code,:_name,:_comment,:_activePhone,:_distryPhone);");
    query.bindValue(":_code", attributes.value("_code"));
    query.bindValue(":_name", attributes.value("_name"));
    query.bindValue(":_comment", attributes.value("_comment"));
    query.bindValue(":_activePhone", attributes.value("_activePhone"));
    query.bindValue(":_distryPhone", attributes.value("_distryPhone"));
    if(!query.exec()){
        getError(&query,"Can`t create new dealer");
        return;
    };
    db.commit();
}

void S::updateDealer(QMap<QString, QString> attributes)
{
    QSqlQuery query(db);
    query.prepare("UPDATE dealers SET _code=:new_code, "
                  "_name=:_name, "
                  "_comment=:_comment, "
                  "_activePhone=:_activePhone, "
                  "_distryPhone=:_distryPhone "
                  " WHERE _code=:_code; ");

    query.bindValue(":new_code", attributes.value("new_code"));
    query.bindValue(":_code", attributes.value("_code"));
    query.bindValue(":_name", attributes.value("_name"));
    query.bindValue(":_comment", attributes.value("_comment"));
    query.bindValue(":_activePhone", attributes.value("_activePhone"));
    query.bindValue(":_distryPhone", attributes.value("_distryPhone"));
    if(!query.exec()){
        getError(&query,"Can`t update dealer where code="+attributes.value("_code"));
        return;
    };
    db.commit();
}

QList<QMap<QString, QString> > S::getFullCall_listForm(QString code)
{
    QList<QMap<QString, QString> > callList;
    QSqlQuery query(db);
    QString sql="SELECT _row,_date,_comment,_source FROM full_call "
                " WHERE _parent=:_parent ORDER BY _row;";
    query.prepare(sql);
    query.bindValue(":_parent", code);
    if(!query.exec()){
        getError(&query,"Can`t get CallDealerList from parent="+code);
        return callList;
    }
    while(query.next()){
        QMap<QString, QString> nextCall;
        nextCall.insert("_row",query.value("_row").toString());
        nextCall.insert("_date",query.value("_date").toString());
        nextCall.insert("_comment",query.value("_comment").toString());
        nextCall.insert("_source",query.value("_source").toString());
        callList.append(nextCall);
    }
    return callList;
}

void S::createFullCall(QMap<QString, QString> attributes)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO full_call(_row,_date,_comment,_source,_parent) VALUES"
                      "(:_row,:_date,:_comment,:_source,:_parent);");
    query.bindValue(":_row", attributes.value("_row").toInt());
    query.bindValue(":_date", attributes.value("_date"));
    query.bindValue(":_comment", attributes.value("_comment"));
    query.bindValue(":_source", attributes.value("_source"));
    query.bindValue(":_parent", attributes.value("_parent"));
    if(!query.exec()){
        getError(&query,"Can`t create new full call");
        return;
    };
    db.commit();
}

void S::updateFullCall(QMap<QString, QString> attributes)
{
    QSqlQuery query(db);
    QString sql="UPDATE full_call SET _date=:_date, "
                " _comment=:_comment, "
                " _source=:_source "
                " WHERE _row=:_row AND _parent=:_parent; ";
    query.prepare(sql);
    query.bindValue(":_row", attributes.value("_row").toInt());
    query.bindValue(":_date", attributes.value("_date"));
    query.bindValue(":_comment", attributes.value("_comment"));
    query.bindValue(":_source", attributes.value("_source"));
    query.bindValue(":_parent", attributes.value("_parent"));
    if(!query.exec()){
        getError(&query,"Can`t update FullCall where parent="+attributes.value("_parent"));
        return;
    };
    db.commit();
}

QList<QMap<QString, QString> > S::getEmail_listForm(QString code)
{
    QList<QMap<QString, QString> > mailList;
    QSqlQuery query(db);
    QString sql="SELECT _row,_date,_mail,_comment FROM email "
                " WHERE _parent=:_parent ORDER BY _row;";
    query.prepare(sql);
    query.bindValue(":_parent", code);
    if(!query.exec()){
        getError(&query,"Can`t get mailList from parent="+code);
        return mailList;
    }
    while(query.next()){
        QMap<QString, QString> nextMail;
        nextMail.insert("_row",query.value("_row").toString());
        nextMail.insert("_date",query.value("_date").toString());
        nextMail.insert("_mail",query.value("_mail").toString());
        nextMail.insert("_comment",query.value("_comment").toString());
        mailList.append(nextMail);
    }
    return mailList;
}

void S::createEmail(QMap<QString, QString> attributes)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO email(_row,_date,_mail,_comment,_parent) VALUES"
                      "(:_row,:_date,:_mail,:_comment,:_parent);");
    query.bindValue(":_row", attributes.value("_row").toInt());
    query.bindValue(":_date", attributes.value("_date"));
    query.bindValue(":_mail", attributes.value("_mail"));
    query.bindValue(":_comment", attributes.value("_comment"));
    query.bindValue(":_parent", attributes.value("_parent"));
    if(!query.exec()){
        getError(&query,"Can`t create new full call");
        return;
    };
    db.commit();
}

void S::updateEmail(QMap<QString, QString> attributes)
{
    QSqlQuery query(db);
    query.prepare("UPDATE email SET _date=:_date, "
                  "_mail=:_mail, "
                  "_comment=:_comment "
                  " WHERE _row=:_row,_parent=:_parent; ");
    query.bindValue(":_date", attributes.value("_date"));
    query.bindValue(":_mail", attributes.value("_mail"));
    query.bindValue(":_comment", attributes.value("_comment"));
    if(!query.exec()){
        getError(&query,"Can`t update Email where parent="+attributes.value("_parent"));
        return;
    };
    db.commit();
}

QString S::getNewRowNomber(QString table, QString parent)
{
    QSqlQuery query(db);
    if(table=="full_call"){
        query.prepare("select ifnull(Max(_row),0) as rows from full_call where _parent=:_parent;");
    }else if(table=="email"){
        query.prepare("select ifnull(Max(_row),0) as rows from email where _parent=:_parent;");
    }
    query.bindValue(":_parent", parent);
    if(!query.exec()){
        getError(&query,"Can`t update Email where parent="+parent);
        return "";
    };
    query.next();
    QString newRow=QString("").setNum( query.value("rows").toInt()+1  );
    return newRow;
}

QList<QMap<QString, QString> > S::getManagers_listForm()
{
    QList<QMap<QString, QString> > mList;
    QSqlQuery query(db);
    if(!query.exec("SELECT _code,_fullName,_nameForOrder FROM managers;")){
        getError(&query,"Can`t get ManagersList");
        return mList;
    }
    while(query.next()){
        QMap<QString, QString> managers;
        managers.insert("_code",query.value("_code").toString());
        managers.insert("_fullName",query.value("_fullName").toString());
        managers.insert("_nameForOrder",query.value("_nameForOrder").toString());
        mList.append(managers);
    }
    return mList;
}

QMap<QString, QString> S::getManagers_ObjectForm(QString _code)
{
    QMap<QString, QString> manager;
    QSqlQuery query(db);
    query.prepare("SELECT _code,_fullName,_nameForOrder FROM managers where _code=:_code;");
    query.bindValue(":_code", _code);
    if(!query.exec()){
        getError(&query,"Can`t get Manager:"+_code);
        return manager;
    }
    query.next();
    manager.insert("_code",query.value("_code").toString());
    manager.insert("_fullName",query.value("_fullName").toString());
    manager.insert("_nameForOrder",query.value("_nameForOrder").toString());
    return manager;
}

QList<QString> S::getAttrTable(QString Table_Name)
{
QList<QString>fiend;
QSqlQuery query(db);
QRegExp whiteDel("\\s");
QRegExp nonwordDel("\\W");
Table_Name=Table_Name.replace(whiteDel,"");
Table_Name=Table_Name.replace(nonwordDel,"");
QString sql="pragma table_info("+Table_Name+");";
if(!query.exec(sql)){
    getError(&query,"Can`t pragma table_info:"+Table_Name);
    return fiend;
}

while(query.next()){
    QString fieldName=query.value("name").toString();
    fiend.append(fieldName);
}
return fiend;
}

QList<QMap<QString, QString> > S::getList(QString Table_Name, QList<QString> attr)
{
QSqlQuery query(db);
QList<QMap<QString, QString> > tableList;
QRegExp whiteDel("\\s");
QRegExp nonwordDel("\\W");
Table_Name=Table_Name.replace(whiteDel,"");
Table_Name=Table_Name.replace(nonwordDel,"");

QString sql="SELECT * FROM "+Table_Name+";";
if(!query.exec(sql)){
    getError(&query,"Can`t get *from table:"+Table_Name);
    return tableList;
}
while(query.next()){
    QMap<QString, QString> row;
    for(QString key:attr){
        row.insert(key,query.value(key).toString());
    }
    tableList.append(row);
}
return tableList;
}

QMap<QString, QString> S::openAnyTable(QMap<QString, QString>  par)
{
   QMap<QString, QString>  attr;
   QSqlQuery query(db);;
   QRegExp whiteDel("\\s");
   QRegExp nonwordDel("\\W");
   QString Table_Name=par.value("Table_Name");
   Table_Name=Table_Name.replace(whiteDel,"");
   Table_Name=Table_Name.replace(nonwordDel,"");

   QString sql="pragma table_info("+Table_Name+");";
   if(!query.exec(sql)){
       getError(&query,"Can`t pragma table_info:"+Table_Name);
       return attr;
   }
   QList<QString>fiends;
   while(query.next()){
       QString fieldName=query.value("name").toString();
       fiends.append(fieldName);
   }
   query.finish();

   sql=par.contains("_code")?"SELECT * FROM "+Table_Name+" WHERE _code=:_code"
   :"SELECT * FROM "+Table_Name+" WHERE _parent=:_parent";


   query.prepare(sql);
   par.contains("_code")?
               query.bindValue(":_code",par.value("_code")):
               query.bindValue(":_parent",par.value("_parent"));
   if(!query.exec()){
       getError(&query,"Can`t select *from:"+Table_Name+" from code or parent");
       return attr;
   }
   query.next();
   for(QString field:fiends){
       attr.insert(field,query.value(field).toString());
    }
   return attr;
}


void S::CreateAnyTable(QMap<QString, QString> attributes)
{
QSqlQuery query(db);
QString table=attributes.value("Table_Name");
QRegExp whiteDel("\\s");
QRegExp nonwordDel("\\W");
table=table.replace(whiteDel,"");
table=table.replace(nonwordDel,"");

QString sql="pragma table_info("+table+");";
if(!query.exec(sql)){
    getError(&query,"Can`t pragma table_info:"+table);
    return;
}
QList<QString>fiend;
while(query.next()){
    QString fieldName=query.value("name").toString();
    fiend.append(fieldName);
}
query.finish();

sql="INSERT INTO "+table+" ( ";
for(int i=0;i!=fiend.length();++i){
    sql+=( (i+1)!=fiend.length()?
    fiend[i]+",":fiend[i]+") VALUES (");
}
for(int i=0;i!=fiend.length();++i){
    sql+=( (i+1)!=fiend.length()?
    ":"+fiend[i]+",":":"+fiend[i]+");");
}
query.prepare(sql);
for(QString value:fiend){
    query.bindValue(":"+value, attributes.value("value"));
}
if(!query.exec()){
    getError(&query,"Can`t Create:"+table);
    return;
};
db.commit();
}
