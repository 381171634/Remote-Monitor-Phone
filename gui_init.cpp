#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gui_init.h"

int GUI_INIT::getPixByPt(int Pt)
{
    return this->DPI*Pt/72;
}

float GUI_INIT::getScaleByPt(int Pt)
{
    return Pt/(2250.0*72/216);
}

int GUI_INIT::getFontByPt(int Pt)
{
    return (int)(this->height * 72.0 / this->DPI * this->getScaleByPt(Pt));
}

void MainWindow::phone_disconn()
{
    changeOnlineState(PHONE_DISCONNECT);
}



void MainWindow::Gui_init()
{
    changeOnlineState(PHONE_DISCONNECT);
    //屏幕适配
    gui_init.screen = QGuiApplication::primaryScreen();
    gui_init.DPI = gui_init.screen->logicalDotsPerInch();
    gui_init.height = gui_init.screen->availableSize().height();
    //设置各插件字体 适应不同屏幕
    ui->lineEdit->setFont(QFont("Consolas", gui_init.getFontByPt(16)));
    ui->pushButton->setFont(QFont("Consolas", gui_init.getFontByPt(16)));
    ui->label_2->setFont(QFont("Consolas", gui_init.getFontByPt(2)));
    ui->tableWidget_3->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    ui->label_3->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    ui->label_4->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    ui->label_7->setFont(QFont("Consolas", gui_init.getFontByPt(12)));

    //历史数据栏
    ui->label_5->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    ui->label_6->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    ui->pushButton_2->setFont(QFont("Consolas", gui_init.getFontByPt(16)));
    //ui->tableWidget_3->horizontalHeader()->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    //设置各插件样式表
    ui->pushButton->setStyleSheet(QString("QPushButton{\
                                  border:%1px groove gray;\
                                  border-radius:%2px;\
                                  color:white;\
                                  background-color:  rgb(40,160,224);\
                                  }\
                                 QPushButton:pressed{\
                                 border-style:inset;\
                                 }")
                                .arg(QString::number(gui_init.getPixByPt(1))).arg(QString::number(gui_init.getPixByPt(8))));
    ui->lineEdit->setStyleSheet(QString("QLineEdit{\
                                    border:%1px groove gray;\
                                    border-radius:%2px;\
                                    }")
                                .arg(QString::number(gui_init.getPixByPt(1))).arg(QString::number(gui_init.getPixByPt(8))));
    ui->groupBox->setStyleSheet(QString("QGroupBox{\
                                    border:%1px groove gray;\
                                    border-radius:%2px;\
                                    }")
                                .arg(QString::number(gui_init.getPixByPt(1))).arg(QString::number(gui_init.getPixByPt(8))));
    ui->groupBox_2->setStyleSheet(QString("QGroupBox{\
                                    border:%1px groove gray;\
                                    border-radius:%2px;\
                                    }")
                                .arg(QString::number(gui_init.getPixByPt(1))).arg(QString::number(gui_init.getPixByPt(8))));
    ui->progressBar->setStyleSheet(QString("QProgressBar{\
                                           border:%1px groove gray;\
                                           border-radius:%2px;\
                                           }\
                                           QProgressBar::chunk {\
                                           border:%1px;\
                                           border-color:rgb(103,192,240);\
                                           border-radius:%2px;\
                                           background:rgb(103,192,240);\
                                            }")
                                       .arg(QString::number(gui_init.getPixByPt(1))).arg(QString::number(gui_init.getPixByPt(8))));
    //历史数据栏目
    ui->pushButton_2->setStyleSheet(QString("QPushButton{\
                                  border:%1px groove gray;\
                                  border-radius:%2px;\
                                  color:white;\
                                  background-color:  rgb(40,160,224);\
                                  }\
                                 QPushButton:pressed{\
                                 border-style:inset;\
                                 }")
                                .arg(QString::number(gui_init.getPixByPt(1))).arg(QString::number(gui_init.getPixByPt(8))));

    //设置各插件高度
    ui->lineEdit->setMinimumHeight(gui_init.getPixByPt( ui->lineEdit->font().pointSize()) * 3);
    ui->pushButton->setMinimumHeight(gui_init.getPixByPt( ui->pushButton->font().pointSize()) * 3);
    ui->tableWidget_3->setMaximumHeight(gui_init.getPixByPt( ui->tableWidget_3->font().pointSize()) * 4);

    //历史数据栏
    ui->label_5->setMinimumHeight(gui_init.getPixByPt( ui->label_5->font().pointSize()) * 2);
    ui->label_6->setMinimumHeight(gui_init.getPixByPt( ui->label_6->font().pointSize()) * 2);
    ui->pushButton_2->setMinimumHeight(gui_init.getPixByPt( ui->pushButton_2->font().pointSize()) * 3);
    ui->progressBar->setMinimumHeight(gui_init.getPixByPt( ui->pushButton_2->font().pointSize()) * 3);
    //触摸滚轮
    gui_init.sc_lasted = QScroller::scroller(ui->tableWidget_3);
    QScroller::ScrollerGestureType gesture_lasted = QScroller::TouchGesture;
    gui_init.sc_lasted->grabGesture(ui->tableWidget_3,gesture_lasted);

    gui_init.sc_history = QScroller::scroller(ui->tableWidget);
    QScroller::ScrollerGestureType gesture_history = QScroller::TouchGesture;
    gui_init.sc_history->grabGesture(ui->tableWidget,gesture_history);


    //读本地保存的ID
    QString dev_id = operateLocalID(LOCAL_ID_READ,"");
    if(!dev_id.isNull())
    {
        ui->lineEdit->setText(dev_id);
    }

    //设置历史表格宽度
    ui->tableWidget->resizeColumnToContents(3);
    ui->tableWidget->resizeColumnToContents(4);


}
