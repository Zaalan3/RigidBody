#include <stdint.h> 
#include <stdbool.h> 
#include <stdlib.h> 
#include <tice.h> 
#include <graphx.h>
#include <keypadc.h>

#include "object.h"
#include "collision.h"
#include "vectormath.h" 

#define G 64

bool drawVerts,drawBBoxes; 

void initialize();
void step(); 
void draw();

AxisProjection penAxis; 

#define startTimer() timer_1_Counter = 0; \
					timer_Control = TIMER1_ENABLE|TIMER1_CPU|TIMER1_UP;
#define stopTimer() timer_Control = TIMER1_DISABLE; 
#define getTimer() timer_1_Counter

void main(void)
{
    bool current,last;
	initialize(); 
	kb_Scan();
	last = false;
	
	while(!kb_IsDown(kb_KeyClear)) { 
		 
		current = kb_IsDown(kb_KeyEnter); 
		startTimer();
		//if(current&&(!last)) 
			step(); 
		stopTimer();
		last = current;
	
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
	
	penAxis.depth = 0;
		
	for(i = 0;i<vlen;i++) { 
		// something like velocity verlet
		if(vert[i].free) { 
			vert[i].v.x = (vert[i].p.x - vert[i].x.x  + vert[i].v.x)>>1;
			vert[i].v.y = (vert[i].p.y - vert[i].x.y + G + vert[i].v.y)>>1;
			vert[i].x = vert[i].p; 
			vert[i].p.x += vert[i].v.x;
			vert[i].p.y += vert[i].v.y;
		} 
	}
	
	solveConstraints();
	buildBoundingBoxes();
	
	for(i = 0;i < glen-1;i++) { 
		for(j=i+1;j<glen;j++) { 
			if(overlap(i,j)) { 
				collideGroups(i,j); 
			} 
		} 
	}

} 

void draw() { 
	uint8_t i,time;
	gfx_ZeroScreen();
	
	if(drawBBoxes) { 
		for(i = 0;i<glen;i++) { 
			int x,y,w,h; 
			x = fxtoi(group[i].xmin);
			y = fxtoi(group[i].ymin);
			w = fxtoi(group[i].xmax) - x;
			h = fxtoi(group[i].ymax) - y;
			gfx_SetColor(0xE8);
			gfx_Rectangle(x*2,y*2,w*2,h*2); 
		}
	} 
	
	for(i = 0;i<clen;i++) { 
		Vertex v0,v1; 
		v0 = vert[constraint[i].v0];
		v1 = vert[constraint[i].v1];
		gfx_SetColor(0xFF);
		gfx_Line(fxtoi(v0.p.x)*2,fxtoi(v0.p.y)*2,fxtoi(v1.p.x)*2,fxtoi(v1.p.y)*2); 
	}
	
	if(drawVerts) { 
		for(i=0;i<vlen;i++) {
			gfx_SetColor(0xC6); 
			gfx_Circle(fxtoi(vert[i].p.x)*2,fxtoi(vert[i].p.y)*2,4); 
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
