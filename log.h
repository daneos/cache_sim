#if !defined(__LOG_H__)
#	define __LOG_H__
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <time.h>

#define BUFSIZE 1024
//-----------------------------------------------------------------------------

FILE *qfd;
FILE *tfd;
//-----------------------------------------------------------------------------

int parser_init(const char *filename);
int parser_exit(void);
char *parse(time_t *timestamp);
//-----------------------------------------------------------------------------

int parser_init(const char *filename)
{
	char *command = (char*)malloc(56+2*strlen(filename));		// length of shell command for py script + .timestamp extension + double filename (enugh also for first command)
	char *qtmpfn = (char*)malloc(5+strlen(filename));			// lenght of .tmp extension + filename
	char *ttmpfn = (char*)malloc(11+strlen(filename));			// lenght of .timestamp extension + filename
	sprintf(qtmpfn, "%s.tmp", filename);
	sprintf(command, "cat %s | cut -d\' \' -f7 > %s", filename, qtmpfn);
	system(command);
	sprintf(ttmpfn, "%s.timestamp", filename);
	sprintf(command, "cat %s | cut -d\' \' -f4 | ./timestamp_gen.py > %s", filename, ttmpfn)
	system(command);
	qfd = fopen(qtmpfn, "r");
	tfd = fopen(ttmpfn, "r");
}
//-----------------------------------------------------------------------------

int parser_exit(void)
{
	fclose(qfd);
	fclose(tfd);
}
//-----------------------------------------------------------------------------

char *parse(time_t *timestamp)
{
	char buffer[BUFSIZE];
	if(fgets(buffer, BUFSIZE, qfd)) 
	{
		buffer[strlen(buffer)-1]='\0';			// remove '\n' at end of string
		fscanf(tfd, "%d\n", timestamp);
		return buffer;
	}
	return NULL;
}
//-----------------------------------------------------------------------------

#endif /* __LOG_H__ */