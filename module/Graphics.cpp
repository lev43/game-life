#include "Graphics.h"
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>
#include "setting.h"
#include "Dot.h"

void Graphics::clear(Display *dpy, Window win, color rgb){
	XWindowAttributes       gwa;
	XGetWindowAttributes(dpy, win, &gwa);//получаем окно
	glViewport(10, 10, gwa.width, gwa.height);//настраиваем место и размер окна

	glClearColor(rgb.red/255, rgb.green/255, rgb.blue/255, rgb.alpha/255);//очищаем окно
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//настройка
	glXSwapBuffers(dpy, win);//
}

void Graphics::draw(Display *dpy, Window win, Dot *dots[size][size], bool pause) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//настройка
	XWindowAttributes gwa;
	XGetWindowAttributes(dpy, win, &gwa);//получаем окно
	for(int i=0; i<size; i++){
			for(int j=0; j<size; j++){
				color rgb;
				if(dots[i][j]->getActive())rgb=white;
				else rgb=black;
				//if(j==0 || j==size-1 || i==0 || i==size-1)rgb=red;
				if(pause && i==size-1 && j==size-1)rgb=red;
				glViewport(j*draw_xy, i*draw_xy, gwa.width/draw_size, gwa.height/draw_size);//настраиваем место и размер окна

				glMatrixMode(GL_PROJECTION);//мод
				glLoadIdentity();
				glOrtho(-1., 1., -1., 1., 1., 20.);

				glMatrixMode(GL_MODELVIEW);//начинаем отрисовку
				glLoadIdentity();
				gluLookAt(1., 1., 10., 1., 1., 0., 0., 1., 0.);
				glBegin(GL_QUADS);//отрисовка
					glColor3f(rgb.red/255, rgb.green/255, rgb.blue/255); glVertex3f(-1./size, -1./size, 0);
					glColor3f(rgb.red/255, rgb.green/255, rgb.blue/255); glVertex3f( 1./size, -1./size, 0);
					glColor3f(rgb.red/255, rgb.green/255, rgb.blue/255); glVertex3f( 1./size,  1./size, 0);
					glColor3f(rgb.red/255, rgb.green/255, rgb.blue/255); glVertex3f(-1./size,  1./size, 0);
				glEnd();//конец отрисовки
			}
		}
}