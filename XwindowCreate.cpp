#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <iostream>
#include <vector>

Window mainwindow;
Display *dpy;

void XwindowInit()
{
  dpy = XOpenDisplay(NULL);

}

int main(){
  Window mainwindow;
  Display *dpy;
  int screen;
  XEvent event;
  Colormap colormap;
  XPixmapFormatValues* pixmapformatvalues;
  int formatcount;

  dpy = XOpenDisplay(NULL);

  std::cout << "Get display connection number" << XConnectionNumber(dpy) << std::endl;
  screen = XDefaultScreen(dpy);
  colormap = XDefaultColormap(dpy, screen);
  std::cout << "There are " << ScreenCount(dpy) << " available" << std::endl;
  pixmapformatvalues = XListPixmapFormats(dpy, &formatcount);
  for (int i =0 ;i<formatcount;i++){
    std::cout << "depth: " << (pixmapformatvalues+i)->depth << std::endl;
    std::cout << "bits_per_pixel " << (pixmapformatvalues+i)->bits_per_pixel << std::endl;
    std::cout << "scanline_pad: " << (pixmapformatvalues+i)->scanline_pad << std::endl;
    

  }

  Visual* CurrentVisual;
  CurrentVisual = DefaultVisual(dpy, screen);
  std::cout << "DefaultVisual\n"
            << "bits_per_rgb:" << CurrentVisual->bits_per_rgb << "\n"
            << "blue_mask:" <<CurrentVisual->blue_mask << "\n"
            << "c_class" << CurrentVisual->c_class << "\n"
            << "green_mask:" << CurrentVisual->green_mask << "\n"
            << "map_entries:" << CurrentVisual->map_entries << "\n"
            << "red_mask:" <<CurrentVisual->red_mask << "\n"
            << "ext_data:" << CurrentVisual->ext_data << "\n"
            << "visualid:" << CurrentVisual->visualid << "\n";

  mainwindow = XCreateWindow(dpy, RootWindow(dpy, screen),
                             20, 20, 1024, 768,  1,
                             DefaultDepth(dpy, screen), InputOutput,
                             DefaultVisual(dpy, screen), 0,NULL);
  

  XSelectInput(dpy, mainwindow, ExposureMask | KeyPressMask);

  XMapWindow(dpy,mainwindow);

  Pixmap mpixmap = XCreatePixmap(dpy, mainwindow, 100, 100, XDefaultDepth(dpy, screen));
  XGCValues gcvalues;
  gcvalues.foreground = WhitePixel(dpy, screen);
  gcvalues.background = BlackPixel(dpy, screen);

  GC mycg = XCreateGC(dpy, mainwindow, GCForeground | GCBackground, &gcvalues);


  gcvalues.foreground = BlackPixel(dpy, screen);
  GC bch = XCreateGC(dpy, mainwindow, GCForeground, &gcvalues);

  int x = 0;
  int y = 0;
  
  while(1)
    {
      XClearArea (dpy, mainwindow, 0, 0, 500, 500, false);
      XNextEvent(dpy,&event);
      XFillRectangle(dpy, mainwindow, bch,0 , 0, 1000, 1000);
      XFillRectangle(dpy, mainwindow, mycg, 20+x, 20+y, 100, 100);
      
      std::cout << x << " and " << y << std::endl;
      if(event.type == Expose)
        {
          std::cout << "Window Create !" << std::endl;
          
        }
      if(event.type == KeyPress)
        {
          char string[25];
          int len;
          KeySym keysym;
          len = XLookupString(&event.xkey, string, 25, &keysym, NULL);
          if (len > 0){
            std::cout << string << std::endl;
          }
          if(string[0] == 'w')
            y+=-10;
          if(string[0] == 's')
            y+=10;
          if(string[0] == 'a')
            x+=-10;
          if(string[0] == 'd')
            x+=10;

        }
    }
  
  XDestroyWindow(dpy, mainwindow);


  XCloseDisplay(dpy);
  return 0;


}
