#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "cache.h"
#include "log.h"

#define MB		*1048576
#define _MB		/1048576.

int main(int argc, char *argv[])
{
	char *filename;
	long long max_size = 1024 MB;
	time_t max_time = 3600;
	int hit = 0;
	int miss = 0;
	char *q;
	time_t now = 0;
	int o;
	int size;

	opterr = 0;
	while((o = getopt(argc, argv, "hf:s:t:")) != -1)
	{
		switch(o)
		{
			case 'f':
				filename = optarg;
				break;
			case 's':
				max_size = atol(optarg) MB;
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
	// fprintf(stderr, "DEBUG: fn:%s;s:%lld;t:%d\n", filename, max_size, max_time);

	cache *c = (cache*)malloc(sizeof(cache));
	cache_init(c, max_time, max_size);
	parser_init(filename);

	int total = 0;
	while(q = parse(&now, &size))
	{
		fprintf(stderr, "-------------------------------------------------------------------------------\nSIZE: %f MB\n", (double)c->size _MB);
		fprintf(stderr, "[%d] Update removed %d entries\n", now, cache_update(c, now));
		cache_update(c, now);
		if(in_cache(c, q) != -1) 
		{
			hit++;
			fprintf(stderr, "[%d] %s in cache, hit\n", now, q);
		}
		else
		{
			miss++;
			long long as;				// must be before reswitch, after goto 'as' exists
			reswitch:
			as = add_str(c, q, now, size);
			switch(as)
			{
				case -1:
					fprintf(stderr, "[%d] Not inserted %s: cache full, reswitching\n", now, q);
					cremove(c, c->root);	// c->root always points to the oldest record
					goto reswitch;			// any other ideas ?
					break;
				case -2:					// should never go here
					fprintf(stderr, "[%d] Not inserted %s: exists in cache\n", now, q);
					break;
				default:
					fprintf(stderr, "[%d] Inserted %s, cache size %lld\n", now, q, as);
					break;
			}
		}
		total++;
	}
	parser_exit();
	printf("%.0f:%d~%d:%d~%f%%\n", (double)max_size _MB, max_time, hit, miss, ((double)hit/(double)total)*100);
	return hit;
}

void print_usage(char *argv[])
{
	printf("\nUsage: %s -f <filename> <options>\n\nOptions:\n", argv[0]);
	printf("\t-f <filename>\t\tspecify log file for simulation\n");
	printf("\t-s <size>\t\tmax size (MB) of cache (defaults to 1024 MB)\n");
	printf("\t-t <time>\t\tmax time (seconds) for entry in cache (defaults to 3600s)\n");
	return;
}