#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Gui_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_returnPressed()
{
    if(ui->lineEdit->text() == QString("点击输入设备ID"))
    {
        ui->lineEdit->text().clear();
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text() == QString("连接设备"))
    {
        proc.socket.connectToHost(SERVER_IP,SERVER_PORT);
        if(proc.socket.waitForConnected(10000))
        {
            connect(&proc.socket,SIGNAL(readyRead()),&proc,SLOT(read_socket()));
            proc.phone_online(ui->lineEdit->text());

            ui->label_3->setEnabled(true);
            ui->groupBox->setEnabled(true);
            ui->label_4->setEnabled(true);
            ui->scrollArea->setEnabled(true);
            ui->pushButton->setText(QString("断开连接"));
        }
        else
        {

        }
    }
    else if(ui->pushButton->text() == QString("断开连接"))
    {
        proc.socket.disconnectFromHost();
        ui->label_3->setEnabled(false);
        ui->groupBox->setEnabled(false);
        ui->label_4->setEnabled(false);
        ui->scrollArea->setEnabled(false);
        ui->pushButton->setText(QString("连接设备"));
    }

}
