#include <QApplication>
#include <QCoreApplication>
#include <QtSql>

#include "mainengine.h"
#include "room.h"
#include"login.h"
#include "ui_mainengine.h"

int main(int argc, char *argv[])//【账号admin，密码123】
{
    QApplication a(argc, argv);

    login l;
    MainEngine main;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); //添加数据库驱动
    db.setHostName("localhost");
    db.setDatabaseName("report");
    db.setUserName("root");
    db.setPassword("");
    db.setPort(3306);
    if(!db.open()) //打开数据库
    {
        qDebug()<<"Fail!";
        return false;
    }
    else
    {
        qDebug()<<"ok!";
    }

    //if(l.exec()==QDialog::Accepted)
    //{
        main.show();
        return a.exec();
    //}
}
