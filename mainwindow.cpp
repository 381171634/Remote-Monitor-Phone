#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>
#include <QDateTime>
#include <QSettings>
#include <QDir>
#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Gui_init();
}

MainWindow::~MainWindow()
{
    changeOnlineState(PHONE_DISCONNECT);
    proc.socket.disconnectFromHost();
    proc.timer.stop();
    delete ui;
}

void MainWindow::on_lineEdit_returnPressed()
{
    if(ui->lineEdit->text() == QString("点击输入设备ID"))
    {
        ui->lineEdit->text().clear();
    }
}

void MainWindow::changeOnlineState(int type)
{
    if(type == PHONE_DISCONNECT)
    {
        ui->label_3->setVisible(false);
        ui->groupBox->setVisible(false);
        ui->groupBox_2->setVisible(false);
        ui->label_4->setVisible(false);
        ui->label_8->setVisible(true);
        ui->progressBar->setVisible(false);
        ui->lineEdit->setEnabled(true);
        ui->pushButton->setText(QString("连接设备"));


        disconnect(&proc.socket,SIGNAL(readyRead()),&proc,SLOT(read_socket()));
        disconnect(&proc.socket,SIGNAL(disconnected()),&proc,SLOT(phone_disconn()));
        disconnect(&proc.socket,SIGNAL(disconnected()),this,SLOT(phone_disconn()));
        disconnect(&proc,SIGNAL(update_record()),this,SLOT(update_record()));

    }
    else if(type == PHONE_ONLINE)
    {
        ui->label_3->setVisible(true);
        ui->groupBox->setVisible(true);
        ui->groupBox_2->setVisible(true);
        ui->label_4->setVisible(true);
        ui->label_8->setVisible(false);
        ui->lineEdit->setEnabled(false);
        ui->pushButton->setText(QString("断开连接"));
    }

}

void MainWindow::on_pushButton_clicked()
{
    int res;
    if(ui->pushButton->text() == QString("连接设备"))
    {
        ui->pushButton->setEnabled(false);
        proc.socket.connectToHost(SERVER_IP,SERVER_PORT);
        if(proc.socket.waitForConnected(10000))
        {
            connect(&proc.socket,SIGNAL(readyRead()),&proc,SLOT(read_socket()));
            connect(&proc.socket,SIGNAL(disconnected()),&proc,SLOT(phone_disconn()));
            connect(&proc.socket,SIGNAL(disconnected()),this,SLOT(phone_disconn()));
            connect(&proc,SIGNAL(update_record()),this,SLOT(update_record()));

            res = proc.phone_online(ui->lineEdit->text());
            if(res == PROC_OK)
            {
                QMessageBox::information(this,"","连接成功");
                operateLocalID(LOCAL_ID_SAVE,ui->lineEdit->text());
                update_LastSample();
                changeOnlineState(PHONE_ONLINE);
            }
            else
            {
                if(res == PROC_TIMEOUT)
                {
                    QMessageBox::information(this,"","通讯超时");
                }
                else if(res == PROC_ERR)
                {
                    QMessageBox::information(this,"","设备未找到");
                }

                changeOnlineState(PHONE_DISCONNECT);
            }
        }
        else
        {
            QMessageBox::information(this,"","服务器连接失败");
            changeOnlineState(PHONE_DISCONNECT);
        }

        ui->pushButton->setEnabled(true);
    }
    else if(ui->pushButton->text() == QString("断开连接"))
    {
        proc.socket.disconnectFromHost();
        changeOnlineState(PHONE_DISCONNECT);
    }

}

QString MainWindow::operateLocalID(int type, QString ID)
{
    QFile fpini;
    QString res;

    QSettings ini("dev_id.ini",QSettings::IniFormat);
    if(type == LOCAL_ID_READ)
    {
        res = ini.value("dev_id/vaild_id").toString();
    }
    else if(type == LOCAL_ID_SAVE)
    {
        ini.setValue("dev_id/vaild_id",ID);
    }

    return  res;
}

void MainWindow::update_LastSample()
{
    int i;
    SampleDataTypedef data;

    memcpy(&data,proc.contentBuf + 2,sizeof(SampleDataTypedef));

    ui->tableWidget_3->setItem(0,0,new QTableWidgetItem(QDateTime::fromTime_t(data.timeTick).toString("yyyy-MM-dd hh:mm:ss")));
    ui->tableWidget_3->setItem(0,1,new QTableWidgetItem(QString::number((float)data.tempture/1000,'f',2)));
    ui->tableWidget_3->setItem(0,2,new QTableWidgetItem(QString::number((float)data.humidity/1000,'f',2)));
    ui->tableWidget_3->setItem(0,3,new QTableWidgetItem(QString::number((float)data.HCHO/1000/1000*1.341,'f',4)));
    ui->tableWidget_3->setItem(0,4,new QTableWidgetItem(QString::number((float)data.CO2/1000,'f',2)));
    ui->tableWidget_3->setItem(0,5,new QTableWidgetItem(QString::number((float)data.cellVoltage/1000,'f',3)));

    for(i = 0; i <ui->tableWidget_3->columnCount();i++)
    {
        ui->tableWidget_3->item(0,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }

    ui->tableWidget_3->resizeColumnToContents(0);
    ui->tableWidget_3->resizeColumnToContents(3);
    ui->tableWidget_3->resizeColumnToContents(4);
}

void MainWindow::update_record()
{
    uint8_t *p = proc.contentBuf;
    uint8_t i,j;
    uint8_t cnt;

    SampleDataTypedef data;

    ui->progressBar->setValue(p[2] * 100 / p[1]);
    cnt = p[3] | (p[4] << 8);

    p += 5;

    table_total_row += cnt;
    ui->tableWidget->setRowCount(table_total_row);

    for(i = 0;i < cnt; i++)
    {
        memcpy(&data,p,sizeof(SampleDataTypedef));

        ui->tableWidget->setItem(table_cur_row,0,new QTableWidgetItem(QDateTime::fromTime_t(data.timeTick).toString("yyyy-MM-dd hh:mm:ss")));
        ui->tableWidget->setItem(table_cur_row,1,new QTableWidgetItem(QString::number((float)data.tempture/1000,'f',2)));
        ui->tableWidget->setItem(table_cur_row,2,new QTableWidgetItem(QString::number((float)data.humidity/1000,'f',2)));
        ui->tableWidget->setItem(table_cur_row,3,new QTableWidgetItem(QString::number((float)data.HCHO/1000/1000*1.341,'f',4)));
        ui->tableWidget->setItem(table_cur_row,4,new QTableWidgetItem(QString::number((float)data.CO2/1000,'f',2)));
        ui->tableWidget->setItem(table_cur_row,5,new QTableWidgetItem(QString::number((float)data.cellVoltage/1000,'f',3)));

        for(j = 0; j <ui->tableWidget->columnCount();j++)
        {
            ui->tableWidget->item(table_cur_row,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }

        table_cur_row++;
        p += sizeof(SampleDataTypedef);
    }

    ui->tableWidget->resizeColumnToContents(0);
    ui->tableWidget->resizeColumnToContents(3);
    ui->tableWidget->resizeColumnToContents(4);

    proc.flag_record_ui_ok = 1;

    ui->tableWidget->scrollToBottom();
}

void MainWindow::on_pushButton_2_clicked()
{
    uint32_t startTick,endTick;
    int res;

    startTick = ui->dateEdit->dateTime().toTime_t();
    endTick = ui->dateEdit_2->dateTime().toTime_t();

    qDebug()<<"start:"<<startTick<<" end:"<<endTick;




    if(endTick == startTick)
    {
        QMessageBox::information(this,"","起止时间不可一致");
    }
    else if(endTick < startTick)
    {
        QMessageBox::information(this,"","结束时间小于起始时间");
    }
    else
    {
        ui->pushButton_2->setVisible(false);
        ui->progressBar->setVisible(true);
        ui->progressBar->setValue(0);
        ui->tableWidget->clearContents();

        table_cur_row = 0;
        table_total_row = 0;

        res = proc.deal_record(startTick,endTick,ui->lineEdit->text());
        if(res == PROC_TIMEOUT)
        {
            QMessageBox::information(this,"","通讯超时");
        }
        else if(res == PROC_ERR)
        {
            QMessageBox::information(this,"","记录未找到");
        }
        else if(res == PROC_OK)
        {
            QMessageBox::information(this,"","读取成功,共" + QString::number(table_total_row) + "条记录");
        }
    }

    table_cur_row = 0;
    table_total_row = 0;

    ui->pushButton_2->setVisible(true);
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
}
