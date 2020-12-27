#include <stdlib.h> 
#include <stdint.h> 
#include <debug.h>

#include "collision.h" 
#include "object.h" 
#include "vectormath.h" 

AxisProjection* projectGroupAxis(uint8_t g1,uint8_t g2); 

// finds axis of least penetration and collides about it
void collideGroups(uint8_t g1,uint8_t g2) { 
	uint8_t ptype,ltype,pg,lg; 
	AxisProjection temp; 
	AxisProjection minaxis; 
	int t; 
	Vec2 d,dir,n;
	Vertex* p;
	Vertex* v0;
	Vertex* v1;

	minaxis = *(projectGroupAxis(g1,g2));
	temp = *(projectGroupAxis(g2,g1)); 
	
	if(temp.depth>minaxis.depth) { 
		minaxis = temp; 
		ptype = group[g2].type;
		ltype = group[g1].type;
		pg = g2; 
		lg = g1;
	} else { 
		ptype = group[g1].type;
		ltype = group[g2].type;
		pg = g1; 
		lg = g2;
	} 
		
	p = minaxis.v; 
	v0 = &vert[minaxis.c->v0];
	v1 = &vert[minaxis.c->v1];
	dir = minaxis.c->dir; 
	
	n.x = fxMul(dir.y,minaxis.depth); 
	n.y = 0-fxMul(dir.x,minaxis.depth);	
	
	
	if(ptype&&ltype) { 
		n.x = fxMul(127,n.x);
		n.y = fxMul(127,n.y);
	}
	
	
	if(ptype==RIGID) { 
		p->x = p->p;	
		p->p.x -= n.x;
		p->p.y -= n.y;
	} 

	if(ltype==RIGID) { 
		d.x = p->p.x - v0->p.x; 
		d.y = p->p.y - v0->p.y;

		t = fxDiv((dotProduct(d,dir)),minaxis.c->length); 
			
		if(v0->free) {
			v0->x = v0->p;
			v0->p.x += fxMul(n.x,256-t);
			v0->p.y += fxMul(n.y,256-t);
		} 

		if(v1->free) {
			v1->x = v1->p;
			v1->p.x += fxMul(n.x,t);
			v1->p.y += fxMul(n.y,t);
		}
	}
}
