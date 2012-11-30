#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "cache.h"
#include "log.h" 

int main(int argc, char *argv[])
{
	char *filename;
	int max_size = 1024;
	time_t max_time = 3600;
	int hit = 0;
	int miss = 0;
	char *q;
	time_t now = 0;
	int o;

	opterr = 0;
	while((o = getopt(argc, argv, "hf:s:t:")) != -1)
	{
		switch(o)
		{
			case 'f':
				filename = optarg;
				break;
			case 's':
				max_size = atoi(optarg);
				break;
			case 't':
				max_time = atoi(optarg);
				break;
			case 'h':
			case '?':
			default:
				print_usage(argv);
				return 0;
				break;
		}
	}
	// printf("DEBUG: fn:%s;s:%d;t:%d\n", filename, max_size, max_time);

	cache *c = (cache*)malloc(sizeof(cache));
	cache_init(c, max_time, max_size);
	parser_init(filename);

	while(q = parse(&now))
	{
		// printf("-------------------------------------------------------------------------------\nCOUNT: %d\n", c->count);
		// printf("[%d] Update removed %d entries\n", now, cache_update(c, now));
		if(in_cache(c, q) != -1) 
		{
			hit++;
			// printf("[%d] %s in cache, hit\n", now, q);
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
					// printf("[%d] Not inserted %s: cache full, reswitching\n", now, q);
					cremove(c, c->root);	// c->root always points to the oldest record
					goto reswitch;			// any other ideas ?
					break;
				case -2:					// should never go here
					// printf("[%d] Not inserted %s: exists in cache\n", now, q);
					break;
				default:
					// printf("[%d] Inserted %s at index %d\n", now, q, as);
					break;
			}
		}
	}
	parser_exit();
	printf("%f%%\n", ((double)hit/(double)miss)*100);
}

void print_usage(char *argv[])
{
	printf("\nUsage: %s -f <filename> <options>\n\nOptions:\n", argv[0]);
	printf("\t-f <filename>\t\tspecify log file for simulation\n");
	printf("\t-s <size>\t\tmax size (entries) of cache (defaults to 1024 entries)\n");
	printf("\t-t <time>\t\tmax time (seconds) for entry in cache (defaults to 3600s)\n");
	return;
}