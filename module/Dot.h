#ifndef DOT_H
#define DOT_H

#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

#include "setting.h"

class Dot{
  private:
    int x, y;
    Display *display;
    Window window;
    const static int _size=size;
  public:
    bool _active=false;
    bool active=false;
    Dot(Display *dpy, Window win, int x=0, int y=0){
      display=dpy;
      window=win;
      this->x=x;
      this->y=y;
    };
    void setActive(bool act);
    void updateActive();
    bool getActive();
    void life(Dot *dots[size][size]);
};

#endif