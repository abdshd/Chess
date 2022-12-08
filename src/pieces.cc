#include "pieces.h"
#include <iostream>
#include <algorithm>

// Helper function to check if a location is an opponent
bool isEnemy(Pieces *piece, int checkLocation)
{
    return (!piece->board->boardState[checkLocation]->isEmpty &&
            piece->board->boardState[checkLocation]->isWhite != piece->isWhite);
}

// check if given move results in on king being in check
bool resultsInCheck(Pieces *piece, int end)
{
    for (auto i : piece->board->boardState)
    {
        if ((i->type == 'k' || i->type == 'K') && i->location == end)
            return false;
    }

    if (piece->type == 'k')
    {
        int x = end % 8;
        int y = (end - (end % 8)) / 8;
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                int possible = (y + j) * 8 + (x + i);
                if ((x + i) >= 0 && (x + i) < 8 && (y + j) >= 0 && (y + j) < 8 && 0 <= possible && possible < 64 && !(i == 0 && j == 0) &&
                    piece->board->boardState[possible]->type == 'K')
                    return true;
            }
        }
    }
    if (piece->type == 'K')
    {
        int x = end % 8;
        int y = (end - (end % 8)) / 8;
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                int possible = (y + j) * 8 + (x + i);
                if ((x + i) >= 0 && (x + i) < 8 && (y + j) >= 0 && (y + j) < 8 && 0 <= possible && possible < 64 && !(i == 0 && j == 0) &&
                    piece->board->boardState[possible]->type == 'k')
                    return true;
            }
        }
    }

    int start = piece->location;
    // making the move
    piece->board->boardState[start]->location = end;
    piece->board->boardState[end]->location = start;
    std::iter_swap(piece->board->boardState.begin() + start, piece->board->boardState.begin() + end);

    // copying the important fields
    bool isWhite = piece->board->boardState[start]->isWhite;
    char type = piece->board->boardState[start]->type;
    bool moved = piece->board->boardState[start]->moved;
    int twoStep = piece->board->boardState[start]->twoStep;
    if (!piece->board->boardState[start]->isEmpty)
    {
        delete piece->board->boardState[start];
        piece->board->boardState[start] = new emptyPiece(piece->board, false, start, ' ');
    }
    // checking if theres a check
    int checkState = 0;
    checkState = piece->board->boardInCheck(false);
    bool isCheck;
    if ((piece->isWhite && (checkState == 1 || checkState == 2)) || (!piece->isWhite && (checkState == -1 || checkState == -2))) isCheck = true;
    else isCheck = false;

    // undoing the move made
    delete piece->board->boardState[start];

    if (type == ' ')
    {
        piece->board->boardState[start] = new emptyPiece(piece->board, isWhite, start, ' ');
    }
    else if (type == 'p' || type == 'P')
    {
        piece->board->boardState[start] = new Pawn(piece->board, isWhite, start, type);
        piece->moved = moved;
        piece->twoStep = twoStep;
    }
    else if (type == 'r' || type == 'R')
    {
        piece->board->boardState[start] = new Rook(piece->board, isWhite, start, type);
        piece->moved = moved;
    }
    else if (type == 'q' || type == 'Q')
    {
        piece->board->boardState[start] = new Queen(piece->board, isWhite, start, type);
    }
    else if (type == 'n' || type == 'N')
    {
        piece->board->boardState[start] = new Knight(piece->board, isWhite, start, type);
    }
    else if (type == 'b' || type == 'B')
    {
        piece->board->boardState[start] = new Bishop(piece->board, isWhite, start, type);
    }

    std::iter_swap(piece->board->boardState.begin() + start, piece->board->boardState.begin() + end);
    piece->board->boardState[start]->location = start;
    piece->board->boardState[end]->location = end;

    for (auto i : piece->board->boardState)
    {
        i->updateMoves(false);
    }
    return isCheck;
}

// add to legalmoves
void addToLegalMoves(bool checkTest, Pieces *piece, int move)
{
    if (checkTest)
    {
        if (resultsInCheck(piece, move))
            piece->board->illegalmoves[piece->location].emplace_back(move);
    }
    if (std::find(piece->legalmoves.begin(), piece->legalmoves.end(), move) == piece->legalmoves.end())
    {
        piece->legalmoves.emplace_back(move);
    }
}

// Piece constructor
Pieces::Pieces(Board *board, bool isWhite, int location, char type, bool isEmpty) : board{board}, isWhite{isWhite}, location{location}, type{type}, isEmpty{isEmpty},
                                                                                    moved{false}, twoStep{-2}, legalmoves{}, illegalmoves{} {}

// Piece destructor
Pieces::~Pieces() {}

// emptyPiece constructor
emptyPiece::emptyPiece(Board *board, bool isWhite, int location, char type) : Pieces{board, isWhite, location, type, true}
{
    moved = true;
}

// emptyPiece updatemoves
void emptyPiece::updateMoves(bool checkTest) {}

// emptyPiece destructor
emptyPiece::~emptyPiece() {}

// Pawn constructor
Pawn::Pawn(Board *board, bool isWhite, int location, char type) : Pieces{board, isWhite, location, type, false} {}

void Pawn::updateMoves(bool checkTest)
{
    legalmoves.clear();
    int x = location % 8;
    int y = (location - (location % 8)) / 8;
    // White Pawns:
    // Check the space immediately above
    if (isWhite && y + 1 <= 7 && board->boardState[(y + 1) * 8 + x]->isEmpty)
    {
        addToLegalMoves(checkTest, this, (y + 1) * 8 + x);
    }
    // Check the space 2 above
    if (isWhite && !moved && ((y + 1) * 8 + x < 64) && ((y + 2) * 8 + x < 64) && board->boardState[(y + 1) * 8 + x]->isEmpty && board->boardState[(y + 2) * 8 + x]->isEmpty)
    {
        addToLegalMoves(checkTest, this, (y + 2) * 8 + x);
    }
    // Check 1 up and 1 left to see if a capture is available
    if (isWhite && y + 1 <= 7 && x - 1 >= 0 && isEnemy(this, (y + 1) * 8 + (x - 1)))
    {
        addToLegalMoves(checkTest, this, (y + 1) * 8 + (x - 1));
    }
    // Check 1 up and 1 right to see if a capture is available
    if (isWhite && y + 1 <= 7 && x + 1 <= 7 && isEnemy(this, (y + 1) * 8 + (x + 1)))
    {
        addToLegalMoves(checkTest, this, (y + 1) * 8 + (x + 1));
    }
    // Check En Passant left
    if (isWhite && x > 0 && board->boardState[y * 8 + x - 1]->type == 'p' &&
        board->boardState[y * 8 + x - 1]->twoStep - board->halfMoves == -1)
    {
        addToLegalMoves(checkTest, this, (y + 1) * 8 + (x - 1));
    }
    // Check En Passant right
    if (isWhite && x < 7 && board->boardState[y * 8 + x + 1]->type == 'p' &&
        board->boardState[y * 8 + x + 1]->twoStep - board->halfMoves == -1)
    {
        addToLegalMoves(checkTest, this, (y + 1) * 8 + (x + 1));
    }
    // Black Pawns:
    // Check the space immediately below
    if (!isWhite && y - 1 >= 0 && board->boardState[(y - 1) * 8 + x]->isEmpty)
    {
        addToLegalMoves(checkTest, this, (y - 1) * 8 + x);
    }

    // Check the space 2 below
    if (!isWhite && !moved && ((y - 1) * 8 + x >= 0) && ((y - 2) * 8 + x >= 0) && board->boardState[(y - 1) * 8 + x]->isEmpty && board->boardState[(y - 2) * 8 + x]->isEmpty)
    {
        addToLegalMoves(checkTest, this, (y - 2) * 8 + x);
    }
    // Check 1 down and 1 left to see if a capture is available
    if (!isWhite && y - 1 >= 0 && x - 1 >= 0 && isEnemy(this, (y - 1) * 8 + (x - 1)))
    {
        addToLegalMoves(checkTest, this, (y - 1) * 8 + (x - 1));
    }
    // Check 1 down and 1 right to see if a capture is available
    if (!isWhite && y - 1 >= 0 && x + 1 <= 7 && isEnemy(this, (y - 1) * 8 + (x + 1)))
    {
        addToLegalMoves(checkTest, this, (y - 1) * 8 + (x + 1));
    }
    // Check En Passant left
    if (!isWhite && x > 0 && board->boardState[y * 8 + x - 1]->type == 'P' &&
        board->boardState[y * 8 + x - 1]->twoStep - board->halfMoves == -1)
    {
        addToLegalMoves(checkTest, this, (y - 1) * 8 + (x - 1));
    }

    // Check En Passant right
    if (!isWhite && x < 7 && board->boardState[y * 8 + x + 1]->type == 'P' &&
        board->boardState[y * 8 + x + 1]->twoStep - board->halfMoves == -1)
    {
        addToLegalMoves(checkTest, this, (y - 1) * 8 + (x + 1));
    }
}

// Pawn destructor
Pawn::~Pawn() {}

// Rook constructor
Rook::Rook(Board *board, bool isWhite, int location, char type) : Pieces{board, isWhite, location, type, false} {}

void Rook::updateMoves(bool checkTest)
{
    legalmoves.clear();
    int x = location % 8;
    int y = (location - (location % 8)) / 8;

    // From the Rook to the top of the board
    for (int i = y + 1; i <= 7; i++)
    {
        if (board->boardState[i * 8 + x]->isEmpty)
        {
            addToLegalMoves(checkTest, this, i * 8 + x);
        }
        else if (isEnemy(this, i * 8 + x))
        {
            addToLegalMoves(checkTest, this, i * 8 + x);
            break;
        }
        else
            break;
    }
    // From the Rook to the bottom of the board
    for (int i = y - 1; i >= 0; i--)
    {
        if (board->boardState[i * 8 + x]->isEmpty)
        {
            addToLegalMoves(checkTest, this, i * 8 + x);
        }
        else if (isEnemy(this, i * 8 + x))
        {
            addToLegalMoves(checkTest, this, i * 8 + x);
            break;
        }
        else
            break;
    }
    // From the Rook to the right of the board
    for (int i = x + 1; i <= 7; i++)
    {
        if (board->boardState[y * 8 + i]->isEmpty)
        {
            addToLegalMoves(checkTest, this, y * 8 + i);
        }
        else if (isEnemy(this, y * 8 + i))
        {
            addToLegalMoves(checkTest, this, y * 8 + i);
            break;
        }
        else
            break;
    }
    // From the Rook to the left of the board
    for (int i = x - 1; i >= 0; i--)
    {
        if (board->boardState[y * 8 + i]->isEmpty)
        {
            addToLegalMoves(checkTest, this, y * 8 + i);
        }
        else if (isEnemy(this, y * 8 + i))
        {
            addToLegalMoves(checkTest, this, y * 8 + i);
            break;
        }
        else
            break;
    }
}

// Rook destructor
Rook::~Rook() {}

// King constructor
King::King(Board *board, bool isWhite, int location, char type) : Pieces{board, isWhite, location, type, false} {}

bool castlingSpace(Pieces *piece, int location, bool checkTest)
{
    if (checkTest)
    {
        return (piece->board->boardState[location]->isEmpty && !resultsInCheck(piece, location));
    }
    return (piece->board->boardState[location]->isEmpty);
}

void King::updateMoves(bool checkTest)
{
    legalmoves.clear();
    int x = location % 8;
    int y = (location - (location % 8)) / 8;

    // Check all surrounding spaces for the King
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int possible = (y + j) * 8 + (x + i);
            if ((x + i) >= 0 && (x + i) < 8 && (y + j) >= 0 && (y + j) < 8 && 0 <= possible && possible < 64 && !(i == 0 && j == 0) && (board->boardState[possible]->isEmpty || board->boardState[possible]->isWhite != isWhite))
            {
                addToLegalMoves(checkTest, this, (y + j) * 8 + (x + i));
            }
        }
    }

    // Castling
    if (isWhite && !moved && !board->WhiteCheck && castlingSpace(this, 5, checkTest) &&
        castlingSpace(this, 6, checkTest) && board->boardState[7]->type == 'R' && !board->boardState[7]->moved)
    {
        legalmoves.emplace_back(6);
    }
    if (isWhite && !moved && !board->WhiteCheck && castlingSpace(this, 3, checkTest) &&
        castlingSpace(this, 2, checkTest) && board->boardState[1]->isEmpty && board->boardState[0]->type == 'R' &&
        !board->boardState[0]->moved)
    {
        legalmoves.emplace_back(2);
    }
    if (!isWhite && !moved && !board->BlackCheck && castlingSpace(this, 61, checkTest) &&
        castlingSpace(this, 62, checkTest) && board->boardState[63]->type == 'r' && !board->boardState[63]->moved)
    {
        legalmoves.emplace_back(62);
    }
    if (!isWhite && !moved && !board->BlackCheck && castlingSpace(this, 59, checkTest) &&
        castlingSpace(this, 58, checkTest) && board->boardState[57]->isEmpty && board->boardState[56]->type == 'r' &&
        !board->boardState[56]->moved)
    {
        legalmoves.emplace_back(58);
    }
}

// King destructor
King::~King() {}

// Queen constructor
Queen::Queen(Board *board, bool isWhite, int location, char type) : Pieces{board, isWhite, location, type, false} {}

void Queen::updateMoves(bool checkTest)
{
    legalmoves.clear();
    int x = location % 8;
    int y = (location - (location % 8)) / 8;
    // Queen x/y sliding:
    // From the Queen to the top of the board
    for (int i = y + 1; i <= 7; i++)
    {
        if (board->boardState[i * 8 + x]->isEmpty)
        {
            addToLegalMoves(checkTest, this, i * 8 + x);
        }
        else if (isEnemy(this, i * 8 + x))
        {
            addToLegalMoves(checkTest, this, i * 8 + x);
            break;
        }
        else
            break;
    }
    // From the Queen to the bottom of the board
    for (int i = y - 1; i >= 0; i--)
    {
        if (board->boardState[i * 8 + x]->isEmpty)
        {
            addToLegalMoves(checkTest, this, i * 8 + x);
        }
        else if (isEnemy(this, i * 8 + x))
        {
            addToLegalMoves(checkTest, this, i * 8 + x);
            break;
        }
        else
            break;
    }
    // From the Queen to the right of the board
    for (int i = x + 1; i <= 7; i++)
    {
        if (board->boardState[y * 8 + i]->isEmpty)
        {
            addToLegalMoves(checkTest, this, y * 8 + i);
        }
        else if (isEnemy(this, y * 8 + i))
        {
            addToLegalMoves(checkTest, this, y * 8 + i);
            break;
        }
        else
            break;
    }
    // From the Queen to the left of the board
    for (int i = x - 1; i >= 0; i--)
    {
        if (board->boardState[y * 8 + i]->isEmpty)
        {
            addToLegalMoves(checkTest, this, y * 8 + i);
        }
        else if (isEnemy(this, y * 8 + i))
        {
            addToLegalMoves(checkTest, this, y * 8 + i);
            break;
        }
        else
            break;
    }

    // Queen diagonal sliding
    bool doubleBreak = false;

    // Checking the up/right diagonal
    for (int i = x + 1; i < 8; i++)
    {
        for (int j = y + 1; j < 8; j++)
        {
            if (abs(i - x) == abs(j - y))
            {
                if (!board->boardState[j * 8 + i]->isEmpty && board->boardState[j * 8 + i]->isWhite == isWhite)
                {
                    doubleBreak = true;
                    break;
                }
                addToLegalMoves(checkTest, this, j * 8 + i);
                if (isEnemy(this, j * 8 + i))
                {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak)
            break;
    }

    doubleBreak = false;
    // Checking the down/right diagonal
    for (int i = x + 1; i < 8; i++)
    {
        for (int j = y - 1; j >= 0; j--)
        {
            if (abs(i - x) == abs(j - y))
            {
                if (!board->boardState[j * 8 + i]->isEmpty && board->boardState[j * 8 + i]->isWhite == isWhite)
                {
                    doubleBreak = true;
                    break;
                }
                addToLegalMoves(checkTest, this, j * 8 + i);
                if (isEnemy(this, j * 8 + i))
                {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak)
            break;
    }

    doubleBreak = false;

    // Checking the down/left diagonal
    for (int i = x - 1; i >= 0; i--)
    {
        for (int j = y - 1; j >= 0; j--)
        {
            if (abs(i - x) == abs(j - y))
            {
                if (!board->boardState[j * 8 + i]->isEmpty && board->boardState[j * 8 + i]->isWhite == isWhite)
                {
                    doubleBreak = true;
                    break;
                }
                addToLegalMoves(checkTest, this, j * 8 + i);
                if (isEnemy(this, j * 8 + i))
                {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak)
            break;
    }

    doubleBreak = false;

    // Checking the up/left diagonal
    for (int i = x - 1; i >= 0; i--)
    {
        for (int j = y + 1; j < 8; j++)
        {
            if (abs(i - x) == abs(j - y))
            {
                if (!board->boardState[j * 8 + i]->isEmpty && board->boardState[j * 8 + i]->isWhite == isWhite)
                {
                    doubleBreak = true;
                    break;
                }
                addToLegalMoves(checkTest, this, j * 8 + i);
                if (isEnemy(this, j * 8 + i))
                {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak)
            break;
    }
}

// Queen destructor
Queen::~Queen() {}

// Knight constructor
Knight::Knight(Board *board, bool isWhite, int location, char type) : Pieces{board, isWhite, location, type, false} {}

void Knight::updateMoves(bool checkTest)
{
    legalmoves.clear();
    int x = location % 8;
    int y = (location - (location % 8)) / 8;
    // Combining these two arrays gives the 8 possible moves for any knight

    // make this into 1 array for pieces2.cc
    int xChange[8] = {-1, 1, -2, -2, -1, 1, 2, 2};
    int yChange[8] = {-2, -2, -1, 1, 2, 2, -1, 1};
    // Looping through all 8 positions and checking if they are empty or capturable
    for (int i = 0; i < 8; i++)
    {
        int possible = (y + yChange[i]) * 8 + x + xChange[i];
        if (0 <= possible && possible < 64 && y + yChange[i] < 8 && x + xChange[i] < 8 && y + yChange[i] >= 0 && x + xChange[i] >= 0 &&
            (board->boardState[possible]->isEmpty || isEnemy(this, possible)))
        {
            addToLegalMoves(checkTest, this, possible);
        }
    }
}

// Knight destructor
Knight::~Knight() {}

// Bishop constructor
Bishop::Bishop(Board *board, bool isWhite, int location, char type) : Pieces{board, isWhite, location, type, false} {}

void Bishop::updateMoves(bool checkTest)
{
    legalmoves.clear();
    int x = location % 8;
    int y = (location - (location % 8)) / 8;

    // create a helper function for the inner portion of the loop

    bool doubleBreak = false;

    // Checking the up/right diagonal
    for (int i = x + 1; i < 8; i++)
    {
        for (int j = y + 1; j < 8; j++)
        {
            if (abs(i - x) == abs(j - y))
            {
                if (!board->boardState[j * 8 + i]->isEmpty && board->boardState[j * 8 + i]->isWhite == isWhite)
                {
                    doubleBreak = true;
                    break;
                }
                addToLegalMoves(checkTest, this, j * 8 + i);
                if (isEnemy(this, j * 8 + i))
                {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak)
            break;
    }
    doubleBreak = false;
    // Checking the down/right diagonal
    for (int i = x + 1; i < 8; i++)
    {
        for (int j = y - 1; j >= 0; j--)
        {
            if (abs(i - x) == abs(j - y))
            {
                if (!board->boardState[j * 8 + i]->isEmpty && board->boardState[j * 8 + i]->isWhite == isWhite)
                {
                    doubleBreak = true;
                    break;
                }
                addToLegalMoves(checkTest, this, j * 8 + i);
                if (isEnemy(this, j * 8 + i))
                {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak)
            break;
    }

    doubleBreak = false;

    // Checking the down/left diagonal
    for (int i = x - 1; i >= 0; i--)
    {
        for (int j = y - 1; j >= 0; j--)
        {
            if (abs(i - x) == abs(j - y))
            {
                if (!board->boardState[j * 8 + i]->isEmpty && board->boardState[j * 8 + i]->isWhite == isWhite)
                {
                    doubleBreak = true;
                    break;
                }
                addToLegalMoves(checkTest, this, j * 8 + i);
                if (isEnemy(this, j * 8 + i))
                {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak)
            break;
    }

    doubleBreak = false;

    // Checking the up/left diagonal
    for (int i = x - 1; i >= 0; i--)
    {
        for (int j = y + 1; j < 8; j++)
        {
            if (abs(i - x) == abs(j - y))
            {
                if (!board->boardState[j * 8 + i]->isEmpty && board->boardState[j * 8 + i]->isWhite == isWhite)
                {
                    doubleBreak = true;
                    break;
                }
                addToLegalMoves(checkTest, this, j * 8 + i);
                if (isEnemy(this, j * 8 + i))
                {
                    doubleBreak = true;
                    break;
                }
            }
        }
        if (doubleBreak)
            break;
    }
}

// Bishop destructor
Bishop::~Bishop() {}
