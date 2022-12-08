#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <unistd.h>
#include "window.h"

using namespace std;

Xwindow::Xwindow(int width, int height)
{

  d = XOpenDisplay(NULL);
  if (d == NULL)
  {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  s = DefaultScreen(d);
  // Set up colours.
  XColor xcolour;
  Colormap cmap;
  std::vector<std::string> color_vals{"white", "black", "red", "green", "blue", "cyan", "yellow", "magenta", "orange", "brown", "Gray41", "Bisque", "Sienna", "Papaya Whip", "Dark Green"};
  int num_colors = color_vals.size();
  cmap = DefaultColormap(d, DefaultScreen(d));
  for (int i = 0; i < num_colors; ++i)
  {
    XParseColor(d, cmap, color_vals[i].c_str(), &xcolour);
    XAllocColor(d, cmap, &xcolour);
    colours[i] = xcolour.pixel;
  }

  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), colours[BKGND]);
  XMapRaised(d, w);
  XSetStandardProperties(d, w, "CS246 - Chess++", "CS246 - Chess++", None, NULL, 0, NULL);
  XSelectInput(d, w, ExposureMask | ButtonPressMask | KeyPressMask);

  Pixmap pix = XCreatePixmap(d, w, width,
                             height, DefaultDepth(d, DefaultScreen(d)));
  gc = XCreateGC(d, pix, 0, (XGCValues *)0);

  XFlush(d);
  XFlush(d);

  XSetForeground(d, gc, colours[black]);

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize);
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d, True);

  usleep(1000);

  // Make sure we don't race against the Window being shown
  XEvent ev;
  while (1)
  {
    XNextEvent(d, &ev);
    if (ev.type == Expose)
      break;
  }
}

Xwindow::~Xwindow()
{
  XFreeGC(d, gc);
  XCloseDisplay(d);
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour)
{
  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[black]);
}

void Xwindow::BlankRectangle(int x, int y, int width, int height, int colour)
{
  XSetForeground(d, gc, colours[colour]);
  int thickness = 3;
  XFillRectangle(d, w, gc, x, y, width, thickness);
  XFillRectangle(d, w, gc, x, y + height - thickness, width, thickness);
  XFillRectangle(d, w, gc, x + width - thickness, y, thickness, height);
  XFillRectangle(d, w, gc, x, y, thickness, height);
  XSetForeground(d, gc, colours[black]);
}

void Xwindow::drawStringBold(int x, int y, string msg)
{
  char **missing_charset_list_return;
  int missing_charset_count_return;
  char *def_string_return;
  std::string fontname = "-*-*-demibold-r-*-*-34-240-100-100-*-203-*-*";
  XFontSet setB = XCreateFontSet(d, fontname.c_str(), &missing_charset_list_return, &missing_charset_count_return, &def_string_return);
  Xutf8DrawString(d, w, setB, DefaultGC(d, s), x, y, msg.c_str(), msg.length());
  XFreeFontSet(d, setB);
}

void Xwindow::drawString(int x, int y, string msg)
{
  char **missing_charset_list_return;
  int missing_charset_count_return;
  char *def_string_return;
  std::string fontname = "-*-*-*-r-*-*-25-*-100-100-*-203-*-*";
  XFontSet set = XCreateFontSet(d, fontname.c_str(), &missing_charset_list_return, &missing_charset_count_return, &def_string_return);
  Xutf8DrawString(d, w, set, DefaultGC(d, s), x, y, msg.c_str(), msg.length());
  XFreeFontSet(d, set);
}

// DRAWS PIECES ON SOHAM"S MAC
void Xwindow::drawSym(int x, int y, string msg)
{
  char **missing_charset_list_return;
  int missing_charset_count_return;
  char *def_string_return;
  std::string fontname = "-*-chess-*-*-*-*-*-*-*-*-*-*-*-*";
  std::string sym;
#pragma region symCONV
  if (msg == "K")
  {
    sym = "k";
  }
  else if (msg == "Q")
  {
    sym = "q";
  }
  else if (msg == "P")
  {
    sym = "p";
  }
  else if (msg == "N")
  {
    sym = "h";
  }
  else if (msg == "R")
  {
    sym = "r";
  }
  else if (msg == "B")
  {
    sym = "b";
  }
  else if (msg == "k")
  {
    sym = "l";
  }
  else if (msg == "q")
  {
    sym = "w";
  }
  else if (msg == "p")
  {
    sym = "o";
  }
  else if (msg == "n")
  {
    sym = "j";
  }
  else if (msg == "r")
  {
    sym = "t";
  }
  else if (msg == "b")
  {
    sym = "n";
  }
#pragma endregion symCONV
  XFontSet setC = XCreateFontSet(d, fontname.c_str(), &missing_charset_list_return, &missing_charset_count_return, &def_string_return);
  Xutf8DrawString(d, w, setC, DefaultGC(d, s), x, y, sym.c_str(), sym.length());
  XFreeFontSet(d, setC);
}
