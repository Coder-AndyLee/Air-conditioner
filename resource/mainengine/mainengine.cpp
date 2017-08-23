#include <QtGui>
#include <QPushButton>
#include <QDebug>
#include <QDesktopServices>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QtSql>
#include <math.h>
#include <windows.h>
#include "mainengine.h"
#include "room.h"
#include "ui_mainengine.h"

room *room_[6];
room *request_handle[3];
room *waiting[3];

MainEngine::MainEngine(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainEngine)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
             this,SLOT(displayError(QAbstractSocket::SocketError)));
    resize(1024,630);
    setWindowTitle("MainEngine");  //设置标题
    set_main_working_state(OFF);
    working_mode=-1;               //initial

    extension_num=0;
    handle_num=0;
    wait_num=0;

    for(int i=0;i<6;i++)
    {
        room_[i]=NULL;
    }


    room_[0]=new room(this,ui->room_num_1_input,ui->target_temp_1_input,ui->current_temp_1_input,ui->current_wind_1_input,ui->fee_1_input,ui->current_energy_1_input,ui->working_state_1_input);
    room_[1]=new room(this,ui->room_num_2_input,ui->target_temp_2_input,ui->current_temp_2_input,ui->current_wind_2_input,ui->fee_2_input,ui->current_energy_2_input,ui->working_state_2_input);
    room_[2]=new room(this,ui->room_num_3_input,ui->target_temp_3_input,ui->current_temp_3_input,ui->current_wind_3_input,ui->fee_3_input,ui->current_energy_3_input,ui->working_state_3_input);
    room_[3]=new room(this,ui->room_num_4_input,ui->target_temp_4_input,ui->current_temp_4_input,ui->current_wind_4_input,ui->fee_4_input,ui->current_energy_4_input,ui->working_state_4_input);
    room_[4]=new room(this,ui->room_num_5_input,ui->target_temp_5_input,ui->current_temp_5_input,ui->current_wind_5_input,ui->fee_5_input,ui->current_energy_5_input,ui->working_state_5_input);
    room_[5]=new room(this,ui->room_num_6_input,ui->target_temp_6_input,ui->current_temp_6_input,ui->current_wind_6_input,ui->fee_6_input,ui->current_energy_6_input,ui->working_state_6_input);

    QTimer *timer=new QTimer(this);
    timer->start(1000);

    connect(timer,SIGNAL(timeout()),this,SLOT(run_time()));
    connect(timer,SIGNAL(timeout()),this,SLOT(data_show()));
    connect(timer,SIGNAL(timeout()),this,SLOT(spy_on()));

    hour = 0;
    minute = 0;
    second = 0;

    count=1;
    update_frequency=1;

    QSqlQuery query;
}

MainEngine::~MainEngine()
{
    delete ui;
}
void MainEngine::sendMessage(QString DATA)
{
    if (tcpSocket->waitForConnected(3000))
    {
        const QByteArray text = DATA.toLocal8Bit();
        const char *dir = text.data();
        tcpSocket->write(dir);

        //tcpSocket->write(Dstream);
        tcpSocket->flush();
        tcpSocket->waitForReadyRead(500);
    }
    else
    {
        qDebug() << "not connected!";
    }
}
void MainEngine::readMessage()
{
    int i;
    tcpSocket->waitForReadyRead(3000);
    qDebug() << "Reading:" << tcpSocket->bytesAvailable();
    message = tcpSocket->readAll().data();
    qDebug()<< message;
    data_handle(message);
    qDebug()<<"request is:"<<recv_request;

    if(main_working_state!=OFF)
    {
        if(recv_request==1 && extension_num<=5)   //开机请求
        {
            for(i=0;i<6;i++)
            {
                if(room_[i]->get_room_num()==-1)
                    break;
            }
            QDateTime open_datetime;
            open_datetime.setDate(date.currentDate());
            open_datetime.addSecs(recv_temp);
            QString open_date=open_datetime.toString("yyyy-MM-dd");

            QSqlQuery query;
            char* room;
            itoa(i+1 , room , 10);

            query.exec(tr("INSERT INTO `on_off` (`room_num`, `time`, `date`) VALUES (")+tr(room)+tr(" , '1', '")+open_date+tr("')"));

            room_[i]->set_room_num(i+1);
            room_[i]->set_target_temp(INIT_TEMP);
            room_[i]->set_current_temp(INIT_TEMP);
            room_[i]->set_current_wind(MEDIUM);
            room_[i]->set_fee(0);
            room_[i]->set_working_state(WAITING);
            set_main_working_state(RUNNING);
            extension_num++;

            QString Order;
            char sr[20],st[20],scw[20],sct[20],sm[20];
            sprintf(sr,"%d",i+1);
            sprintf(st,"%d",INIT_TEMP);
            sprintf(scw,"%d",MEDIUM);
            sprintf(sct,"%d",INIT_TEMP);
            sprintf(sm,"%d",working_mode);
            Order=part_1+part_2+sr+part_10+part_3+sct+part_10+part_5+st+part_10+part_6+sm+part_10+part_4+scw+part_10+part_8+part_9;

            sendMessage(Order);

        }
        else if(recv_request==2) //关机、风速、温度请求
        {
            if(on==2)   //关机请求
            {
                int flag=0;
                extension_num--;
                for(int i=0;i<handle_num;i++)
                {
                    if(request_handle[i]->room_num==room_id)
                    {
                        request_handle[i]->set_working_state(OFF);
                        room_[room_id-1]->clear_data();
                        room_[room_id-1]->room_num=-1;
                        flag=1;
                        break;
                    }
                }

                for(int i=0;i<wait_num;i++)
                {
                    if(waiting[i]->room_num==room_id)
                    {
                        room_[room_id-1]->clear_data();
                        room_[room_id-1]->room_num=-1;
                        for(int j=i;j<wait_num-1;j++)
                        {
                            waiting[j]=waiting[j+1];
                        }
                        waiting[wait_num]=NULL;
                        wait_num--;
                        flag=2;
                        break;
                    }
                }

                if(flag==0)
                {
                    room_[room_id-1]->clear_data();
                    room_[room_id-1]->room_num=-1;
                }

                if(extension_num==0)
                    set_main_working_state(WAITING);
            }
            else if(on==1)   //????
            {
                room_[room_id-1]->set_working_state(WAITING);

            }
            if(recv_temp != -1)//温度请求
            {
                int flag=0;
                room_[room_id-1]->set_target_temp(recv_temp);
                for(int i=0;i<handle_num;i++)
                {
                    if(request_handle[i]->get_room_num()==room_id)
                    {
                        flag=1;
                        break;
                    }
                }
                for(int i=0;i<wait_num;i++)
                {
                    if(waiting[i]->get_room_num()==room_id)
                    {
                        flag=2;
                        break;
                    }
                }

                if(flag==0)
                {
                    if(main_working_state==RUNNING)
                    {
                        request_handle[handle_num]=room_[room_id-1];
                        handle_num++;
                        room_[room_id-1]->set_working_state(RUNNING);

                        int get_time=0;
                        double estimate_fee=0;
                        get_time = abs((recv_temp - room_[room_id-1]->get_current_temp())/0.2);
                        estimate_fee = (float)get_time/12.0;//即  60/5

                        QDateTime quest_datetime;
                        quest_datetime.setTime(time.currentTime());
                        quest_datetime.setDate(date.currentDate());

                        QString open_date=quest_datetime.toString("yyyy-MM-dd hh:mm:ss");
                        quest_datetime=quest_datetime.addSecs(get_time);

                        QString end_date= quest_datetime.toString("yyyy-MM-dd hh:mm:ss");
                        QSqlQuery query;
                        query.exec(tr("INSERT INTO `request` (`room_num`, `start_time`, `end_time` ,`start_temp`,`end_temp`,`fee`) VALUES ( ")+tr("%1").arg(room_id)+tr(", '")+open_date+tr("' , '")+end_date+tr("' , ")+tr("%1").arg(room_[room_id-1]->get_current_temp())+tr(" , ")+tr("%1").arg(recv_temp)+tr(" , ")+tr("%1").arg(estimate_fee)+tr(")"));

                        if(handle_num==3)
                            set_main_working_state(RUNNING_BUSY);
                    }
                    else if(main_working_state==RUNNING_BUSY)
                    {
                        waiting[wait_num]=room_[room_id-1];
                        wait_num++;
                        room_[room_id-1]->set_working_state(WAIT_FOR_HANDLE);
                    }
                }

            }
            if(recv_wind!=-1&&room_id!=-1)//风速请求
            {
                room_[room_id-1]->set_current_wind(recv_wind);
            }
        }
        else if(recv_request==4) //关机
        {
            int flag=0;
            extension_num--;
            for(int i=0;i<handle_num;i++)
            {
                if(request_handle[i]->room_num==room_id)
                {
                    request_handle[i]->set_working_state(OFF);
                    room_[room_id-1]->clear_data();
                    room_[room_id-1]->room_num=-1;
                    flag=1;
                    break;
                }
            }

            for(int i=0;i<wait_num;i++)
            {
                if(waiting[i]->room_num==room_id)
                {
                    room_[room_id-1]->clear_data();
                    room_[room_id-1]->room_num=-1;
                    for(int j=i;j<wait_num-1;j++)
                    {
                        waiting[j]=waiting[j+1];
                    }
                    waiting[wait_num]=NULL;
                    wait_num--;
                    flag=2;
                    break;
                }
            }

            if(flag==0)
            {
                room_[room_id-1]->clear_data();
                room_[room_id-1]->room_num=-1;
            }

            if(extension_num==0)
                set_main_working_state(WAITING);
        }
     }
}

void MainEngine::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString(); //输出错误信息
}

void MainEngine::newConnect()
{
    //tcpSocket->abort(); //取消已有的连接
    tcpSocket->connectToHost("localhost",8888);//("10.105.248.225",8888);//
    //连接到主机，这里从界面获取主机地址和端口号
}

void MainEngine::on_open_main_clicked()
{
    if(main_working_state==OFF) //开机
    {
        newConnect();
        sendMessage(Order1_1);
        QMessageBox::information(this,tr("Result"),tr("Turn ON Success!"),QMessageBox::Ok);
        set_main_working_state(WAITING);

        QMessageBox change_mode;
        change_mode.setWindowTitle("Change Mode");
        change_mode.setText("Choose Mode");
        QPushButton *heat_mode = change_mode.addButton(tr("Heat"),QMessageBox::ActionRole);
        QPushButton *cool_mode = change_mode.addButton(tr("Cool"),QMessageBox::ActionRole);
        QPushButton *cancel = change_mode.addButton(tr("Cancel"),QMessageBox::RejectRole);
        //customMsgBox.setIconPixmap(QPixmap(":/images/linuxredhat.png"));
        //设置按钮背景图
        change_mode.exec();

        if(change_mode.clickedButton() == heat_mode)
        {
            set_working_mode(HEAT);
        }
        if(change_mode.clickedButton() == cool_mode)
        {
            set_working_mode(COOL);

        }

        //初始化主机的时间参数
        dt.setTime(time.currentTime());
        dt.setDate(date.currentDate());

        current_date=dt.toString("yyyy-MM-dd hh:mm:ss dddd");
        ui->start_time_input->setText(current_date);
        set_update_frequency(1);
    }
    else
    {
        QMessageBox::information(this,tr("Warning"),tr("Already Turned ON!"),QMessageBox::Ok);
    }
}

void MainEngine::on_close_main_clicked()
{
    tcpSocket->close();
    if(main_working_state==OFF)
    {
        QMessageBox::warning(this,tr("Warning"),tr("Already Shutted Down!"),QMessageBox::Ok);
    }
    else
    {
        QMessageBox::information(this,tr("Result"),tr("Shut Down Success!"),QMessageBox::Ok);
        //执行关机后的数据存储
        set_main_working_state(OFF);

        ui->working_mode_input->clear();
        ui->start_time_input->clear();
        ui->running_time_input->clear();
        ui->update_frequency_input->clear();

        for(int i=0;i<6;i++)
        {
            room_[i]->clear_data();
        }
    }
}

void MainEngine::on_set_working_mode_clicked()
{
    if(main_working_state==OFF)
    {
        QMessageBox::warning(this,tr("Warning"),tr("The Main Machine is OFF!"),QMessageBox::Ok);
    }
    else
    {
        QMessageBox change_mode;
        change_mode.setWindowTitle("Change Mode");
        change_mode.setText("Choose Mode");
        QPushButton *heat_mode = change_mode.addButton(tr("Heat"),QMessageBox::ActionRole);
        QPushButton *cool_mode = change_mode.addButton(tr("Cool"),QMessageBox::ActionRole);
        QPushButton *cancel = change_mode.addButton(tr("Cancel"),QMessageBox::RejectRole);
        //customMsgBox.setIconPixmap(QPixmap(":/images/linuxredhat.png"));
        //设置按钮背景图
        change_mode.exec();
        if(change_mode.clickedButton() == heat_mode)
        {
            set_working_mode(HEAT);
        }
        if(change_mode.clickedButton() == cool_mode)
        {
            set_working_mode(COOL);
        }
                 QString Order;
                 room_[0]->set_target_temp(25);
                 room_[1]->set_target_temp(25);
                 room_[2]->set_target_temp(25);
                 room_[3]->set_target_temp(25);
                 room_[4]->set_target_temp(25);
                 room_[5]->set_target_temp(25);
                 char sm[20],st[20];
                 sprintf(st,"%d",25);
                 sprintf(sm,"%d",working_mode);
                 Order=part_1+part_5+st+part_10+part_6+sm+part_10+part_11+part_9;
                 sendMessage(Order);
    }
}

void MainEngine::on_update_frequency_clicked()
{
    bool ok;
    if (main_working_state == OFF)
    {
        QMessageBox::information(this,tr("Tip"),tr("The Main engine is OFF!"),QMessageBox::Ok);
    }
    else
    {
        set_update_frequency(QInputDialog::getInt(this,tr("Set Update Frequency"),
                                                  tr("Choose number:"),1,1,5,1,&ok));
        count=1;
    }
}

void MainEngine::on_report_clicked()
{
    if(main_working_state==OFF)
        QMessageBox::warning(this,tr("warning"),tr("The Main Machine is OFF!"),QMessageBox::Ok);
    else
        QDesktopServices::openUrl(QUrl(QLatin1String("http://localhost/report/html/index.php")));
}

void MainEngine::run_time()   //运行时间
{
    QString run_time,tmp;
    if(main_working_state!=OFF)
    {
        if(working_mode!=-1)
        {
            if(++second>59)
            {
                second=0;
                if(++minute>59)
                {
                   minute=0;
                   hour++;

                }
            }
            tmp.setNum(hour);
            if(hour <10)
            {
                run_time+='0';
            }
            run_time+=tmp;
            run_time+=':';

            tmp.setNum(minute);
            if(minute<10)
            {
                run_time+='0';
            }
            run_time+=tmp;
            run_time+=':';
            tmp.setNum(second);
            if(second<10)
            {
                run_time+='0';
            }
            run_time+=tmp;
            ui->running_time_input->setText(run_time);
        }
    }
}

void MainEngine::data_show()
{
    if(main_working_state!=OFF)
    {
        if(update_frequency==count)
        {
            count=1;
            for(int i=0;i<6;i++)
            {
                if(room_[i]->get_room_num()!=-1)
                {
                    room_[i]->show_room_num();
                    room_[i]->show_target_temp();
                    room_[i]->show_current_temp();
                    room_[i]->show_current_wind();
                    room_[i]->show_fee();
                    room_[i]->show_working_state();

                    QString Order;
                    char sr[20]={0},sct[20]={0},scw[20]={0},st[20]={0},sfee[20]={0};
                    sprintf(sr,"%d",room_[i]->get_room_num());
                    sprintf(sct,"%f",room_[i]->get_current_temp());
                    sprintf(scw,"%d",room_[i]->get_current_wind());
                    sprintf(st,"%d",room_[i]->get_target_temp());
                    sprintf(sfee,"%f",room_[i]->get_fee());

                    QString state_send;
                    if(room_[i]->get_working_state()==OFF)
                    {
                        state_send="2";//SLAVE_OFF
                    }
                    if(room_[i]->get_working_state()==RUNNING)
                    {
                        state_send="1";//SLAVE_ON
                    }
                    if(room_[i]->get_working_state()==WAITING)
                    {
                        state_send="3";//SLAVE_WAITING
                    }
                    Order=part_1+part_2+sr+part_10+part_4+scw+part_10+part_5+st+part_10+part_fee+sfee+part_10+part_3+sct+part_10+part_12+",\"host\":1,\"on\":"+state_send+part_9;
                    sendMessage(Order);
                 }
            }
        }
        else
            count++;
    }
}

void MainEngine::spy_on()
{
    for(int i=0;i<handle_num;i++)
    {
        if(request_handle[i]->get_working_state()==WAITING||request_handle[i]->get_working_state()==OFF)
        {
            if(wait_num!=0)
            {
                request_handle[i]=waiting[0];
                request_handle[i]->set_working_state(RUNNING);

                int get_time=0;
                double estimate_fee=0;
                get_time = abs((request_handle[i]->get_target_temp() - request_handle[i]->get_current_temp())/0.2);
                estimate_fee = (float)get_time/12.0;//即  60/5

                QDateTime quest_datetime;
                quest_datetime.setTime(time.currentTime());
                quest_datetime.setDate(date.currentDate());

                QString open_date=quest_datetime.toString("yyyy-MM-dd hh:mm:ss");
                quest_datetime=quest_datetime.addSecs(get_time);

                QString end_date= quest_datetime.toString("yyyy-MM-dd hh:mm:ss");
                QSqlQuery query;
                query.exec(tr("INSERT INTO `request` (`room_num`, `start_time`, `end_time` ,`start_temp`,`end_temp`,`fee`) VALUES ( ")+tr("%1").arg(request_handle[i]->get_room_num())+tr(", '")+open_date+tr("' , '")+end_date+tr("' , ")+tr("%1").arg(request_handle[i]->get_current_temp())+tr(" , ")+tr("%1").arg(request_handle[i]->get_target_temp())+tr(" , ")+tr("%1").arg(estimate_fee)+tr(")"));

                for(int j=0;j<wait_num-1;j++)
                {
                    waiting[j]=waiting[j+1];
                }
                waiting[wait_num-1]=NULL;
                wait_num--;
            }
            else
            {

                for(int j=i;j<handle_num-1;j++)
                {
                    request_handle[j]=request_handle[j+1];
                }
                request_handle[handle_num-1]=NULL;
                handle_num--;
                if(handle_num==0)
                {
                    set_main_working_state(WAITING);
                }
                else
                {
                    set_main_working_state(RUNNING);
                }
            }
        }

    }
}

void MainEngine::set_main_working_state(int temp)
{
    main_working_state=temp;
    if(temp==RUNNING)
        ui->working_state_input->setText("RUNNING");
    else if(temp==WAITING)
       ui->working_state_input->setText("WAITING");
    else if(temp==RUNNING_BUSY)
       ui->working_state_input->setText("RUNNING_BUSY");
    else
       ui->working_state_input->setText("OFF");

}
void MainEngine::set_working_mode(int temp)
{
    working_mode=temp;
    if(working_mode==HEAT)
        ui->working_mode_input->setText("HEAT");
    else if(working_mode==COOL)
        ui->working_mode_input->setText("COOL");
}

void MainEngine::set_update_frequency(int temp)
{
    QString update_f;
    update_frequency=temp;
    update_f=QString::number(update_frequency);
    ui->update_frequency_input->setText(update_f);
}

int MainEngine::get_main_working_state()
{
    return main_working_state;
}
int MainEngine::get_working_mode()
{
    return working_mode;
}
int MainEngine::get_update_frequency()
{
    return update_frequency;
}

void MainEngine::data_handle(QString MESS)
{
        int i = 0,j,k,questlock=0;
        char buf[20]={0},buff[10]={0};
        recv_request = -1;   //接收的请求类型
        recv_temp = -1;
        recv_wind = -1;
        room_id = -1;
        on = -1;
        string d;
        string D[9];
        string str ;
        str = MESS.toStdString();
        //str="[{\"roomNum\":2,\"temp\":25.1,\"requestType\":1}]";
        //cout<<str<<endl;
        while(str[i] != NULL){
            memset(buf,0,sizeof(char)*20);
            memset(buff,0,sizeof(char)*2);
           if(str[i]=='\"')
           {
            j=0;
            i++;
            while(str[i]!='\"')
            {
               //cout<<str[i];
               buf[j]=str[i];
               //cout<<buf[j];
               j++;
               i++;
                  }
            i=i+2;
            k=0;
            while(str[i]=='0'||str[i]=='1'||str[i]=='2'
             ||str[i]=='3'||str[i]=='4'||str[i]=='5'
             ||str[i]=='6'||str[i]=='7'||str[i]=='8'
             ||str[i]=='9'||str[i]=='.'){
                buff[k]=str[i];
                i++;
                k++;
             }
           d=buf;
           //cout<<d<<endl;
           if(d=="roomNum")      {D[0]=buff; room_id = atoi(buff);     }
           if(d=="on")           {D[1]=buff; on = atoi(buff);          }
           if(d=="temp")         {D[2]=buff;                           }
           if(d=="tarTemp")      {D[3]=buff; recv_temp = atoi(buff);   }
           if(d=="speed")        {D[4]=buff; recv_wind = atoi(buff);   }
           if(d=="mode")         {D[5]=buff;                           }
           if(d=="cost")         {D[6]=buff;                           }
           if(d=="host")         {D[7]=buff;                           }
           if(d=="requestType")  {D[8]=buff; recv_request = atoi(buff);}
           }
           else
             i++;
        }
}



