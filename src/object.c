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
	{{0,119},{0,0},{0,0},false},
	
	{{159,100},{0,0},{0,0},false},	
};

Constraint constraint[clen] = 
{ 
	//box 1
	{0,1,16*256,1,{0,0}},
	{1,2,16*256,1,{0,0}},
	{2,3,16*256,1,{0,0}},
	{3,0,16*256,1,{0,0}},
	{0,2,2896*2,1,{0,0}},
	{1,3,2896*2,1,{0,0}},
	
	{4,5,16*256,1,{0,0}},
	{5,6,16*256,1,{0,0}},
	{6,7,16*256,1,{0,0}},
	{7,4,16*256,1,{0,0}},
	{4,6,2896*2,1,{0,0}},
	{5,7,2896*2,1,{0,0}},
	
	{8,9,80*256,0,{0,0}},	
	{9,10,20*256,0,{0,0}},	
	{10,11,80*256,0,{0,0}},	
	{11,8,20*256,0,{0,0}},	
	
	{10,12,20*256,0,{0,0}},	
	
};

Group group[glen] =
{ 
	{0,3, 0,3,true, 0,0,0,0},
	{4,7, 6,9,true, 0,0,0,0},
	{8,11, 12,15,false, 0,0,0,0},
	{10,12, 16,16,false, 0,0,0,0},
}; 


void buildBoundingBoxes() { 
	uint8_t i,g;
	int maxx,minx,maxy,miny; 
	Vertex* v; 
	
	for(g = 0;g<glen;g++) { 
		if(!group[g].free) continue; 
		
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