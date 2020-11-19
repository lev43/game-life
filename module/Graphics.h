#ifndef GRAPHICS_H
#define GRAPHICS_H

#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>
#include "color.h"
#include "colors.h"
#include "setting.h"
#include "Dot.h"

class Graphics{
  public:
    Graphics(){};
    void clear(Display *dpy, Window win, color rgb=white);
    void draw(Display *dpy, Window win, Dot *dots[size][size], bool pause=0);
};

#endif