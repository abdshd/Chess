#ifndef __TEXTOBSERVER_H__
#define __TEXTOBSERVER_H__

#include "observer.h"

class Game;
class Xwindow;

class textObserver : public Observer
{
    Game *subject;

public:
    textObserver(Game *sub);
    void notify() override;
    ~textObserver() = default;
};

#endif
