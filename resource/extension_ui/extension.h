#ifndef EXTENSION_H
#define EXTENSION_H

#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
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

#include<QDateTime>
//从机运行状态
#define ON 1
#define OFF 0

//工作模式
#define HEAT 0
#define COOL 1

//风速
#define LOW 1
#define MEDIUM 2
#define HIGH 3

//温度范围
#define HEAT_MAX_TEMP 30
#define HEAT_MIN_TEMP 25
#define COOL_MAX_TEMP 25
#define COOL_MIN_TEMP 18
#define INIT_TEMP 25//房间初始温度


namespace Ui {
class Extension;
}

class Extension : public QMainWindow
{
    Q_OBJECT

public:
    explicit Extension(QWidget *parent = 0);
    ~Extension();
    void Extension::run_time();
    void set_room_num(int temp);
    void set_target_temp(int temp);
    void set_current_temp(float temp);
    void set_current_wind(int temp);
    void set_fee(float fee);
    void set_working_state(int temp);
    void set_main_working_state(int temp);
    void set_main_working_mode(int temp);

    int get_room_num();
    int get_target_temp();
    float get_current_temp();
    int get_current_wind();
    float get_fee();
    int get_working_state();
    int get_main_working_state();
    int get_main_working_mode();
    void datahandle(QString MESS);

    Ui::Extension get_ui();
private slots:
    void on_open_extension_clicked();

    void on_close_extension_clicked();

    void on_set_temperature_clicked();

    void on_set_wind_clicked();

    void newConnect(); //连接服务器
    void readMessage();  //接收数据
    void sendMessage(QString DATA); //发送数据
    void displayError(QAbstractSocket::SocketError);  //显示错误


private:
    Ui::Extension *ui;

    QDateTime dt;
    QTime time;
    QDate date;
    QString current_date;


    int room_num;
    int target_temp;
    float current_temp;
    int current_wind;
    float fee;
    int working_state;
    int main_working_mode;

    int second;
    int minute;
    int hour;

    int recv_request;   //接收的请求类型
    int room_id;
    float recv_curTemp;
    int recv_temp;
    int recv_wind;
    int recv_mode;
    float recv_fee;

    QTcpSocket *tcpSocket;
    QString message;  //存放从服务器接收到的字符串
};

#endif // EXTENSION_H
