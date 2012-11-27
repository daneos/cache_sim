#if !defined(__CACHE_H__)
#	define __CACHE_H__
//-----------------------------------------------------------------------------

#include <time.h>
#include <string.h>
//-----------------------------------------------------------------------------

typedef struct {
				time_t add_time;
				char *q;
				celement *prev;
				celement *next;
} celement;

typedef struct {
				celement *root;
				int count;
} cache;
//-----------------------------------------------------------------------------

void cache_init(cache *c);
int cache_update(cache *c, time_t maxtime);
int add_str(cache *c, char *str, int maxsize);
int remove(cache *c, celement *p);
int in_cache(cache *c, char *str);
//-----------------------------------------------------------------------------

int cache_init(cache *c)
{
	c->root = NULL;
	c->count = 0;
}
//-----------------------------------------------------------------------------

int cache_update(cache *c, time_t maxtime)
{
	int removed = 0;
	time_t curr = time(NULL);
	if(c->root == NULL) return -1;
	celement *p = c->root;
	do
	{
		if((curr - p->add_time) >= maxtime)
		{
			remove(c, p);
			removed++;
		}
	}
	while(p = p->next)
	return removed;
}
//-----------------------------------------------------------------------------

int add_str(cache *c, char *str, int maxsize)
{
	if(c->count >= maxsize) return -1;
	if(in_cache(c, str)) return -2;
	else if(c->root == NULL)
	{
		c->root = (celement*)malloc(sizeof(celement));
		c->root->q = (celement*)malloc(strlen(str));
		strncpy(c->root->q, str, strlen(str));
		c->root->add_time = time(NULL);
		c->root->next = NULL;
		c->root->prev = c->root;
	}
	else
	{
		c->root->prev->next = (celement*)malloc(sizeof(celement));
		c->root->prev->next->q = (celement*)malloc(strlen(str));
		strncpy(c->root->prev->next->q, str, strlen(str));
		c->root->prev->next->add_time = time(NULL);
		c->root->prev->next->next = NULL;
		c->root->prev->next->prev = c->root->prev;
		c->root->prev = c->root->prev->next;
	}
	return ++c->count;
}
//-----------------------------------------------------------------------------

int remove(cache *c, celement *p)
{
	p->prev->next = p->next;
	if(p->next != NULL) p->next->prev = p->prev;
	free(p->q);
	free(p);
	return 0;
}
//-----------------------------------------------------------------------------

int in_cache(cache *c, char *str)
{
	int i = 0;
	if(c->root == NULL) return -1;
	celement *p = c->root;
	do
	{
		if(!strncmp(p->q, str, strlen(str))) return i;
		i++
	}
	while(p = p->next)
	return removed;
}

#endif /* __CACHE_H__ */