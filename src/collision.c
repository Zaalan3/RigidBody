#include <stdlib.h> 
#include <stdint.h> 

#include "collision.h" 
#include "object.h" 
#include "vectormath.h" 

#define RESTITUTION 448
#define FRICTION 240

typedef struct { 
	int depth; // penetration depth
	uint8_t v; // deepest penetrating vertex
	uint8_t c; // constraint describing axis
} AxisProjection; 

static AxisProjection projectGroupAxis(uint8_t g,uint8_t c); 

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
	
	c = group[g1].cstart;
	minaxis = projectGroupAxis(g2,c);
	c++; 
	
	for(;c<=group[g1].cend;c++) { 
		temp = projectGroupAxis(g2,c); 
		
		if(temp.depth>=0) return; // break if axis doesn't overlap
				
		if(minaxis.depth<temp.depth) { 
			minaxis=temp;
		} 
	} 
	
	for(c=group[g2].cstart;c<=group[g2].cend;c++) { 
		temp = projectGroupAxis(g1,c); 
		
		if(temp.depth>=0) return;
				
		if(minaxis.depth<temp.depth) { 
			minaxis=temp;
		}
	}
		
	p = &vert[minaxis.v]; 
	v0 = &vert[constraint[minaxis.c].v0];
	v1 = &vert[constraint[minaxis.c].v1];
	dir = constraint[minaxis.c].dir; 
	
	minaxis.depth = fxMul(RESTITUTION,minaxis.depth); 
	n.x = fxMul(dir.y,minaxis.depth); 
	n.y = 0-fxMul(dir.x,minaxis.depth);
	
	if(p->free) { 
		int orth = fxMul(FRICTION,dotProduct(p->v,dir));
		p->v.x = fxMul(dir.x,orth);
		p->v.y = fxMul(dir.y,orth);
		p->x = p->p;
		p->p.x -= n.x;
		p->p.y -= n.y;
	} 
	
	if(v0->free||v1->free) { 
		d.x = p->p.x - v0->p.x; 
		d.y = p->p.y - v0->p.y;
		t = (256*dotProduct(d,dir)) / constraint[minaxis.c].length; 
		
		if(v0->free) {
			v0->v.x = 0;
			v0->v.y = 0;
			v0->x = v0->p;
			v0->p.x += fxMul(n.x,256-t);
			v0->p.y += fxMul(n.y,256-t);
		} 
	
		if(v1->free) {
			v1->v.x = 0;
			v1->v.y = 0;
			v1->x = v1->p;
			v1->p.x += fxMul(n.x,t);
			v1->p.y += fxMul(n.y,t);
		} 
	}	
} 

AxisProjection projectGroupAxis(uint8_t g,uint8_t c) { 
	uint8_t i;
	int depth; 
	AxisProjection a; 
	Vec2 d,n; 
	Vec2 v0 = vert[constraint[c].v0].p; 
	
	Vec2 v = vert[group[g].vstart].p;
	
	n.x = constraint[c].dir.y;
	n.y = 0-constraint[c].dir.x;
	
	a.c = c; 
	a.v = group[g].vstart;
	d.x = v.x - v0.x;
	d.y = v.y - v0.y;
	a.depth = dotProduct(d,n); 
	
	for(i=a.v+1;i<=group[g].vend;i++) { 
		v = vert[i].p;
		d.x = v.x - v0.x;
		d.y = v.y - v0.y;
		depth = dotProduct(d,n);
		
		if(depth < a.depth) { 
			a.v = i; 
			a.depth = depth; 
		} 
	} 
	
	return a;
} 