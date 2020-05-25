#ifndef OBJ_H 
#define OBJ_H 

#include <stdint.h> 
#include <stdbool.h> 

#include "vectormath.h"

typedef struct { 
	Vec2 p,x,v; 
	bool free; // free vertexes can move, nonfree(fixed) can't.
} Vertex; 

typedef struct { 
	uint8_t v0,v1; // defining vertices
	int length; // equilibrium length 
	uint8_t k; // stiffness of string
	Vec2 dir;   // normalized direction vector 
} Constraint; 

typedef struct { 
	uint8_t vstart,vend; // vertices of group
	uint8_t cstart,cend; // constraints that define collision
	bool free;
	// bounding box
	int xmin,xmax;
	int ymin,ymax;
} Group;

void buildBoundingBoxes(void); 

#define overlap(a,b) ((group[a].xmax>group[b].xmin)&&(group[b].xmax>group[a].xmin)&&(group[a].ymax>group[b].ymin)&&(group[b].ymax>group[a].ymin))
						
#define vlen 13
extern Vertex vert[vlen]; 

#define clen 17
extern Constraint constraint[clen]; 

#define glen 4
extern Group group[glen]; 

#endif