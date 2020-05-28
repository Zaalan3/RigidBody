#include "object.h" 
#include "vectormath.h" 

Vertex vert[vlen] = 
{ 
	{{60,64},{0,0},{0,0},true},
	{{76,64},{0,0},{0,0},true}, 
	{{76,80},{0,0},{0,0},true},
	{{60,80},{0,0},{0,0},true},
	
	{{32,32},{0,0},{0,0},true},
	{{48,32},{0,0},{0,0},true}, 
	{{48,48},{0,0},{0,0},true},
	{{35,48},{0,0},{0,0},true},
	
	{{0,70},{0,0},{0,0},false},
	{{120,100},{0,0},{0,0},false},	
	{{120,119},{0,0},{0,0},false},	
	{{159,110},{0,0},{0,0},false},	

};

Constraint constraint[clen] = 
{ 
	//box 1
	{0,1,12*256,128,{0,0}},
	{1,2,12*256,128,{0,0}},
	{2,3,12*256,128,{0,0}},
	{3,0,12*256,128,{0,0}},
	{0,2,2896*1.5,128,{0,0}},
	{1,3,2896*1.5,128,{0,0}},
	
	{4,5,12*256,128,{0,0}},
	{5,6,12*256,128,{0,0}},
	{6,7,12*256,128,{0,0}},
	{7,4,12*256,128,{0,0}},
	{4,6,2896*1.5,128,{0,0}},
	{5,7,2896*1.5,128,{0,0}},
		
	{8,9,80*256,0,{0,0}},	
	{9,10,20*256,0,{0,0}},	
	{10,11,20*256,0,{0,0}},	
};

Group group[glen] =
{ 
	{0,3, 0,3,true, 0,0,0,0},
	{4,7, 6,9,true, 0,0,0,0},
	{8,10, 12,13,false, 0,0,0,0},
	{10,11, 14,14,false, 0,0,0,0},
}; 

/*
void solveConstraints() {
	uint8_t i;
	int dlen;
	uint8_t k;
	Vec2 dist; 
	Vertex* v0; 
	Vertex* v1;
	
	for(i=0;i<clen;i++) { 
		k = constraint[i].k;
		
		if(k == 0) 
			break;
		
		v0 = &vert[constraint[i].v0];
		v1 = &vert[constraint[i].v1];
			
		dist.x = v1->p.x - v0->p.x;
		dist.y = v1->p.y - v0->p.y;
	
		dlen = fxMul(k,normalize(&dist) - constraint[i].length); 
		constraint[i].dir = dist; 
	
		dist.x = fxMul(dlen,dist.x); 
		dist.y = fxMul(dlen,dist.y);
		
		if(v0->free) { 
			v0->p.x += dist.x;
			v0->p.y += dist.y;
		} 
	
		if(v1->free) {
			v1->p.x -= dist.x;
			v1->p.y -= dist.y;
		} 
	} 
} 
*/

void buildBoundingBoxes() { 
	uint8_t i,g;
	int maxx,minx,maxy,miny; 
	Vertex* v; 
	
	for(g = 0;g<glen;g++) { 
		if(!group[g].free) break; 
		
		v = &vert[group[g].vstart];
	
		maxx = v->p.x;
		minx = maxx; 
		maxy = v->p.y;
		miny = maxy;
		v++; 
	
		for(i = 0;i < (group[g].vend - group[g].vstart);i++) { 
			maxx = max(v->p.x,maxx);
			minx = min(v->p.x,minx);
			maxy = max(v->p.y,maxy);
			miny = min(v->p.y,miny);
			v++; 
		} 
	
		group[g].xmin = minx;
		group[g].xmax = maxx;
		group[g].ymin = miny;
		group[g].ymax = maxy;
	} 
} 

