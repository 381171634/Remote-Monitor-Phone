#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QtDebug>
#include <QScroller>
#include <QMessageBox>

#include "gui_init.h"
#include "proc.h"
namespace Ui {
class MainWindow;
}

#define TVOC_THR        (0.06f)

typedef enum{
    LOCAL_ID_READ =0 ,
    LOCAL_ID_SAVE
}opLocalIDTypedef;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    GUI_INIT gui_init;                  //界面初始化类
    PROC     proc;                      //协议通道类

    uint16_t table_total_row = 0;       //历史记录总行数
    uint16_t table_cur_row = 0;         //当前行数

    void Gui_init();
    void changeOnlineState(int type);
    QString operateLocalID(int type,QString ID);
private slots:
    void on_lineEdit_returnPressed();

    void on_pushButton_clicked();

    void update_LastSample();

    void update_record();

    void phone_disconn();

    void fadein_lineEdit();
    void fadein_pushBotton();
    void fadein_label();
    void fadein_end();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
