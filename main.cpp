#include "XwindowInit.h"

int main()
{
  myWindow window;
  window.initXserver();
  for(;;);
  window.closeXserver();
  
  return 0;
}
