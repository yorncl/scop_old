#ifndef OBJ_H

#include "../lib/libft/libft.h"

typedef struct vertex_st{
	double x;	
	double y;	
	double z;	
	double w;	
} vertex;	

typedef struct face_st{
	u_int64_t v1;	
	u_int64_t v2;	
	u_int64_t v3;	
} face;	

typedef struct obj_st
{
	char *name;
	t_list* vertices;
	t_list* faces;
} Obj;

void clear_obj();

#endif
