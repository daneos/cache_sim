#include <stdio.h>
#include "cache.h"
#include "log.h" 

int main(void)
{
	int max_size = 1024;
	time_t max_time = 3600;
	int hit = 0;
	int miss = 0;
	char *q;
	int i = 0;
	time_t now = 0;

	cache *c = (cache*)malloc(sizeof(cache));
	cache_init(c, max_time, max_size);
	parser_init("test.log");

	while(q = parse(&now))
	{
		printf("-------------------------------------------------------------------------------\nCOUNT: %d\n", c->count);
		printf("[%d] Update removed %d entries\n", now, cache_update(c, now));
		if(in_cache(c, q) != -1) 
		{
			hit++;
			printf("[%d] %s in cache, hit\n", now, q);
		}
		else
		{
			miss++;
			int as;				// must be before reswitch, after goto 'as' exists
			reswitch:
			as = add_str(c, q, now);
			switch(as)
			{
				case -1:
					printf("[%d] Not inserted %s: cache full, reswitching\n", now, q);
					cremove(c, c->root);	// c->root always points to the oldest record
					goto reswitch;			// any other ideas ?
					break;
				case -2:
					printf("[%d] Not inserted %s: exists in cache\n", now, q);
					break;
				default:
					printf("[%d] Inserted %s at index %d\n", now, q, as);
					break;
			}
		}
	}
	parser_exit();
	printf("\nHit: %d\nMiss: %d\nEF: %f\n", hit, miss, (double)hit/(double)miss);
}