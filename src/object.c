#include "object.h" 
#include "vectormath.h" 

Vertex vert[vlen] = 
{ 
	{{130,32},{0,0},true},
	{{142,32},{0,0},true}, 
	{{142,48},{0,0},true},
	{{130,48},{0,0},true},
	
	{{10,32},{0,0},true},
	{{20,32},{0,0},true}, 
	{{20,48},{0,0},true},
	{{10,48},{0,0},true},
	
	{{64,32},{0,0},true},
	{{80,32},{0,0},true}, 
	{{80,48},{0,0},true},
	{{64,48},{0,0},true},
	
	{{60,0},{0,0},true},
	{{72,0},{0,0},true}, 
	{{72,16},{0,0},true},
	{{60,16},{0,0},true},
	
	{{80,8},{0,0},false},
	
	{{0,70},{0,0},false},
	{{50,100},{0,0},false},	
	{{50,119},{0,0},false},	
	{{120,119},{0,0},false},	
	{{159,70},{0,0},false},	
};

Constraint constraint[clen] = 
{ 
	//box 1
	{0,1,16*256,127,{0,0}},
	{1,2,32*256,127,{0,0}},
	{2,3,16*256,127,{0,0}},
	{3,0,32*256,127,{0,0}},
	{0,2,9159,127,{0,0}},
	{1,3,9159,127,{0,0}},
	
	{4,5,12*256,127,{0,0}},
	{5,6,12*256,127,{0,0}},
	{6,7,12*256,127,{0,0}},
	{7,4,12*256,127,{0,0}},
	{4,6,2896*1.5,127,{0,0}},
	{5,7,2896*1.5,127,{0,0}},
	
	{8,9,12*256,127,{0,0}},
	{9,10,12*256,127,{0,0}},
	{10,11,12*256,127,{0,0}},
	{11,8,12*256,127,{0,0}},
	{8,10,2896*1.5,127,{0,0}},
	{9,11,2896*1.5,127,{0,0}},
	
	{12,13,12*256,127,{0,0}},
	{13,14,16*256,127,{0,0}},
	{14,15,12*256,127,{0,0}},
	{15,12,16*256,127,{0,0}},
	{12,14,5120,127,{0,0}},
	{13,15,2560,127,{0,0}},

	{12,16,16*256,80,{0,0}},
	
	{17,18,0,0,{0,0}},	
	{18,19,0,0,{0,0}},	
	{19,20,0,0,{0,0}},	
	{20,21,0,0,{0,0}},	
};

Group group[glen] =
{ 
	{0,3, 0,3, RIGID, 0,0,0,0},
	{4,7, 6,9, RIGID, 0,0,0,0},
	{8,11, 12,15, RIGID, 0,0,0,0},
	{12,15, 18,21, RIGID, 0,0,0,0},
	{17,18, 25,25, TERRAIN, 0,0,0,0},
	{18,19, 26,26, TERRAIN, 0,0,0,0},
	{19,20, 27,27, TERRAIN,0,0,0,0},
	{20,21, 28,28, TERRAIN,0,0,0,0},
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
		if(group[g].type==TERRAIN) break; 
		
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

