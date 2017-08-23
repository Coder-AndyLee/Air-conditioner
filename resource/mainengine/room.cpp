#include<QString>
#include<QTimer>
#include <QTime>
#include<QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include "room.h"
#include "ui_mainengine.h"

room::room(MainEngine *zhuji_,QLabel *room_num_a,QLabel *target_temp_a,QLabel *current_temp_a,
           QLabel *current_wind_a,QLabel *fee_a,QLabel *energy_a,QLabel *working_state_a)
{
    QTimer *timer=new QTimer;
    timer->start(1000);

    room_num_area=room_num_a;
    target_temp_area=target_temp_a;
    current_temp_area=current_temp_a;
    current_wind_area=current_wind_a;
    fee_area=fee_a;
    energy_area=energy_a;
    working_state_area=working_state_a;

    zhuji = zhuji_;
    room_num=-1;
    connect(timer,SIGNAL(timeout()),this,SLOT(compute_fee()));
    connect(timer,SIGNAL(timeout()),this,SLOT(temp_change()));
}

room::~room()
{

}

void room::compute_fee()
{
    if(working_state==RUNNING)
    {
        if(current_wind==HIGH)
        {
            fee=fee+0.11;
        }
        else if(current_wind==LOW)
        {
            fee=fee+0.07;
        }
        else
        {
            fee=fee+0.08;
        }
    }
}

void room::temp_change()
{
    work_mode=zhuji->get_working_mode();//!!!!!!!!!!!
    if(working_state==RUNNING)
    {
       if(target_temp>current_temp)
       {
           if(work_mode==HEAT)
           {
                if(current_wind==HIGH)
                {
                    current_temp=current_temp+0.04;
                }
                else if(current_wind==MEDIUM)
                {
                    current_temp=current_temp+0.02;
                }
                else
                {
                    current_temp=current_temp+0.01;
                }

                if(target_temp <= current_temp)
                {
                    set_working_state(WAITING);
                    //发送待机信号
                }
           }
           else if(work_mode==COOL)
               set_working_state(WAITING);
        }
       else if(target_temp< current_temp)
       {
           if(work_mode==COOL)
           {
                if(current_wind==HIGH)
                {
                    current_temp=current_temp-0.04;

                }
                else if(current_wind==MEDIUM)
                {
                    current_temp=current_temp-0.02;
                }
                else
                {
                    current_temp=current_temp-0.01;
                }

                if(target_temp >= current_temp)
                {
                    set_working_state(WAITING);
                }
           }
           else if(work_mode==HEAT)
               set_working_state(WAITING);
       }
    }

       else if(working_state==WAITING)
       {
            if(target_temp-current_temp>=1||target_temp-current_temp<=-1)
                set_working_state(RUNNING);
            else
            {
                if(work_mode==HEAT)
                    current_temp=current_temp-0.02;
                else if(work_mode==COOL)
                    current_temp=current_temp+0.02;
            }
       }
}


void room::set_room_num(int temp)
{
    room_num=temp;
}
void room::set_target_temp(int temp)
{
    target_temp=temp;
}
void room::set_current_temp(float temp)
{
    current_temp=temp;
}
void room::set_current_wind(int temp)
{
    current_wind=temp;
}
void room::set_fee(float temp)
{
    fee=temp;
}
void room::set_working_state(int temp)
{
    working_state=temp;

}
void room::show_room_num()
{
        if(working_state!=OFF)
        {
            QString room_n=QString::number(room_num);
            room_num_area->setText(room_n);
        }
        else
            room_num_area->clear();

}

void room::show_target_temp()
{

        if(working_state!=OFF)
        {
            QString target_t=QString::number(target_temp);
            target_temp_area->setText(target_t);
        }
        else
            target_temp_area->clear();

}

void room::show_current_temp()
{

        if(working_state!=OFF)
        {
            QString current_t=QString::number(current_temp,'f',2);
            current_temp_area->setText(current_t);
        }
        else
            current_temp_area->clear();

}

void room::show_current_wind()
{

        if(working_state!=OFF)
        {
            if(current_wind==HIGH)
                current_wind_area->setText("HIGH");
            else if(current_wind==MEDIUM)
                current_wind_area->setText("MEDIUM");
            else
                current_wind_area->setText("LOW");
        }
        else
            current_wind_area->clear();

}

void room::show_fee()
{
    if(working_state!=OFF)
    {
        QString fee_t=QString::number(fee,'f',2);
        QString energy_t=QString::number(fee/5.0,'f',2);
        fee_area->setText(fee_t);
        energy_area->setText(energy_t);
    }
    else
    {
        fee_area->clear();
        energy_area->clear();
    }
}

void room::show_working_state()
{
        if(working_state==RUNNING)
            working_state_area->setText("RUNNING");
        else if(working_state==WAITING)
            working_state_area->setText("WAITING");
        else if(working_state==WAIT_FOR_HANDLE)
            working_state_area->setText("WAIT_FOR_HANDLE");
        else
            working_state_area->clear();
}

 void room::clear_data()
 {
     room_num_area->clear();
     current_temp_area->clear();
     target_temp_area->clear();
     fee_area->clear();
     energy_area->clear();
     working_state_area->clear();
     current_wind_area->clear();
 }

int room::get_room_num()
{
    return room_num;
}
int room::get_target_temp()
{
    return target_temp;
}
float room::get_current_temp()
{
    return current_temp;
}
int room::get_current_wind()
{
    return current_wind;
}
float room::get_fee()
{
    return fee;
}
int room::get_working_state()
{
    return working_state;
}
