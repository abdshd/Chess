#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <string>

class Pieces;

class Board
{

    // Fields for Board
public:
    std::vector<Pieces *> boardState;
    std::vector<std::vector<int>> illegalmoves;
    int halfMoves;
    int moves;
    bool WhiteCheck;
    bool BlackCheck;
    int eval;
    bool whiteTurn;

    // Methods for Board:
public:
    int boardInCheck(bool checkTest);
    Board(std::string fen);
    bool isValid();
    void notifyStateChange(bool checkTest);
    ~Board();
};

#endif
