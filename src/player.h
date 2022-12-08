#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "board.h"

class Rook;

class Player
{

public:
    bool isWhite;
    Board *board;
    Player(bool isWhite, Board *board);
    virtual ~Player() = 0;

    void resign();
    void move(int start, int end);
    virtual void getmove() = 0;
    virtual void promote(int piece) = 0;
    void castle(Rook *rook);
};

#endif
