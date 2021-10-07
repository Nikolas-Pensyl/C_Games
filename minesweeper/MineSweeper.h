#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <QMouseEvent>
#include <QWidget>
#include <QtGui>
#include <QTime>
#include <QPainter>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>
#include <QInputDialog>
#include <QMessageBox>

class mine
{
public:
    mine(int nval = 9);
    bool getChecked();
    int getVal();
    void setVal(int newVal);
    void setChecked();
private:
    int val;
    bool checked = false;
};


class cords {
public:
    int getX();
    int getY();
    void setX(int X);
    void setY(int Y);

private:
    int x = -1;
    int y = -1 ;
};

class Field : public QWidget {
    Q_OBJECT
    public:
        Field(QWidget *parent = nullptr);

        QWidget* getWidget();
        int doDrawing(QPainter &painter);
        void initGame();
        void loadImages();
        void mousePressEvent(QMouseEvent *);
        int getWidth();
        int getHeight();
        QString getMessage();

        bool getWin();
        bool getAlive();


    protected:
        void paintEvent(QPaintEvent *);
        void timerEvent(QTimerEvent *);


    private:
        QImage zero;
        QImage one;
        QImage two;
        QImage three;
        QImage four;
        QImage five;
        QImage six;
        QImage seven;
        QImage eight;
        QImage mine;
        QImage flag;
        QImage block;

        bool alive = true;
        bool win = false;

        static const int B_WIDTH = 75;
        static const int B_HEIGHT = 75;
        static const int DELAY = 1;

        int dim;
        int amount_of_mines;
        int timerId;
        int studying = 0;

        std::vector<std::vector<std::vector<int>>> mineField;
        std::vector<cords> cord;




        void expose();
        int checkWin();

        QImage getImage(int num);

};



#endif // MINESWEEPER_H
