#ifndef MAINENGINE_H
#define MAINENGINE_H
#include <QMainWindow>
#include <QMessageBox>
#include<QWidget>
#include<QDateTime>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <QtNetwork/QTcpSocket>
using namespace std;
const QString Order1_1 = "choosePrio-0";//注册主机
const QString Order1_2 = "choosePrio-1";//注册分机
const QString part_1 = "msg-[{";
const QString part_2 = "\"roomNum\":";
const QString part_3 = "\"temp\":";
const QString part_4 = "\"speed\":";
const QString part_5 = "\"tarTemp\":";
const QString part_6 = "\"mode\":";
const QString part_7 = "\"requestType\":1";
const QString part_8 = "\"requestType\":2";
const QString part_11 = "\"requestType\":3";
const QString part_12 = "\"requestType\":4";
const QString part_9 = "}]";
const QString part_10 =",";
const QString part_13 = "\"on\":";
const QString part_fee = "\"cost\":";



namespace Ui {
class MainEngine;
}

class MainEngine : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainEngine(QWidget *parent = 0);
    ~MainEngine();
    void set_main_working_state(int temp);
    void set_working_mode(int temp);
    void set_update_frequency(int temp);

    int get_main_working_state();
    int get_working_mode();
    int get_update_frequency();
    void data_handle(QString MESS);

private slots:
    void on_open_main_clicked();
    void on_close_main_clicked();
    void on_set_working_mode_clicked();
    void on_update_frequency_clicked();
    void on_report_clicked();

    void newConnect(); //连接服务器
    void readMessage();  //接收数据
    void sendMessage(QString DATA); //发送数据
    void displayError(QAbstractSocket::SocketError);  //显示错误



    void run_time();
    void data_show();
    void spy_on();

private:
    Ui::MainEngine *ui;

    int main_working_state;
    int working_mode;
    int update_frequency;

    QDateTime dt;
    QTime time;
    QDate date;
    QString current_date;

    int recv_request;   //接收的请求类型

    int room_id;
    int recv_temp;
    int recv_wind;
    int on;

    int extension_num;  //当前从机数
    int handle_num;     //当前正在处理的从机数
    int wait_num;       //当前正在排队的从机数

    int hour;
    int minute;
    int second;

    int count;           //刷新频率的计数器

    QTcpSocket *tcpSocket;
    QString message;  //存放从服务器接收到的字符串

};

#endif // MAINENGINE_H
