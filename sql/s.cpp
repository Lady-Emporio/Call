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
        <<"CREATE INDEX IF NOT EXISTS e_index ON email (_row, _parent);";
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
