#ifndef CHESS_H
#define CHESS_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <iostream>
#include <QMouseEvent>
#include <vector>
#include <QMessageBox>
#include "buttons.h"

class Chess : public QWidget
{
    Q_OBJECT
public:
    Chess(QWidget *parent = nullptr);
    void loadImages();
    int doDrawing(QPainter &painter);
    QString getMessage();
    void mousePressEvent(QMouseEvent *e);
    void mouseMove(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    int getWidth();
    int getHeight();
    void reset();

private:
    std::vector<std::vector<int>> CBoard; //0 if empty, number represents  a piece
    std::vector<std::vector<std::vector<int>>> prev_moves; // Holds the board of each previous position
    std::vector<std::vector<int>> movers; //[[current x, current y, new x, new y], ...]
    std::vector<std::vector<int>> movesmade; //[[current x, current y, new x, new y, piece value, take vale =0], ...]

    QImage BKnight; // -3
    QImage WKnight; // 3
    QImage BKing; // -5
    QImage WKing; // 5
    QImage BQueen; // -6
    QImage WQueen; // 6
    QImage BRook; // -2
    QImage WRook; // 2
    QImage BBishop; // -4
    QImage WBishop; // 4
    QImage BPawn; // -1
    QImage WPawn; // 1
    QImage board;


    bool black_turn = false;
    bool check = false;
    QImage getImage(int num);
    void possibleMoves();
    bool isLegal();
    bool checkMate();
    bool inCheck(std::vector<int> move);
    bool inCheck();
    void addMove();
    void drawButton(QPainter &painter, Buttons button);
    std::vector<int> getKing(std::vector<std::vector<int>> Board);
    std::vector<std::vector<int>> getMoves(int x, int y, bool black, std::vector<std::vector<int>> Board);

    bool hold = false;
    int mouseX = 0;
    int mouseY = 0;
    int sx_coord = -1;
    int sy_coord= -1;
    int ex_coord= -1;
    int ey_coord= -1;
    int px_coord =-1;
    int py_coord = -1;
    int game_end = 0; //1=white win //2=black win //3=tie
    int prev_index = 0;
    int height = 1000;
    int width = 1200;
    Buttons button_back;
    Buttons button_forward;


};



#endif // CHESS_H
