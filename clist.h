#ifndef __CLIST_H__
#define __CLIST_H__
#include <stdio.h>
#include <stdlib.h>
/*ͨ��˫������:
 *����ڵ���ڴ�������������������ʽ���ù���
 *ÿ���ڵ����ŵ����������ڴ�Ĺ������û����й���
 */


struct element {
	struct element 	*next,*prev;
	struct clist	*list;
	void		*value;
	int		vlen;
};


struct clist {
	int		len;
	struct element	root;
};


#define CLIST_STRUCT_INIT(list) {0, {&(list).root, &(list).root}}
extern struct clist *clist_init(void);


extern struct element *clist_push_front(struct clist *list,
				void *value, int vlen);
extern struct element *clist_push_back(struct clist *list,
				void *value, int vlen);

//return the first element or NULL
extern struct element *clist_front(const struct clist *list);
//return the last element or NULL
extern struct element *clist_back(const struct clist *list);

//return the next element of this
extern struct element *clist_next(const struct element *this);
//return the prev element of this
extern struct element *clist_prev(const struct element *this);

extern int clist_len(const struct clist *list);

//if ok,return the value of e
extern void *clist_remove(struct clist *list, struct element *e);

#endif
