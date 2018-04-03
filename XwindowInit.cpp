


#include "XwindowInit.h"

myWindow::myWindow()
{
  initXserver();
  createWindow(100, 100, "google");
  std::cout << "Run have reach here myWindow" << std::endl;
}

void myWindow::setDefaultWindowHints()
{
  //The content can be grep from "window.c" in line : 267
  memset(&context, 0, sizeof(context));
  context.client = FENGINE_API;
  context.source = FENGINE_NATIVE_CONTEXT_API;
  context.marjor = 4;
  context.minor  = 3;

  memset(&windowConfig, 0, sizeof(windowConfig));
  windowConfig.resizeable = true;
  windowConfig.visible = true;
  windowConfig.decorated = true;
  windowConfig.focused = true;
  windowConfig.autoIconify = true;
  windowConfig.centerCursor = true;

  memset(&framebuffer, 0, sizeof(framebuffer));
  framebuffer.redBits = 8;
  framebuffer.greenBits =8;
  framebuffer.blueBits = 8;
  framebuffer.alphaBits = 8;
  framebuffer.depthBits = 24;
  framebuffer.stencilBits = 8;
  framebuffer.doublebuffer = true;

  
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

Window myWindow::createWindow(int width, int height, const char *title)
{
  if(!title)
    {
      std::cout << "No Window Specify" << std::endl;
      exit(1);
    }
  //mwindow* window = (mwindow*)calloc(1, sizeof(mwindow));

  fbConfig fbconfig;
  ctxConfig ctxconfig;
  winConfig winconfig;
  Fengwindow* window = (Fengwindow*)malloc(sizeof(Fengwindow));

  setDefaultWindowHints();
  fbconfig = framebuffer;
  ctxconfig = context;
  winconfig = windowConfig;

  winconfig.width = width;
  winconfig.height = height;
  winconfig.title = title;
  //Skip the share
  window->videmode.width = width;
  window->videmode.height = height; 
  window->videmode.redBits = fbconfig.redBits;
  window->videmode.greenBits = fbconfig.greenBits;
  window->videmode.blueBits = fbconfig.blueBits;
  window->videmode.refreshRate = 60;

  window->resizeable = winconfig.resizeable;
  window->decorated = winconfig.decorated;
  window->autoIconify = winconfig.autoIconify;
  window->floating = winconfig.floating;
  window->cursorMode = 100;

  window->minwindth = F_NUM_DONT_CARE;
  window->minheight = F_NUM_DONT_CARE;
  window->maxheight = F_NUM_DONT_CARE;
  window->maxwidth  = F_NUM_DONT_CARE;
  window->numer     = F_NUM_DONT_CARE;
  window->denom     = F_NUM_DONT_CARE;

  if(!FPlatformCreateWindow(window,&winconfig,&ctxconfig,&fbconfig))
    {
      std::cout << "Error: failed to initialize window 148" << std::endl;
    }
}

int myWindow::FPlatformCreateWindow(Fengwindow *window, winConfig *wndconfig, ctxConfig *ctxconfig, fbConfig *fbconfig)
{
  Visual* visual;
  int depth;

  if(!FChooseVisualGLX(wndconfig, ctxconfig,fbconfig, &visual, &depth))
    {
      std::cout << "FChooseVisualGLX failed" << std::endl;
      closeXserver();
      exit(0);
    }
  if(!createNativeWindow(window,wndconfig,visual,depth))
    {
      std::cout << "create Native window error"  << std::endl;
      closeXserver();
      exit(0);
    }
}

bool myWindow::createNativeWindow(Fengwindow *window,winConfig *wndconfig,Visual *visual,int depth)
{
  x11window->colormap = XCreateColormap(windowDisplay,mainWindow,visual,AllocNone);
  {
    XSetWindowAttributes wa;
    const  ulong wamask = CWBorderPixel | CWColormap | CWEventMask;

    wa.colormap = x11window->colormap;
    wa.border_pixel = 0;
    wa.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
      PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
      ExposureMask | FocusChangeMask | VisibilityChangeMask |
      EnterWindowMask | LeaveWindowMask | PropertyChangeMask;

    x11window->handle = XCreateWindow(windowDisplay,mainWindow,0,0,
                                   wndconfig->width,wndconfig->height,
                                   0,depth,InputOutput,
                                   visual,wamask,&wa);
    std::cout << "Run have reach here Native" << std::endl;
    exit(10);
    if(x11window->handle == NULL)
      std::cout << "Failed to create XCreateWindow" << std::endl;
    XSaveContext(windowDisplay, x11window->handle, x11context, (XPointer)x11window);

  }
    }

int main()
{
  myWindow mw;  
  for(;;);
  mw.closeXserver();
}
