#ifndef GAMECHOOSER_H
#define GAMECHOOSER_H

#include <QWidget>
#include <MineSweeper.h>
#include <chess.h>
#include <AtariBreakout.h>
#include <buttons.h>
#include <iostream>
#include <QTime>
#include <QPainter>
#include <string>

class GameChooser : public QWidget
{
    Q_OBJECT
public:

    GameChooser(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *e);

private:
    int val = -1;
    int prev_val = -1;
    int timerId;
    static const int amount_butt = 4;
    Buttons buttons[amount_butt];
    void Mines();
    const int width = 1000;
    const int height = 1000;
    int chosenWidth = width;
    const int DELAY = 100;
    bool init = false;
    Field field;
    Chess chess;
    AtariBreakout atari;
    int studying = 0;
    void setUp();
    Buttons yes;
    Buttons no;
};

#endif // GAMECHOOSER_H
