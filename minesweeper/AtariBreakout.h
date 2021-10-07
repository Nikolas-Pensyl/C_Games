#ifndef ATARIBREAKOUT_H
#define ATARIBREAKOUT_H

#include <QWidget>
#include <vector>
#include <random>
#include <cmath>
#include <QPainter>
#include <windows.h>
#include <QTimer>
#include <QKeyEvent>

class Ball {

public:
    Ball(int X, int Y, int r, int v);
    void move();
    void setVel(int V);
    void setDir(int d);
    int getX();
    int getY();
    int getRad();
    int getDir();
    void wall();
    int Xspeed;
    int Yspeed;

private:
    bool isCollided(Ball b);

    int ball_x;
    int ball_y;
    int speed;
    int dir;
    int dir_rad;
    int rad;
};

class Brick {
public:
    Brick(int live, int x_coord, int y_coord, int w, int h);
    int getX();
    int getY();
    void setLives(int lives);
    int getLives();
    void collide(Ball ball);
    int getWidth();
    int getHeight();
private:
    int lives = 1;
    int height;
    int width;
    int brick_x;
    int brick_y;

};


class AtariBreakout : public QWidget {
    Q_OBJECT
public:
    AtariBreakout(QWidget *parent = nullptr);
    void initial();
    int doDrawing(QPainter &p);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

private:

   void isCollided();
   int level = 1;
   int brickCount = 0;
   int balls_count = 0;
   int rows = 0;
   int cols = 0;
   int brickWidth;
   int brickHeight;
   int width = 1000;
   int height = 600;
   void levelCleared();
   int reseting = 0;
   int paddle_x = 425;
   int paddle_y = 550;
   int p_speed = 10;
   int p_length = 150;
   int p_height = 20;
   bool left;
   bool right;

   std::vector<std::vector<Brick>> bricks;
   std::vector<Ball> balls;
   std::vector<Ball>::iterator baller;
};

#endif // ATARIBREAKOUT_H
