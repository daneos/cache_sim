#include <stdio.h>
#include "cache.h"
#include "log.h" 

int main(void)
{
	int max_size = 1024;
	time_t max_time = 1;
	int hit = 0;
	int miss = 0;
	char *q;
	int i = 0;

	cache *c = (cache*)malloc(sizeof(cache));
	cache_init(c, max_time, max_size);


	q = parse(++i);
	while(q)
	{
		printf("[%d] Update removed %d entries\n", time(NULL), cache_update(c));
		if(in_cache(c, q) != -1) 
		{
			hit++;
			printf("[%d] %s in cache, hit\n", time(NULL), q);
		}
		else
		{
			miss++;
			int as = add_str(c, q);
			switch(as)
			{
				case -1:
					printf("[%d] Not inserted %s: cache full\n", time(NULL), q);
					break;
				case -2:
					printf("[%d] Not inserted %s: exists in cache\n", time(NULL), q);
					break;
				default:
					printf("[%d] Inserted %s at index %d\n", time(NULL), q, as);
					break;
			}
		}
		q = parse(++i);
	}
	printf("\nHit: %d\nMiss: %d\n", hit, miss);
}