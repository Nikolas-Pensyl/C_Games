#include "gamechooser.h"
GameChooser::GameChooser(QWidget *parent) : QWidget(parent)
{
    setUp();
    for(int i = 0; i<amount_butt; i++) {
        Buttons button((i%3)*250 +100, (int)(i/3)*150 + 150, 200, 100);
        buttons[i] = button;
        if(i ==0) {
            buttons[i].setString("Quit");
        } else if(i==1){
            buttons[i].setString("Pong");
        } else if(i==2) {
            buttons[i].setString("MineSweeper");
        } else if(i==3) {
            buttons[i].setString("Chess");
        }
    }


    Buttons button(1000, 1000/2-25, 90, 50, "yes");
    yes = button;
    Buttons buttone(1100, 1000/2-25, 90, 50, "no");
    no = buttone;

    timerId = startTimer(DELAY);
}

void GameChooser::setUp() {
    setStyleSheet("background-color:black;");
    setFixedSize(width-100, height-100);
    setMouseTracking(true);
    studying = 0;
}

void GameChooser::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter p(this);
    if(val == -1) {
        //Chooser
        init = false;
        chosenWidth = width;
        for(int i = 0; i<amount_butt; i++) {
            buttons[i].drawButton(p, buttons[i]);
        }

    } else if(val == 0) {
        this->close();
    } else if(val==-2) {

        QString message;
        QFont font("Courier", 15, QFont::DemiBold);
        QFontMetrics fm(font);
        p.setFont(font);
        if(prev_val==1) {

        } else if(prev_val==2) {
            field.doDrawing(p);
            message = field.getMessage();
            setFixedSize(field.getWidth()+200, field.getHeight());
            yes.setX(field.getWidth()+5);
            no.setX(field.getWidth()+105);
        } else if(prev_val==3) {
            chess.doDrawing(p);
            message = chess.getMessage();
            setFixedSize(chess.getWidth()+200, chess.getHeight());
            yes.setX(chess.getWidth()-195);
            no.setX(chess.getWidth()-90);
        }
        p.setPen(QColor("white"));
        int textWidth = fm.horizontalAdvance(message);
        p.drawText(chosenWidth-75-textWidth/2, 450, message);
        yes.drawButton(p, yes);
        no.drawButton(p, no);
    } else if(val==1) {
        //Atari Breakout
        if(!init) {
            init = true;
            chosenWidth = 1000;
            setFixedSize(chosenWidth, 600);
        }
        val = atari.doDrawing(p);
    } else if(val == 2) {
        //MineSweeper
        if(! init) {
            field.initGame();
            field.loadImages();
            init = true;
            chosenWidth = field.getWidth();
            setFixedSize(chosenWidth, field.getHeight());
            setStyleSheet("background-color:gray;");
        }
       if(studying ==  0 || (field.getAlive() && !field.getWin())) {
           val =  field.doDrawing(p);
           if(!field.getAlive() || field.getWin()) {
               studying++;
           }
       } else {
           val = -2;
       }

    }
    else if(val ==3) {
        if(!init) {
            chosenWidth = chess.getWidth();
            setFixedSize(chosenWidth, chess.getHeight());
            chess.reset();
            chess.loadImages();

            init = true;
        }

        val = chess.doDrawing(p);
    }
}

void GameChooser::timerEvent(QTimerEvent *e){
    Q_UNUSED(e);

    repaint();
}

void GameChooser::mouseMoveEvent(QMouseEvent *e) {
    if(val == -1) {
        for(int i = 0; i<amount_butt; i++) {
            if(buttons[i].isClicked(e)) {
                val = i;
            }
            buttons[i].isHovering(e);
        }
    }
    if(val == 3) {
        chess.mouseMove(e);
    } else if(val==-2) {
        yes.isHovering(e);
        no.isHovering(e);
    }

}

void GameChooser::mouseReleaseEvent(QMouseEvent *e) {
    if(val==3) {
        chess.mouseReleaseEvent(e);
    }
}

void GameChooser::keyPressEvent(QKeyEvent *event) {
    if(val==1) {
        atari.keyPressEvent(event);
    }
}

void GameChooser::keyReleaseEvent(QKeyEvent *e) {
    if(val==1) {
        atari.keyReleaseEvent(e);
    }
}

void GameChooser::mousePressEvent(QMouseEvent *e) {
    if(val == -1) {
        for(int i = 0; i<amount_butt; i++) {
            if(buttons[i].isClicked(e)) {
                val = i;
                prev_val = i;
            }
        }
    } else if (val == 2) {
        field.mousePressEvent(e);
    } else if (val == 3) {
        chess.mousePressEvent(e);
    } else if(val == -2) {
        if(yes.isClicked(e)) {
            val = prev_val;
        } else if(no.isClicked(e)) {
            setUp();
            val = -1;
        }
        init = false;
    }

}



