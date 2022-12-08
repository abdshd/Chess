#include "board.h"
#include "pieces.h"
#include <iostream>
#include <algorithm>
#include <vector>

// Board constructor, takes in input as a fen string and parses to build board
Board::Board(std::string input) : illegalmoves{64, std::vector<int>(0)}, halfMoves{0}, moves{1}, WhiteCheck{false}, BlackCheck{false}, eval{0}
{
    int row = 7;
    int col = 0;
    int i = 0;
    boardState.resize(64);
    while (input[i] != ' ')
    {
        char curr = input[i];
        int index = (8 * row) + col;
        if (input[i + 2] == 'w')
        {
            whiteTurn = true;
        }
        else if (input[i + 2] == 'b')
        {
            whiteTurn = false;
        }

        if (curr == '/')
        {
            row--;
            col = 0;
        }
        else if (isdigit(curr))
        {
            int skip = curr - '0';
            for (int g = 0; g < skip; g++)
            {
                boardState[index + g] = new emptyPiece(this, false, index + g, ' ');
                col++;
            }
        }
        else
        {
            if (curr == 'k')
            {
                boardState[index] = new King(this, false, index, 'k');
            }
            else if (curr == 'q')
            {
                boardState[index] = new Queen(this, false, index, 'q');
            }
            else if (curr == 'p')
            {
                boardState[index] = new Pawn(this, false, index, 'p');
            }
            else if (curr == 'n')
            {
                boardState[index] = new Knight(this, false, index, 'n');
            }
            else if (curr == 'r')
            {
                boardState[index] = new Rook(this, false, index, 'r');
            }
            else if (curr == 'b')
            {
                boardState[index] = new Bishop(this, false, index, 'b');
            }
            else if (curr == 'K')
            {
                boardState[index] = new King(this, true, index, 'K');
            }
            else if (curr == 'Q')
            {
                boardState[index] = new Queen(this, true, index, 'Q');
            }
            else if (curr == 'P')
            {
                boardState[index] = new Pawn(this, true, index, 'P');
            }
            else if (curr == 'N')
            {
                boardState[index] = new Knight(this, true, index, 'N');
            }
            else if (curr == 'R')
            {
                boardState[index] = new Rook(this, true, index, 'R');
            }
            else if (curr == 'B')
            {
                boardState[index] = new Bishop(this, true, index, 'B');
            }
            col++;
        }
        i++;
    }
}

// Tells pieces to update their legal moves
void Board::notifyStateChange(bool checkTest)
{
    for (auto i : boardState)
    {
        i->updateMoves(checkTest);
    }
    if (checkTest)
    {
        for (auto i : boardState)
        {
            for (auto j : illegalmoves[i->location])
            {
                i->legalmoves.erase(std::remove(i->legalmoves.begin(), i->legalmoves.end(), j), i->legalmoves.end());
            }
        }
    }
    for (auto &i : illegalmoves)
    {
        i.clear();
    }
}

// Board Destructor
Board::~Board()
{
    for (auto i : boardState)
    {
        delete i;
    }
}

// Checks if the board has an active check and returns according to the following format
// Normal Board 0
// White Check  1
// Black Check -1
// White Checkmate 2
// White Checkmate -2
// Stalemate 3

int Board::boardInCheck(bool checkTest)
{
    if (!checkTest)
    {
        for (auto i : boardState)
        {
            i->updateMoves(false);
        }
    }
    else
    {
        notifyStateChange(true);
    }

    std::vector<int> blackMoves;
    std::vector<int> whiteMoves;
    int whiteKingPosition = -1;
    int blackKingPosition = -1;
    for (auto i : boardState)
    {
        if (i->type == 'K' || i->type == 'k')
        {
            (i->type == 'K') ? whiteKingPosition = i->location : blackKingPosition = i->location;
        }
        if (!i->isEmpty)
        {
            for (auto move : i->legalmoves)
            {
                (i->isWhite) ? whiteMoves.emplace_back(move) : blackMoves.emplace_back(move);
            }
        }
    }

    int result = 0;
    WhiteCheck = false;
    BlackCheck = false;

    if (whiteTurn && std::find(whiteMoves.begin(), whiteMoves.end(), blackKingPosition) != whiteMoves.end())
    {
        BlackCheck = true;
        result = -1;
    }
    if (whiteTurn && blackMoves.empty() && BlackCheck)
    {
        result = -2;
    }
    if (whiteTurn && blackMoves.empty() && !BlackCheck)
    {
        result = 3;
    }
    if (!whiteTurn && std::find(blackMoves.begin(), blackMoves.end(), whiteKingPosition) != blackMoves.end())
    {
        WhiteCheck = true;
        result = 1;
    }
    if (!whiteTurn && whiteMoves.empty() && WhiteCheck)
    {
        result = 2;
    }
    if (!whiteTurn && whiteMoves.empty() && !WhiteCheck)
    {
        result = 3;
    }

    return result;
}

// Checks if the current board is valid to start a game
bool Board::isValid()
{
    int whiteKing = 0;
    int blackKing = 0;
    int whitePawns = 0;
    int blackPawns = 0;
    int pieceCount = 0;

    for (auto piece : boardState)
    {
        if (piece->type == 'k')
        {
            blackKing++;
        }
        else if (piece->type == 'K')
        {
            whiteKing++;
        }
        else if (piece->type == 'p')
        {
            blackPawns++;
        }
        else if (piece->type == 'P')
        {
            whitePawns++;
        }
        if (!piece->isEmpty)
        {
            pieceCount++;
        }
    }

    if (whiteKing != 1 || blackKing != 1 || whitePawns > 8 || blackPawns > 8)
    {
        std::cout << "Wrong Piece Count 1" << std::endl;
        return false;
    }

    if (pieceCount < 3)
    {
        std::cout << "Wrong Piece Count 2" << std::endl;
        return false;
    }

    if (this->boardInCheck(false) != 0)
    {
        std::cout << "Board in check" << std::endl;
        return false;
    }

    for (int i = 0; i < 8; i++)
    {
        if (boardState[i]->type == 'P')
        {
            std::cout << "Wrong White Pawn" << std::endl;
            return false;
        }
    }

    for (int j = 56; j < 63; j++)
    {
        if (boardState[j]->type == 'p')
        {
            std::cout << "Wrong Black Pawn" << std::endl;
            return false;
        }
    }

    return true;
}
