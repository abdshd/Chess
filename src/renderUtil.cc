#include "window.h"
#include "renderUtil.h"
#include <vector>

// Some functions used to convert indices and characters
int mapFiles(char file)
{
    if (file == 'a')
    {
        return 0;
    }
    else if (file == 'b')
    {
        return 1;
    }
    else if (file == 'c')
    {
        return 2;
    }
    else if (file == 'd')
    {
        return 3;
    }
    else if (file == 'e')
    {
        return 4;
    }
    else if (file == 'f')
    {
        return 5;
    }
    else if (file == 'g')
    {
        return 6;
    }
    else
    {
        return 7;
    }
}

std::string getPieceSym(char name)
{
    if (name == 'k')
    {
        return "♔";
    }
    else if (name == 'q')
    {
        return "♕";
    }
    else if (name == 'r')
    {
        return "♖";
    }
    else if (name == 'b')
    {
        return "♗";
    }
    else if (name == 'n')
    {
        return "♘";
    }
    else if (name == 'p')
    {
        return "♙";
    }
    else if (name == 'K')
    {
        return "♚";
    }
    else if (name == 'Q')
    {
        return "♛";
    }
    else if (name == 'R')
    {
        return "♜";
    }
    else if (name == 'B')
    {
        return "♝";
    }
    else if (name == 'N')
    {
        return "♞";
    }
    else
    {
        return "♟︎";
    }
}

// Print a text board given an 64 val char vector
void txt_printer(std::vector<char> grid)
{
    std::cout << " +-----------------+" << std::endl;
    for (int i = 7; i >= 0; i--)
    {
        std::cout << i + 1 << "| ";
        for (int j = 0; j < 8; j++)
        {
            char curr = grid.at((8 * i) + j);
            if (curr == ' ')
            {
                if ((i + j) % 2 != 0)
                {
                    if (j < 7)
                    {
                        std::cout << "  ";
                    }
                    else
                    {
                        std::cout << "  |";
                        ;
                    }
                }
                else
                {
                    if (j < 7)
                    {
                        std::cout << "- ";
                    }
                    else
                    {
                        std::cout << "- |";
                    }
                }
            }
            else
            {
                if (j < 7)
                {
                    std::cout << getPieceSym(curr) << " ";
                }
                else
                {
                    std::cout << getPieceSym(curr) << " |";
                }
            }
        }
        std::cout << std::endl;
    }
    std::cout << " +-----------------+" << std::endl;
    std::cout << "   A B C D E F G H" << std::endl;
}

// Print a gfx window given a 64 val char vector and a window
void gfx_printer(Xwindow *win, std::vector<char> grid, std::vector<char> *opti)
{
    int shift = 50;
    for (int a = 0; a < 8; a++)
    {
        for (int b = 0; b < 8; b++)
        {
            int index = (8 * (7 - b)) + a; // Printing direction is not same as board so we offset
            char curr = grid.at(index);    // Fetch Current Char
            if (curr != opti->at(index))
            { // Optimization - Store board and only print if changed
                if ((b + a) % 2 != 0)
                {
                    win->fillRectangle((a * 50) + shift, (b * 50) + shift, 50, 50, 8); // Print Black Square
                }
                else
                {
                    win->fillRectangle((a * 50) + shift, (b * 50) + shift, 50, 50, 0);
                }
                opti->at(index) = curr; // Reset Value in Optimization index
                std::string s(1, curr);
                win->drawString(a * 50 + 15 + shift, b * 50 + 35 + shift, s); // Print Piece Name
            }
        }
    }
    // Print Board Borders
    win->fillRectangle(shift, shift, 400, 5, 1);
    win->fillRectangle(shift, shift, 5, 400, 1);
    win->fillRectangle(400 + shift, shift, 5, 405, 1);
    win->fillRectangle(shift, 400 + shift, 400, 5, 1);
}
