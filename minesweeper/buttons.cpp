#include "buttons.h"


Buttons::Buttons(int X, int Y, int wi, int he, QString t)
{
    x = X;
    y = Y;
    width = wi;
    height = he;
    colorr = QColor("black");
    revcolor = QColor("white");
    text = t;
    hov = false;
}

void Buttons::setColor(QColor c) {
    colorr = c;
    setRevColor(QColor(255-c.red(), 255-c.green(), 255-c.blue()));
}

void Buttons::setRevColor(QColor c){
    revcolor = c;
}

QColor Buttons::getColor() {
    return colorr;
}

QColor Buttons::getRevColor() {
    return revcolor;
}

int Buttons::getX() {
    return x;
}

int Buttons::getY() {
    return y;
}

int Buttons::getWidth() {
    return width;
}

int Buttons::getHeight() {
    return height;
}

void Buttons::setString(QString t) {
    text = t;
}

QString Buttons::getString(){
    return text;
}

bool Buttons::getHov() {
    return hov;
}



bool Buttons::isHovering(QMouseEvent *e) {
    Q_UNUSED(e);
    int mouseX = e->x();
    int mouseY = e->y();
    hov = mouseX>x && mouseX<=x+width && mouseY>y && mouseY<y+height;
    return mouseX>x && mouseX<=x+width && mouseY>y && mouseY<y+height;
}

bool Buttons::isClicked(QMouseEvent *e) {
    Q_UNUSED(e);
    int buttons = e->button();
    return isHovering(e) && buttons==Qt::LeftButton;
}

void Buttons::drawButton(QPainter &painter, Buttons button) {
    QColor c = button.getColor();
    QColor cRev = button.getRevColor();
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    painter.setFont(font);

    if(button.getHov()) {
        c = button.getRevColor();
        cRev = button.getColor();

    }

    painter.setBrush(cRev);
    painter.setPen(cRev);
    painter.drawRect(button.getX()-2, button.getY()-2, button.getWidth()+4, button.getHeight()+4);
    int textWidth = fm.horizontalAdvance(button.getString());
    painter.setBrush(c);
    painter.drawRect(button.getX(), button.getY(), button.getWidth(), button.getHeight());
    painter.drawText(button.getX()+button.getWidth()/2-textWidth/2, button.getY()+button.getHeight()/2, button.getString());
}

void Buttons::setX(int X) {
    x = X;
}

