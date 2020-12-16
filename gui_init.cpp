#include <QDateTime>
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
    ui->tableWidget_3->horizontalHeader()->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    ui->label_3->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    ui->label_4->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    ui->label_7->setFont(QFont("Consolas", gui_init.getFontByPt(12)));

    //历史数据栏
    ui->label_5->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    ui->label_6->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    ui->dateEdit->setFont(QFont("Consolas", gui_init.getFontByPt(16)));
    ui->dateEdit_2->setFont(QFont("Consolas", gui_init.getFontByPt(16)));
    ui->pushButton_2->setFont(QFont("Consolas", gui_init.getFontByPt(16)));
    ui->tableWidget->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    ui->tableWidget->horizontalHeader()->setFont(QFont("Consolas", gui_init.getFontByPt(12)));
    //
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
                                           border-color:rgb(40,160,224);\
                                           border-radius:%2px;\
                                           background:rgb(40,160,224);\
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

    //设置表格宽度

    ui->tableWidget_3->horizontalHeader()->setDefaultSectionSize((int)(gui_init.screen->availableSize().width() / 6 * 1.2));
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize((int)(gui_init.screen->availableSize().width() / 6 * 1.2));


    //自动设置历史查询时间，默认为当前日期后一天的31天前
    ui->dateEdit_2->setDate(QDate::currentDate().addDays(1));
    ui->dateEdit->setDate(ui->dateEdit_2->date().addDays(-31));

    //设置label显示大logo
    QImage Image;
    Image.load(":/resource/pic/pic_icon.png");
    QPixmap pixmap = QPixmap::fromImage(Image);
    QPixmap fitpixmap = pixmap.scaled(gui_init.screen->availableSize().width(), gui_init.screen->availableSize().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    ui->label_8->setPixmap(fitpixmap);

    //淡入
    //先蒙版遮住
    gui_init.m_grah[0] = new QGraphicsOpacityEffect(ui->label_8);
    gui_init.m_grah[0]->setOpacity(0);
    ui->label_8->setGraphicsEffect(gui_init.m_grah[0]);

    gui_init.m_grah[1] = new QGraphicsOpacityEffect(ui->pushButton);
    gui_init.m_grah[1]->setOpacity(0);
    ui->pushButton->setGraphicsEffect(gui_init.m_grah[1]);

    gui_init.m_grah[2] = new QGraphicsOpacityEffect(ui->lineEdit);
    gui_init.m_grah[2]->setOpacity(0);
    ui->lineEdit->setGraphicsEffect(gui_init.m_grah[2]);

    gui_init.m_grah[3] = new QGraphicsOpacityEffect(ui->label_7);
    gui_init.m_grah[3]->setOpacity(0);
    ui->label_7->setGraphicsEffect(gui_init.m_grah[3]);

    //依次淡入 label-pushbotton-lineedit
    gui_init.m_proper[0] = new QPropertyAnimation(gui_init.m_grah[0],"opacity",this);
    gui_init.m_proper[0]->setDuration(800);
    gui_init.m_proper[0]->setStartValue(0);
    gui_init.m_proper[0]->setEndValue(1);

    connect(gui_init.m_proper[0],SIGNAL(finished()),this,SLOT(fadein_pushBotton()));
    gui_init.m_proper[0]->start();

}

void MainWindow::fadein_pushBotton()
{
    gui_init.m_proper[1] = new QPropertyAnimation(gui_init.m_grah[1],"opacity",this);
    gui_init.m_proper[1]->setDuration(300);
    gui_init.m_proper[1]->setStartValue(0);
    gui_init.m_proper[1]->setEndValue(1);

    connect(gui_init.m_proper[1],SIGNAL(finished()),this,SLOT(fadein_lineEdit()));
    gui_init.m_proper[1]->start();
}

void MainWindow::fadein_lineEdit()
{
    gui_init.m_proper[2] = new QPropertyAnimation(gui_init.m_grah[2],"opacity",this);
    gui_init.m_proper[2]->setDuration(300);
    gui_init.m_proper[2]->setStartValue(0);
    gui_init.m_proper[2]->setEndValue(1);

    connect(gui_init.m_proper[2],SIGNAL(finished()),this,SLOT(fadein_label()));
    gui_init.m_proper[2]->start();
}

void MainWindow::fadein_label()
{
    gui_init.m_proper[3] = new QPropertyAnimation(gui_init.m_grah[3],"opacity",this);
    gui_init.m_proper[3]->setDuration(100);
    gui_init.m_proper[3]->setStartValue(0);
    gui_init.m_proper[3]->setEndValue(1);

    connect(gui_init.m_proper[3],SIGNAL(finished()),this,SLOT(fadein_end()));
    gui_init.m_proper[3]->start();
}

void MainWindow::fadein_end()
{
    int i;

    for(i = 0;i < 4; i++)
    {
        gui_init.m_grah[i]->deleteLater();
        gui_init.m_proper[i]->deleteLater();
    }
}
