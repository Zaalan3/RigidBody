#include <stdint.h> 
#include <stdbool.h> 
#include <stdlib.h> 
#include <tice.h> 
#include <graphx.h>
#include <keypadc.h>
#include <debug.h>

#include "object.h"
#include "collision.h"
#include "vectormath.h" 

#define G 16

bool drawVerts,drawBBoxes; 

void initialize();
void step(); 
void draw();

#define startTimer() timer_1_Counter = 0; \
					timer_Control = TIMER1_ENABLE|TIMER1_CPU|TIMER1_UP;
#define stopTimer() timer_Control = TIMER1_DISABLE; 
#define getTimer() timer_1_Counter

void main(void)
{

	initialize(); 
	kb_Scan();
	
	while(!kb_IsDown(kb_KeyClear)) { 

		step(); 
			
		drawVerts = kb_IsDown(kb_Key1);
		drawBBoxes = kb_IsDown(kb_Key2);
		
		draw(); 
		kb_Scan();
	} 
	
	gfx_End(); 
}
	
void initialize() { 
	uint8_t i;
	gfx_Begin(); 
	gfx_SetTextFGColor(0xFE);
	gfx_ZeroScreen(); 
	gfx_SetDrawBuffer(); 
	gfx_ZeroScreen();
	
	drawVerts = false; 
	drawBBoxes = false; 
		
	for(i = 0;i<vlen;i++) { 
		vert[i].p.x = itofx(vert[i].p.x);
		vert[i].p.y = itofx(vert[i].p.y);
		vert[i].x.x = vert[i].p.x;
		vert[i].x.y = vert[i].p.y;
	}
	
	for(i = 0;i<clen;i++) { 
		Vec2 dist; 
		Vertex* v0; 
		Vertex* v1;
	
		v0 = &vert[constraint[i].v0];
		v1 = &vert[constraint[i].v1]; 
	
		dist.x = v1->p.x - v0->p.x;
		dist.y = v1->p.y - v0->p.y;
		normalize(&dist);
		constraint[i].dir = dist;
	}
	
	for(i = 0;i<glen;i++) { 
		uint8_t j;
		int maxx,minx,maxy,miny; 
		Vertex* v;
		v = &vert[group[i].vstart];
	
		maxx = v->p.x;
		minx = maxx; 
		maxy = v->p.y;
		miny = maxy;
		v++; 
	
		for(j = 0;j < (group[i].vend - group[i].vstart);j++) { 
			maxx = max(v->p.x,maxx);
			minx = min(v->p.x,minx);
			maxy = max(v->p.y,maxy);
			miny = min(v->p.y,miny);
			v++; 
		} 
	
		group[i].xmin = minx;
		group[i].xmax = maxx;
		group[i].ymin = miny;
		group[i].ymax = maxy;
	}
} 

void step() { 
	uint8_t i,j;
		
	//dbg_ClearConsole();
	
	startTimer();

	for(i = 0;i<vlen;i++) { 
		if(vert[i].free) { 
			int vx,vy;
			vx = vert[i].p.x - vert[i].x.x;
			vy = vert[i].p.y - vert[i].x.y + G;
			vert[i].x = vert[i].p; 
			vert[i].p.x += vx;
			vert[i].p.y += vy;
		} 
	}
		
	solveConstraints();
	buildBoundingBoxes();
	for(i = 0;i < glen-1;i++) { 
		if(group[i].type == TERRAIN) break;
		for(j=i+1;j<glen;j++) { 
			if(overlap(i,j)) { 
				collideGroups(i,j); 
			} 
		} 
	}
	stopTimer();
} 

void draw() { 
	uint8_t i,time;
	gfx_ZeroScreen();
	
	for(i = 0;i<clen;i++) { 
		Vertex v0,v1; 
		v0 = vert[constraint[i].v0];
		v1 = vert[constraint[i].v1];
		gfx_SetColor(0xFF);
		gfx_Line(fxtoi(v0.p.x),fxtoi(v0.p.y),fxtoi(v1.p.x),fxtoi(v1.p.y)); 
	}
	
	if(drawBBoxes) { 
		for(i = 0;i<glen;i++) { 
			int x,y,w,h; 
			x = fxtoi(group[i].xmin);
			y = fxtoi(group[i].ymin);
			w = fxtoi(group[i].xmax) - x;
			h = fxtoi(group[i].ymax) - y;
			gfx_SetColor(0xE8);
			gfx_Rectangle(x,y,w,h); 
		}
	} 
	
	if(drawVerts) { 
		for(i=0;i<vlen;i++) {
			gfx_SetColor(0xC6); 
			gfx_Circle(fxtoi(vert[i].p.x),fxtoi(vert[i].p.y),4); 
		} 
	}
		 
	gfx_SetTextXY(20,10);
	gfx_PrintUInt(getTimer(),0);
	
	time = (getTimer()/75543);
	gfx_SetTextXY(20,20);

	gfx_PrintUInt(time/10,1);
	gfx_PrintChar('.'); 
	gfx_PrintUInt(time%10,1);
		
	gfx_SwapDraw();
} 
