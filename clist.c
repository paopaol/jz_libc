#include <stdio.h>
#include <stdlib.h>
#include "clist.h"


struct clist *clist_init(void)
{
	struct clist 	*l;

	l = calloc(sizeof(struct clist), 1);
	if(!l){
		return NULL;
	}
	l->root.next = &l->root;
	l->root.prev = &l->root;
	l->len = 0;
	return l;
}



//insert e after at
static struct element *_clist_insert(struct clist *list, 
				struct element *e, struct element *at)
{
	struct element *n;

	if(list == NULL || e == NULL || at == NULL){
		return NULL;
	}
	n = at->next;

	at->next = e;
	e->prev = at;
	e->next = n;
	n->prev = e;
	e->list = list;
	list->len++;
	return e;
}


static struct element *_clist_insert_value(struct clist *list,
						void *value, int vlen, 
						struct element *at)
{
	struct element *e;

	e = calloc(sizeof(struct element), 1);
	if(!e){
		return NULL;
	}
	e->value = value;
	e->vlen = vlen;

	if(!_clist_insert(list, e, at)){
		goto cleanup_e;	
	}
	return e;
cleanup_e:
	free(e);
	return NULL;
}


struct element *clist_push_front(struct clist *list,
				void *value, int vlen)
{
	return _clist_insert_value(list, value, vlen, &list->root);
}

struct element *clist_push_back(struct clist *list,
				void *value, int vlen)
{
	return _clist_insert_value(list, value, vlen, list->root.prev);
}


//return the next element of this
struct element *clist_next(const struct element *this)
{
	struct element *p;	

	p = this->next;
	//if p is not root, then return p
	if(this->list != NULL && p != &this->list->root){
		return p;
	}
	return NULL;
}
//return the prev element of this
struct element *clist_prev(const struct element *this)
{
	struct element *p;	

	p = this->prev;
	//if p is not root, then return p
	if(this->list != NULL && p != &this->list->root){
		return p;
	}
	return NULL;
}

//return the first element or NULL
struct element *clist_front(const struct clist *list)
{
	if(list->len == 0){
		return NULL;
	}
	return list->root.next;
}
//return the last element or NULL
struct element *clist_back(const struct clist *list)
{
	if(list->len == 0){
		return NULL;
	}
	return list->root.prev;
}

int clist_len(const struct clist *list)
{
	return list->len;
}

//_clist_remove remove e from list, decrements list->len, return e->value
static void *_clist_remove(struct clist *list, struct element *e)
{
	void 	*value;

	e->prev->next = e->next;
	e->next->prev = e->prev;
	e->prev = NULL;
	e->next = NULL;
	list->len--;
	value = e->value;
	free(e);
	return value;
}

void *clist_remove(struct clist *list, struct element *e)
{
	void 	*value;

	//if e is not root
	if(e->list == list){
		value = _clist_remove(list, e);
	}else {
		value = NULL;	
	}
	return value;
}


