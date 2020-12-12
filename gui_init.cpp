#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::Gui_init()
{
    //屏幕适配
    screen = QGuiApplication::primaryScreen();
    fitScreen.DPI = screen->logicalDotsPerInch();
    fitScreen.height = screen->availableSize().height();
    //设置各插件字体 适应不同屏幕
    ui->lineEdit->setFont(QFont("Consolas", fitScreen.getFontByPt(16)));
    ui->pushButton->setFont(QFont("Consolas", fitScreen.getFontByPt(16)));
    ui->label_2->setFont(QFont("Consolas", fitScreen.getFontByPt(2)));
    ui->tableWidget_3->setFont(QFont("Consolas", fitScreen.getFontByPt(12)));
    ui->label_3->setFont(QFont("Consolas", fitScreen.getFontByPt(12)));
    ui->label_4->setFont(QFont("Consolas", fitScreen.getFontByPt(12)));
    ui->label_7->setFont(QFont("Consolas", fitScreen.getFontByPt(12)));

    //历史数据栏
    ui->label_5->setFont(QFont("Consolas", fitScreen.getFontByPt(12)));
    ui->label_6->setFont(QFont("Consolas", fitScreen.getFontByPt(12)));
    ui->pushButton_2->setFont(QFont("Consolas", fitScreen.getFontByPt(16)));
    //ui->tableWidget_3->horizontalHeader()->setFont(QFont("Consolas", fitScreen.getFontByPt(12)));
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
                                .arg(QString::number(fitScreen.getPixByPt(1))).arg(QString::number(fitScreen.getPixByPt(8))));
    ui->lineEdit->setStyleSheet(QString("QLineEdit{\
                                    border:%1px groove gray;\
                                    border-radius:%2px;\
                                    }")
                                .arg(QString::number(fitScreen.getPixByPt(1))).arg(QString::number(fitScreen.getPixByPt(8))));
    ui->groupBox->setStyleSheet(QString("QGroupBox{\
                                    border:%1px groove gray;\
                                    border-radius:%2px;\
                                    }")
                                .arg(QString::number(fitScreen.getPixByPt(1))).arg(QString::number(fitScreen.getPixByPt(8))));
    ui->groupBox_2->setStyleSheet(QString("QGroupBox{\
                                    border:%1px groove gray;\
                                    border-radius:%2px;\
                                    }")
                                .arg(QString::number(fitScreen.getPixByPt(1))).arg(QString::number(fitScreen.getPixByPt(8))));
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
                                .arg(QString::number(fitScreen.getPixByPt(1))).arg(QString::number(fitScreen.getPixByPt(8))));

    //设置各插件高度
    ui->lineEdit->setMinimumHeight(fitScreen.getPixByPt( ui->lineEdit->font().pointSize()) * 3);
    ui->pushButton->setMinimumHeight(fitScreen.getPixByPt( ui->pushButton->font().pointSize()) * 3);
    ui->tableWidget_3->setMaximumHeight(fitScreen.getPixByPt( ui->tableWidget_3->font().pointSize()) * 4);

    //历史数据栏
    ui->label_5->setMinimumHeight(fitScreen.getPixByPt( ui->label_5->font().pointSize()) * 2);
    ui->label_6->setMinimumHeight(fitScreen.getPixByPt( ui->label_6->font().pointSize()) * 2);
    ui->pushButton_2->setMinimumHeight(fitScreen.getPixByPt( ui->pushButton_2->font().pointSize()) * 3);
    //触摸滚轮
    sc_lasted = QScroller::scroller(ui->tableWidget_3);
    QScroller::ScrollerGestureType gesture_lasted = QScroller::TouchGesture;
    sc_lasted->grabGesture(ui->tableWidget_3,gesture_lasted);

    sc_history = QScroller::scroller(ui->tableWidget);
    QScroller::ScrollerGestureType gesture_history = QScroller::TouchGesture;
    sc_history->grabGesture(ui->tableWidget,gesture_history);
}
