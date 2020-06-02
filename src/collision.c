#include <stdlib.h> 
#include <stdint.h> 

#include "collision.h" 
#include "object.h" 
#include "vectormath.h" 

extern AxisProjection penAxis; 

AxisProjection projectGroupAxis(uint8_t g1,uint8_t g2); 

// finds axis of least penetration and collides about it
void collideGroups(uint8_t g1,uint8_t g2) { 
	uint8_t c; 
	AxisProjection temp; 
	AxisProjection minaxis; 
	int t; 
	Vec2 d,dir,n;
	Vertex* p;
	Vertex* v0;
	Vertex* v1;

	minaxis = projectGroupAxis(g1,g2);
	if(minaxis.depth>0) return; 
	temp = projectGroupAxis(g2,g1); 
	if(temp.depth>0) return; 

	if(temp.depth>minaxis.depth)
		minaxis = temp; 
	
	penAxis = minaxis;
	
	p = minaxis.v; 
	v0 = &vert[minaxis.c->v0];
	v1 = &vert[minaxis.c->v1];
	dir = minaxis.c->dir; 
	
	n.x = fxMul(dir.y,minaxis.depth); 
	n.y = 0-fxMul(dir.x,minaxis.depth);
	
	if((v0->free||v1->free)&&p->free) { 
		n.x = n.x>>1;
		n.y = n.y>>1;
	} 

	if(p->free) { 
		int proj = fxMul(p->v.x,dir.y) - fxMul(p->v.y,dir.x);
		int projx = fxMul(proj,dir.y); 
		int projy =	fxMul(proj,0 - dir.x); 
		p->v.x = (p->v.x - projx)>>1 - projx;
		p->v.y = (p->v.y - projy)>>1 - projy;
		p->p.x -= n.x;
		p->p.y -= n.y;
		p->x = p->p;
	} 

	if(v0->free||v1->free) { 
		d.x = p->p.x - v0->p.x; 
		d.y = p->p.y - v0->p.y;
		t = (256*dotProduct(d,dir)) / minaxis.c->length; 

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

/*
AxisProjection projectGroupAxis(uint8_t g1,uint8_t g2) { 
	uint8_t i,c;
	int depth;
	AxisProjection a;
	Vec2 d,n;
	Vec2 v0,v;
	AxisProjection minax = {255,0,-8388608};

	for(c = group[g2].cstart;c<=group[g2].cend;c++) {
		v0 = vert[constraint[c].v0].p;
		v = vert[group[g1].vstart].p;

		n.x = constraint[c].dir.y;
		n.y = 0-constraint[c].dir.x;

		a.c = c;
		a.v = group[g1].vstart;
		a.depth = fxMul(v.x - v0.x,n.x) + fxMul(v.y - v0.y,n.y);

		for(i=a.v+1;i<=group[g1].vend;i++) {
			v = vert[i].p;

			depth = fxMul(v.x - v0.x,n.x) + fxMul(v.y - v0.y,n.y);

			if(depth < a.depth) {
				a.v = i;
				a.depth = depth;
			}
		}

		if(a.depth>0) {
			minax.c = 255;
			break;
		} else if ( a.depth > minax.depth ) {
			minax = a;
		}
	}

	return minax;
} 
*/
