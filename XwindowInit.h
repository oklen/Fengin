#ifndef XWINDOWINIT_H
#define XWINDOWINIT_H
#include <X11/Xresource.h>
#include <stdlib.h>
#include <iostream>
#include <limits.h>
#include <cstring>
#include <X11/Xlib.h>

class myWindow
{

public:
  explicit myWindow();
  void initXserver();
  void closeXserver();
  void getSystemContentScale(float&,float&);
 
private:
  
  Display *windowDisplay;
  int  windowScreenNum;
  Window mainWindow;
  int contentScaleX = 1024;
  int contentScaleY = 768;
  
};
#endif
