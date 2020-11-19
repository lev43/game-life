#include "Dot.h"
#include "Graphics.h"
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>
#include "colors.h"
#include <iostream>

void Dot::setActive(bool act){
  active=act;
}
void Dot::updateActive(){
  _active=active;
}
bool Dot::getActive(){
  return _active;
}
void Dot::life(Dot *dots[size][size]){
	int neighbors=0;
	for(int j=-1;j<2;j++)
		for(int i=-1;i<2;i++){
			if((j==0 && i==0) || (y+i>=size || y+i<0 || x+j>=size || x+j<0))continue;
  			if(dots[y+i][x+j]->getActive())neighbors++;
 			}
	if(neighbors<3 || neighbors==5)setActive(false);
  if(neighbors>5 || neighbors==3)setActive(true);
}