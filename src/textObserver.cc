#include <iostream>
#include <string>
#include <vector>
#include "game.h"
#include "textObserver.h"
#include "renderUtil.h"
#include "pieces.h"
using namespace std;

textObserver::textObserver(Game *sub) : subject{sub} {};

// Print text board
void textObserver::notify()
{
    cout << " +-----------------+" << endl;
    for (int i = 7; i >= 0; i--)
    {
        cout << i + 1 << "| ";
        for (int j = 0; j < 8; j++)
        {
            char curr = subject->getState((8 * i) + j);
            if (curr == ' ')
            {
                if ((i + j) % 2 != 0)
                {
                    if (j < 7)
                    {
                        cout << "  ";
                    }
                    else
                    {
                        cout << "  |";
                        ;
                    }
                }
                else
                {
                    if (j < 7)
                    {
                        cout << "- ";
                    }
                    else
                    {
                        cout << "- |";
                    }
                }
            }
            else
            {
                if (j < 7)
                {
                    cout << getPieceSym(curr) << " ";
                }
                else
                {
                    cout << getPieceSym(curr) << " |";
                }
            }
        }
        cout << endl;
    }
    cout << " +-----------------+" << endl;
    cout << "   A B C D E F G H" << endl;
}
