#ifndef __GFXOB__
#define __GFXOB__

#include "observer.h"
#include "window.h"
#include <vector>
class Game;

class graphicObserver : public Observer
{
    // Private Fields
    Xwindow *win;
    Game *subject;
    std::vector<char> grid; // Used to optimise rendering
public:
    int charColor(char c);
    graphicObserver(Game *sub);
    void notify() override;
    virtual ~graphicObserver();
    void square_highlight(int index);
    void square_unhighlight(int index);
};

#endif
