#ifndef XWINDOWINIT_H
#define XWINDOWINIT_H
#include <X11/Xresource.h>
#include <stdlib.h>
#include <iostream>
#include <limits.h>
#include <cstring>
#include <X11/Xlib.h>
#include <pthread.h>
#include <GL/glx.h>
#include "./XglxManager.h"


#define FENGINE_API 0x00001
#define FENGINE_NATIVE_CONTEXT_API 0x000010


struct tlsPosix {
  bool allocate;
  pthread_key_t key;
};

struct window_hints
{

  int content;
  int source;
  int major;
  int minor;

};

struct Fvidmode
{
  int width;
  int height;
  int redBits;
  int greenBits;
  int blueBits;
  int refreshRate;
};




struct Fengwindow
{
  bool resizeable;
  bool decorated;
  bool autoIconify;
  bool floating;
  bool sholeClose;
  void* userPointer;
  Fvidmode videmode;
  //Fmoniter moniter;
  //Fcursor cursor;

  int minwindth,minheight;
  int maxwidth,maxheight;
  int numer,denom;

  bool stickKeys;
  bool stickMouseButtons;
  bool lockKeyMode;
  int cursorMode;
  char mouseButtons[4];
  char keys[30];
  double virtualCursorPosX,virtualCurosrPosY;

};
mwindow *x11window = new mwindow;

class myWindow
{
public:
  explicit myWindow();
  void initXserver();
  void closeXserver();
  void getSystemContentScale(float&,float&);
  void setDefaultWindowHints();
  Window createWindow(int width,int height,const char* title);
  int FPlatformCreateWindow(Fengwindow* window,winConfig* wndconfig,ctxConfig* ctxconfig,fbConfig* cbconfig);
bool createNativeWindow(Fengwindow* window,winConfig* wndconfig,Visual* visual,int depth);

private:

  
  
  Window mainWindow;
  int contentScaleX = 1024;
  int contentScaleY = 768;
  window_hints hints = {};
  fbConfig framebuffer;
  ctxConfig context;
  XContext x11context;
  winConfig windowConfig;
  int refreshRate = 0;
  void *Fglxhandle;

};
#endif
