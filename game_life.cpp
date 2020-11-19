//Библиотеки//
#include <chrono>
#include <thread>
#include<stdio.h>
#include<stdlib.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>
#include<iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include "timercpp.h"

#include "Graphics.h"
#include "Dot.h"
#include "colors.h"

#include "setting.h"

Graphics gra;

using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using namespace std;
using std::chrono::system_clock;
//Экран, окно, и другое
Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XEvent                  xev;



int main(int argc, char *argv[]) {
	dpy = XOpenDisplay(NULL);//получаем экран
	if(dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(0);//Если экран не получен, выходим
	}				
	root = DefaultRootWindow(dpy);//Получаем корневое окно
	vi = glXChooseVisual(dpy, 0, att);//Визуальное инфо?

	if(vi == NULL) {
	printf("\n\tno appropriate visual found\n\n");
		exit(0);//Если vi нету, выходим
	} 
	else {
		printf("\n\tvisual %p selected\n", (void *)vi->visualid); /* %p creates hexadecimal output like in glxinfo */
	}//Сообщаем что все удалось
	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);//Создаем цветную карту

	swa.colormap = cmap;//Настройка
	swa.event_mask = ExposureMask | KeyPressMask;
	//Создание окна
	win = XCreateWindow(dpy, root, 0, 0, 1000, 1000, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	XSizeHints sizeHints;
  sizeHints.flags = PMinSize | PMaxSize;
  sizeHints.min_width = win_size; sizeHints.min_height = win_size;
  sizeHints.max_width = win_size; sizeHints.max_height = win_size;
  XSetWMNormalHints(dpy, win, &sizeHints);

	XMapWindow(dpy, win);//Показываем окно
	XStoreName(dpy, win, "Game 'Life'");//Имя окна
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);//Настройки
	glXMakeCurrent(dpy, win, glc);
	glEnable(GL_DEPTH_TEST);

	//Настройка захвата клавиш
	XSelectInput(dpy, win, KeyPressMask  | KeymapStateMask | ButtonPressMask | ButtonMotionMask);
	XMapWindow(dpy, win);
	Dot dot(dpy, win);
	Dot *dots[size][size];
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			dots[i][j]=new Dot(dpy, win, j, i);
			dots[i][j]->updateActive();
		}
	}
	bool pause=false;
	chrono::milliseconds delay=5ms;
	int pX, pY;
	while(1){
		while(XPending(dpy)){
			//Захват клавиш
			XNextEvent(dpy, &xev);
			if(xev.type==KeyPress){
				if(xev.xkey.keycode==9){
						glXMakeCurrent(dpy, None, NULL);
						glXDestroyContext(dpy, glc);
						XDestroyWindow(dpy, win);
						XCloseDisplay(dpy);
						exit(0);
				}
				if(xev.xkey.keycode==65){
						pause=!pause;
				}
				if(xev.xkey.keycode==114){
					if(delay>0ms)
					delay-=5ms;
				}
				if(xev.xkey.keycode==113){
					if(delay<1000ms)
					delay+=5ms;
				}
				if(xev.xkey.keycode==24){
					for(int i=0; i<size; i++)
						for(int j=0; j<size; j++){
							if(rand()%2)dots[i][j]->setActive(true);
							else dots[i][j]->setActive(false);
							dots[i][j]->updateActive();
						}
				}
				if(xev.xkey.keycode==26)
					for(int i=0;i<size;i++)
						for(int j=0;j<size;j++){
							dots[i][j]->setActive(false);
							dots[i][j]->updateActive();
						}
				//cout<<xev.xkey.keycode<<endl;
			}
			if(xev.type==MotionNotify && pause){
				cout<<xev.xmotion.state<<endl;
				int x=ceil(float(xev.xbutton.x)/(win_size-1)*size)-1, y=size-ceil(float(xev.xbutton.y)/(win_size-1)*size);
				if(x==pX && y==pY)continue;
				pX=x; pY=y;
				dots[y][x]->setActive(xev.xmotion.state==272 || xev.xmotion.state==256 || xev.xmotion.state==258 || xev.xmotion.state==8448 || xev.xmotion.state==8464);
				dots[y][x]->updateActive();
			}
			if(xev.type==ButtonPress){
				//cout<<ceil(float(xev.xbutton.x)/999*size)<<":"<<ceil(float(xev.xbutton.y)/999*size)<<endl;
				//cout<<xev.xbutton.button<<endl;
				if(xev.xbutton.button==4){
					if(delay>0ms)
					delay-=10ms;
				}
				if(xev.xbutton.button==5){
					if(delay<1000ms)
					delay+=10ms;
				}
				int x=ceil(float(xev.xbutton.x)/(win_size-1)*size)-1, y=size-ceil(float(xev.xbutton.y)/(win_size-1)*size);
				//cout<<x<<":"<<y<<endl;
				dots[y][x]->setActive(xev.xbutton.button==1);
				dots[y][x]->updateActive();
			}
		}
		if(!pause){
			for(int i=0; i<size; i++)
			for(int j=0; j<size; j++){
				dots[i][j]->life(dots);
			}
			for(int i=0; i<size; i++)
				for(int j=0; j<size; j++)
					dots[i][j]->updateActive();
		}
		gra.draw(dpy, win, dots, pause);
		glXSwapBuffers(dpy, win);//
		sleep_for(delay);
	}
}