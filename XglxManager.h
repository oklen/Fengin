/***********************************
This is a file contain the glx manager functino
*************************************************/
#ifndef XGLMANANGER_H
#define XGLMANANGER_H
#include <GL/glx.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#define F_NUM_DONT_CARE 0x00003
Display *windowDisplay;
int  windowScreenNum;

struct mwindow
{
  Colormap colormap;
  Window handle;
  XIC ic;

  bool overrideRedirect;
  bool iconified;
  bool maximized;

  //Whether the visual supports framebuffer transparency
  bool transparent;

  //Cache position and size used to filter out duplicate event
  int width,height;
  int xpos,pos;

  //The last received cursor position ,regardless of source
  int lastCursorPosX, lastCursorPosY;
  //The last position the cursor was warped to by GLFW
  int warpCursorPosX, waroCursorPosY;

  //The time of the last KeyPress event
  Time lastKeyTime;
};

struct ctxConfig
{
  int client;
  int source;
  int marjor;
  int minor;
  bool forward;
  bool debug;
  bool noerror;
  int profile;
  int robustness;
  int release;
  mwindow* share;
};

struct winConfig
{
  int width;
  int height;
  const char* title;
  bool resizeable;
  bool visible;
  bool decorated;
  bool focused;
  bool autoIconify;
  bool floating;
  bool maximized;
  bool centerCursor;
  struct ns {
    bool retina;
    char frameName[20];
  };
  struct x11{
    char className[256];
    char instanceName[256];
  };
};

struct fbConfig
{
  int redBits;
  int greenBits;
  int blueBits;
  int alphaBits;
  int depthBits;
  int stencilBits;
  int accumRedBits;
  int accumGreenBits;
  int accumBlueBits;
  int accumAlphaBits;
  int auxBuffers;
  bool stereo;
  int samples;
  bool sRGB;
  bool doublebuffer;
  bool transparent;
  uint * handlt;

};




const fbConfig* FChooseFBConfig(const fbConfig* desired,
                                         const fbConfig* alternatives,
                                         unsigned int count)
{
    unsigned int i;
    unsigned int missing, leastMissing = -1;
    unsigned int colorDiff, leastColorDiff = -1;
    unsigned int extraDiff, leastExtraDiff = -1;
    const fbConfig* current;
    const fbConfig* closest = NULL;
    std::cout << "Get Count is " << count << std::endl;

    for (i = 0;  i < count;  i++)
    {
        current = alternatives + i;

        if (desired->stereo > 0 && current->stereo == 0)
        {
            // Stereo is a hard constraint
            continue;
        }

        if (desired->doublebuffer != current->doublebuffer)
        {
            // Double buffering is a hard constraint
            continue;
        }

        // Count number of missing buffers
        {
            missing = 0;

            if (desired->alphaBits > 0 && current->alphaBits == 0)
                missing++;

            if (desired->depthBits > 0 && current->depthBits == 0)
                missing++;

            if (desired->stencilBits > 0 && current->stencilBits == 0)
                missing++;

            if (desired->auxBuffers > 0 &&
                current->auxBuffers < desired->auxBuffers)
            {
                missing += desired->auxBuffers - current->auxBuffers;
            }

            if (desired->samples > 0 && current->samples == 0)
            {
                // Technically, several multisampling buffers could be
                // involved, but that's a lower level implementation detail and
                // not important to us here, so we count them as one
                missing++;
            }

            if (desired->transparent != current->transparent)
                missing++;
        }

        // These polynomials make many small channel size differences matter
        // less than one large channel size difference

        // Calculate color channel size difference value
        {
            colorDiff = 0;

            if (desired->redBits != F_NUM_DONT_CARE)
            {
                colorDiff += (desired->redBits - current->redBits) *
                             (desired->redBits - current->redBits);
            }

            if (desired->greenBits != F_NUM_DONT_CARE)
            {
                colorDiff += (desired->greenBits - current->greenBits) *
                             (desired->greenBits - current->greenBits);
            }

            if (desired->blueBits != F_NUM_DONT_CARE)
            {
                colorDiff += (desired->blueBits - current->blueBits) *
                             (desired->blueBits - current->blueBits);
            }
        }

        // Calculate non-color channel size difference value
        {
            extraDiff = 0;

            if (desired->alphaBits != F_NUM_DONT_CARE)
            {
                extraDiff += (desired->alphaBits - current->alphaBits) *
                             (desired->alphaBits - current->alphaBits);
            }

            if (desired->depthBits != F_NUM_DONT_CARE)
            {
                extraDiff += (desired->depthBits - current->depthBits) *
                             (desired->depthBits - current->depthBits);
            }

            if (desired->stencilBits != F_NUM_DONT_CARE)
            {
                extraDiff += (desired->stencilBits - current->stencilBits) *
                             (desired->stencilBits - current->stencilBits);
            }

            if (desired->accumRedBits != F_NUM_DONT_CARE)
            {
                extraDiff += (desired->accumRedBits - current->accumRedBits) *
                             (desired->accumRedBits - current->accumRedBits);
            }

            if (desired->accumGreenBits != F_NUM_DONT_CARE)
            {
                extraDiff += (desired->accumGreenBits - current->accumGreenBits) *
                             (desired->accumGreenBits - current->accumGreenBits);
            }

            if (desired->accumBlueBits != F_NUM_DONT_CARE)
            {
                extraDiff += (desired->accumBlueBits - current->accumBlueBits) *
                             (desired->accumBlueBits - current->accumBlueBits);
            }

            if (desired->accumAlphaBits != F_NUM_DONT_CARE)
            {
                extraDiff += (desired->accumAlphaBits - current->accumAlphaBits) *
                             (desired->accumAlphaBits - current->accumAlphaBits);
            }

            if (desired->samples != F_NUM_DONT_CARE)
            {
                extraDiff += (desired->samples - current->samples) *
                             (desired->samples - current->samples);
            }

            if (desired->sRGB && !current->sRGB)
                extraDiff++;
        }

        // Figure out if the current one is better than the best one found so far
        // Least number of missing buffers is the most important heuristic,
        // then color buffer size match and lastly size match for other buffers

        if (missing < leastMissing)
            closest = current;
        else if (missing == leastMissing)
        {
            if ((colorDiff < leastColorDiff) ||
                (colorDiff == leastColorDiff && extraDiff < leastExtraDiff))
            {
                closest = current;
            }
        }

        if (current == closest)
        {
            leastMissing = missing;
            leastColorDiff = colorDiff;
            leastExtraDiff = extraDiff;
        }
    }

    return closest;
}


static bool chooseGLXFBConfig(const fbConfig* desired,GLXFBConfig* result)
{
  GLXFBConfig* nativeConfigs;
  fbConfig* usableConfigs;
  const fbConfig* closest;
  int i,nativeCount,usableCount;
  const char* vendor;
  bool trustWindowBit = true;

  vendor = glXGetClientString(windowDisplay, 1);
  nativeConfigs =
    glXGetFBConfigs(windowDisplay,windowScreenNum,&nativeCount);
  if(!nativeConfigs || !nativeCount)
    {
      printf("GLX:No GLXFBConfigs returned");
      return false;
    }
  usableConfigs = (fbConfig*)malloc(sizeof(fbConfig));
  usableCount = 0;
  std::cout << "Get nativeCount:"  << nativeCount << std::endl;

  for(i=0;i < nativeCount ;i++)
    {
       GLXFBConfig n = nativeConfigs[i];
      fbConfig* u = usableConfigs + usableCount;

      int value;
  if(!(glXGetFBConfigAttrib(windowDisplay, n, GLX_RENDER_TYPE,&value)))
    {
      std::cout << "glXGetFBConfigAttrib failed in " << i  << std::endl;
      continue;
    }

      if(!(glXGetVisualFromFBConfig(windowDisplay, n)) &GLX_WINDOW_BIT)
        if(trustWindowBit)
          continue;

      //Skip transparent

      u->redBits = glXGetFBConfigAttrib(windowDisplay, n, GLX_RED_SIZE, &value);
      u->greenBits = glXGetFBConfigAttrib(windowDisplay,n,GLX_GREEN_SIZE,&value);
      u->blueBits = glXGetFBConfigAttrib(windowDisplay,n,GLX_BLUE_SIZE,&value);
      u->alphaBits = glXGetFBConfigAttrib(windowDisplay,n,GLX_ALPHA_SIZE,&value);
      u->depthBits = glXGetFBConfigAttrib(windowDisplay,n,GLX_DEPTH_SIZE,&value);
      u->stencilBits = glXGetFBConfigAttrib(windowDisplay,n,GLX_STENCIL_SIZE,&value);

      u->accumAlphaBits = glXGetFBConfigAttrib(windowDisplay,n,GLX_ACCUM_ALPHA_SIZE,&value);
      u->accumBlueBits = glXGetFBConfigAttrib(windowDisplay,n,GLX_ACCUM_BLUE_SIZE,&value);
      u->accumGreenBits = glXGetFBConfigAttrib(windowDisplay,n,GLX_ACCUM_GREEN_SIZE,&value);
      u->accumRedBits = glXGetFBConfigAttrib(windowDisplay,n,GLX_ACCUM_RED_SIZE,&value);

      u->auxBuffers = glXGetFBConfigAttrib(windowDisplay,n,GLX_AUX_BUFFERS,&value);

      if(glXGetFBConfigAttrib(windowDisplay, n, GLX_STEREO, &value))
        u->stereo = true;
      if(glXGetFBConfigAttrib(windowDisplay, n, GLX_DOUBLEBUFFER, &value))
      u->doublebuffer = true;
      u->handlt = (uint*)n;
      usableCount++;
      std::cout << "Get usableCount is " << usableCount << std::endl;

    }
  closest = FChooseFBConfig(desired, usableConfigs, usableCount);
  if(closest)
    *result = (GLXFBConfig) closest->handlt;

  XFree(nativeConfigs);
  free(usableConfigs);

  return closest != NULL;
}

bool FChooseVisualGLX(const winConfig* wndconfig,
                              const ctxConfig* ctxconfig,
                              const fbConfig* fbconfig,
                              Visual** visual, int* depth)
{
  GLXFBConfig native;
  XVisualInfo* result;

  if (!chooseGLXFBConfig(fbconfig, &native))
    {
      std::cout << "chooseGLXFBConfig failed 351" << std::endl;
      return false;
    }

  result = glXGetVisualFromFBConfig(windowDisplay, native);
  if (!result)
    {
      std::cout << "glxGetVisualFromFBConfig failed 358" << std::endl;
      return false;
    }

  printf("Done threre");
  *visual = result->visual;
  *depth  = result->depth;

  XFree(result);
  return true;
}

#endif
