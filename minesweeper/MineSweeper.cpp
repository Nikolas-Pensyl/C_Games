#include <MineSweeper.h>
#include <iostream>

using namespace std;


Field::Field(QWidget *parent) : QWidget(parent) {

}

QWidget* Field::getWidget() {
    return this;
}


void Field::loadImages() {
    mine.load("../Images/bomb.png");
    zero.load("../Images/0.png");
    one.load("../Images/1.png");
    two.load("../Images/2.png");
    three.load("../Images/3.png");
    four.load("../Images/4.png");
    five.load("../Images/5.png");
    six.load("../Images/6.png");
    seven.load("../Images/7.png");
    eight.load("../Images/8.png");
    flag.load("../Images/flagged.png");
    block.load("../Images/facingDown.png");

}

void Field::initGame() {
    mineField.clear();
    win = false;
    alive = true;
    bool ok = false;
    int i;
    setStyleSheet("background-color:gray;");
    while(!ok) {
       i = QInputDialog::getInt(this, tr("QInputDialog::getInteger()"),
                                    tr("Dimensions of mine field:"), 10, 5, 100, 1, &ok);
    }
       dim = i;
    setFixedSize(dim*B_WIDTH, dim*B_HEIGHT);

    amount_of_mines = dim*dim*.2;

    for(int i = 0; i<dim; i++) {
        vector<vector<int>> mineLine;
        for(int j =0; j<dim; j++) {
            std::vector<int> mine = {0, 0};
            mineLine.push_back(mine);
        }
        mineField.push_back(mineLine);
    }

    srand(time(NULL));
    for(int k = 0; k<amount_of_mines; k++) {
      int x, y;
      do {
       x = rand()%dim;
       y = rand()%dim;
        } while(mineField[y][x][0]==-1);
      cout<<x<<", "<<y<<endl;
       mineField[y][x][0] = -1;
       for(int i = 0; i<3; i++) {
            for(int j = 0; j<3; j++) {
                if(y-1+j>=0 && y-1+j<dim && x-1+i>=0 && x-1+i<dim && mineField[y-1+j][x-1+i][0] != -1){
                      mineField[y-1+j][x-1+i][0] +=1;
                }
            }
        }
    }
    cout<<endl<<endl;
    for(int i = 0; i<dim; i++) {
        for(int j= 0; j<dim; j++) {
            cout<<mineField[i][j][0]<<", ";
        }
        cout<<endl;
    }

}




void Field::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);


    QPainter painter(this);
    doDrawing(painter);
}

void Field::timerEvent(QTimerEvent *e) {

    Q_UNUSED(e);

    repaint();
}

void Field::mousePressEvent(QMouseEvent *e) {
    Q_UNUSED(e);
    int buttons = e->buttons();
    int mouseX = e->x();
    int mouseY = e->y();
    cout<<mouseX<<", "<<mouseY<<", "<<(buttons==Qt::LeftButton);
    if(buttons==Qt::LeftButton) {
        if(alive && !win) {
            cords corder;
            corder.setX(mouseX/75);
            corder.setY(mouseY/75);
            cord.push_back(corder);
            expose();
            mineField[mouseY/75][mouseX/75][1] = 1;
            if(mineField[mouseY/75][mouseX/75][0] == -1) {
                alive = false;
                for(int i = 0; i<dim; i++){
                    for(int j = 0; j<dim; j++) {
                        mineField[i][j][1] = 1;
                    }
                }
            }
        }
    }
    if(buttons==Qt::RightButton) {
        if(mineField[mouseY/75][mouseX/75][1] == 2) {
            mineField[mouseY/75][mouseX/75][1] = 0;
        }
        else {
            mineField[mouseY/75][mouseX/75][1] = 2;
        }
    }
}

int Field::doDrawing(QPainter &painter) {
        for(int i = 0; i<dim; i++) {
            for(int j= 0; j<dim; j++) {
                int num = 10;
                if(mineField[i][j][1]==1) {
                    num = mineField[i][j][0];

                    if(num==-1) {
                        num = 9;
                    }
                }

                if(mineField[i][j][1]==2) {
                    num = 11;
                }
                if(num == 9) {

                    painter.drawImage(j*75+13, i*75+13, mine);
                } else {
                    painter.drawImage(j*75, i*75, getImage(num));
                }

            }
        }
        return checkWin();
}

int Field::checkWin() {
    int count = 0;
    for(int i = 0; i<dim; i++) {
        for(int j = 0; j<dim; j++) {
            if(mineField[i][j][1] == 2 || mineField[i][j][1] == 0) {
               count ++;
            }
        }
    }
    if(count == amount_of_mines) {
        win = true;
    }
    return 2;
}

QString Field::getMessage() {
    QString message = "You lost :( try agian??";
    if(win){
        message = "Congrats! you won!!! play Agian??";
    }

    return message;
}
QImage Field::getImage(int num) {
    if(num == 0) {
        return zero;
    } else if (num ==1) {
        return one;
    } else if (num ==2) {
        return two;
    } else if (num ==3) {
        return three;
    } else if (num ==4) {
        return four;
    } else if (num ==5) {
        return five;
    } else if (num ==6) {
        return six;
    } else if (num ==7) {
        return seven;
    } else if (num ==8) {
        return eight;
    } else if (num ==9) {
        return mine;
    } else if (num ==10) {
        return block;
    }
        return flag;
}

void Field::expose(){
    int count = 0;
    while(count!=cord.size()) {
        int x = cord[count].getX();
        int y = cord[count].getY();
        if(mineField[y][x][0]==0) {
            for(int i = 0; i<3; i++) {
                if(i-1+x>=0 && i-1+x<mineField.size()) {
                    for(int j = 0; j<3; j++) {
                        if(y+j-1>=0 && y+j-1<mineField.size() && mineField[y+j-1][x+i-1][0] == 0 && mineField[y+j-1][x-1+i][1]!=1){
                            mineField[y+j-1][x+i-1][1] = 1;
                            cords corder;
                            corder.setX(x+i-1);
                            corder.setY(y+j-1);
                            cord.push_back(corder);
                        }
                        else if(y+j-1>=0 && y+j-1<mineField.size()){
                            mineField[y+j-1][x+i-1][1] = 1;
                        }
                    }
                }
            }
        }
        cord.erase(cord.begin());
    }

}

bool Field::getAlive() {
    return alive;
}

bool Field::getWin() {
    return win;
}

int Field::getWidth() {
    return dim*B_WIDTH;
}

int Field::getHeight() {
    return dim*B_HEIGHT;
}

//Cords Class
int cords::getX() {
    return x;
}

int cords::getY() {
    return y;
}

void cords::setX(int X){
    x = X;
}

void cords::setY(int Y){
    y = Y;
}

