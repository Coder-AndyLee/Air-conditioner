#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    ui->passwordlineEdit->setEchoMode(QLineEdit::Password);

}

login::~login()
{
    delete ui;
}


void login::on_loginButton_clicked()
{
    if(ui->userlineEdit->text()==tr("admin")&&ui->passwordlineEdit->text()==tr("123"))
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this,tr("warning"),tr("username or password is wrong"),QMessageBox::Yes);
    }
}

void login::on_cancelButton_clicked()
{
    this->close();
}
