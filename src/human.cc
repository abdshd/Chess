#include "human.h"
#include "pieces.h"
#include <iostream>

using namespace std;

Human::Human(bool isWhite, Board *board, graphicObserver *obs) :
    Player{isWhite, board} gfx{obs} {};

Human::~Human(){};

void Human::getmove()
{

    char input_c;
    int input_n;
    int start;
    int end;

    while (true)
    {

        // Reading starting position
        cin >> input_c;

        if (cin.fail())
        {
            throw std::invalid_argument("Unable to read ending column. Try again.");
        }

        if (('a' <= input_c) && (input_c <= 'h'))
        {
            start = (input_c - 'a');
        }
        else
        {
            throw std::invalid_argument("Invalid ending column. Try again.");
        }

        cin >> input_n;

        if (cin.fail())
        {
            throw std::invalid_argument("Unable to read ending row. Try again.");
        }

        if ((1 <= input_n) && (input_n <= 8))
        {
            start += (input_n - 1) * 8;
        }
        else
        {
            throw std::invalid_argument("Invalid ending row. Try again.");
        }

        Pieces *piece = board->boardState[start];

        for (auto i : piece->legalmoves)
        {
            gfx->square_highlight(i);
        }

        // Reading end position

        cin >> input_c;

        if (cin.fail())
        {
            throw std::invalid_argument("Unable to read ending column. Try again.");
        }

        if (('a' <= input_c) && (input_c <= 'h'))
        {
            end = (input_c - 'a');
        }
        else
        {
            throw std::invalid_argument("Invalid ending column. Try again.");
        }

        cin >> input_n;

        if (cin.fail())
        {
            throw std::invalid_argument("Unable to read ending row. Try again.");
        }

        if ((1 <= input_n) && (input_n <= 8))
        {
            end += (input_n - 1) * 8;
        }
        else
        {
            throw std::invalid_argument("Invalid ending row. Try again.");
        }

        for (auto i : piece->legalmoves)
        {
            gfx->square_unhighlight(i);
        }

        move(start, end);

        break;
    }
}

void Human::promote(int piece)
{
    int x = piece % 8;
    int y = (piece - (piece % 8)) / 8;
    char row = x + 'a';
    bool isWhiteReplace = board->boardState[piece]->isWhite;

    delete board->boardState[piece];
    char newPiece;
    while (true)
    {

        std::cout << "Promote pawn at " << row << y + 1 << ". Enter desired promotion (q/n/b/r):" << endl;
        cin >> newPiece;
        if (newPiece == 'q') {
            board->boardState[piece] = new Queen(board, isWhiteReplace, piece, 'Q');
        }
        else if (newPiece == 'n') {
            board->boardState[piece] = new Knight(board, isWhiteReplace, piece, 'N');
        }
        else if (newPiece == 'b') {
            board->boardState[piece] = new Bishop(board, isWhiteReplace, piece, 'B');
        }
        else if (newPiece == 'r') {
            board->boardState[piece] = new Rook(board, isWhiteReplace, piece, 'R');
        }
        else
        {
            std::cout << "Invalid type. Try again" << endl;
            continue;
        }
        break;
    }
}
