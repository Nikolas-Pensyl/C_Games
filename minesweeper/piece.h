#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QWidget>
class Piece
{
public:
   Piece(QImage q);
   QImage getImager();

private:
   QImage p;

};

#endif // PIECE_H
