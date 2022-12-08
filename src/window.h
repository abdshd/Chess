#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <X11/Xlib.h>
#include <iostream>
#include <string>
#define BKGND 11
#define FORGND 12
#define HGLT 7

class Xwindow
{
  Display *d;
  Window w;
  int s;
  GC gc;
  unsigned long colours[100];

public:
  Xwindow(int width = 500, int height = 500); // Constructor; displays the window.
  ~Xwindow();                                 // Destructor; destroys the window.
  Xwindow(const Xwindow &) = delete;
  Xwindow &operator=(const Xwindow &) = delete;

  // Available colours.
  enum
  {
    white = 0,
    black,
    red,
    green,
    blue,
    cyan,
    yellow,
    magenta,
    orange,
    brown,
    Gray41,
    Bisque,
    Sienna,
    Papaya_Whip,
    Dark_Green
  };

  // Draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour = black);
  void BlankRectangle(int x, int y, int width, int height, int colour = red);

  // Draws strings
  void drawString(int x, int y, std::string msg);
  void drawStringBold(int x, int y, std::string msg);
  void drawSym(int x, int y, std::string msg);
};

#endif
