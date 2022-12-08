#ifndef __COMPUTER_H__
#define __COMPUTER_H__

#include "player.h"

class Computer: public Player {
    public:
        std::vector<std::vector<int>> personalMoves;
        std::vector<std::vector<int>> enemyMoves;
    public:
        Computer(bool isWhite, Board* board);
        void updateData(int level);
        virtual void getmove() = 0;
        void promote(int piece);
};

class Level1 : public Computer
{
public:
    void getmove();
    Level1(bool isWhite, Board *board);
};

class Level2 : public Computer
{
public:
    void getmove();
    Level2(bool isWhite, Board *board);
};

class Level3 : public Computer
{
public:
    void getmove();
    Level3(bool isWhite, Board *board);
};

class Level4 : public Computer
{
public:
    void getmove();
    Level4(bool isWhite, Board *board);
};

#endif
