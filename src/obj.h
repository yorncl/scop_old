#ifdef OBJ_H

#include <list>

using namespace std;

typedef struct v_st{
	double x;	
	double y;	
	double z;	
	double w;	
} v_st;	



typedef obj_st
{
	char *name;
	list<vertex_st> vertices;
} obj_st;

#endif
