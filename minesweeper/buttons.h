#ifndef BUTTONS_H
#define BUTTONS_H

#include <QWidget>
#include <QMouseEvent>
#include <QTime>
#include <QPainter>
#include <string>

class Buttons
{

public:
     Buttons(int X=0, int Y=0, int wi=0, int he=0, QString = "");
     bool isClicked(QMouseEvent *e);
     bool isHovering(QMouseEvent *e);
     QColor getColor();
     QColor getRevColor();
     void setColor(QColor c);
     void setRevColor(QColor c);
     void updateTime();
     void setString(QString t);
     void drawButton(QPainter &painter, Buttons button);
     QString getString();
    int getX();
    int getY();
    int getHeight();
    int getWidth();
    bool getHov();
    void setHov();
    void setX(int X);
private:
     int x;
     int y;
     int width;
     int height;
     QString text;
     QColor colorr;
     QColor revcolor;
     bool hov;


signals:

};

#endif // BUTTONS_H
