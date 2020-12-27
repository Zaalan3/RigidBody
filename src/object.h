#ifndef OBJ_H 
#define OBJ_H 

#include <stdint.h> 
#include <stdbool.h> 

#include "vectormath.h"

typedef struct { 
	Vec2 p,x; 
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
	uint8_t type; 
	// bounding box
	int xmin,xmax;
	int ymin,ymax;
} Group;

extern void solveConstraints(void);

void buildBoundingBoxes(void); 

#define overlap(a,b) ((group[a].xmax>group[b].xmin)&&(group[b].xmax>group[a].xmin)&&(group[a].ymax>group[b].ymin)&&(group[b].ymax>group[a].ymin))

enum OBJECT_TYPE { 
	TERRAIN = 0, 
	RIGID,
	BALL
};
	
#define vlen 22
extern Vertex vert[vlen]; 

#define clen 29
extern Constraint constraint[clen]; 

#define glen 8
extern Group group[glen]; 

#endif