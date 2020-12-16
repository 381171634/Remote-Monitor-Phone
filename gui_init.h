#ifndef GUI_INIT_H
#define GUI_INIT_H

#include <QScroller>
#include <QScreen>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class GUI_INIT
{
public:
    int DPI;
    int height;
    QScroller *sc_lasted = nullptr;
    QScroller *sc_history = nullptr;
    QScreen *screen = nullptr;

    QGraphicsOpacityEffect *m_grah[4];
    QPropertyAnimation     *m_proper[4];

    int getPixByPt(int Pt);
    float getScaleByPt(int Pt);
    int getFontByPt(int Pt);

};

#endif // GUI_INIT_H
