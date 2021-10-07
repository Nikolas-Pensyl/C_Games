#include "gameover.h"

GameOver::GameOver(QWidget *parent) : QWidget(parent)
{

}

int GameOver::getMessage(QString message) {
    bool ok = false;
    QString text = "";
    message += "type Yes/No: ";
    while(!ok && checkText(text)==-1) {
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             message, QLineEdit::Normal,
                                             text, &ok);
    }
    return checkText(text);
}

int GameOver::checkText(QString text) {

    if(text.toLower()==QString::fromStdString("y") || text.toLower()=="yes") {
        return 1;
    } else if(text.toLower()=="n" || text.toLower()=="no") {
        return 0;
    }
    return -1;
}
