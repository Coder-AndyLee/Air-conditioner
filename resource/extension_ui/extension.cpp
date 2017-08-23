#include <QString>
#include "extension.h"
#include "ui_extension.h"
#include <QTimer>
#include <QTime>

Extension::Extension(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Extension)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    //tcpServer = new QTcpServer(this);

    resize(700,500);
    setWindowTitle("Extension");//设置标题

    set_working_state(OFF);
    set_room_num(-1);

    QTimer *timer=new QTimer(this);
    timer->start(1000);

    connect(timer,SIGNAL(timeout()),this,SLOT(run_time()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
             this,SLOT(displayError(QAbstractSocket::SocketError)));
    hour = 0;
    minute = 0;
    second = 0;
}

Extension::~Extension()
{
    delete ui;
}
void Extension::sendMessage(QString DATA)
{
    if (tcpSocket->waitForConnected(3000))
    {
            qDebug() << "connected!";
            const QByteArray text = DATA.toLocal8Bit();
            const char *dir = text.data();
            tcpSocket->write(dir);

            //tcpSocket->write(Dstream);
            tcpSocket->waitForBytesWritten(1000);
            tcpSocket->waitForReadyRead(500);
    }
    else
    {
        qDebug() << "not connected!";
    }
}
void Extension::readMessage()
{
    tcpSocket->waitForReadyRead(500);
    qDebug() << "Reading:" << tcpSocket->bytesAvailable();

    message = tcpSocket->readAll().data();
    datahandle(message);
    qDebug()<<message;

    if(working_state == ON)
    {
        if(recv_request==2)
        {
            if(room_num == -1)
            {

                set_room_num(room_id);
                set_target_temp(recv_temp);
                set_current_temp(recv_curTemp);
                set_current_wind(recv_wind);
                set_main_working_mode(recv_mode);
            }
        }
        else if(recv_request == 3)
        {
            set_main_working_mode(recv_mode);
            set_target_temp(recv_temp);
        }
        else if(recv_request == 4)
        {
            if(room_id == room_num)
            {
                set_current_temp(recv_curTemp);
                set_target_temp(recv_temp);
                set_current_wind(recv_wind);
                set_fee(recv_fee);

            }
        }
    }
}
void Extension::newConnect()
{
    //blockSize = 0; //初始化其为0
    //tcpSocket->abort(); //取消已有的连接
    tcpSocket->connectToHost("10.105.248.225",8888);//("10.105.248.225",8888);//
    //连接到主机，这里从界面获取主机地址和端口号

    }
void Extension::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString(); //输出错误信息
}
void Extension::on_open_extension_clicked()//开机
{
    if (working_state != OFF)
    {
        QMessageBox::information(this,tr("Warning"),tr("Already Open!"),QMessageBox::Ok);
    }
    else
    {
        set_working_state(ON);
        //初始工作模式的初始化需要在接收主机报文时设置
        newConnect();//连接

        sendMessage(Order1_2);//注册

        QString Order;
        Order = part_1+part_7+part_9;
        sendMessage(Order);//开机请求111

        char room_num_char[3];
        itoa(room_num,room_num_char,10);
        Order = part_1+part_2+room_num_char+part_10+part_13+"1,"+part_7+part_9;//【！！！！】
        sendMessage(Order);//开机请求

        QMessageBox::information(this,tr("Result"),tr("Open Success!"),QMessageBox::Ok);
        dt.setTime(time.currentTime());
        dt.setDate(date.currentDate());
        current_date=dt.toString("yyyy-MM-dd hh:mm:ss");
        ui->start_time_input->setText(current_date);
   }
}

void Extension::on_close_extension_clicked()//关机
{
    if(working_state == OFF)
    {
        QMessageBox::information(this,tr("Warning"),tr("Already Shut Down!"),QMessageBox::Ok);
    }
    else
    {
        int on = 2;
        QString Order;
        char sr[20],so[20];
        sprintf(sr,"%d",room_num);
        sprintf(so,"%d",on);
        Order=part_1+part_2+sr+part_10+part_13+so+part_10+part_8+part_9;
        sendMessage(Order);

        set_room_num(-1);
        set_main_working_mode(-1);
        set_working_state(OFF);
        set_current_temp(NULL);
        set_target_temp(NULL);
        set_current_wind(NULL);
        ui->current_fee_input->clear();
        ui->current_energy_input->clear();
        ui->start_time_input->clear();

        //加通信内容，向服务器断开连接
        QMessageBox::information(this,tr("Result"),tr("Shut Down Success!"),QMessageBox::Ok);
    }
}

void Extension::on_set_temperature_clicked()
{
    bool ok;
    if (working_state == OFF)
    {
        QMessageBox::information(this,tr("Warning"),tr("The Extension is Closed!"),QMessageBox::Ok);
    }
    else
    {
        if(main_working_mode == HEAT)
        {
            target_temp = QInputDialog::getDouble(this,tr("Set Temperature"),tr("Choose Temperature:"),INIT_TEMP,25,30,1,&ok);
            QString Order;
            char sr[20],st[20];
            sprintf(sr,"%d",room_num);
            sprintf(st,"%d",target_temp);
            Order=part_1+part_2+sr+part_10+part_5+st+part_10+part_8+part_9;
            sendMessage(Order);
        }
        else if(main_working_mode==COOL)
        {
            target_temp = QInputDialog::getDouble(this,tr("Set Temperature"),tr("Choose Temperature:"),INIT_TEMP,18,25,1,&ok);
            QString Order;
            char sr[20],st[20];
            sprintf(sr,"%d",room_num);
            sprintf(st,"%d",target_temp);
            Order=part_1+part_2+sr+part_10+part_5+st+part_10+part_8+part_9;
            sendMessage(Order);
        }
    }
}

void Extension::on_set_wind_clicked()
{
    bool ok;
    if(working_state == OFF)
    {
        QMessageBox::information(this,tr("Warning"),tr("The Extension is Closed!"),QMessageBox::Ok);
    }
    else
    {
        //current_wind = QInputDialog::getInt(this,tr("Set WindSpeed"),
        //                                    tr("Choose WindSpeed:"),2,1,3,1,&ok);
        QMessageBox set_wind;
        set_wind.setWindowTitle("Set Wind Speed");
        set_wind.setText("Set Speed");
        QPushButton *high = set_wind.addButton(tr("High"),QMessageBox::ActionRole);
        QPushButton *medium = set_wind.addButton(tr("Medium"),QMessageBox::ActionRole);
        QPushButton *low= set_wind.addButton(tr("Low"),QMessageBox::ActionRole);
        QPushButton *cancel = set_wind.addButton(tr("Cancel"),QMessageBox::RejectRole);
        //customMsgBox.setIconPixmap(QPixmap(":/images/linuxredhat.png"));
        //设置按钮背景图
        set_wind.exec();

        if(set_wind.clickedButton() == high)
        {
            set_current_wind(HIGH);
        }
        if(set_wind.clickedButton() == medium)
        {
            set_current_wind(MEDIUM);
        }
        if(set_wind.clickedButton() == low)
        {
            set_current_wind(LOW);
        }


        QString Order;
        char sr[20],scw[20];
        sprintf(sr,"%d",room_num);
        sprintf(scw,"%d",current_wind);
        Order=part_1+part_2+sr+part_10+part_4+scw+part_10+part_8+part_9;
        sendMessage(Order);
    }
}

void Extension::set_room_num(int temp)
{
    room_num=temp;
    if(temp == -1)
        ui->room_num_input->clear();
    else
    {
        ui->room_num_input->setText(QString::number(temp,10));
    }

}
void Extension::set_target_temp(int temp)
{
    target_temp=temp;
    if(temp == NULL)
        ui->target_temp_input->clear();
    else
        ui->target_temp_input->setText(QString::number(temp,10));
}
void Extension::set_current_temp(float temp)
{
    current_temp=temp;
    if(temp == NULL)
        ui->current_temp_input->clear();
    else
        ui->current_temp_input->setText(QString("%1").arg(temp));
}
void Extension::set_current_wind(int temp)
{
    current_wind=temp;
    if(temp == HIGH)
        ui->current_wind_input->setText("High");
    else if(temp == MEDIUM)
        ui->current_wind_input->setText("Medium");
    else if(temp == LOW)
        ui->current_wind_input->setText("Low");
    else if(temp == NULL)
        ui->current_wind_input->clear();
}
void Extension::set_fee(float temp)
{
    fee=temp;
    QString fee_t=QString::number(fee,'f',2);
    QString energy_t=QString::number(fee/5.0,'f',2);
    ui->current_fee_input->setText(fee_t);
    ui->current_energy_input->setText(energy_t);
}
void Extension::set_working_state(int temp)
{
    working_state=temp;
}
void Extension::set_main_working_mode(int temp)
{
    main_working_mode = temp;
    if(main_working_mode==-1)
        ui->working_mode_input->clear();
    else if(main_working_mode == COOL)
        ui->working_mode_input->setText("COOL");
    else if(main_working_mode == HEAT)
        ui->working_mode_input->setText("HEAT");
}

int Extension::get_room_num()
{
    return room_num;
}
int Extension::get_target_temp()
{
    return target_temp;
}
float Extension::get_current_temp()
{
    return current_temp;
}
int Extension::get_current_wind()
{
    return current_wind;
}
float Extension::get_fee()
{
    return fee;
}
int Extension::get_working_state()
{
    return working_state;
}
int Extension::get_main_working_mode()
{
    return main_working_mode;
}
Ui::Extension Extension::get_ui()
{
    return *ui;
}
void Extension::datahandle(QString MESS)
{
    int i = 0,j,k,questlock=0;
    char buf[20]={0},buff[10]={0};
    recv_request = -1;   //接收的请求类型
    recv_temp = -1;
    recv_wind = -1;
    room_id = -1;
    recv_mode = -1;
    recv_curTemp = -1;
    string d;
    string D[10];
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
       if(d=="on")           {D[1]=buff;                           }
       if(d=="temp")         {D[2]=buff; recv_curTemp=atof(buff);  }
       if(d=="tarTemp")      {D[3]=buff; recv_temp = atoi(buff); qDebug()<<recv_temp<<"  %%%";
           }

       if(d=="speed")        {D[4]=buff; recv_wind = atoi(buff);   }
       if(d=="mode")         {D[5]=buff; recv_mode = atoi(buff);   }
       //if(d=="cost")         {D[6]=buff;                           }
       if(d=="host")         {D[7]=buff;                           }
       if(d=="cost")          {D[8]=buff; recv_fee = atof(buff);    }
       if(d=="requestType")
          {
               questlock++;
               if(questlock == 1)
               {
                  D[9]=buff;
                  recv_request = atoi(buff);
               }
          }
       }
       else
         i++;
    }
}
