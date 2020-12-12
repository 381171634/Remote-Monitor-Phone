#include "mainwindow.h"
#include "ui_mainwindow.h"

int FitScreen::getPixByPt(int Pt)
{
    return this->DPI*Pt/72;
}

float FitScreen::getScaleByPt(int Pt)
{
    return Pt/(2250.0*72/216);
}

int FitScreen::getFontByPt(int Pt)
{
    return (int)(this->height * 72.0 / this->DPI * this->getScaleByPt(Pt));
}

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
