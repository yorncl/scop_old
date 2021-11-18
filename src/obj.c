#include "obj.h"
#include <stdlib.h>

void clear_obj(Obj* obj)
{
	ft_lstclear(&obj->vertices, free);
	ft_lstclear(&obj->faces, free);
	free(obj->name);
	free(obj);
}
