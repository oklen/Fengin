#include "XwindowInit.h"

myWindow::myWindow()
{
  
}

void myWindow::initXserver()
{
  XInitThreads();
  XrmInitialize();

  windowDisplay = XOpenDisplay(NULL);
  if(!windowDisplay)
    {
  const char* display = std::getenv("DISPLAY");
  if(display)
    {
      std::cout << "Fail to find %s" << display;
    }
  else
    {
      std::cout << "DISPLAY environment variable is missing!" << std::endl;
    }
    }

  windowScreenNum = DefaultScreen(windowDisplay);
  mainWindow  = RootWindow(windowDisplay, windowScreenNum);
  
  
}

void myWindow::closeXserver()
{
  XCloseDisplay(windowDisplay);
}

void myWindow::getSystemContentScale(float &x,float &y)
{
  float xdpi = DisplayWidth(windowDisplay, windowScreenNum) *
    25.4f / DisplayWidthMM(windowDisplay, windowScreenNum);
  float ydpi = DisplayHeight(windowDisplay, windowScreenNum) *
    25.4 / DisplayHeightMM(windowDisplay, windowScreenNum);

  char *rms = XResourceManagerString(windowDisplay);
  std::cout << rms << std::endl;

  if(rms)
    {

      XrmDatabase db = XrmGetStringDatabase(rms);
      if(db){
      char *type =  NULL;
      XrmValue value;

      if(XrmGetResource(db, "Xft.dpi", "Xft.Dpi", &type, &value))
        {
          if(type&& strcmp(type,"String") == 0)
            xdpi = ydpi = atof(value.addr);
        }

      XrmDestroyDatabase(db);
      }
    }
  x = xdpi/96.f;
  y = ydpi/96.f;
}
