#include <stdlib.h> 
#include "vectormath.h" 

int normalize(Vec2* v) {
	int length,invl; 
	
	/*
	int x = abs(v->x);
	int y = abs(v->y);
	 
	if(y>x) { 
		int temp = x; 
		x = y; 
		y = x; 
	} 
	length = max(fxMul(254,x)+fxMul(48,y),fxMul(216,x)+fxMul(142,y));
	*/
	
	length = 16*sqrtInt(fxMul(v->x,v->x)+fxMul(v->y,v->y));
	invl = (256*256)/length; 
	v->x = fxMul(v->x,invl);
	v->y = fxMul(v->y,invl);
	return length;
} 
