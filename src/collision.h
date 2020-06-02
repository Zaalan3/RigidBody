#ifndef COLLIDE_H 
#define COLLIDE_H

#include <stdint.h> 
#include "object.h"

typedef struct { 
	Constraint* c; // constraint describing axis
	Vertex* v; // deepest penetrating vertex
	int depth; // penetration depth
} AxisProjection;

void collideGroups(uint8_t g1,uint8_t g2); 

#endif