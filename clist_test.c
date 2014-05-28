#include <stdio.h>
#include <stdlib.h>
#include "clist.h"
#include "jz_string.h"


//eg:
struct goods {
	int id;
};

static struct goods *new_goods(void)
{
	return malloc(sizeof(struct goods));
}


int main(void)
{
	struct clist 	goods_list = CLIST_STRUCT_INIT(goods_list);
	struct goods	*g;
	int		i;
	struct element	*e, *tmp;
	int		err;






	for(i = 0; i < 5; i++){
		g = new_goods();
		g->id = i;
		//此处插入的数据一定是要在堆里放着的，不然容易出问题，
		//数据的插入不存在拷贝行为
		if(!clist_push_back(&goods_list, g, sizeof(g))){
			free(g);
			err = 3;
			goto cleanup_list;
		}
	}
	for(e = clist_front(&goods_list);e != NULL; e = clist_next(e)){
		g = (struct goods *)e->value;
		printf("%d\n", g->id);
	}
	for(e = clist_front(&goods_list);e != NULL; ){
		g = (struct goods *)e->value;
		if(g->id == 1 || g->id == 3){
			tmp = clist_next(e);
			clist_remove(&goods_list, e);
			e = tmp;
			free(g);
		}else {
			e = clist_next(e);
		}
	}
	err = 0;

cleanup_list:
	while(clist_len(&goods_list) > 0){
		e = clist_back(&goods_list);
		g = (struct goods *)clist_remove(&goods_list, e);	
		if(g){
			printf("rm %d\n", g->id);
			free(g);
		}
	}
	return err;
}






