#ifndef ROOM_H
#define ROOM_H

#include<QObject>
#include <QLabel>
#include<QDateTime>
#include "mainengine.h"

#define SLAVE_ON 1
#define SLAVE_OFF 2
#define SLAVE_WAITING 3
//运行状态

#define OFF          0
#define RUNNING      1
#define WAITING      2
#define RUNNING_BUSY 3
#define WAIT_FOR_HANDLE 4

//工作模式
#define HEAT 0
#define COOL 1

//风速
#define LOW    1
#define MEDIUM 2
#define HIGH   3

//温度范围
#define HEAT_MAX_TEMP 30
#define HEAT_MIN_TEMP 25
#define COOL_MAX_TEMP 25
#define COOL_MIN_TEMP 18
#define INIT_TEMP     25

class room : public QMainWindow
{
    Q_OBJECT
public:
    friend class MainEngine;
    explicit room(MainEngine *zhuji,QLabel *room_num_a,QLabel *target_temp_a,QLabel *current_temp_a,
                  QLabel *current_wind_a,QLabel *fee_a,QLabel *energy_a,QLabel *working_state_a);
    ~room();
    void set_room_num(int temp);
    void set_target_temp(int temp);
    void set_current_temp(float temp);
    void set_current_wind(int temp);
    void set_fee(float temp);
    void set_working_state(int temp);

    int get_room_num();
    int get_target_temp();
    float get_current_temp();
    int get_current_wind();
    float get_fee();
    int get_working_state();

    void show_room_num();
    void show_target_temp();
    void show_current_temp();
    void show_current_wind();
    void show_fee();
    void show_working_state();

    void clear_data();


private slots:
    void compute_fee();
    void temp_change();

private:
    int room_num;
    int target_temp;
    float current_temp;
    int current_wind;
    float fee;
    float energy;
    int working_state;
    int work_mode;
    int main_working_state;


    QLabel *room_num_area;
    QLabel *target_temp_area;
    QLabel *current_temp_area;
    QLabel *current_wind_area;
    QLabel *fee_area;
    QLabel *energy_area;
    QLabel *working_state_area;
    MainEngine *zhuji;
};


#endif // ROOM_H
