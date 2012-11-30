#if !defined(__CACHE_H__)
#	define __CACHE_H__
//-----------------------------------------------------------------------------

#include <time.h>
#include <string.h>
//-----------------------------------------------------------------------------

typedef struct __ce {
				time_t add_time;
				char *q;
				struct __ce *prev;
				struct __ce *next;
} celement;

typedef struct __ca {
				celement *root;
				int count;
				time_t maxtime;
				int maxsize;
} cache;
//-----------------------------------------------------------------------------

void cache_init(cache *c, const time_t maxtime, const int maxsize);
int cache_update(cache *c, const time_t now);
int add_str(cache *c, const char *str, const time_t now);
int cremove(cache *c, celement *p);
int in_cache(cache *c, const char *str);
//-----------------------------------------------------------------------------

void cache_init(cache *c, const time_t maxtime, const int maxsize)
{
	c->root = NULL;
	c->count = 0;
	c->maxtime = maxtime;
	c->maxsize = maxsize;
}
//-----------------------------------------------------------------------------

int cache_update(cache *c, const time_t now)
{
	// printf("DEBUG: cache_update()\n");
	int removed = 0;
	if(c->root == NULL) return -1;
	celement *p = c->root;
	do
	{
		if((now - p->add_time) >= c->maxtime)
		{
			cremove(c, p);
			removed++;
		}
	}
	while(p = p->next);
	return removed;
}
//-----------------------------------------------------------------------------

int add_str(cache *c, const char *str, const time_t now)
{
	// printf("DEBUG: add_str()\n");
	if(c->count >= c->maxsize) return -1;
	if(in_cache(c, str) != -1) return -2;
	else if(c->root == NULL)
	{
		c->root = (celement*)malloc(sizeof(celement));
		c->root->q = (celement*)malloc(strlen(str));
		strncpy(c->root->q, str, strlen(str));
		c->root->add_time = now;
		c->root->next = NULL;
		c->root->prev = c->root;
	}
	else
	{
		c->root->prev->next = (celement*)malloc(sizeof(celement));
		c->root->prev->next->q = (celement*)malloc(strlen(str));
		strncpy(c->root->prev->next->q, str, strlen(str));
		c->root->prev->next->add_time = now;
		c->root->prev->next->next = NULL;
		c->root->prev->next->prev = c->root->prev;
		c->root->prev = c->root->prev->next;
	}
	return ++c->count;
}
//-----------------------------------------------------------------------------

int cremove(cache *c, celement *p)
{
	// printf("DEBUG: cremove()\n");
	if(p == c->root)
	{
		// printf("DEBUG: p == c->root\n");
		// printf("DEBUG: p=%p;pn=%p;pp=%p\n", p, p->next, p->prev);
		if(p->next == NULL)
		{
			c->root = NULL;
		}
		else
		{
			p->next->prev = p->prev;
			// printf("DEBUG: prev\n");
			c->root = p->next;
			// printf("DEBUG: root\n");
		}
	}
	else				// shouldn't go there unless someone will remove entries from the middle of cache
	{
		// printf("DEBUG: p != c->root\n");
		p->prev->next = p->next;
		if(p->next != NULL) p->next->prev = p->prev;
	}
	// printf("DEBUG: free()\n");
	free(p->q);
	free(p);
	return --c->count;
}
//-----------------------------------------------------------------------------

int in_cache(cache *c, const char *str)
{
	// printf("DEBUG: in_cache()\n");
	int i = 0;
	if(c->root == NULL) return -1;
	celement *p = c->root;
	do
	{
		if(!strncmp(p->q, str, strlen(str))) return i;
		i++;
	}
	while(p = p->next);
	return -1;
}
//-----------------------------------------------------------------------------

#endif /* __CACHE_H__ */