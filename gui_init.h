#ifndef GUI_INIT_H
#define GUI_INIT_H

#include <QScroller>
#include <QScreen>

class GUI_INIT
{
public:
    int DPI;
    int height;
    QScroller *sc_lasted = nullptr;
    QScroller *sc_history = nullptr;
    QScreen *screen = nullptr;

    int getPixByPt(int Pt);
    float getScaleByPt(int Pt);
    int getFontByPt(int Pt);

};

#endif // GUI_INIT_H
