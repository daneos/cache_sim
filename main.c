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
	parser_init("test.log");

	while(q = parse())
	{
		printf("-------------------------------------------------------------------------------\nCOUNT: %d\n", c->count);
		printf("[%d] Update removed %d entries\n", time(NULL), cache_update(c));
		if(in_cache(c, q) != -1) 
		{
			hit++;
			printf("[%d] %s in cache, hit\n", time(NULL), q);
		}
		else
		{
			miss++;
			int as;				// Taki bajzel, przez jedno goto
			reswitch:
			as = add_str(c, q);
			switch(as)
			{
				case -1:
					printf("[%d] Not inserted %s: cache full, reswitching\n", time(NULL), q);
					cremove(c, c->root);
					goto reswitch;			// TODO: Poprawic, bo az boli
					break;
				case -2:
					printf("[%d] Not inserted %s: exists in cache\n", time(NULL), q);
					break;
				default:
					printf("[%d] Inserted %s at index %d\n", time(NULL), q, as);
					break;
			}
		}
	}
	parser_exit();
	printf("\nHit: %d\nMiss: %d\nEF: %f\n", hit, miss, (double)hit/(double)miss);
}