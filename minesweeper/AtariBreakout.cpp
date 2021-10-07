#include "AtariBreakout.h"
#include <iostream>

Ball::Ball(int X, int Y, int r, int v) {

    ball_x = X;
    ball_y = Y;
    speed = v;
    do {
        dir = rand()%160+10;
        dir_rad = dir*M_PI/180;
        Xspeed = v*cos(dir_rad);
        Yspeed = v*sin(dir_rad);
    } while(Yspeed<2);
    rad = r;
}

void Ball::move() {
    ball_x -=Xspeed;
    ball_y -=Yspeed;
}

void Ball::wall() {
    if(ball_x+rad>1000) {
        setDir(getDir()+rand()%6-3);
        Xspeed *=-1;
        move();
    } else if(ball_x-rad<0) {
        setDir(getDir()+rand()%6-3);
        Xspeed *=-1;
        move();
    } else if(ball_y-rad<0) {
        setDir(getDir()+rand()%6-3);
        Yspeed *=-1;
        move();
    } else if(ball_y>600) {
        setDir(getDir()+rand()%6-3);
        Yspeed *=-1;
        move();
    }
}

int Ball::getX() {
    return ball_x;
}

int Ball::getY() {
    return ball_y;
}

int Ball::getDir() {
    return dir;
}

int Ball::getRad() {
    return rad;
}

void Ball::setVel(int V) {
    speed = V;
    Xspeed = speed*cos(dir);
    Yspeed = speed*sin(dir);
}

void Ball::setDir(int d) {
    int xer = Xspeed/abs(Xspeed);
    int yer = Yspeed/abs(Yspeed);
    dir = d%360;
    dir_rad = dir*M_PI/180;
    Xspeed = speed*cos(dir_rad);
    Yspeed = speed*sin(dir_rad);
    Xspeed *=xer;
    Yspeed *=yer;

}

Brick::Brick(int live, int x_coord, int y_coord, int w, int h) {
    lives = live;
    brick_x = x_coord;
    brick_y = y_coord;
    width = w;
    height = h;
}

void Brick::collide(Ball ball) {
    if(lives>0) {
        if(ball.getX()+ball.getRad() >=brick_x && ball.getX()-ball.getRad()<=brick_x) {
            if(ball.getY()>=brick_y && ball.getY()>=brick_y+height) {
                ball.setDir(ball.getDir()+90+(rand()%6)-3);
                lives--;
            }
        } else if(ball.getX()-ball.getRad() <=brick_x+width && ball.getX()+ball.getRad()>=brick_x+width) {
            if(ball.getY()>=brick_y && ball.getY()>=brick_y+height) {
                ball.setDir(ball.getDir()-(90+(rand()%6)-3));
                lives--;
            }
        } else if(ball.getY()+ball.getRad() >=brick_y && ball.getY()-ball.getRad()<=brick_y) {
            if(ball.getX()>=brick_x && ball.getX()>=brick_x+width) {
                ball.setDir(ball.getDir()-(180+(rand()%6)-3));
                lives--;
            }
        } else if(ball.getY()+ball.getRad() >=brick_y+height && ball.getY()-ball.getRad()<=brick_y+height) {
            if(ball.getX()>=brick_x && ball.getX()>=brick_x+width) {
                ball.setDir(ball.getDir()+180+(rand()%6)-3);
                lives--;
            }
        }
    }
}

int Brick::getWidth() {
    return width;
}

int Brick::getHeight() {
    return height;
}

int Brick::getLives() {
    return lives;
}

int Brick::getX() {
    return brick_x;
}

int Brick::getY() {
    return brick_y;
}



AtariBreakout::AtariBreakout(QWidget *parent) {
 srand(time(NULL));
 Ball ball(width/2, 75, 5, 10);
 balls.push_back(ball);
}

void AtariBreakout::initial() {
    setFixedSize(1000, 600);
}

int AtariBreakout::doDrawing(QPainter &p) {
    p.setBrush(QColor("white"));
    p.setPen(QColor("white"));
    for(int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            if(bricks[i][j].getLives()>0) {
                p.drawRect(bricks[i][j].getX(), bricks[i][j].getY(), bricks[i][j].getWidth(), bricks[i][j].getHeight());
            }
        }
    }
    for(baller = balls.begin(); baller!=balls.end(); baller++) {
        baller->move();
        baller->wall();
        std::cout<<baller->getDir()<<std::endl;
        p.drawEllipse(baller->getX(), baller->getY(), baller->getRad(), baller->getRad());
    }
    if(left && paddle_x>0) {
        paddle_x = paddle_x-p_speed;
    } else if(right && paddle_x+p_length<width) {
        paddle_x = paddle_x+p_speed;
    }
    p.drawRect(paddle_x, paddle_y, p_length, p_height);
    if(reseting!=0) {
        QString message = "Reseting in: ";
        QString messag;
        messag.setNum(reseting);
        message += messag;
        p.setPen(QColor("white"));
        QFont font("Courier", 15, QFont::DemiBold);
        QFontMetrics fm(font);
        int textWidth = fm.horizontalAdvance(message);
        p.setFont(font);
        p.drawText(width/2-textWidth/2, height/2, message);
        reseting--;
    }
    return 1;
}

void AtariBreakout::isCollided() {
    for(int i = 0; i<balls_count; i++) {
        for(int j = 0; j<=rows; j++) {
            for(int k=0; k<=cols; k++) {
                if(bricks[j][k].getLives()>0) {
                    bricks[j][k].collide(balls[i]);
                    if(bricks[j][k].getLives()==0) {
                        brickCount--;
                        if(brickCount==0) {
                            levelCleared();
                        }
                    }
                }
                if(balls[i].getX()-balls[i].getRad()<=0) {
                    balls[i].setDir(balls[i].getDir()-(87+rand()%6));
                } else if(balls[i].getX()+balls[i].getRad()>=width) {
                    balls[i].setDir(balls[i].getDir()+(87+rand()%6));
                } else if(balls[i].getY()-balls[i].getRad()<=0) {
                    balls[i].setDir(balls[i].getDir()-(177+rand()%6));
                } else if(balls[i].getY()+balls[i].getRad()>=height) {
                    balls[i].setDir(balls[i].getDir()+(177+rand()%6));
                }
            }
        }
    }
}

void AtariBreakout::levelCleared() {
    level++;
    bricks.clear();
    rows = 3 + (level-1)/2;
    cols = 10 + (level-1);
    brickWidth = (width/cols)-4;
    brickHeight = 50;
    if(rows*brickHeight>450) {
        brickHeight = (450/rows)-4;
    }
    for(int i = 0; i<rows; i++) {
        std::vector<Brick> bricker;
        for(int j=0; j<cols; j++) {
            Brick brick(1, (brickWidth+4)*j+2, (brickHeight+4)*i-2, brickWidth, brickHeight);
            bricker.push_back(brick);
        }
        bricks.push_back(bricker);
    }
    reseting = 3;
}

void AtariBreakout::keyPressEvent(QKeyEvent *e) {
    int key = e->key();
    switch(key) {
        case Qt::Key_Left: {
            right = false;
            left = true;
            break;
        }
        case Qt::Key_Right: {
            right = true;
            left = false;
            break;
        }
    }
}

void AtariBreakout::keyReleaseEvent(QKeyEvent *e) {
    int key = e->key();
    switch(key) {
        case Qt::Key_Left: {
            left = false;
            break;
        }
    case Qt::Key_Right: {
            right = false;
            break;
        }
    }
}

