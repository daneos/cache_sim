#if !defined(__LOG_H__)
#	define __LOG_H__
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <time.h>

#define BUFSIZE 1024
//-----------------------------------------------------------------------------

char *qtmpfn;
char *ttmpfn;
char *stmpfn;
char *ctmpfn;
FILE *qfd;
FILE *tfd;
FILE *sfd;
FILE *cfd;
//-----------------------------------------------------------------------------

int parser_init(const char *filename);
int parser_exit(void);
char *parse(time_t *timestamp, int *size, int *code);
//-----------------------------------------------------------------------------

int parser_init(const char *filename)
{
	char *command = (char*)malloc(56+2*strlen(filename));		// length of shell command for py script + .timestamp extension + double filename (enugh also for first command)
	qtmpfn = (char*)malloc(5+strlen(filename));					// lenght of .tmp extension + filename
	ttmpfn = (char*)malloc(11+strlen(filename));				// lenght of .timestamp extension + filename
	stmpfn = (char*)malloc(6+strlen(filename));					// lenght of .size extension + filename
	ctmpfn = (char*)malloc(6+strlen(filename));					// lenght of .code extension + filename
	sprintf(qtmpfn, "%s.tmp", filename);
	sprintf(command, "cat %s | cut -d\' \' -f7 > %s", filename, qtmpfn);
	system(command);
	sprintf(stmpfn, "%s.size", filename);
	sprintf(command, "cat %s | cut -d\' \' -f10 > %s", filename, stmpfn);
	system(command);
	sprintf(ctmpfn, "%s.code", filename);
	sprintf(command, "cat %s | cut -d\' \' -f9 > %s", filename, ctmpfn);
	system(command);
	sprintf(ttmpfn, "%s.timestamp", filename);
	sprintf(command, "cat %s | cut -d\' \' -f4 | ./timestamp_gen.py > %s", filename, ttmpfn);
	system(command);
	qfd = fopen(qtmpfn, "r");
	tfd = fopen(ttmpfn, "r");
	sfd = fopen(stmpfn, "r");
	cfd = fopen(ctmpfn, "r");
}
//-----------------------------------------------------------------------------

int parser_exit(void)
{
	char *command = (char*)malloc(4+strlen(ttmpfn));		// enough for 'rm <fn>' for every tmp filename
	fclose(qfd);
	fclose(tfd);
	fclose(sfd);
	fclose(cfd);
	sprintf(command, "rm %s", qtmpfn);
	system(command);
	sprintf(command, "rm %s", stmpfn);
	system(command);
	sprintf(command, "rm %s", ctmpfn);
	system(command);
	sprintf(command, "rm %s", ttmpfn);
	system(command);
}
//-----------------------------------------------------------------------------

char *parse(time_t *timestamp, int *size, int *code)
{
	char buffer[BUFSIZE];
	if(fgets(buffer, BUFSIZE, qfd)) 
	{
		buffer[strlen(buffer)-1]='\0';			// remove '\n' at end of string
		fscanf(tfd, "%d\n", timestamp);
		fscanf(sfd, "%d\n", size);
		fscanf(cfd, "%d\n", code);
		return buffer;
	}
	return NULL;
}
//-----------------------------------------------------------------------------

#endif /* __LOG_H__ */