#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QtDebug>
#include <QScroller>
namespace Ui {
class MainWindow;
}

class FitScreen
{
public:
    int DPI;
    int height;
    int getPixByPt(int Pt);
    float getScaleByPt(int Pt);
    int getFontByPt(int Pt);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QScroller *sc_lasted = nullptr;
    QScroller *sc_history = nullptr;

    FitScreen fitScreen;
    QScreen *screen = nullptr;

    void Gui_init();
private slots:
    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
