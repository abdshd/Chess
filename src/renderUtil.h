#ifndef __RENDERU_H__
#define __RENDERU_H__

#include "window.h"
#include <vector>

int mapFiles(char file);
std::string getPieceSym(char name);

void txt_printer(std::vector<char> grid);
void gfx_printer(Xwindow *win, std::vector<char> grid, std::vector<char> *opti);

#endif
