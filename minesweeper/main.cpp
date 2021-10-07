#include <QApplication>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <MineSweeper.h>
#include <gamechooser.h>

//This program plays minesweeper in the terminal

using namespace std;

void game() {

}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    std::cout<<"test";
    GameChooser game;
   game.show();

    //Field field;

   // field.show();



    return app.exec();
}



