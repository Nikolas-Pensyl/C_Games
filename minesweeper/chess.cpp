#include "chess.h"
#include "buttons.h"

Chess::Chess(QWidget *parent) : QWidget(parent)
{
    //reset();
}

void Chess::reset() {
    CBoard.clear();
    black_turn = false;
    hold = false;
    std::vector<int> vec;
    for(int i = 0; i<8; i++) {
        vec.push_back(0);
    }
    for(int i=0; i<8; i++) {
        CBoard.push_back(vec);
    }
    prev_moves.clear();
    for(int i=0; i<8; i++) {
        for(int j = 2; j<=5; j++) {
            CBoard[i][j] = 0;
        }
        //pawns
        CBoard[i][6] = 1;
        CBoard[i][1] = -1;
    }


    //white is positive
    CBoard[0][7] = 2; //Rook
    CBoard[7][7] = 2;
    CBoard[2][7] = 4; //Bishop
    CBoard[5][7] = 4;
    CBoard[1][7] = 3; //Knight
    CBoard[6][7] = 3;
    CBoard[4][7] = 5; //King
    CBoard[3][7] = 6; //Queen


    //black is negative
    CBoard[0][0] = -2; //Rook
    CBoard[7][0] = -2;
    CBoard[2][0] = -4; //Bishop
    CBoard[5][0] = -4;
    CBoard[1][0] = -3; //Knight
    CBoard[6][0] = -3;
    CBoard[4][0] = -5; //King
    CBoard[3][0] = -6; //Queen
    prev_moves.push_back(CBoard);
    prev_index = 0;
    possibleMoves();
    game_end = 0;
}

void Chess::loadImages() {



     BKnight.load("../Chess Pieces/Black Knight");
     WKnight.load("../Chess Pieces/White Knight");;
     BKing.load("../Chess Pieces/Black King");;
     WKing.load("../Chess Pieces/White King");
     BQueen.load("../Chess Pieces/Black Queen");
     WQueen.load("../Chess Pieces/White Queen");
     BRook.load("../Chess Pieces/Black Rook");
     WRook.load("../Chess Pieces/White Rook");
     BBishop.load("../Chess Pieces/Black Bishop");
     WBishop.load("../Chess Pieces/White Bishop");
     BPawn.load("../Chess Pieces/Black Pawn");
     WPawn.load("../Chess Pieces/White Pawn");
     board.load("../Chess Pieces/board");


     button_back = Buttons(board.width()+10, board.height()-70, 80, 60, "<");
     button_forward = Buttons(board.width()+110, board.height()-70, 80, 60, ">");




}

void Chess::possibleMoves() {

    check = inCheck();
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++){
            std::vector<std::vector<int>> mover;
            if(CBoard[i][j]==-8 && black_turn) {
                CBoard[i][j] =-1;
            } else if(CBoard[i][j]==8 && !black_turn) {
                CBoard[i][j]=1;
            }
            mover = getMoves(i, j, black_turn, CBoard);
            for(int m = 0; m<mover.size(); m++) {
                if(!inCheck(mover[m])) {
                  movers.push_back(mover[m]);
                }
            }
        }
    }
}

int Chess::doDrawing(QPainter &painter) {
    painter.drawImage(0, 0, board);
    if(px_coord==-1) {
        for(int i=0; i<8; i++) {
            for(int j=0; j<8; j++) {
                if(hold && sx_coord==i && sy_coord==j) {
                    painter.drawImage(mouseX-62 , mouseY-62, getImage(CBoard[i][j]));


                }
                else if(CBoard[i][j]!=0) {
                    painter.drawImage(i*125 +12 , j*125 +12, getImage(CBoard[i][j]));
                }

            }
        }
    } else {
        int neg = 1;
        if(black_turn) {
            neg = -1;
        }
        painter.drawImage(12, py_coord*125+12, getImage(6*neg)); //Queen
        painter.drawImage(262, py_coord*125+12, getImage(4*neg)); //Bishop
        painter.drawImage(512, py_coord*125+12, getImage(3*neg)); //Knight
        painter.drawImage(762, py_coord*125+12, getImage(11*neg)); //Rook
    }


    for(std::vector<std::vector<int>>::iterator pizza = movers.begin(); pizza!=movers.end(); pizza++) {
        std::vector<int> move = *pizza;
        if(move[0]==sx_coord && move[1]==sy_coord) {
            painter.setOpacity(50);
            painter.setBrush(QBrush(Qt::darkGreen));
            painter.drawEllipse(move[2]*125+45, move[3]*125+45, 30, 30);
        }
    }

    button_back.drawButton(painter, button_back);
    button_forward.drawButton(painter, button_forward);

    if(checkMate()) {
        return -2;
    }
    return 3;
}

std::vector<std::vector<int>> Chess::getMoves(int x, int y, bool black,std::vector<std::vector<int>> Board) {
    int num = Board[x][y]; //piece identifier
    std::vector<std::vector<int>> moves;
    std::vector<int> move;
    if(black) {
        //Pawn
        if(num == -1 || num ==-8) {
            if(Board[x][y+1]==0) {
                move = {x, y, x, y+1};
                moves.push_back(move);
                if(y==1 && Board[x][y+2]==0) {
                    move = {x, y, x, 3};
                    moves.push_back(move);
                }
            }
            if(x-1>=0 && (Board[x-1][y+1]>0 || (Board[x-1][y]==8 && Board[x-1][y+1]>=0))) {
                move = {x, y, x-1, y+1};
                moves.push_back(move);
            }
            if(x+1<=7 && (Board[x+1][y+1]>0 || (Board[x+1][y]==8 && Board[x+1][y+1]>=0))) {
                move = {x, y, x+1, y+1};
                moves.push_back(move);
            }
        }


        //Rook
        if(num==-2 || num==-11) {
            int count = 1;
            bool right = true;
            bool left = true;
            bool up = true;
            bool down = true;
            while(up || down || left || right) {
                //right
                if(right && count+x!=8 && Board[x+count][y]>=0) {
                    move = {x, y, x+count, y};
                    moves.push_back(move);
                    if(Board[x+count][y]!=0) {
                        right = false;
                    }
                } else if (right && (count+x==8 || Board[x+count][y]<0)) {
                    right = false;
                }
                //left
                if(left && x-count!=-1 && Board[x-count][y]>=0) {
                    move = {x, y, x-count, y};
                    moves.push_back(move);
                    if(Board[x-count][y]!=0) {
                        left = false;
                    }
                } else if (left && (x-count==-1 || Board[x-count][y]<0)) {
                    left = false;
                }
                //up
                if(up && y-count!=-1 && Board[x][y-count]>=0) {
                    move = {x, y, x, y-count};
                    moves.push_back(move);
                    if(Board[x][y-count]!=0) {
                        up = false;
                    }
                } else if (up && (y-count==-1 || Board[x][y-count]<0)) {
                    up = false;
                }
                //down
                if(down && count+y!=8 && Board[x][y+count]>=0) {
                    move = {x, y, x, y+count};
                    moves.push_back(move);
                    if(Board[x][y+count]!=0) {
                        down = false;
                    }
                } else if (down && (count+y==8 || Board[x][y+count]<0)) {
                    down = false;
                }
                count++;
            }
        }

        //Bishop
        if(num ==-4) {
            int count = 1;
            bool upright = true;
            bool upleft = true;
            bool downright = true;
            bool downleft = true;
            while(upright || upleft || downright || downleft) {
                //upright
                if(upright && count+x!=8 && y-count!=-1 && Board[x+count][y-count]>=0) {
                    move = {x, y, x+count, y-count};
                    moves.push_back(move);
                    if(Board[x+count][y-count]!=0) {
                        upright = false;
                    }
                } else if (upright && (count+x==8 || y-count==-1 || Board[x+count][y-count]<0)) {
                    upright = false;
                }
                //upleft
                if(upleft && x-count!=-1 && y-count!=-1 && Board[x-count][y-count]>=0) {
                    move = {x, y, x-count, y-count};
                    moves.push_back(move);
                    if(Board[x-count][y-count]!=0) {
                        upleft = false;
                    }
                } else if (upleft && (x-count==-1 || y-count==-1 || Board[x-count][y-count]<0)) {
                    upleft = false;
                }
                //downright
                if(downright && count+y!=8 && count+x!=8 && Board[x+count][y+count]>=0) {
                    move = {x, y, x+count, y+count};
                    moves.push_back(move);
                    if(Board[x+count][y+count]!=0) {
                        downright = false;
                    }
                } else if (downright && (count+y==8 || x+count==8 || Board[x+count][y+count]<0)) {
                    downright = false;
                }
                //downleft
                if(downleft && count+y!=8 && x-count!=-1 && Board[x-count][y+count]>=0) {
                    move = {x, y, x-count, y+count};
                    moves.push_back(move);
                    if(Board[x-count][y+count]!=0) {
                        downleft = false;
                    }
                } else if (downleft && (count+y==8 || x-count==-1 || Board[x-count][y+count]<0)) {
                    downleft = false;
                }
                count++;
            }
        }

        //Queen
        if(num ==-6) {
            int count = 1;
            bool upright = true;
            bool upleft = true;
            bool downright = true;
            bool downleft = true;
            bool left = true;
            bool right = true;
            bool up = true;
            bool down = true;
            while(upright || upleft || downright || downleft || up || down ||left || right) {
                //upright
                if(upright && count+x!=8 && y-count!=-1 && Board[x+count][y-count]>=0) {
                    move = {x, y, x+count, y-count};
                    moves.push_back(move);
                    if(Board[x+count][y-count]!=0) {
                        upright = false;
                    }
                } else if (upright && (count+x==8 || y-count==-1 || Board[x+count][y-count]<0)) {
                    upright = false;
                }
                //upleft
                if(upleft && x-count!=-1 && y-count!=-1 && Board[x-count][y-count]>=0) {
                    move = {x, y, x-count, y-count};
                    moves.push_back(move);
                    if(Board[x-count][y-count]!=0) {
                        upleft = false;
                    }
                } else if (upleft && (x-count==-1 || y-count==-1 || Board[x-count][y-count]<0)) {
                    upleft = false;
                }
                //downright
                if(downright && count+y!=8 && count+x!=8 && Board[x+count][y+count]>=0) {
                    move = {x, y, x+count, y+count};
                    moves.push_back(move);
                    if(Board[x+count][y+count]!=0) {
                        downright = false;
                    }
                } else if (downright && (count+y==8 || x+count==8 || Board[x+count][y+count]<0)) {
                    downright = false;
                }
                //downleft
                if(downleft && count+y!=8 && x-count!=-1 && Board[x-count][y+count]>=0) {
                    move = {x, y, x-count, y+count};
                    moves.push_back(move);
                    if(Board[x-count][y+count]!=0) {
                        downleft = false;
                    }
                } else if (downleft && (count+y==8 || x-count==-1 || Board[x-count][y+count]<0)) {
                    downleft = false;
                }
                //right
                if(right && count+x!=8 && Board[x+count][y]>=0) {
                    move = {x, y, x+count, y};
                    moves.push_back(move);
                    if(Board[x+count][y]!=0) {
                        right = false;
                    }
                } else if (right && (count+x==8 || Board[x+count][y]<0)) {
                    right = false;
                }
                //left
                if(left && x-count!=-1 && Board[x-count][y]>=0) {
                    move = {x, y, x-count, y};
                    moves.push_back(move);
                    if(Board[x-count][y]!=0) {
                        left = false;
                    }
                } else if (left && (x-count==-1 || Board[x-count][y]<0)) {
                    left = false;
                }
                //up
                if(up && y-count!=-1 && Board[x][y-count]>=0) {
                    move = {x, y, x, y-count};
                    moves.push_back(move);
                    if(Board[x][y-count]!=0) {
                        up = false;
                    }
                } else if (up && (y-count==-1 || Board[x][y-count]<0)) {
                    up = false;
                }
                //down
                if(down && count+y!=8 && Board[x][y+count]>=0) {
                    move = {x, y, x, y+count};
                    moves.push_back(move);
                    if(Board[x][y+count]!=0) {
                        down = false;
                    }
                } else if (down && (count+y==8 || Board[x][y+count]<0)) {
                    down = false;
                }
                count++;
            }
        }

        if(num==-5 || num==-10) {
            //king
            if(x-1!=-1) {
                if (Board[x-1][y]>=0) {
                    move = {x, y, x-1, y};
                    moves.push_back(move);
                }
                if(y+1!= 8 && Board[x-1][y+1]>=0) {
                    move = {x, y, x-1, y+1};
                    moves.push_back(move);
                }
                if(y-1!=-1 && Board[x-1][y-1]>=0) {
                    move = {x, y, x-1, y-1};
                    moves.push_back(move);
                }
            }
            if(x+1!=8) {
                if (Board[x+1][y]>=0) {
                    move = {x, y, x+1, y};
                    moves.push_back(move);
                }
                if(y+1!= 8 && Board[x+1][y+1]>=0) {
                    move = {x, y, x+1, y+1};
                    moves.push_back(move);
                }
                if(y-1!=-1 && Board[x+1][y-1]>=0) {
                    move = {x, y, x+1, y-1};
                    moves.push_back(move);
                }
            }
            if(y+1!=8 && Board[x][y+1]>=0) {
                move = {x, y, x, y+1};
                moves.push_back(move);
            }
            if(y-1!=-1 && Board[x][y-1]>=0) {
                move = {x, y, x, y-1};
                moves.push_back(move);
            }
            if(num!=-10 && !check) {
                bool left = true;
                bool right = true;
                if(Board[0][y]!=-2) {
                    left = false;
                }
                if(Board[7][y]!=-2) {
                    right = false;
                }
                for(int i= 1; (x+i<7 || x-i>0); i++) {
                    if(left && x-i>0 && Board[x-i][y]!=0) {
                        left = false;
                    }
                    if(right && x+i<7 && Board[x+i][y]!=0) {
                        right = false;
                    }
                }
                for(int i= 2; (x+i<7 || x-i>-1); i++) {
                    if(left && x-i>-1) {
                        move = {x, y, x-i, y};
                        moves.push_back(move);
                    }
                    if(right && x+i<8) {
                        move = {x, y, x+i, y};
                        moves.push_back(move);
                    }
                }
            }
        }

        if(num==-3) {
            //Knight
            if(y+2<8 && x+1<8 && Board[x+1][y+2]>=0) {
                move = {x, y, x+1, y+2};
                moves.push_back(move);
            }
            if(y+1<8 && x+2<8 && Board[x+2][y+1]>=0) {
                move = {x, y, x+2, y+1};
                moves.push_back(move);
            }
            if(y+2<8 && x-1>-1 && Board[x-1][y+2]>=0) {
                move = {x, y, x-1, y+2};
                moves.push_back(move);
            }
            if(y+1<8 && x-2>-1 && Board[x-2][y+1]>=0) {
                move = {x, y, x-2, y+1};
                moves.push_back(move);
            }
            if(y-2>-1 && x+1<8 && Board[x+1][y-2]>=0) {
                move = {x, y, x+1, y-2};
                moves.push_back(move);
            }
            if(y-1>-1 && x+2<8 && Board[x+2][y-1]>=0) {
                move = {x, y, x+2, y-1};
                moves.push_back(move);
            }
            if(y-2>-1 && x-1>-1 && Board[x-1][y-2]>=0) {
                move = {x, y, x-1, y-2};
                moves.push_back(move);
            }
            if(y-1>-1 && x-2>-1 && Board[x-2][y-1]>=0) {
                move = {x, y, x-2, y-1};
                moves.push_back(move);
            }
        }
    } else {

        //Pawn
        if(num == 1 || num ==8) {
            if(Board[x][y-1]==0) {
                move = {x, y, x, y-1};
                moves.push_back(move);
                if(y==6 && Board[x][y-2]==0) {
                    move = {x, y, x, 4};
                    moves.push_back(move);
                }
            }
            if(x-1>=0 && (Board[x-1][y-1]<0 || (Board[x-1][y]==-8 && Board[x-1][y-1]==0))) {
                move = {x, y, x-1, y-1};
                moves.push_back(move);
            }
            if(x+1<=7 && (Board[x+1][y-1]<0 || (Board[x+1][y]==-8 && Board[x+1][y-1]==0))) {
                move = {x, y, x+1, y-1};
                moves.push_back(move);
            }
        }


        //Rook
        if(num==2 || num== 11) {
            int count = 1;
            bool right = true;
            bool left = true;
            bool up = true;
            bool down = true;
            while(up || down || left || right) {
                //right
                if(right && count+x!=8 && Board[x+count][y]<=0) {
                    move = {x, y, x+count, y};
                    moves.push_back(move);
                    if(Board[x+count][y]!=0) {
                        right = false;
                    }
                } else if (right && (count+x==8 || Board[x+count][y]>0)) {
                    right = false;
                }
                //left
                if(left && x-count!=-1 && Board[x-count][y]<=0) {
                    move = {x, y, x-count, y};
                    moves.push_back(move);
                    if(Board[x-count][y]!=0) {
                        left = false;
                    }
                } else if (left && (x-count==-1 || Board[x-count][y]>0)) {
                    left = false;
                }
                //up
                if(up && y-count!=-1 && Board[x][y-count]<=0) {
                    move = {x, y, x, y-count};
                    moves.push_back(move);
                    if(Board[x][y-count]!=0) {
                        up = false;
                    }
                } else if (up && (y-count==-1 || Board[x][y-count]>0)) {
                    up = false;
                }
                //down
                if(down && count+y!=8 && Board[x][y+count]<=0) {
                    move = {x, y, x, y+count};
                    moves.push_back(move);
                    if(Board[x][y+count]!=0) {
                        down = false;
                    }
                } else if (down && (count+y==8 || Board[x][y+count]>0)) {
                    down = false;
                }
                count++;
            }
        }

        //Bishop
        if(num ==4) {
            int count = 1;
            bool upright = true;
            bool upleft = true;
            bool downright = true;
            bool downleft = true;
            while(upright || upleft || downright || downleft) {
                //upright
                if(upright && count+x!=8 && y-count!=-1 && Board[x+count][y-count]<=0) {
                    move = {x, y, x+count, y-count};
                    moves.push_back(move);
                    if(Board[x+count][y-count]!=0) {
                        upright = false;
                    }
                } else if (upright && (count+x==8 || y-count==-1 || Board[x+count][y-count]>0)) {
                    upright = false;
                }
                //upleft
                if(upleft && x-count!=-1 && y-count!=-1 && Board[x-count][y-count]<=0) {
                    move = {x, y, x-count, y-count};
                    moves.push_back(move);
                    if(Board[x-count][y-count]!=0) {
                        upleft = false;
                    }
                } else if (upleft && (x-count==-1 || y-count==-1 || Board[x-count][y-count]>0)) {
                    upleft = false;
                }
                //downright
                if(downright && count+y!=8 && count+x!=8 && Board[x+count][y+count]<=0) {
                    move = {x, y, x+count, y+count};
                    moves.push_back(move);
                    if(Board[x+count][y+count]!=0) {
                        downright = false;
                    }
                } else if (downright && (count+y==8 || x+count==8 || Board[x+count][y+count]>0)) {
                    downright = false;
                }
                //downleft
                if(downleft && count+y!=8 && x-count!=-1 && Board[x-count][y+count]<=0) {
                    move = {x, y, x-count, y+count};
                    moves.push_back(move);
                    if(Board[x-count][y+count]!=0) {
                        downleft = false;
                    }
                } else if (downleft && (count+y==8 || x-count==-1 || Board[x-count][y+count]>0)) {
                    downleft = false;
                }
                count++;
            }
        }

        //Queen
        if(num ==6) {
            int count = 1;
            bool upright = true;
            bool upleft = true;
            bool downright = true;
            bool downleft = true;
            bool left = true;
            bool right = true;
            bool up = true;
            bool down = true;
            while(upright || upleft || downright || downleft || up || down ||left || right) {
                //upright
                if(upright && count+x!=8 && y-count!=-1 && Board[x+count][y-count]<=0) {
                    move = {x, y, x+count, y-count};
                    moves.push_back(move);
                    if(Board[x+count][y-count]!=0) {
                        upright = false;
                    }
                } else if (upright && (count+x==8 || y-count==-1 || Board[x+count][y-count]>0)) {
                    upright = false;
                }
                //upleft
                if(upleft && x-count!=-1 && y-count!=-1 && Board[x-count][y-count]<=0) {
                    move = {x, y, x-count, y-count};
                    moves.push_back(move);
                    if(Board[x-count][y-count]!=0) {
                        upleft = false;
                    }
                } else if (upleft && (x-count==-1 || y-count==-1 || Board[x-count][y-count]>0)) {
                    upleft = false;
                }
                //downright
                if(downright && count+y!=8 && count+x!=8 && Board[x+count][y+count]<=0) {
                    move = {x, y, x+count, y+count};
                    moves.push_back(move);
                    if(Board[x+count][y+count]!=0) {
                        downright = false;
                    }
                } else if (downright && (count+y==8 || x+count==8 || Board[x+count][y+count]>0)) {
                    downright = false;
                }
                //downleft
                if(downleft && count+y!=8 && x-count!=-1 && Board[x-count][y+count]<=0) {
                    move = {x, y, x-count, y+count};
                    moves.push_back(move);
                    if(Board[x-count][y+count]!=0) {
                        downleft = false;
                    }
                } else if (downleft && (count+y==8 || x-count==-1 || Board[x-count][y+count]>0)) {
                    downleft = false;
                }
                //right
                if(right && count+x!=8 && Board[x+count][y]<=0) {
                    move = {x, y, x+count, y};
                    moves.push_back(move);
                    if(Board[x+count][y]!=0) {
                        right = false;
                    }
                } else if (right && (count+x==8 || Board[x+count][y]>0)) {
                    right = false;
                }
                //left
                if(left && x-count!=-1 && Board[x-count][y]<=0) {
                    move = {x, y, x-count, y};
                    moves.push_back(move);
                    if(Board[x-count][y]!=0) {
                        left = false;
                    }
                } else if (left && (x-count==-1 || Board[x-count][y]>0)) {
                    left = false;
                }
                //up
                if(up && y-count!=-1 && Board[x][y-count]<=0) {
                    move = {x, y, x, y-count};
                    moves.push_back(move);
                    if(Board[x][y-count]!=0) {
                        up = false;
                    }
                } else if (up && (y-count==-1 || Board[x][y-count]>0)) {
                    up = false;
                }
                //down
                if(down && count+y!=8 && Board[x][y+count]<=0) {
                    move = {x, y, x, y+count};
                    moves.push_back(move);
                    if(Board[x][y+count]!=0) {
                        down = false;
                    }
                } else if (down && (count+y==8 || Board[x][y+count]>0)) {
                    down = false;
                }
                count++;
            }
        }

        if(num==5 || num== 10) {
            //king
            if(x-1!=-1) {
                if (Board[x-1][y]<=0) {
                    move = {x, y, x-1, y};
                    moves.push_back(move);
                }
                if(y+1!= 8 && Board[x-1][y+1]<=0) {
                    move = {x, y, x-1, y+1};
                    moves.push_back(move);
                }
                if(y-1!=-1 && Board[x-1][y-1]<=0) {
                    move = {x, y, x-1, y-1};
                    moves.push_back(move);
                }
            }
            if(x+1!=8) {
                if (Board[x+1][y]<=0) {
                    move = {x, y, x+1, y};
                    moves.push_back(move);
                }
                if(y+1!= 8 && Board[x+1][y+1]<=0) {
                    move = {x, y, x+1, y+1};
                    moves.push_back(move);
                }
                if(y-1!=-1 && Board[x+1][y-1]<=0) {
                    move = {x, y, x+1, y-1};
                    moves.push_back(move);
                }
            }
            if(y+1!=8 && Board[x][y+1]<=0) {
                move = {x, y, x, y+1};
                moves.push_back(move);
            }
            if(y-1!=-1 && Board[x][y-1]<=0) {
                move = {x, y, x, y-1};
                moves.push_back(move);
            }
            if(num!=10 && !check) {
                bool left = true;
                bool right = true;
                if(Board[0][y]!=2) {
                    left = false;
                }
                if(Board[7][y]!=2) {
                    right = false;
                }
                for(int i= 1; (x+i<7 || x-i>0); i++) {
                    if(left && x-i>0 && Board[x-i][y]!=0) {
                        left = false;
                    }
                    if(right && x+i<7 && Board[x+i][y]!=0) {
                        right = false;
                    }
                }
                for(int i= 2; (x+i<7 || x-i>-1); i++) {
                    if(left && x-i>-1) {
                        move = {x, y, x-i, y};
                        moves.push_back(move);
                    }
                    if(right && x+i<8) {
                        move = {x, y, x+i, y};
                        moves.push_back(move);
                    }
                }
            }
        }


        if(num==3) {
            //Knight
            if(y+2<8 && x+1<8 && Board[x+1][y+2]<=0) {
                move = {x, y, x+1, y+2};
                moves.push_back(move);
            }
            if(y+1<8 && x+2<8 && Board[x+2][y+1]<=0) {
                move = {x, y, x+2, y+1};
                moves.push_back(move);
            }
            if(y+2<8 && x-1>-1 && Board[x-1][y+2]<=0) {
                move = {x, y, x-1, y+2};
                moves.push_back(move);
            }
            if(y+1<8 && x-2>-1 && Board[x-2][y+1]<=0) {
                move = {x, y, x-2, y+1};
                moves.push_back(move);
            }
            if(y-2>-1 && x+1<8 && Board[x+1][y-2]<=0) {
                move = {x, y, x+1, y-2};
                moves.push_back(move);
            }
            if(y-1>-1 && x+2<8 && Board[x+2][y-1]<=0) {
                move = {x, y, x+2, y-1};
                moves.push_back(move);
            }
            if(y-2>-1 && x-1>-1 && Board[x-1][y-2]<=0) {
                move = {x, y, x-1, y-2};
                moves.push_back(move);
            }
            if(y-1>-1 && x-2>-1 && Board[x-2][y-1]<=0) {
                move = {x, y, x-2, y-1};
                moves.push_back(move);
            }
        }
    }
    return moves;
}

QImage Chess::getImage(int num) {
    if(num == -1 || num==-8) {
        return BPawn;
    } else if (num == -2 || num == -11) {
        return BRook;
    } else if (num ==-3) {
        return BKnight;
    } else if (num == -4) {
        return BBishop;
    } else if (num == -5 || num == -10) {
        return BKing;
    } else if(num == -6) {
        return BQueen;
    } else if(num == 1 || num == 8) {
        return WPawn;
    } else if (num == 2 || num == 11) {
        return WRook;
    } else if (num ==3) {
        return WKnight;
    } else if (num == 4) {
        return WBishop;
    } else if (num == 5 || num == 10) {
        return WKing;
    } else if (num ==6) {
        return WQueen;
    }

}

void Chess::mousePressEvent(QMouseEvent *e) {
    int buttons = e->buttons();
    int mouseX = e->x();
    int mouseY = e->y();

    if(buttons==Qt::LeftButton &&!hold && game_end==0) {
        sx_coord = mouseX/125;
        sy_coord = mouseY/125;
        if(sx_coord>-1 && sx_coord<8 && sy_coord>-1 && sy_coord<8 && px_coord==-1) {
            if((CBoard[sx_coord][sy_coord]<0 && black_turn) || (CBoard[sx_coord][sy_coord]>0 && !black_turn)) {
                hold = true;
            }
        } else if(button_back.isClicked(e) && prev_moves.size()>1 && prev_index!=0) {
            prev_index--;
            CBoard = prev_moves[prev_index];
            black_turn = !black_turn;
            movers.clear();
            possibleMoves();
        } else if(button_forward.isClicked(e) && prev_index+1<prev_moves.size()) {
            prev_index++;
            CBoard = prev_moves[prev_index];
            black_turn = !black_turn;
            movers.clear();
            possibleMoves();
        }
    }
}

void Chess::mouseMove(QMouseEvent *e) {
    mouseX = e->x();
    mouseY = e->y();

    button_back.isHovering(e);
    button_forward.isHovering(e);

}

void Chess::mouseReleaseEvent(QMouseEvent *e) {
    if(game_end==0) {
        ex_coord = mouseX/125;
        ey_coord = mouseY/125;
        if(hold&& px_coord==-1) {
            if((ex_coord>-1 && ex_coord<8 && ey_coord>-1 && ey_coord<8) && (sx_coord!=ex_coord || sy_coord!=ey_coord)) {
                std::vector<int> move = { sx_coord, sy_coord, ex_coord, ey_coord};
                for(int i = 0; i<movers.size(); i++) {
                    if(movers[i]==move) {
                        int num = CBoard[sx_coord][sy_coord];
                        if(num==1 && sy_coord==6 && ey_coord==4) {
                            CBoard[sx_coord][sy_coord] = 8;
                        }
                        if(num==-1 && sy_coord==1 && ey_coord==3) {
                            CBoard[sx_coord][sy_coord] = -8;
                        }
                        if(num==1 && sy_coord==6 && ey_coord==4) {
                            CBoard[sx_coord][sy_coord] = 8;
                        }
                        if(num==-1 && sy_coord==1 && ey_coord==3) {
                            CBoard[sx_coord][sy_coord] = -8;
                        }
                        if(sx_coord-ex_coord==1 && (num== 1 || num==-1 || num==8 ||num==-8) && (CBoard[sx_coord-1][sy_coord] ==1 || CBoard[sx_coord-1][sy_coord]==8 || CBoard[sx_coord-1][sy_coord]==-1 || CBoard[sx_coord-1][sy_coord]==-8) && num*CBoard[sx_coord-1][sy_coord]<0) {
                            CBoard[ex_coord][ey_coord] = num;
                            CBoard[sx_coord-1][sy_coord] = 0;
                            CBoard[sx_coord][sy_coord] = 0;
                            movesmade.push_back({sx_coord, sy_coord, ex_coord, ey_coord, num, CBoard[ex_coord][ey_coord]});
                            if((ey_coord==0 || ey_coord==7) && (num==-1 || num== 1 || num== -8 || num== 8)) {
                                px_coord = ex_coord;
                                py_coord = ey_coord;
                                CBoard[sx_coord][sy_coord] = 0;
                            }
                        } else if(sx_coord-ex_coord==-1 && (num== 1 || num==-1 || num==8 ||num==-8) && (CBoard[sx_coord+1][sy_coord] ==1 || CBoard[sx_coord+1][sy_coord]==8 || CBoard[sx_coord+1][sy_coord]==-1 || CBoard[sx_coord+1][sy_coord]==-8) && num*CBoard[sx_coord+1][sy_coord]<0) {
                            CBoard[ex_coord][ey_coord] = num;
                            CBoard[sx_coord+1][sy_coord] = 0;
                            CBoard[sx_coord][sy_coord] = 0;
                            if((ey_coord==0 || ey_coord==7) && (num==-1 || num== 1 || num== -8 || num== 8)) {
                                px_coord = ex_coord;
                                py_coord = ey_coord;
                                CBoard[sx_coord][sy_coord] = 0;
                            }
                        } else if((ey_coord==0 || ey_coord==7) && (num==-1 || num== 1 || num== -8 || num== 8)) {
                            px_coord = ex_coord;
                            py_coord = ey_coord;
                            CBoard[sx_coord][sy_coord] = 0;
                        } else if((sx_coord-ex_coord>=2 || sx_coord-ex_coord<=-2) && (num==5 || num==-5)) {
                                if(sx_coord-ex_coord>=2) {
                                    CBoard[sx_coord-2][sy_coord] = num;
                                    CBoard[sx_coord][sy_coord] = 0;
                                    CBoard[sx_coord-1][sy_coord] = CBoard[0][sy_coord];
                                    CBoard[0][sy_coord] = 0;
                                    movesmade.push_back({sx_coord, sy_coord, sx_coord-2, sy_coord, num, 0});
                                } else {
                                    CBoard[sx_coord+2][sy_coord] = CBoard[sx_coord][sy_coord];
                                    CBoard[sx_coord][sy_coord] = 0;
                                    CBoard[sx_coord+1][sy_coord] = CBoard[7][sy_coord];
                                    CBoard[7][sy_coord] = 0;
                                }
                        } else {
                            CBoard[ex_coord][ey_coord] = CBoard[sx_coord][sy_coord];
                            CBoard[sx_coord][sy_coord] = 0;
                            if(CBoard[ex_coord][ey_coord]==2) {
                               CBoard[ex_coord][ey_coord] = 11;
                            } else if (CBoard[ex_coord][ey_coord] ==-2) {
                                CBoard[ex_coord][ey_coord] = -11;
                            } else if (CBoard[ex_coord][ey_coord] ==-5) {
                                CBoard[ex_coord][ey_coord] = -10;
                            } else if (CBoard[ex_coord][ey_coord] ==5) {
                                CBoard[ex_coord][ey_coord] = 10;
                            }
                        }
                        addMove();
                        if(px_coord==-1 && isLegal()) {

                            movers.clear();
                            hold = false;
                            black_turn = !black_turn;
                            possibleMoves();
                            break;
                        } else if(isLegal() && px_coord!=-1) {
                            CBoard = prev_moves[prev_moves.size()-2];
                            prev_moves.pop_back();
                            break;
                        } else {
                            CBoard = prev_moves[prev_moves.size()-2];
                            prev_moves.pop_back();
                            break;
                        }
                    }
                }


            }
            if(hold) {
                hold = false;
            }
        } else if(px_coord!=-1) {
            if(ey_coord==py_coord) {
                int neg = 1;
                if(black_turn) {
                    neg = -1;
                }
                if(ex_coord==0) {
                    CBoard[px_coord][py_coord] = 6*neg;
                    px_coord=-1;
                    py_coord=-1;

                    movers.clear();
                    hold = false;
                    black_turn = !black_turn;
                    possibleMoves();
                    addMove();
                } else if(ex_coord==2) {
                    CBoard[px_coord][py_coord] = 4*neg;
                    px_coord=-1;
                    py_coord=-1;

                    movers.clear();
                    hold = false;
                    black_turn = !black_turn;
                    possibleMoves();
                    addMove();
                } else if(ex_coord==4) {
                    CBoard[px_coord][py_coord] = 3*neg;
                    px_coord=-1;
                    py_coord=-1;

                    movers.clear();
                    hold = false;
                    black_turn = !black_turn;
                    possibleMoves();
                    addMove();
                } else if(ex_coord==6) {
                    CBoard[px_coord][py_coord] = 11*neg;
                    px_coord=-1;
                    py_coord=-1;

                    movers.clear();
                    hold = false;
                    black_turn = !black_turn;
                    possibleMoves();
                    addMove();
                }
            }

        }
    } else {

    }

}

std::vector<int> Chess::getKing(std::vector<std::vector<int>> Board) {
    std::vector<int> king;
    for(int i = 0; i<8; i++) {
        for(int j = 0; j<8; j++) {
            if(((Board[i][j]==-5 || Board[i][j] ==-10) && black_turn) || ((Board[i][j]==5 || Board[i][j] ==10) && !black_turn)) {
                king.push_back(i);
                king.push_back(j);
                return king;
            }
        }
    }
    return king;
}

bool Chess::isLegal() {
    std::vector<std::vector<int>> nextmoves;
    std::vector<int> king;
    king = getKing(CBoard);
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++){

            std::vector<std::vector<int>> mover;

            mover = getMoves(i, j, !black_turn, CBoard);
            for(int m = 0; m<mover.size(); m++) {
                nextmoves.push_back(mover[m]);
                if(mover[m][2] == king[0] && mover[m][3] == king[1]) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Chess::inCheck(std::vector<int> move) {
    std::vector<std::vector<int>> nextmoves;
    std::vector<std::vector<int>> Board = CBoard;
    std::vector<int> king;

    if((Board[move[0]][move[1]]==5 || Board[move[0]][move[1]]==-5) && move[1]==move[3] && std::abs(move[0]-move[2])>=2) {
        if(move[0]-move[2]>1) {
            Board[move[0]-2][move[1]] = 10*(abs(Board[move[0]][move[1]])/Board[move[0]][move[1]]);
            Board[move[0]+1][move[1]] = 11*(abs(Board[move[0]][move[1]])/Board[move[0]][move[1]]);
            Board[move[0]][move[1]] = 0;
            Board[0][move[1]] = 0;
        } else {
            Board[move[0]+2][move[1]] = 10*(abs(Board[move[0]][move[1]])/Board[move[0]][move[1]]);
            Board[move[0]-1][move[1]] = 11*(abs(Board[move[0]][move[1]])/Board[move[0]][move[1]]);
            Board[move[0]][move[1]] = 0;
            Board[7][move[1]] = 0;
        }
    } else if((Board[move[0]][move[1]] == -1 || Board[move[0]][move[1]] == 1) && Board[move[2]][move[1]]*Board[move[0]][move[1]]==-8) {
        Board[move[2]][move[1]] = 0;
        Board[move[2]][move[3]] = Board[move[0]][move[1]];
    } else {
        Board[move[2]][move[3]] = Board[move[0]][move[1]];
        Board[move[0]][move[1]] = 0;
    }

    king = getKing(Board);
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++){

            std::vector<std::vector<int>> mover;

            mover = getMoves(i, j, !black_turn, Board);
            for(int m = 0; m<mover.size(); m++) {
                nextmoves.push_back(mover[m]);

                if(mover[m][2] == king[0] && mover[m][3] == king[1]) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Chess::inCheck() {
    std::vector<int> king;
    king = getKing(CBoard);
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++){

            std::vector<std::vector<int>> mover;

            mover = getMoves(i, j, !black_turn, CBoard);
            for(int m = 0; m<mover.size(); m++) {
                if(mover[m][2] == king[0] && mover[m][3] == king[1]) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Chess::checkMate() {
    if(movers.size()==0) {
        game_end = 3;
        if(inCheck()) {
            game_end = 2;
            if(black_turn) {
                game_end = 1;
            }
        }
        return true;
    }
    return false;
}

void Chess::addMove() {
    while(prev_index!=prev_moves.size()-1) {
        prev_moves.pop_back();
    }
    prev_moves.push_back(CBoard);
    prev_index++;
}

QString Chess::getMessage() {
    QString message = "stale mate :( play agian??";
    if(game_end==2){
        message = "Black wins!!!! play agian";
    } else if(game_end==1) {
        message = "White wins!!!! play agian";
    }

    return message;
}

int Chess::getWidth() {
    return width;
}

int Chess::getHeight() {
    return height;
}
