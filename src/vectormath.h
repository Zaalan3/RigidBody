#ifndef VECMATH
#define VECMATH

typedef struct { 
	int x,y; 
} Vec2;

#define dotProduct(a,b) (fxMul(a.x,b.x) + fxMul(a.y,b.y))

#define max(x,y) ((x>y)?(x):(y))
#define min(x,y) ((x<y)?(x):(y))
#define sign(x) ((x<0)?-1:1)

// normalizes vector 
// returns length of original vector.
int normalize(Vec2* v); 

#define itofx(x) (x<<8)

int fxtoi(int x); 

int sqrtInt(int x); 

void initFxMul(void);
 
int fxMul(int x,int y); 

int fxDiv(int num,int dem); 

#endif