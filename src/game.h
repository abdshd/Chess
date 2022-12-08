#ifndef __GAME_H__
#define __GAME_H__
#include "window.h"
#include <vector>
#include <string>
#include "board.h"
#include "observer.h"

class Piece;
class Player;

class Game
{

    // Fields for Game
private:
    Player *whitePlayer;
    Player *blackPlayer;
    std::vector<Observer *> observers;
    Board *board;

public:
    int result;

    // Methods for game
public:
    Game(Board *brd, std::string white, std::string black);
    void start();
    void attach(Observer *o);
    void detach(Observer *o);
    void notifyObservers();
    char getState(int index);
};

#endif
