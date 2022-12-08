#include "player.h"
#include "pieces.h"
#include "board.h"
#include <algorithm>
#include <iostream>

using namespace std;

Player::Player(bool isWhite, Board *board) : isWhite(isWhite), board{board} {};
Player::~Player() {}

void Player::move(int start, int end)
{
    Pieces *piece = board->boardState[start];

    // Checks if the square (represented as an index to the
    //    boardState) contains and empty.

    if (board->boardState[start]->isEmpty)
    {
        throw std::invalid_argument("Moving blank piece! Try again");
    }

    // if (start == end) {
    //     cerr << "Start and end positions cannot be the same! Try again!" << endl;
    //  }

    if (board->whiteTurn != board->boardState[start]->isWhite)
    {
        throw std::invalid_argument("Moving opponent's piece! Try again");
    }

    // Add exception moving your piece to a square with an existing pieces of the same colour

    // Iterators for finding the end move in the selected pieces
    //  legal moves.

    if (start == end || std::find(piece->legalmoves.begin(), piece->legalmoves.end(), end) == piece->legalmoves.end())
    {
        throw std::invalid_argument("Move is not legal! Try again.");
    }

    // Update moved field
    // Castling
    if (piece->type == 'K' && end == 6 && !piece->moved)
    {
        std::iter_swap(board->boardState.begin() + start, board->boardState.begin() + end);
        std::iter_swap(board->boardState.begin() + 7, board->boardState.begin() + 5);
        board->boardState[end]->location = end;
        board->boardState[5]->location = 5;
        piece->moved = true;
        return;
    }

    else if (piece->type == 'K' && end == 2 && !piece->moved)
    {
        std::iter_swap(board->boardState.begin() + start, board->boardState.begin() + end);
        std::iter_swap(board->boardState.begin() + 0, board->boardState.begin() + 3);
        board->boardState[end]->location = end;
        board->boardState[3]->location = 3;
        piece->moved = true;
        return;
    }

    else if (piece->type == 'k' && end == 62 && !piece->moved)
    {
        std::cout << "castled" << std::endl;
        std::iter_swap(board->boardState.begin() + start, board->boardState.begin() + end);
        std::iter_swap(board->boardState.begin() + 63, board->boardState.begin() + 61);
        board->boardState[end]->location = end;
        board->boardState[61]->location = 61;
        piece->moved = true;
        return;
    }

    else if (piece->type == 'k' && end == 58 && !piece->moved)
    {
        std::iter_swap(board->boardState.begin() + start, board->boardState.begin() + end);
        std::iter_swap(board->boardState.begin() + 56, board->boardState.begin() + 59);
        board->boardState[end]->location = end;
        board->boardState[59]->location = 59;
        piece->moved = true;
        return;
    }
    piece->moved = true;

    // Changes the location in the selected pieces
    //  and swaps the contents stored in the start
    //  index and end index.
    board->boardState[start]->location = end;
    board->boardState[end]->location = start;
    std::iter_swap(board->boardState.begin() + start, board->boardState.begin() + end);

    // Pawn promotion
    if ((end < 64 && end > 55 && board->boardState[end]->type == 'P') || (end < 8 && end >= 0 && board->boardState[end]->type == 'p'))
    {
        promote(end);
    }

    // If pawn moves 2 spaces, update pawn field
    if (board->boardState[end]->type == 'P' && end - start == 16)
    {
        // implement casting later
        board->boardState[end]->twoStep = board->halfMoves;
    }
    if (board->boardState[end]->type == 'p' && start - end == 16)
    {
        // implement casting later
        board->boardState[end]->twoStep = board->halfMoves;
    }

    // En Passant
    int change = start - end;
    if (board->boardState[end]->type == 'p' && (change == 9 || change == 7) && board->boardState[start]->type == 'P' &&
        (board->boardState[start]->twoStep - board->halfMoves == -1))
    {
        int killAt = (start - end == 9) ? start - 1 : start + 1;
        delete board->boardState[killAt];
        board->boardState[killAt] = new emptyPiece(board, false, killAt, ' ');
    }
    else if (board->boardState[end]->type == 'P' && (change == -7 || change == -9) && board->boardState[start]->type == 'p' &&
             (board->boardState[start]->twoStep - board->halfMoves == -1))
    {
        int killAt = (end - start == 7) ? start - 1 : start + 1;
        delete board->boardState[killAt];
        board->boardState[killAt] = new emptyPiece(board, false, killAt, ' ');
    }

    // Capturing
    if (!board->boardState[start]->isEmpty)
    {
        board->moves = -1;
        delete board->boardState[start];
        board->boardState[start] = new emptyPiece(board, false, start, ' ');
    }

    if (board->boardState[start]->type == 'P' || board->boardState[start]->type == 'p')
    {
        board->moves = -1;
    }
    board->moves++;
    return;
}
